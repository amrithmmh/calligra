/* This file is part of the KDE project
   Copyright (C) 2001-2005 David Faure <faure@kde.org>

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
   Boston, MA 02110-1301, USA.
*/

#ifndef kozoomhandler_h
#define kozoomhandler_h

#include <koguiutils_export.h>
#include <KoZoomMode.h>
#include <KoViewConverter.h>


/**
 * This class handles the zooming and DPI stuff (conversions between
 * postscript pt values and pixels). If the internal data of your
 * document does not work with postscript points (for instance raster
 * image pixels), you need to some additional converting yourself.
 *
 * An instance of KoZoomHandler operates at a given zoom (see
 * setZoomAndResolution() and setZoom()) so there is usually one
 * instance of KoZoomHandler per view.
 */
class KOGUIUTILS_EXPORT KoZoomHandler : public KoViewConverter
{


public:

    KoZoomHandler();
    virtual ~KoZoomHandler();


    /**
     * Change the zoom factor to @p z (e.g. 150 for 150%)
     * and/or change the resolution of the output device, given in DPI.
     * This is done on startup, when zooming, and when printing.
     * The same call combines both so that all the updating done behind
     * the scenes is done only once, even if both zoom and DPI must be changed.
     */
    virtual void setZoomAndResolution( int zoom, int dpiX, int dpiY );

    /**
     * @return the conversion factor between pt and pixel, that
     * takes care of the zoom and the DPI setting.
     * Use zoomIt(pt) instead, though.
     */
    double zoomedResolutionX() const { return m_zoomedResolutionX; }
    double zoomedResolutionY() const { return m_zoomedResolutionY; }

    double resolutionX() const { return m_resolutionX; }
    double resolutionY() const { return m_resolutionY; }

    /**
     * Zoom factor for X. Equivalent to zoomedResolutionX()/resolutionX()
     */
    double zoomFactorX() const { return m_zoomedResolutionX / m_resolutionX; }
    /**
     * Zoom factor for Y. Equivalent to zoomedResolutionY()/resolutionY()
     */
    double zoomFactorY() const { return m_zoomedResolutionY / m_resolutionY; }

    /**
     * Set a resolution for X and Y of the output device.
     * The zoom factor is not changed.
     */
    void setResolution( double resolutionX, double resolutionY );

    /**
     * Set the resolution for X and Y to the display values reported by KGlobal.
     * The zoom factor is not changed.
     */
    void setResolutionToStandard( );

    /**
     * Set the zoomed resolution for X and Y.
     * Compared to the setZoom... methods, this allows to set a different
     * zoom factor for X and for Y.
     */
    virtual void setZoomedResolution( double zoomedResolutionX, double zoomedResolutionY );

    /**
     * Change the zoom level, keeping the resolution unchanged.
     * @param zoom the zoom factor (e.g. 100 for 100%)
     * \deprecated
     */
    void setZoom( int zoom );
    /**
     * Change the zoom level, keeping the resolution unchanged.
     * @param zoom the zoom factor (e.g. 1.0 for 100%)
     */
    void setZoom( double zoom );

    /**
     * Change the zoom mode
     * @param zoomMode the zoom mode.
     */
    void setZoomMode( KoZoomMode::Mode zoomMode ) { m_zoomMode = zoomMode; }
    /**
     * @return the global zoom factor (e.g. 100 for 100%).
     * Only use this to display to the user, don't use in calculations
     */
    int zoomInPercent() const { return qRound(m_zoom * 100); }
    /**
     * @return the global zoom mode (e.g. KoZoomMode::ZOOM_WIDTH).
     * use this to determine how to zoom
     */
    KoZoomMode::Mode zoomMode() const { return m_zoomMode; }

    // Input: pt. Output: pixels. Resolution and zoom are applied.
    KDE_DEPRECATED int zoomItXOld( double z ) const
        {
            return qRound( m_zoomedResolutionX * z );
        }

    KDE_DEPRECATED int zoomItYOld( double z ) const
        {
            return qRound( m_zoomedResolutionY * z );
        }

    double zoomItX( double z ) const
        {
            return m_zoomedResolutionX * z;
        }

    double zoomItY( double z ) const
        {
            return m_zoomedResolutionY * z ;
        }

    KDE_DEPRECATED QPoint zoomPointOld( const QPointF & p ) const;

    KDE_DEPRECATED QRect zoomRectOld( const QRectF & r ) const;

    /**
     * Returns the size in pixels for a input size in points.
     *
     * This function can return a size with 1 pixel to less, depending
     * on the reference point and the width and/or the zoom level.
     * It's save to use if the starting point is (0/0).
     * You can use it if you don't know the starting point yet
     * (like when inserting a picture), but then please take
     * care of it afterwards, when you know the reference point.
     */
    KDE_DEPRECATED QSize zoomSizeOld( const QSizeF & s ) const;

    // Input: pixels. Output: pt.
    KDE_DEPRECATED double unzoomItXOld( int x ) const
        {
            return static_cast<double>( x ) / m_zoomedResolutionX;
        }

    KDE_DEPRECATED double unzoomItYOld( int y ) const
        {
            return static_cast<double>( y ) / m_zoomedResolutionY;
        }

    double unzoomItX( double x ) const
        {
            return  x / m_zoomedResolutionX;
        }

    double unzoomItY( double y ) const
        {
            return  y / m_zoomedResolutionY;
        }

    KDE_DEPRECATED QPointF unzoomPointOldF( const QPoint & p ) const;

    KDE_DEPRECATED QRectF unzoomRectOldF (const QRect & r ) const;

    // KoViewConverter-interface methods

    /**
     * Convert a coordinate in pt to pixels.
     * @param documentPoint the point in the document coordinate system of a KoShape.
     */
    QPointF documentToView( const QPointF &documentPoint ) const;

    /**
     * Convert a coordinate in pixels to pt.
     * @param viewPoint the point in the coordinate system of the widget, or window.
     */
    QPointF viewToDocument( const QPointF &viewPoint ) const;

    /**
     * Convert a rectangle in pt to pixels.
     * @param documentRect the rect in the document coordinate system of a KoShape.
     */
    QRectF documentToView( const QRectF &documentRect ) const;

    /**
     * Convert a rectangle in pixels to pt.
     * @param viewRect the rect in the coordinate system of the widget, or window.
     */
    QRectF viewToDocument( const QRectF &viewRect ) const;

    /**
     * Convert a size in pt to pixels.
     * @param documentSize the size in pt.
     * @return the size in pixels.
     */
    virtual QSizeF documentToView( const QSizeF& documentSize ) const;

    /**
     * Convert a size in pixels to pt.
     * @param viewSize the size in pixels.
     * @return the size in pt.
     */
    virtual QSizeF viewToDocument( const QSizeF& viewSize ) const;

    /**
     * Convert a single x coordinate in pt to pixels.
     * @param documentX the x coordinate in pt.
     * @return the x coordinate in pixels.
     */
    virtual double documentToViewX( double documentX ) const;

    /**
     * Convert a single y coordinate in pt to pixels.
     * @param documentY the y coordinate in pt.
     * @return the y coordinate in pixels.
     */
    virtual double documentToViewY( double documentY ) const;

    /**
     * Convert a single x coordinate in pixels to pt.
     * @param viewX the x coordinate in pixels.
     * @return the x coordinate in pt.
     */
    virtual double viewToDocumentX( double viewX ) const;

    /**
     * Convert a single y coordinate in pixels to pt.
     * @param viewY the y coordinate in pixels.
     * @return the y coordinate in pt.
     */
    virtual double viewToDocumentY( double viewY ) const;

    /**
     * Get the zoom levels of the individual x and y axis. Copy them to the pointer parameters.
     * @param zoomX a pointer to a double which will be modified to set the horizontal zoom.
     * @param zoomY a pointer to a double which will be modified to set the vertical zoom.
     */
    void zoom(double *zoomX, double *zoomY) const;




protected:
    double m_zoom;
    KoZoomMode::Mode m_zoomMode;

    double m_resolutionX;
    double m_resolutionY;
    double m_zoomedResolutionX;
    double m_zoomedResolutionY;

};

#endif
