// -*- Mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4; -*-
/* This file is part of the KDE project
   Copyright (C) 1998, 1999 Reginald Stadlbauer <reggie@kde.org>

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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "kplineobject.h"
#include "kpresenter_utils.h"
#include "KPLineObjectIface.h"
#include "koPointArray.h"

#include <qpainter.h>
#include <qwmatrix.h>
#include <qdom.h>
#include <kdebug.h>
#include <kozoomhandler.h>
#include <math.h>

using namespace std;

KPLineObject::KPLineObject()
    : KPShadowObject()
{
    lineBegin = L_NORMAL;
    lineEnd = L_NORMAL;
    lineType = LT_HORZ;
}

KPLineObject::KPLineObject( const QPen &_pen, LineEnd _lineBegin,
                            LineEnd _lineEnd, LineType _lineType )
    : KPShadowObject( _pen )
{
    lineBegin = _lineBegin;
    lineEnd = _lineEnd;
    lineType = _lineType;
}

KPLineObject &KPLineObject::operator=( const KPLineObject & )
{
    return *this;
}

DCOPObject* KPLineObject::dcopObject()
{
    if ( !dcop )
        dcop = new KPLineObjectIface( this );
    return dcop;
}

QDomDocumentFragment KPLineObject::save( QDomDocument& doc, double offset )
{
    QDomDocumentFragment fragment=KPShadowObject::save(doc, offset);
    if (lineType!=LT_HORZ)
        fragment.appendChild(KPObject::createValueElement("LINETYPE", static_cast<int>(lineType), doc));
    if (lineBegin!=L_NORMAL)
        fragment.appendChild(KPObject::createValueElement("LINEBEGIN", static_cast<int>(lineBegin), doc));
    if (lineEnd!=L_NORMAL)
        fragment.appendChild(KPObject::createValueElement("LINEEND", static_cast<int>(lineEnd), doc));
    return fragment;
}

double KPLineObject::load(const QDomElement &element)
{
    double offset=KPShadowObject::load(element);
    QDomElement e=element.namedItem("LINETYPE").toElement();
    if(!e.isNull()) {
        int tmp=0;
        if(e.hasAttribute("value"))
            tmp=e.attribute("value").toInt();
        lineType=static_cast<LineType>(tmp);
    }
    e=element.namedItem("LINEBEGIN").toElement();
    if(!e.isNull()) {
        int tmp=0;
        if(e.hasAttribute("value"))
            tmp=e.attribute("value").toInt();
        lineBegin=static_cast<LineEnd>(tmp);
    }
    e=element.namedItem("LINEEND").toElement();
    if(!e.isNull()) {
        int tmp=0;
        if(e.hasAttribute("value"))
            tmp=e.attribute("value").toInt();
        lineEnd=static_cast<LineEnd>(tmp);
    }
    return offset;
}

void KPLineObject::paint( QPainter* _painter, KoZoomHandler*_zoomHandler,
                          bool /*drawingShadow*/, bool drawContour )
{
    double ow = ext.width();
    double oh = ext.height();
    int _w = pen.width();

    QPen pen2;
    if ( drawContour ) {
        pen2 = QPen( Qt::black, 1, Qt::DotLine );
        _painter->setRasterOp( Qt::NotXorROP );
    }
    else {
        pen2 = pen;
        pen2.setWidth( _zoomHandler->zoomItX( pen.width() ) );
    }
    _painter->setPen( pen2 );


    KoSize diff1( 0, 0 ), diff2( 0, 0 );

    float _angle = 0;
    switch ( lineType )
    {
    case LT_HORZ: {
        _angle = 0;
    } break;
    case LT_VERT: {
        _angle = 90 ;
    } break;
    case LT_LU_RD: {
        KoRect _rect = KoRect( orig, ext );
        KoPoint pnt1 = _rect.topLeft();
        KoPoint pnt2 = _rect.bottomRight();

        _angle = KoPoint::getAngle( pnt1, pnt2 ) - 180.0;
    } break;
    case LT_LD_RU: {
        KoRect _rect = KoRect( orig, ext );
        KoPoint pnt1 = _rect.bottomLeft();
        KoPoint pnt2 = _rect.topRight();

        _angle = KoPoint::getAngle( pnt1, pnt2 ) - 180.0;
    } break;
    }

    if ( lineBegin != L_NORMAL )
        diff1 = getOffset( lineBegin, _w, _angle + 180.0);

    if ( lineEnd != L_NORMAL )
        diff2 = getOffset( lineEnd, _w, _angle );

    switch ( lineType )
    {
    case LT_HORZ: {
        if ( lineBegin != L_NORMAL && !drawContour )
            drawFigure( lineBegin, _painter,
                        KoPoint( 0, oh / 2.0 ),
                        pen2.color(), _w, 180.0, _zoomHandler );

        if ( lineEnd != L_NORMAL && !drawContour )
            drawFigure( lineEnd, _painter,
                        KoPoint( ow , oh / 2.0),
                        pen2.color(), _w, 0.0, _zoomHandler );

        _painter->drawLine( _zoomHandler->zoomItX( - diff1.width() ),
                            _zoomHandler->zoomItY( oh / 2 ),
                            _zoomHandler->zoomItX( ow - diff2.width() ),
                            _zoomHandler->zoomItY( oh / 2) );
    } break;
    case LT_VERT: {
        if ( lineBegin != L_NORMAL && !drawContour )
            drawFigure( lineBegin, _painter,
                        KoPoint( ow / 2.0, 0 ),
                        pen2.color(), _w, 270.0, _zoomHandler );

        if ( lineEnd != L_NORMAL && !drawContour )
            drawFigure( lineEnd, _painter,
                        KoPoint( ow / 2.0, oh ),
                        pen2.color(), _w, 90.0, _zoomHandler );

        _painter->drawLine( _zoomHandler->zoomItX( ow / 2 ),
                            _zoomHandler->zoomItX( - diff1.height() ),
                            _zoomHandler->zoomItX( ow / 2 ),
                            _zoomHandler->zoomItY( oh - diff2.height() ) );
    } break;
    case LT_LU_RD: {
        if ( lineBegin != L_NORMAL && !drawContour ) {
            _painter->save();
            drawFigure( lineBegin, _painter,
                        KoPoint( 0, 0 ), pen2.color(),
                        _w, _angle + 180, _zoomHandler );
            _painter->restore();
        }
        if ( lineEnd != L_NORMAL && !drawContour ) {
            _painter->save();
            _painter->translate( _zoomHandler->zoomItX( ow ),
                                 _zoomHandler->zoomItY( oh ) );
            drawFigure( lineEnd, _painter,
                        KoPoint( 0, 0 ), pen2.color(),
                        _w, _angle, _zoomHandler );
            _painter->restore();

        }
        _painter->drawLine( _zoomHandler->zoomItX( - diff1.width() ),
                            _zoomHandler->zoomItY( - diff1.height() ),
                            _zoomHandler->zoomItX( ow - diff2.width() ),
                            _zoomHandler->zoomItY( oh - diff2.height() ) );
    } break;
    case LT_LD_RU: {
        if ( lineBegin != L_NORMAL && !drawContour ) {
            _painter->save();
            _painter->translate( _zoomHandler->zoomItX( 0 ),
                                 _zoomHandler->zoomItY( oh ) );
            drawFigure( lineBegin, _painter,
                        KoPoint( 0, 0 ), pen2.color(),
                        _w, _angle + 180,_zoomHandler );
            _painter->restore();
        }
        if ( lineEnd != L_NORMAL && !drawContour ) {
            _painter->save();
            _painter->translate( _zoomHandler->zoomItX( ow ),
                                 _zoomHandler->zoomItY( 0 ) );
            drawFigure( lineEnd, _painter,
                        KoPoint( 0, 0 ), pen2.color(),
                        _w, _angle,_zoomHandler );
            _painter->restore();
        }
        _painter->drawLine( _zoomHandler->zoomItX( - diff1.width() ),
                            _zoomHandler->zoomItY( oh - diff1.height() ),
                            _zoomHandler->zoomItX( ow - diff2.width() ),
                            _zoomHandler->zoomItY( - diff2.height() ));
    } break;
    }
}

void KPLineObject::flip( bool horizontal )
{
    KPObject::flip( horizontal );
    if ( horizontal )
    {
        if ( lineType == LT_LU_RD )
            lineType = LT_LD_RU;
        else if ( lineType == LT_LD_RU )
            lineType = LT_LU_RD;
    }
    else
    {
        if ( lineType == LT_LU_RD )
            lineType = LT_LD_RU;
        else if ( lineType == LT_LD_RU )
            lineType = LT_LU_RD;

        LineEnd tmp = lineBegin;
        lineBegin = lineEnd;
        lineEnd = tmp;
    }
}

KoSize KPLineObject::getRealSize() const {
    KoSize size( ext );

    if ( angle != 0.0 ) {
        float angInRad = angle * M_PI / 180;
        switch (lineType ) {
        case LT_HORZ: 
            size.setWidth( ext.width() * cos( angInRad ) + pen.width() * sin( angInRad ) );
            size.setHeight( ext.width() * sin( angInRad ) + pen.width() * cos( angInRad ) );
            break;
        case LT_VERT:
            size.setWidth( pen.width() * cos( angInRad ) + ext.height() * sin( angInRad ) );
            size.setHeight( pen.width() * sin( angInRad ) + ext.height() * cos( angInRad ) );
            break;
        case LT_LU_RD:
        case LT_LD_RU: 
            {
                KoPointArray points(2);
                if ( lineType == LT_LU_RD ) {  
                    points.setPoint( 0, 0, 0 );
                    points.setPoint( 1, ext.width(), ext.height() );
                }
                else {
                    points.setPoint( 0, 0, ext.height() );
                    points.setPoint( 1, ext.width(), 0 );
                }
                KoPoint realOrig(orig);
                getRealSizeAndOrigFromPoints( points, angle, size, realOrig );
            } break;
        }
    }
    else if ( lineType == LT_HORZ ) {
        size.setHeight( pen.width() );
    }
    else if ( lineType == LT_VERT ) {
        size.setWidth( pen.width() );
    }
      
    return size;
}

KoPoint KPLineObject::getRealOrig() const {
    KoPoint realOrig( orig );
    
    if ( angle != 0.0 ) { 
        KoPointArray points(2);
        switch ( lineType ) {
            case LT_HORZ:
                points.setPoint( 0, 0, ext.height() / 2.0 );
                points.setPoint( 1, ext.width(), ext.height() / 2.0 );
                break;
            case LT_VERT:
                points.setPoint( 0, ext.width() / 2.0, 0 );
                points.setPoint( 1, ext.width() / 2.0, ext.height() );
                break;
            case LT_LU_RD:
                points.setPoint( 0, 0, 0 );
                points.setPoint( 1, ext.width(), ext.height() );
                break;
            case LT_LD_RU: 
                points.setPoint( 0, 0, ext.height() );
                points.setPoint( 1, ext.width(), 0 );
                break;
        }
        KoSize size( ext );
        getRealSizeAndOrigFromPoints( points, angle, size, realOrig );
    }

    return realOrig;
}
