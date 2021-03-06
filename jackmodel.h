#ifndef JACKMODEL_H
#define JACKMODEL_H

#include <QAbstractTableModel>

#include "jackportstate.h"

class JackModel : public QAbstractTableModel
{
public:
    JackModel(QObject *parent = nullptr);
    ~JackModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    JackPortState m_jps;

private slots:
    void before_update();
    void update();
};

#endif // JACKMODEL_H
