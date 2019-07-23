#include "booleaneditor.h"

#include <QEvent>
#include <QPainter>

BooleanEditor::BooleanEditor(QWidget *parent)
    : QItemDelegate(parent)
{

}

void BooleanEditor::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    const int size = qMin(option.rect.width(), option.rect.height());
    const int brightness = index.model()->data(index, Qt::DisplayRole).toInt();
    const double radius = (size / 2.0) - (brightness * size / 2.0);
    if (qFuzzyIsNull(radius))
        return;

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    if (option.state & QStyle::State_Selected)
        painter->setBrush(option.palette.highlightedText());
    else
        painter->setBrush(option.palette.text());

    painter->drawEllipse(QRectF(option.rect.x() + option.rect.width() / 2 - radius,
                                option.rect.y() + option.rect.height() / 2 - radius,
                                2 * radius, 2 * radius));
    painter->restore();
}

bool BooleanEditor::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease){
        model->setData(index, !model->data(index).toBool());
        event->accept();
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

QSize BooleanEditor::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(15, 15);
}
