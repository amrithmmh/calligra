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

#ifndef kpobject_h
#define kpobject_h

#include <qpoint.h>
#include <qpen.h>
#include <qbrush.h>
#include <qcursor.h>

#include <global.h>

#include <kpgradient.h>
#include <klocale.h>
#include <koPoint.h>
#include <koSize.h>
#include <koRect.h>

class QPainter;
class DCOPObject;
class QDomDocumentFragment;
class QDomDocument;
class QDomElement;
class KoZoomHandler;

/******************************************************************/
/* Class: KPObject                                                */
/******************************************************************/

class KPObject
{
public:
    KPObject();
    virtual ~KPObject();


    virtual DCOPObject* dcopObject();

    virtual void setSelected( bool _selected )
    { selected = _selected; }
    virtual void rotate( float _angle )
    { angle = _angle; }
    virtual void setSize( const KoSize & _size )
    { setSize( _size.width(), _size.height() ); }
    virtual void setSize(double _width, double _height )
    { ext = KoSize( _width > 20 ? _width : 20, _height > 20 ? _height : 20 ); }
    virtual void setOrig( const KoPoint & _point )
    { orig = _point; }
    virtual void setOrig( double _x, double _y )
    { orig = KoPoint( _x, _y ); }
    virtual void moveBy( const KoPoint &_point )
    { orig = orig + _point; }
    virtual void moveBy( double _dx, double _dy )
    { orig = orig + KoPoint( _dx, _dy ); }
    virtual void resizeBy( const KoSize & _size )
    { resizeBy( _size.width(), _size.height() ); }
    virtual void resizeBy( double _dx,double _dy )
    { ext = ext + KoSize( _dx + ext.width() > 20 ? _dx : 0, _dy + ext.height() > 20 ? _dy : 0 ); }

    virtual void setShadowParameter(int _distance,ShadowDirection _direction,const QColor &_color)
    {
	    shadowDistance = _distance;
	    shadowDirection = _direction;
	    shadowColor = _color;
    }

    virtual void setShadowDistance( int _distance )
    { shadowDistance = _distance; }
    virtual void setShadowDirection( ShadowDirection _direction )
    { shadowDirection = _direction; }
    virtual void setShadowColor( const QColor & _color )
    { shadowColor = _color; }
    virtual void setEffect( Effect _effect )
    { effect = _effect; }
    virtual void setEffect2( Effect2 _effect2 )
    { effect2 = _effect2; }
    virtual void setPresNum( int _presNum )
    { presNum = _presNum; }
    virtual void setDisappear( bool b )
    { disappear = b; }
    virtual void setDisappearNum( int num )
    { disappearNum = num; }
    virtual void setEffect3( Effect3 _effect3)
    { effect3 = _effect3; }
    virtual void setAppearTimer( int _appearTimer )
    { appearTimer = _appearTimer; }
    virtual void setDisappearTimer( int _disappearTimer )
    { disappearTimer = _disappearTimer; }
    virtual void setAppearSoundEffect( bool b )
    { appearSoundEffect = b; }
    virtual void setDisappearSoundEffect( bool b )
    { disappearSoundEffect = b; }
    virtual void setAppearSoundEffectFileName( const QString & _a_fileName )
    { a_fileName = _a_fileName; }
    virtual void setDisappearSoundEffectFileName( const QString &_d_fileName )
    { d_fileName = _d_fileName; }

    virtual QDomDocumentFragment save( QDomDocument& doc, int offset );
    virtual int load(const QDomElement &element);

    virtual ObjType getType() const
    { return OT_UNDEFINED; }
    virtual QString getTypeString() const
    { return QString(); }

    virtual KoRect getBoundingRect( KoZoomHandler *_zoomHandler ) const;
    virtual bool isSelected() const
    { return selected; }
    virtual float getAngle() const
    { return angle; }
    virtual int getShadowDistance() const
    { return shadowDistance; }
    virtual ShadowDirection getShadowDirection() const
    { return shadowDirection; }
    virtual QColor getShadowColor() const
    { return shadowColor; }
    virtual KoSize getSize() const
    { return ext; }
    virtual KoPoint getOrig() const
    { return orig; }
    KoRect getRect() const
    { return KoRect( getOrig(), getSize() ); }
    virtual Effect getEffect() const
    { return effect; }
    virtual Effect2 getEffect2() const
    { return effect2; }
    virtual Effect3 getEffect3() const
    { return effect3; }
    virtual int getAppearTimer() const
    { return appearTimer; }
    virtual int getDisappearTimer() const
    { return disappearTimer; }
    virtual bool getAppearSoundEffect() const
    { return appearSoundEffect; }
    virtual bool getDisappearSoundEffect() const
    { return disappearSoundEffect; }
    virtual QString getAppearSoundEffectFileName() const
    { return a_fileName; }
    virtual QString getDisappearSoundEffectFileName() const
    { return d_fileName; }
    virtual int getPresNum() const
    { return presNum; }
    virtual int getSubPresSteps() const
    { return 0; }
    virtual bool getDisappear() const
    { return disappear; }
    virtual int getDisappearNum() const
    { return disappearNum; }

    virtual void setOwnClipping( bool _ownClipping )
    { ownClipping = _ownClipping; }
    virtual void setSubPresStep( int _subPresStep )
    { subPresStep = _subPresStep; }
    virtual void doSpecificEffects( bool _specEffects, bool _onlyCurrStep = true )
    { specEffects = _specEffects; onlyCurrStep = _onlyCurrStep; }

    /**
     * The main drawing method.
     * @param drawSelection if true and the object is selected, draw the handles around it
     * (this can be set to false when printing, when generating previews etc.)
     */
    virtual void draw( QPainter *_painter, KoZoomHandler*_zoomHandler, bool drawSelection );

    virtual bool contains( const KoPoint &_point ) const;
    virtual bool intersects( const KoRect & _rect ) const;
    virtual QCursor getCursor( const KoPoint &_point, ModifyType &_modType ) const;

    virtual void activate( QWidget * /*_widget*/)
    {; }
    virtual void deactivate()
    {; }

    virtual void removeFromObjList()
    { inObjList = false; doDelete(); }
    virtual void addToObjList()
    { inObjList = true; }
    virtual void incCmdRef()
    { cmds++; }
    virtual void decCmdRef()
    { cmds--; doDelete(); }

    virtual void setSticky( bool b ) { sticky = b; }
    bool isSticky() const { return sticky; }

    static void setupClipRegion( QPainter *painter, const QRegion &clipRegion );

    virtual void setOrigPointInGroup( const KoPoint &_point ) { origTopLeftPointInGroup = _point; }
    virtual KoPoint getOrigPointInGroup()const { return origTopLeftPointInGroup; }

    virtual void setOrigSizeInGroup( const KoSize &_size ) { origSizeInGroup = _size; }
    virtual KoSize getOrigSizeInGroup() const{ return origSizeInGroup; }
    void paintSelection( QPainter *_painter,KoZoomHandler *_zoomHandler );
    virtual void zoomObject(){};
protected:
    /**
     * Modifies x and y to add the shadow offsets
     */
    void getShadowCoords( double& _x, double& _y,KoZoomHandler *_zoomHandler ) const;

    virtual void doDelete();

    QDomElement createValueElement(const QString &tag, int value, QDomDocument &doc);
    QDomElement createGradientElement(const QString &tag, const QColor &c1, const QColor &c2,
                                      int type, bool unbalanced, int xfactor, int yfactor, QDomDocument &doc);
    void toGradient(const QDomElement &element, QColor &c1, QColor &c2, BCType &type,
                    bool &unbalanced, int &xfactor, int &yfactor) const;

    QDomElement createPenElement(const QString &tag, const QPen &pen, QDomDocument &doc);
    QPen toPen(const QDomElement &element) const;

    QDomElement createBrushElement(const QString &tag, const QBrush &brush, QDomDocument &doc);
    QBrush toBrush(const QDomElement &element) const;

    QColor retrieveColor(const QDomElement &element, const QString &cattr="color",
                         const QString &rattr="red", const QString &gattr="green",
                         const QString &battr="blue") const;

    float angle;
    KoPoint orig;
    KoSize ext;
    int shadowDistance;
    ShadowDirection shadowDirection;
    QColor shadowColor;
    Effect effect;
    Effect2 effect2;
    Effect3 effect3;
    int appearTimer, disappearTimer;
    QString a_fileName, d_fileName;
    int presNum, disappearNum;

    bool disappear:1;
    bool appearSoundEffect:1;
    bool disappearSoundEffect:1;
    bool selected:1;
    bool specEffects:1;
    bool onlyCurrStep:1;
    bool ownClipping:1;
    bool inObjList:1;
    bool resize:1;
    bool sticky:1;

    float presFakt;
    KoPoint oldOrig;
    KoSize oldExt;
    int subPresStep;
    int cmds;

    DCOPObject *dcop;

    static const QString &tagORIG, &attrX, &attrY,
        &tagSIZE, &attrWidth, &attrHeight, &tagSHADOW,
        &attrDistance, &attrDirection, &attrColor, &attrC1,
        &attrC2, &attrType, &attrUnbalanced, &attrXFactor,
        &attrYFactor, &attrStyle, &tagEFFECTS, &attrEffect,
        &attrEffect2, &tagPRESNUM, &tagANGLE,
        &tagDISAPPEAR, &attrDoit, &attrNum, &tagFILLTYPE,
        &tagGRADIENT, &tagPEN, &tagBRUSH, &attrValue;

    KoPoint origTopLeftPointInGroup;
    KoSize origSizeInGroup;

private:
    // Don't copy or assign it
    KPObject(const KPObject &rhs);
    KPObject &operator=(const KPObject &rhs);
};


class KP2DObject : public KPObject
{
public:
    KP2DObject();
    KP2DObject( const QPen &_pen, const QBrush &_brush, FillType _fillType,
                const QColor &_gColor1, const QColor &_gColor2, BCType _gType,
                  bool _unbalanced, int _xfactor, int _yfactor );
    virtual ~KP2DObject() { if ( gradient ) delete gradient; }

    KP2DObject &operator=( const KP2DObject & );

    virtual void setSize( double _width, double _height );
    virtual void setSize( const KoSize & _size )
    { setSize( _size.width(), _size.height() ); }
    virtual void resizeBy( double _dx, double _dy );

    virtual void setPen( const QPen &_pen )
    { pen = _pen; }
    virtual void setBrush( const QBrush &_brush )
    { brush = _brush; }
    virtual void setFillType( FillType _fillType );
    virtual void setGColor1( const QColor &_gColor1 )
    { if ( gradient ) gradient->setColor1( _gColor1 ); gColor1 = _gColor1; }
    virtual void setGColor2( const QColor &_gColor2 )
    { if ( gradient ) gradient->setColor2( _gColor2 ); gColor2 = _gColor2; }
    virtual void setGType( BCType _gType )
    { if ( gradient ) gradient->setBackColorType( _gType ); gType = _gType; }
    virtual void setGUnbalanced( bool b )
    { if ( gradient ) gradient->setUnbalanced( b ); unbalanced = b; }
    virtual void setGXFactor( int f )
    { if ( gradient ) gradient->setXFactor( f ); xfactor = f; }
    virtual void setGYFactor( int f )
    { if ( gradient ) gradient->setYFactor( f ); yfactor = f; }

    virtual QPen getPen() const
    { return pen; }
    virtual QBrush getBrush() const
    { return brush; }
    virtual FillType getFillType() const
    { return fillType; }
    virtual QColor getGColor1() const
    { return gColor1; }
    virtual QColor getGColor2() const
    { return gColor2; }
    virtual BCType getGType() const
    { return gType; }
    virtual bool getGUnbalanced() const
    { return unbalanced; }
    virtual int getGXFactor() const
    { return xfactor; }
    virtual int getGYFactor() const
    { return yfactor; }

    virtual QDomDocumentFragment save( QDomDocument& doc, int offset );
    virtual int load(const QDomElement &element);

    virtual void draw( QPainter *_painter, KoZoomHandler*_zoomHandler, bool drawSelection );

protected:
    virtual void paint( QPainter* /*_painter*/,KoZoomHandler* /*_zoomHandler*/  ) {}

    QPen pen;
    QBrush brush;
    QColor gColor1, gColor2;
    BCType gType;
    FillType fillType;
    bool unbalanced;
    int xfactor, yfactor;

    KPGradient *gradient;
    bool drawShadow;
};

#endif
