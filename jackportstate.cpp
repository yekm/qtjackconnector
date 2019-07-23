#include "jackportstate.h"

#include <algorithm>
#include <stdexcept>

JackPortState::JackPortState(QObject *parent)
    : QThread(parent)
{
    m_jack_client = jack_client_open("qjackconnector", JackNoStartServer, nullptr);
    jack_set_port_registration_callback(m_jack_client, &JackPortState::port_reg, this);
    jack_set_port_rename_callback(m_jack_client, &JackPortState::port_rename, this);
}

JackPortState::~JackPortState()
{
    jack_client_close(m_jack_client);
}

bool JackPortState::update()
{
    jpd_container jpd1, jpd2;
    jpd1 = get_port_data(JackPortIsInput);
    jpd2 = get_port_data(JackPortIsOutput);
    if (jpd1 != m_ins || jpd2 != m_outs)
    {
        emit about_to_update_connections();
        QMutexLocker l(&m_mutex);
        m_ins = jpd1;
        m_outs = jpd2;
        emit connections_updated();
        return true;
    }
    return false;
}

void JackPortState::run()
{
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        update();
        QThread::msleep(100);
    }
}

JackPortState::jpd_container JackPortState::get_port_data(JackPortFlags flags) const
{
    jpd_container port_data;
    const char ** ports = jack_get_ports(m_jack_client, nullptr, nullptr, flags);
    for (int i = 0; ports && ports[i]; ++i)
    {
        JackPortData jpd;
        jpd.port = jack_port_by_name(m_jack_client, ports[i]);
        jpd.full_name = ports[i];

        auto pos = jpd.full_name.find_first_of(":");
        jpd.group = jpd.full_name.substr(0, pos);
        jpd.name = jpd.full_name.substr(pos+1);

        const char ** connections = jack_port_get_all_connections(m_jack_client, jpd.port);
        for (int j = 0; connections && connections[j]; ++j)
        {
            jpd.connections.emplace_back(connections[j]);
        }
        jack_free(connections);

        port_data.emplace_back(jpd);
    }
    jack_free(ports);
    return port_data;
}

const JackPortState::jpd_container &JackPortState::get_ins() const
{
    return m_ins;
}

const JackPortState::jpd_container & JackPortState::get_outs() const
{
    return m_outs;
}

const JackPortState::JackPortData & JackPortState::get(std::string name) const
{
    auto pd = get_by_name(m_ins, name);
    if (pd == m_ins.end())
    {
        pd = get_by_name(m_outs, name);
        if (pd == m_outs.end())
            throw std::runtime_error("no such port: " + name);
    }
    return *pd;
}

/*
JackPortState::JackPortData &JackPortState::get(jack_port_t *port)
{
    auto pd = get_by_port(m_ins, port);
    if (pd == m_ins.end())
    {
        pd = get_by_port(m_outs, port);
        if (pd == m_outs.end())
            throw std::runtime_error("no such port");
    }
    return *pd;
}
*/

void JackPortState::port_reg(jack_port_id_t, int, void *arg)
{
    // TODO:
    static_cast<JackPortState*>(arg)->update();
}

void JackPortState::port_rename(jack_port_id_t port, const char *old_name, const char *new_name, void *arg)
{
    // TODO:
    /*
    auto * jps = static_cast<JackPortState*>(arg);
    auto & jpd = jps->get(jack_port_by_id(jps->m_jack_client, port));
    jpd.name = new_name;
    */
    static_cast<JackPortState*>(arg)->update();
}

void JackPortState::port_conn(jack_port_id_t a, jack_port_id_t b, int connect, void *arg)
{
    // TODO:
    static_cast<JackPortState*>(arg)->update();
}

JackPortState::jpd_container::const_iterator JackPortState::get_by_name(const JackPortState::jpd_container &c, std::string full_name) const
{
    return std::find_if(c.cbegin(), c.cend(),
                                  [&](const JackPortData & d){
        return d.full_name == full_name;
    });
}

JackPortState::jpd_container::const_iterator JackPortState::get_by_port(const JackPortState::jpd_container &c, jack_port_t *port) const
{
    return std::find_if(c.cbegin(), c.cend(),
                                  [&](const JackPortData & d){
        return d.port == port;
    });
}

