#include "jackmodel.h"

#include <QDebug>
#include <QSize>

JackModel::JackModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_jps(this)
{
    connect(&m_jps, &JackPortState::about_to_update_connections,
            this, &JackModel::before_update);
    connect(&m_jps, &JackPortState::connections_updated,
            this, &JackModel::update);
    m_jps.start();
}

JackModel::~JackModel()
{
    m_jps.requestInterruption();
    m_jps.wait();
}

int JackModel::rowCount(const QModelIndex &) const
{
    return m_jps.get_ins().size();
}

int JackModel::columnCount(const QModelIndex &) const
{
    return m_jps.get_outs().size();
}

bool JackModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    emit dataChanged(index, index);
    return true;
}

QVariant JackModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    bool c = m_jps.get_ins().at(index.row()).is_connected(m_jps.get_outs().at(index.column()));
    return qVariantFromValue(!c);
}

QVariant JackModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Orientation::Horizontal)
            return QString::fromStdString(m_jps.get_outs().at(section).name);
        else
            return QString::fromStdString(m_jps.get_ins().at(section).name);
    }
    return QVariant();
}

void JackModel::before_update()
{
    emit layoutAboutToBeChanged();
    beginResetModel();
}

void JackModel::update()
{
    emit layoutChanged();
    endResetModel();
}

