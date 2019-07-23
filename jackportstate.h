#ifndef JACKPORTSTATE_H
#define JACKPORTSTATE_H

#include <jack/jack.h>

#include <string>
#include <deque>
#include <map>
#include <algorithm>

#include <QThread>
#include <QMutex>

class JackPortState : public QThread
{
    Q_OBJECT

public:
    struct JackPortData
    {
        jack_port_t * port;
        std::string name, group, full_name;
        std::deque<std::string> connections;
        bool is_connected(JackPortData port) const
        {
            return is_connected(port.full_name);
        }
        bool is_connected(std::string port) const
        {
            return std::find_if(connections.begin(), connections.end(),
                                [&](std::string s)
            {
                return s == port;
            }) != connections.end();
        }
        //auto operator<=>(const JackPortData&) const = default;
        bool operator==(const JackPortData& o) const
        {
            return port == o.port &&
                    full_name == o.full_name &&
                    connections == o.connections;
        }
    };
    // typedef std::map<std::string, std::deque<std::string>> connection_groups;
    typedef std::deque<JackPortData> jpd_container;

    JackPortState(QObject *parent = nullptr);
    ~JackPortState() override;
    bool update();

    jpd_container get_port_data(JackPortFlags flags) const;

    const jpd_container & get_ins() const;
    const jpd_container & get_outs() const;

    const JackPortData &get(std::string name) const;
    //JackPortData &get(jack_port_t * port);

    static void port_reg(jack_port_id_t, int, void *arg);
    static void port_rename(jack_port_id_t port, const char *old_name, const char *new_name, void *arg);
    static void port_conn(jack_port_id_t a, jack_port_id_t b, int connect, void *arg);

signals:
    void about_to_update_connections();
    void connections_updated();

private:

    void run() override;

    QMutex m_mutex;

    jpd_container::const_iterator get_by_name(const jpd_container & c, std::string full_name) const;
    jpd_container::const_iterator get_by_port(const jpd_container & c, jack_port_t * port) const;

    jack_client_t *m_jack_client;
    jpd_container m_ins, m_outs;
};

#endif // JACKPORTSTATE_H
