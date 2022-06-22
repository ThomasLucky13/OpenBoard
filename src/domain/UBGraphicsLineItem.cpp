#include "UBGraphicsLineItem.h"

#include "frameworks/UBGeometryUtils.h"
#include "UBGraphicsScene.h"
#include "domain/UBGraphicsPolygonItem.h"
#include "domain/UBGraphicsStroke.h"

#include "core/memcheck.h"

#include <iostream>

UBGraphicsLineItem::UBGraphicsLineItem (QGraphicsItem * parent)
    : QGraphicsLineItem(parent)
    , mHasAlpha(false)
    , mOriginalWidth(-1)
    , mIsNominalLine(false)
    , mStroke(0)
    , mpGroup(NULL)
{
    // NOOP
    initialize();
}


UBGraphicsLineItem::UBGraphicsLineItem (const QLineF & line, QGraphicsItem * parent)
    : QGraphicsLineItem(line, parent)
    , mOriginalWidth(-1)
    , mIsNominalLine(false)
    , mStroke(0)
    , mpGroup(NULL)
{
    // NOOP
    initialize();
}


UBGraphicsLineItem::UBGraphicsLineItem (const QLineF& pLine, qreal pWidth)
    : QGraphicsLineItem(pLine)
    , mOriginalLine(pLine)
    , mOriginalWidth(pWidth)
    , mIsNominalLine(true)
    , mStroke(0)
    , mpGroup(NULL)
{
    // NOOP
    initialize();
}

UBGraphicsLineItem::UBGraphicsLineItem (const QLineF& pLine, qreal pStartWidth, qreal pEndWidth)
    : QGraphicsLineItem(pLine)
    , mOriginalLine(pLine)
    , mOriginalWidth(pEndWidth)
    , mIsNominalLine(true)
    , mStroke(0)
    , mpGroup(NULL)
{
    // NOOP
    initialize();
}


void UBGraphicsLineItem::initialize()
{
    setData(UBGraphicsItemData::itemLayerType, QVariant(itemLayerType::DrawingItem)); //Necessary to set if we want z value to be assigned correctly
    setUuid(QUuid::createUuid());
}

void UBGraphicsLineItem::setUuid(const QUuid &pUuid)
{
    UBItem::setUuid(pUuid);
    setData(UBGraphicsItemData::ItemUuid, QVariant(pUuid)); //store item uuid inside the QGraphicsItem to fast operations with Items on the scene
}

void UBGraphicsLineItem::clearStroke()
{
    if (mStroke!=NULL)
    {
        //mStroke->remove(this);
        if (mStroke->polygons().empty())
            delete mStroke;
        mStroke = NULL;
    }
}

UBGraphicsLineItem::~UBGraphicsLineItem()
{
    //clearStroke();
}

void UBGraphicsLineItem::setStrokesGroup(UBGraphicsStrokesGroup *group)
{
    mpGroup = group;
}

void UBGraphicsLineItem::setStroke(UBGraphicsStroke* stroke)
{
    if (stroke) {
        clearStroke();

        mStroke = stroke;
        //mStroke->addPolygon(this);
    }
}

UBGraphicsStroke* UBGraphicsLineItem::stroke() const
{
    return mStroke;
}


void UBGraphicsLineItem::setColor(const QColor& pColor)
{
    QPen pen = QPen(pColor);
    pen.setStyle(style());
    if (style()==Qt::PenStyle::DotLine)
    {
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    }
    pen.setWidth(mOriginalWidth);
    QGraphicsLineItem::setPen(pen);

    mHasAlpha = (pColor.alphaF() < 1.0);
}


QColor UBGraphicsLineItem::color() const
{
    return QGraphicsLineItem::pen().color();
}

void UBGraphicsLineItem::setStyle(const Qt::PenStyle& style)
{
    QPen pen = QPen(color());
    pen.setStyle(style);
    if (style==Qt::PenStyle::DotLine)
    {
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    }
    pen.setWidth(mOriginalWidth);
    QGraphicsLineItem::setPen(pen);
}

Qt::PenStyle UBGraphicsLineItem::style() const
{
    return QGraphicsLineItem::pen().style();
}


UBItem* UBGraphicsLineItem::deepCopy() const
{
    UBGraphicsLineItem* copy = new UBGraphicsLineItem(line());
    copyItemParameters(copy);
    return copy;
}


void UBGraphicsLineItem::copyItemParameters(UBItem *copy) const
{
    UBGraphicsLineItem *cp = dynamic_cast<UBGraphicsLineItem*>(copy);
    if (cp)
    {
        cp->mOriginalLine = this->mOriginalLine;
        cp->mOriginalWidth = this->mOriginalWidth;
        cp->mIsNominalLine = this->mIsNominalLine;

        cp->setTransform(transform());
        cp->setPen(this->pen());
        cp->mHasAlpha = this->mHasAlpha;

        cp->setColorOnDarkBackground(this->colorOnDarkBackground());
        cp->setColorOnLightBackground(this->colorOnLightBackground());

        cp->setZValue(this->zValue());
        cp->setData(UBGraphicsItemData::ItemLayerType, this->data(UBGraphicsItemData::ItemLayerType));
    }
}

void UBGraphicsLineItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    if(mHasAlpha && scene() && scene()->isLightBackground())
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter->setRenderHints(QPainter::Antialiasing);

    QGraphicsLineItem::paint(painter, option, widget);
}

UBGraphicsScene* UBGraphicsLineItem::scene()
{
    return qobject_cast<UBGraphicsScene*>(QGraphicsLineItem::scene());
}
