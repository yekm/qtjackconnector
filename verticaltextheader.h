#ifndef VERTICALTEXTHEADER_H
#define VERTICALTEXTHEADER_H

#include <QHeaderView>

// https://stackoverflow.com/a/30634217

class VerticalTextHeader : public QHeaderView
{
public:
    VerticalTextHeader(QWidget *parent = nullptr);

    QSize sizeHint() const override;
protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    QSize sectionSizeFromContents(int logicalIndex) const override;

private:
    QString get_data(int index) const;
    QFont m_font;
    QFontMetrics m_metrics;
};

#endif // VERTICALTEXTHEADER_H
