#ifndef UBGRAPHICSVECTORITEM_H
#define UBGRAPHICSVECTORITEM_H

#include <QtGui>
#include "core/UB.h"
#include "domain/UBGraphicsStrokesGroup.h"
#include "UBItem.h"

class UBItem;
class UBGraphicsScene;

class UBGraphicsVectorItem: public QGraphicsLineItem, public UBItem, public UBGraphicsItem
{

public:
    UBGraphicsVectorItem();
public:

      UBGraphicsVectorItem(QGraphicsItem * parent = 0 );
      UBGraphicsVectorItem(const QLineF& line, qreal pWidth);
      UBGraphicsVectorItem(const QLineF& pLine, qreal pStartWidth, qreal pEndWidth);
      UBGraphicsVectorItem(const QLineF & line, QGraphicsItem * parent = 0);

      ~UBGraphicsVectorItem();

      void initialize();

      void setUuid(const QUuid &pUuid);

      void setColor(const QColor& color);
      void setStyle(const UBVectorStyle::Enum& _style);
      UBVectorStyle::Enum getStyle()
      {
          return style;
      }

      QColor color() const;

      virtual UBGraphicsScene* scene();

      enum { Type = UBGraphicsItemType::VectorItemType };

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

      QList<QPointF> linePoints();

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

      void SetDelegate();

      QList<QGraphicsLineItem*> SubLines()
      {
          return sublines;
      }

      UBGraphicsStrokesGroup* StrokeGroup()
      {
          return mStrokeGroup;
      }

  protected:
      void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
      virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
      void setSublines();

  private:

      bool mHasAlpha;

      QLineF mOriginalLine;
      qreal mOriginalWidth;
      bool mIsNominalLine;

      QColor mColorOnDarkBackground;
      QColor mColorOnLightBackground;
      QList<QGraphicsLineItem *> sublines;
      UBVectorStyle::Enum style;

      UBGraphicsStrokesGroup* mStrokeGroup;
};

#endif // UBGRAPHICSVECTORITEM_H
