#ifndef UBMULTIDRAWWIDGET_H
#define UBMULTIDRAWWIDGET_H

#include <QWidget>
#include <QtCore>

#include "core/UB.h"
#include "domain/UBItem.h"

class UBMultiDrawWidget : public UBItem, public QWidget, public QGraphicsRectItem
{
public:
    UBMultiDrawWidget(QList<QLineF>* linesList, qreal width, QColor color, QColor bgColor, QWidget* parent) ;
    virtual ~UBMultiDrawWidget();
    enum { Type = UBGraphicsItemType::MultiDrawItemType };

    virtual int type() const
    {
        return Type;
    }

    virtual UBItem* deepCopy() const;

    virtual void copyItemParameters(UBItem *copy) const;
    void setSizePos(QRect newRect);
    QList<QLineF>* getLines();

signals:
    void endDraw();
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    // Events
    virtual void    keyPressEvent(QKeyEvent *event);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;
    void addline(QTouchEvent *event);
    void paintTouchEvent();

private:
    QPointF lastPoint_t, endPoint_t, lastPoint_m, endPoint_m;
    QPixmap* pix;
    qreal penWidth;
    QPen pen;
    QRect cp;
    QPainter* pp;
    QList<QLineF>* lines;
    QColor backgroundColor;
};

#endif // UBMULTIDRAWWIDGET_H
