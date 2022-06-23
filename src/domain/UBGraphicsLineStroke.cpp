/*
 * Copyright (C) 2015-2022 Département de l'Instruction Publique (DIP-SEM)
 *
 * Copyright (C) 2013 Open Education Foundation
 *
 * Copyright (C) 2010-2013 Groupement d'Intérêt Public pour
 * l'Education Numérique en Afrique (GIP ENA)
 *
 * This file is part of OpenBoard.
 *
 * OpenBoard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * OpenBoard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenBoard. If not, see <http://www.gnu.org/licenses/>.
 */




#include "UBGraphicsLineStroke.h"

#include "UBGraphicsLineItem.h"

#include "board/UBBoardController.h"
#include "core/UBApplication.h"
#include "core/memcheck.h"
#include "domain/UBGraphicsScene.h"

#include "frameworks/UBGeometryUtils.h"

UBGraphicsLineStroke::UBGraphicsLineStroke(UBGraphicsScene *scene)
    :mScene(scene)
{
    mAntiScaleRatio = 1./(UBApplication::boardController->systemScaleFactor() * UBApplication::boardController->currentZoom());
}


UBGraphicsLineStroke::~UBGraphicsLineStroke()
{
    foreach(UBGraphicsLineItem* line, mLines)
        line->setStroke(NULL);

    mLines.clear();
}


void UBGraphicsLineStroke::addLine(UBGraphicsLineItem* line)
{
    remove(line);
    mLines << line;
}

void UBGraphicsLineStroke::remove(UBGraphicsLineItem* lineItem)
{
    int n = mLines.indexOf(lineItem);
    if (n>=0)
        mLines.removeAt(n);
}

QList<UBGraphicsLineItem*> UBGraphicsLineStroke::lines() const
{
    return mLines;
}


bool UBGraphicsLineStroke::hasPressure()
{
    if (mLines.count() > 2)
    {
        qreal nominalWidth = mLines.at(0)->originalWidth();

        foreach(UBGraphicsLineItem* line, mLines)
        {
            if (!line->isNominalLine() || line->originalWidth() != nominalWidth)
                return true;
        }
        return false;
    }
    else
        return true;
}


UBGraphicsLineStroke* UBGraphicsLineStroke::deepCopy()
{
    UBGraphicsLineStroke* clone = new UBGraphicsLineStroke();

    return clone;
}

bool UBGraphicsLineStroke::hasAlpha() const
{
    if (mLines.length() > 0 && mLines.at(0)->color().alphaF() != 1.0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void UBGraphicsLineStroke::clear()
{
    if(!mLines.empty()){
        mLines.clear();
    }
}
