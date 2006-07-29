/* This file is part of the KDE project
 Copyright (C) 2003-2004 Alexander Dymo <adymo@mksat.net>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/
#ifndef REPORTFOOTER_H
#define REPORTFOOTER_H

#include "commdefs.h"
#include "band.h"

namespace Kudesigner
{

class ReportFooter: public Band
{
public:
    ReportFooter( int x, int y, int width, int height, Canvas *canvas );

    virtual int rtti() const
    {
        return Rtti_ReportFooter;
    }
    virtual void draw( QPainter &painter );
    virtual QString getXml();
};

}

#endif
