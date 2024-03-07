#include "UBMultiDrawWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

UBMultiDrawWidget::UBMultiDrawWidget(QList<QLineF>* linesList, QWidget* parent): QWidget{parent}
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    QVBoxLayout* vLayout = new QVBoxLayout();
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addSpacerItem(new QSpacerItem(24, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
    QPushButton* exitButton = new QPushButton("exit");
    connect(exitButton, &QPushButton::clicked, [this](){
        emit windowTitleChanged("");
    });
    hLayout->addWidget(exitButton);
    vLayout->addLayout(hLayout);
    vLayout->addSpacerItem(new QSpacerItem(0, 24, QSizePolicy::Preferred, QSizePolicy::Expanding));
    setLayout(vLayout);

    pix = new QPixmap();
    penWidth = 1;
    pen = QPen(Qt::black, 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen.setColor(QColor(0, 0, 0));
    cp = QDesktopWidget().availableGeometry();

    setAttribute(Qt::WA_AcceptTouchEvents, true);
    pix = new QPixmap(cp.width(), cp.height());
    pix->fill(Qt::white);
    pp = new QPainter(pix);
    pp->setPen(pen);

    lines = linesList;
}

UBMultiDrawWidget::~UBMultiDrawWidget()
{
    // NOOP
}

UBItem* UBMultiDrawWidget::deepCopy() const
{
   UBMultiDrawWidget* copy = new UBMultiDrawWidget(lines, dynamic_cast<QWidget*>(parent()));

    copyItemParameters(copy);

   // TODO UB 4.7 ... complete all members ?

   return copy;
}

void UBMultiDrawWidget::copyItemParameters(UBItem *copy) const
{
    UBMultiDrawWidget *cp = dynamic_cast<UBMultiDrawWidget*>(copy);
    if (cp)
    {
        //cp->setPos(this->pos());
        //cp->setRect(this->rect());
        //cp->setTransform(this->transform());
    }
}


QVariant UBMultiDrawWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{

    return QGraphicsRectItem::itemChange(change, value);
}

void UBMultiDrawWidget::keyPressEvent(QKeyEvent *event)
{
    QGraphicsItem::keyPressEvent(event);
    if (event->key() == Qt::Key_C)
    {
        pix->fill(Qt::white);
        QWidget::update();
    }
}

void UBMultiDrawWidget::setSizePos(QRect newRect)
{
    setGeometry(newRect);
    setPos(newRect.x(), newRect.y());
    cp = newRect;
    pix = new QPixmap(cp.width(), cp.height());
}

void UBMultiDrawWidget::paintEvent(QPaintEvent *event)
{
    //qWarning() << "开始绘制";
    int distance = sqrt(pow((lastPoint_m.x() - endPoint_m.x()),2) + pow((lastPoint_m.y() - endPoint_m.y()),2)) + 1;
    distance = sqrt(distance);
    if (distance > 6)
        distance = 6;
    else if(distance < 4)
        distance = 4;
    pen.setWidthF(penWidth);
    pp->setPen(pen);
    pp->drawLine(lastPoint_m, endPoint_m);
    QLineF line;
    line.setP1(lastPoint_m);
    line.setP2(endPoint_m);
    lines->append(line);
    lastPoint_m = endPoint_m;
    QPainter painter = QPainter(this);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *pix);
    QWidget::update();
}

void UBMultiDrawWidget::paintTouchEvent()
{
    int distance = sqrt(pow((lastPoint_m.x() - endPoint_m.x()),2) + pow((lastPoint_m.y() - endPoint_m.y()),2)) + 1;
    distance = sqrt(distance);
    if (distance > 6)
        distance = 6;
    else if(distance < 4)
        distance = 4;
    pen.setWidthF(penWidth);
    pp->setPen(pen);
    pp->drawLine(lastPoint_m, endPoint_m);
    lastPoint_m = endPoint_m;
    QPainter painter = QPainter(this);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *pix);
}


void UBMultiDrawWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        lastPoint_m = event->pos();
        endPoint_m = lastPoint_m;
    }
}

void UBMultiDrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() && Qt::LeftButton)
    {
        endPoint_m = event->pos();
        QWidget::update();
    }
}

void UBMultiDrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        endPoint_m = event->pos();
        QWidget::update();
    }
}

bool UBMultiDrawWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this)
    {
        if (event->type() == QEvent::TouchBegin)
        {

        }
        if (event->type() == QEvent::TouchUpdate || event->type() == QEvent::TouchEnd)
        {
            qWarning() << "TouchEvent";
            addline(static_cast<QTouchEvent*>(event));
            return true;
        }
        if (event->type() == QEvent::MouseButtonDblClick || event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
            if (mouse_event != nullptr && mouse_event->source() == Qt::MouseEventSynthesizedBySystem)
            {
                mouse_event->ignore();
                return true;
            }
        }

        if (event->type() == QEvent::Paint)
        {
            paintEvent(nullptr);
            return true;
        }

    }
    return QWidget::eventFilter(watched, event);
}

void UBMultiDrawWidget::addline(QTouchEvent *event)
{
    QList <QTouchEvent::TouchPoint> touchPoints = event->touchPoints();
    foreach (QTouchEvent::TouchPoint point, touchPoints)
    {
        qWarning() << point.ellipseDiameters().width();
        penWidth = point.ellipseDiameters().width();
        lastPoint_m = point.lastPos();
        endPoint_m = point.pos();
        paintEvent(nullptr);
    }
}

QList<QLineF>* UBMultiDrawWidget::getLines()
{
    return lines;
}
