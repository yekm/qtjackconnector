#ifndef BOOLEANEDITOR_H
#define BOOLEANEDITOR_H

#include <QStyleOptionViewItem>
#include <QItemDelegate>

class BooleanEditor : public QItemDelegate
{
    Q_OBJECT
public:
    explicit BooleanEditor(QWidget *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

signals:

public slots:
};

#endif // BOOLEANEDITOR_H
