#include "verticaltextheader.h"


#include <QPainter>

VerticalTextHeader::VerticalTextHeader(QWidget *parent)
    : QHeaderView(Qt::Orientation::Horizontal, parent)
    , m_font("helvetica", 10)
    , m_metrics(m_font)
{

}

QSize VerticalTextHeader::sizeHint() const
{
    auto cc = model()->columnCount();
    int max_w = 0;
    for (int i=0; i<cc; ++i)
    {
        auto m = m_metrics.horizontalAdvance(get_data(i));
        if (m > max_w)
            max_w = m;
    }
    return QSize(15, max_w + 20);
}

void VerticalTextHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    auto data = get_data(logicalIndex);
    painter->rotate(-90);
    painter->setFont(m_font);
    auto descent = m_metrics.descent();
    painter->drawText(- rect.height() + 10,
                      rect.left() + (rect.width() + descent) / 2,
                      data);
}

QSize VerticalTextHeader::sectionSizeFromContents(int logicalIndex) const
{
    return QSize(15, 0);
}

QString VerticalTextHeader::get_data(int index) const
{
    return model()->headerData(index, orientation()).toString();
}
