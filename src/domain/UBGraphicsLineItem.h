#ifndef UBGRAPHICSLINEITEM_H
#define UBGRAPHICSLINEITEM_H

#include <QtGui>
#include "core/UB.h"
#include "UBItem.h"
#include "UBGraphicsStrokesGroup.h"
#include "domain/UBGraphicsGroupContainerItem.h"

class UBItem;
class UBGraphicsScene;
class UBGraphicsLineStroke;

class UBGraphicsLineItem : public QGraphicsLineItem, public UBItem
{

    public:

        UBGraphicsLineItem(QGraphicsItem * parent = 0 );
        UBGraphicsLineItem(const QLineF& line, qreal pWidth);
        UBGraphicsLineItem(const QLineF& pLine, qreal pStartWidth, qreal pEndWidth);
        UBGraphicsLineItem(const QLineF & line, QGraphicsItem * parent = 0);

        ~UBGraphicsLineItem();

        void initialize();

        void setUuid(const QUuid &pUuid);

        void setStrokesGroup(UBGraphicsStrokesGroup* group);
        UBGraphicsStrokesGroup* strokesGroup() const{return mpGroup;}
        void setColor(const QColor& color);
        void setStyle(const Qt::PenStyle& style);

        QColor color() const;
        Qt::PenStyle style() const;

        virtual UBGraphicsScene* scene();

        inline void subtract(UBGraphicsLineItem *pi)
        {
            //if (boundingRect().intersects(pi->boundingRect()))
            //{
                //QLineF subtractedLine = line().substracted(pi->line());

                /*if (line() != subtractedLine)
                {
                    mIsNominalLine = false;
                    QGraphicsLineItem::setLine(subtractedLine);
                }*/
            //}
        }

        inline void subtractIntersecting(UBGraphicsLineItem *pi)
        {
            /*QLineF subtractedLine = line();.substracted(pi->line())
            if (line() != subtractedLine)
            {
                mIsNominalLine = false;
                QGraphicsLineItem::setLine(subtractedLine);
            }*/
        }

        enum { Type = UBGraphicsItemType::LineItemType };

        virtual int type() const
        {
            return Type;
        }

        void setLine(const QLineF pLine)
        {
            mIsNominalLine = false;
            QGraphicsLineItem::setLine(pLine);
        }

        virtual UBItem* deepCopy() const;

        virtual void copyItemParameters(UBItem *copy) const;

        QLineF originalLine() { return mOriginalLine;}
        qreal originalWidth() { return mOriginalWidth;}
        bool isNominalLine() {return mIsNominalLine;}

        void setNominalLine(bool isNominalLine) { mIsNominalLine = isNominalLine; }

        QColor colorOnDarkBackground() const
        {
            return mColorOnDarkBackground;
        }

        void setColorOnDarkBackground(QColor pColorOnDarkBackground)
        {
            mColorOnDarkBackground = pColorOnDarkBackground;
        }

        QColor colorOnLightBackground() const
        {
            return mColorOnLightBackground;
        }

        void setColorOnLightBackground(QColor pColorOnLightBackground)
        {
            mColorOnLightBackground = pColorOnLightBackground;
        }

        void setStroke(UBGraphicsLineStroke* stroke);
        UBGraphicsLineStroke* stroke() const;

    protected:
        void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);


    private:

        void clearStroke();

        bool mHasAlpha;

        QLineF mOriginalLine;
        qreal mOriginalWidth;
        bool mIsNominalLine;

        QColor mColorOnDarkBackground;
        QColor mColorOnLightBackground;

        UBGraphicsLineStroke* mStroke;
        UBGraphicsStrokesGroup* mpGroup;

};

#endif // UBGRAPHICSLINEITEM_H

