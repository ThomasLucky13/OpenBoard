#include "UBGraphicsVectorItem.h"

#include "frameworks/UBGeometryUtils.h"
#include "UBGraphicsScene.h"
#include "core/memcheck.h"
#include <iostream>

UBGraphicsVectorItem::UBGraphicsVectorItem (QGraphicsItem * parent)
    : QGraphicsLineItem(parent)
    , mHasAlpha(false)
    , mOriginalWidth(-1)
    , mIsNominalLine(false)
{
    // NOOP
    initialize();
}

UBGraphicsVectorItem::UBGraphicsVectorItem (const QLineF & line, QGraphicsItem * parent)
    : QGraphicsLineItem(line, parent)
    , mOriginalWidth(-1)
    , mIsNominalLine(false)
{
    // NOOP
    initialize();
}


UBGraphicsVectorItem::UBGraphicsVectorItem (const QLineF& pLine, qreal pWidth)
    : QGraphicsLineItem(pLine)
    , mOriginalLine(pLine)
    , mOriginalWidth(pWidth)
    , mIsNominalLine(true)
{
    // NOOP
    initialize();
}

UBGraphicsVectorItem::UBGraphicsVectorItem (const QLineF& pLine, qreal pStartWidth, qreal pEndWidth)
    : QGraphicsLineItem(pLine)
    , mOriginalLine(pLine)
    , mOriginalWidth(pEndWidth)
    , mIsNominalLine(true)
{
    // NOOP
    initialize();
}


void UBGraphicsVectorItem::initialize()
{
    //setData(UBGraphicsItemData::itemLayerType, QVariant(itemLayerType::DrawingItem)); //Necessary to set if we want z value to be assigned correctly
    setDelegate(new UBGraphicsItemDelegate(this, 0, GF_COMMON
                                           | GF_RESPECT_RATIO
                                           | GF_REVOLVABLE
                                           | GF_FLIPPABLE_ALL_AXIS));
    setUuid(QUuid::createUuid());
    setData(UBGraphicsItemData::itemLayerType, QVariant(itemLayerType::ObjectItem)); //Necessary to set if we want z value to be assigned correctly
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setSublines();

    mStrokeGroup = new UBGraphicsStrokesGroup();
    mStrokeGroup->setTransform(this->transform());
    UBGraphicsItem::assignZValue(mStrokeGroup, this->zValue());

    if(this->transform().isIdentity())
        this->setTransform(mStrokeGroup->transform());

    mStrokeGroup->addToGroup(this);
}

void UBGraphicsVectorItem::setSublines()
{
    for(int i = 0; i < sublines.length(); ++i)
    {
        delete sublines[i];
    }
    if ((style == UBVectorStyle::To) || (style == UBVectorStyle::FromTo))
    {
        QLineF thisLine = QLineF(this->line().p2().x(), this->line().p2().y(),this->line().p2().x()+this->line().length()/10, this->line().p2().y());
        thisLine.setAngle(this->line().angle() - 135);
        sublines.push_back(new QGraphicsLineItem(thisLine));

        thisLine = QLineF(this->line().p2().x(), this->line().p2().y(),this->line().p2().x()+this->line().length()/10, this->line().p2().y());
        thisLine.setAngle(this->line().angle() + 135);
        sublines.push_back(new QGraphicsLineItem(thisLine));
    }

    if ((style == UBVectorStyle::From) || (style == UBVectorStyle::FromTo))
    {
        QLineF thisLine = QLineF(this->line().p1().x(), this->line().p1().y(),this->line().p1().x()+this->line().length()/10, this->line().p1().y());
        thisLine.setAngle(this->line().angle() - 45);
        sublines.push_back(new QGraphicsLineItem(thisLine));

        thisLine = QLineF(this->line().p1().x(), this->line().p1().y(),this->line().p1().x()+this->line().length()/10, this->line().p1().y());
        thisLine.setAngle(this->line().angle() + 45);
        sublines.push_back(new QGraphicsLineItem(thisLine));
    }

}

void UBGraphicsVectorItem::setUuid(const QUuid &pUuid)
{
    UBItem::setUuid(pUuid);
    setData(UBGraphicsItemData::ItemUuid, QVariant(pUuid)); //store item uuid inside the QGraphicsItem to fast operations with Items on the scene
}

UBGraphicsVectorItem::~UBGraphicsVectorItem()
{
    for (int i = 0; i < sublines.size(); ++i)
        delete sublines[i];
    sublines.clear();
}

void UBGraphicsVectorItem::setColor(const QColor& pColor)
{
    QPen pen = QPen(pColor);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setWidth(mOriginalWidth);
    QGraphicsLineItem::setPen(pen);
    for (int i = 0; i < sublines.size(); ++i)
        sublines[i]->setPen(pen);
    mHasAlpha = (pColor.alphaF() < 1.0);
}


QColor UBGraphicsVectorItem::color() const
{
    return QGraphicsLineItem::pen().color();
}

void UBGraphicsVectorItem::setStyle(const UBVectorStyle::Enum& _style)
{
    style = _style;
    setSublines();
    for (int i = 0; i < sublines.size(); ++i)
        sublines[i]->setPen(this->pen());
}

QList<QPointF> UBGraphicsVectorItem::linePoints()
{
    QList<QPointF> points = QList<QPointF>();
    qreal incr = 1/line().length();
    if (incr<0) incr*=-1;
    if (incr>0)
    {
       for (qreal t = 0; t <= 1; t+=incr)
       {
           points.push_back(line().pointAt(t));
       }
    }
    return points;
}

UBItem* UBGraphicsVectorItem::deepCopy() const
{
    UBGraphicsVectorItem* copy = new UBGraphicsVectorItem(line());
    copyItemParameters(copy);
    return copy;
}


void UBGraphicsVectorItem::copyItemParameters(UBItem *copy) const
{
    UBGraphicsVectorItem *cp = dynamic_cast<UBGraphicsVectorItem*>(copy);
    if (cp)
    {
        cp->mOriginalLine = this->mOriginalLine;
        cp->mOriginalWidth = this->mOriginalWidth;
        cp->mIsNominalLine = this->mIsNominalLine;

        cp->setTransform(transform());
        cp->setPos(pos());
        cp->setPen(this->pen());
        cp->mHasAlpha = this->mHasAlpha;

        cp->setColorOnDarkBackground(this->colorOnDarkBackground());
        cp->setColorOnLightBackground(this->colorOnLightBackground());

        cp->setFlag(QGraphicsItem::ItemIsMovable, true);
        cp->setFlag(QGraphicsItem::ItemIsSelectable, true);
        cp->setZValue(this->zValue());
        cp->setData(UBGraphicsItemData::ItemLayerType, this->data(UBGraphicsItemData::ItemLayerType));
        cp->setData(UBGraphicsItemData::ItemLocked, this->data(UBGraphicsItemData::ItemLocked));
    }
}

void UBGraphicsVectorItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    QStyleOptionGraphicsItem styleOption = QStyleOptionGraphicsItem(*option);
    if(mHasAlpha && scene() && scene()->isLightBackground())
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter->setRenderHints(QPainter::Antialiasing);

    QGraphicsLineItem::paint(painter, option, widget);
    for (int i = 0; i <sublines.size(); ++i)
    {
        sublines[i]->paint(painter, option, widget);
    }
    Delegate()->postpaint(painter, &styleOption, widget);
}

UBGraphicsScene* UBGraphicsVectorItem::scene()
{
    return qobject_cast<UBGraphicsScene*>(QGraphicsLineItem::scene());
}

void UBGraphicsVectorItem::SetDelegate()
{
    Delegate()->createControls();
}

QVariant UBGraphicsVectorItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    QVariant newValue = Delegate()->itemChange(change, value);
        UBGraphicsItem *item = dynamic_cast<UBGraphicsItem*>(this);
        if (item)
        {
            item->Delegate()->positionHandles();
        }

    return QGraphicsItem::itemChange(change, newValue);
}
