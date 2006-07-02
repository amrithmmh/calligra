/* This file is part of the KDE project
   Copyright (C) 2001 Andrea Rizzi <rizzi@kde.org>
	              Ulrich Kuettler <ulrich.kuettler@mailbox.tu-dresden.de>

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

#include <qfontmetrics.h>
#include <qstring.h>

#include <kdebug.h>
#include <KoGlobal.h>

#include "cmstyle.h"
#include "contextstyle.h"
#include "esstixfontstyle.h"
#include "symbolfontstyle.h"


KFORMULA_NAMESPACE_BEGIN


ContextStyle::ContextStyle()
    : symbolFont( "Symbol" ),
      defaultColor(Qt::black), numberColor(Qt::blue),
      operatorColor(Qt::darkGreen), errorColor(Qt::darkRed),
      emptyColor(Qt::blue), helpColor( Qt::gray ), m_sizeFactor( 0 )
{
//     kdDebug() << "ContextStyle::ContextStyle" << endl
//               << "defaultFont: " << defaultFont.rawName() << endl
//               << "nameFont: " << nameFont.rawName() << endl
//               << "numberFont: " << numberFont.rawName() << endl
//               << "operatorFont: " << operatorFont.rawName() << endl
//               << "symbolFont: " << symbolFont.rawName() << endl;

    textStyleValues[ displayStyle      ].setup( 1. );
    textStyleValues[ textStyle         ].setup( 1. );
    textStyleValues[ scriptStyle       ].setup( .7 );
    textStyleValues[ scriptScriptStyle ].setup( .49 );

    m_baseTextStyle = displayStyle;

    lineWidth = 1;
    linearMovement = false;
    centerSymbol = true;
    m_syntaxHighlighting = true;

    m_fontStyle = 0;
}


ContextStyle::~ContextStyle()
{
    delete m_fontStyle;
}


void ContextStyle::init( bool init )
{
    setup();
    setFontStyle( m_fontStyleName, init );
}


void ContextStyle::setFontStyle( const QString& fontStyle, bool init )
{
    delete m_fontStyle;
    m_fontStyleName = fontStyle;
    if ( m_fontStyleName == "tex" ) {
        m_fontStyle = new CMStyle();
        if ( !m_fontStyle->init( this , init ) ) {
        }
    }
    else if ( m_fontStyleName == "esstix" ) {
        m_fontStyle = new EsstixFontStyle();
        if ( !m_fontStyle->init( this ) ) {
        }
    }
    else {
        // The SymbolFontStyle is always expected to work.
        m_fontStyle = new SymbolFontStyle();
        m_fontStyle->init( this );
    }
}


const SymbolTable& ContextStyle::symbolTable() const
{
    return *( m_fontStyle->symbolTable() );
}


void ContextStyle::readConfig( KConfig* config, bool init )
{
    config->setGroup( "kformula Font" );
    QString fontName = config->readEntry( "defaultFont", "Times,12,-1,5,50,1,0,0,0,0" );
    defaultFont.fromString( fontName );
    fontName = config->readEntry( "nameFont", "Times,12,-1,5,50,0,0,0,0,0" );
    nameFont.fromString( fontName );
    fontName = config->readEntry( "numberFont", "Times,12,-1,5,50,0,0,0,0,0" );
    numberFont.fromString( fontName );
    fontName = config->readEntry( "operatorFont", "Times,12,-1,5,50,0,0,0,0,0" );
    operatorFont.fromString( fontName );
    QString baseSize = config->readEntry( "baseSize", "20" );
    m_baseSize = baseSize.toInt();

    m_fontStyleName = config->readEntry( "fontStyle" );

    if ( m_fontStyleName.isEmpty() ) {
        if (CMStyle::missingFonts( init ).isEmpty())
            m_fontStyleName = "tex";
        else if (EsstixFontStyle::missingFonts().isEmpty())
    	    m_fontStyleName = "esstix";
        else
            m_fontStyleName = "symbol";
    }

#if 0
    m_requestedFonts = config->readListEntry( "usedMathFonts" );
    if ( m_requestedFonts.size() == 0 ) {
        m_requestedFonts.push_back( "esstixone" );
        m_requestedFonts.push_back( "esstixtwo" );
        m_requestedFonts.push_back( "esstixthree" );
        m_requestedFonts.push_back( "esstixfour" );
        m_requestedFonts.push_back( "esstixfive" );
        m_requestedFonts.push_back( "esstixsix" );
        m_requestedFonts.push_back( "esstixseven" );
        m_requestedFonts.push_back( "esstixeight" );
        m_requestedFonts.push_back( "esstixnine" );
        m_requestedFonts.push_back( "esstixten" );
        m_requestedFonts.push_back( "esstixeleven" );
        m_requestedFonts.push_back( "esstixtwelve" );
        m_requestedFonts.push_back( "esstixthirteen" );
        m_requestedFonts.push_back( "esstixfourteen" );
        m_requestedFonts.push_back( "esstixfifteen" );
        m_requestedFonts.push_back( "esstixsixteen" );
        m_requestedFonts.push_back( "esstixseventeen" );
//         m_requestedFonts.push_back( "mt extra" );
//         m_requestedFonts.push_back( "mt symbol" );
//         m_requestedFonts.push_back( "euclid math one" );
//         m_requestedFonts.push_back( "euclid math two" );
//         m_requestedFonts.push_back( "euclid symbol" );
    }
#endif

    // There's no gui right anymore but I'll leave it here...
    config->setGroup( "kformula Color" );
    defaultColor  = config->readColorEntry( "defaultColor",  &defaultColor );
    numberColor   = config->readColorEntry( "numberColor",   &numberColor );
    operatorColor = config->readColorEntry( "operatorColor", &operatorColor );
    emptyColor    = config->readColorEntry( "emptyColor",    &emptyColor );
    errorColor    = config->readColorEntry( "errorColor",    &errorColor );
    helpColor     = config->readColorEntry( "helpColor",     &helpColor );

    m_syntaxHighlighting = config->readBoolEntry( "syntaxHighlighting", true );
}

void ContextStyle::setZoomAndResolution( int zoom, int dpiX, int dpiY )
{
    KoZoomHandler::setZoomAndResolution( zoom, dpiX, dpiY );
}

bool ContextStyle::setZoomAndResolution( int zoom, double zoomX, double zoomY, bool, bool )
{
    bool changes = m_zoom != zoom || m_zoomedResolutionX != zoomX || m_zoomedResolutionY != zoomY;
    m_zoom = zoom;
    m_zoomedResolutionX = zoomX;
    m_zoomedResolutionY = zoomY;
    return changes;
}

QColor ContextStyle::getNumberColor()   const
{
    if ( edit() && syntaxHighlighting() ) {
        return numberColor;
    }
    return getDefaultColor();
}

QColor ContextStyle::getOperatorColor() const
{
    if ( edit() && syntaxHighlighting() ) {
        return operatorColor;
    }
    return getDefaultColor();
}

QColor ContextStyle::getErrorColor()    const
{
    if ( edit() && syntaxHighlighting() ) {
        return errorColor;
    }
    return getDefaultColor();
}

QColor ContextStyle::getEmptyColor()    const
{
    if ( edit() && syntaxHighlighting() ) {
        return emptyColor;
    }
    return getDefaultColor();
}

QColor ContextStyle::getHelpColor()     const
{
    if ( edit() && syntaxHighlighting() ) {
        return helpColor;
    }
    return getDefaultColor();
}

void ContextStyle::setDefaultColor( const QColor& color )
{
    defaultColor = color;
}
void ContextStyle::setNumberColor( const QColor& color )
{
    numberColor = color;
}
void ContextStyle::setOperatorColor( const QColor& color )
{
    operatorColor = color;
}
void ContextStyle::setErrorColor( const QColor& color )
{
    errorColor = color;
}
void ContextStyle::setEmptyColor( const QColor& color )
{
    emptyColor = color;
}
void ContextStyle::setHelpColor( const QColor& color )
{
    helpColor = color;
}

#if 0
const QStringList& ContextStyle::requestedFonts() const
{
    return m_requestedFonts;
}

void ContextStyle::setRequestedFonts( const QStringList& list )
{
    m_requestedFonts = list;
    //table.init( this );
}
#endif

double ContextStyle::getReductionFactor( TextStyle tstyle ) const
{
    return textStyleValues[ tstyle ].reductionFactor;
}

luPt ContextStyle::getAdjustedSize( TextStyle tstyle, double factor ) const
{
    return qRound( ptToLayoutUnitPt( m_sizeFactor 
                                     * m_baseSize 
                                     * getReductionFactor( tstyle )
                                     * factor ) );
}

luPixel ContextStyle::getSpace( TextStyle tstyle, SpaceWidth space, double factor ) const
{
    switch ( space ) {
    case NEGTHIN: return -getThinSpace( tstyle, factor );
    case THIN:    return getThinSpace( tstyle, factor );
    case MEDIUM:  return getMediumSpace( tstyle, factor );
    case THICK:   return getThickSpace( tstyle, factor );
    case QUAD:    return getQuadSpace( tstyle, factor );
    }
    return 0;
}

luPixel ContextStyle::getThinSpace( TextStyle tstyle, double factor ) const
{
    return ptToPixelX( m_sizeFactor
                       * textStyleValues[ tstyle ].thinSpace( quad )
                       * factor );
}

luPixel ContextStyle::getMediumSpace( TextStyle tstyle, double factor ) const
{
    return ptToPixelX( m_sizeFactor
                       * textStyleValues[ tstyle ].mediumSpace( quad ) 
                       * factor );
}

luPixel ContextStyle::getThickSpace( TextStyle tstyle, double factor ) const
{
    return ptToPixelX( m_sizeFactor
                       * textStyleValues[ tstyle ].thickSpace( quad ) 
                       * factor );
}

luPixel ContextStyle::getQuadSpace( TextStyle tstyle, double factor ) const
{
    return ptToPixelX( m_sizeFactor
                       * textStyleValues[ tstyle ].quadSpace( quad ) 
                       * factor );
}

luPixel ContextStyle::axisHeight( TextStyle tstyle, double factor ) const
{
    //return ptToPixelY( textStyleValues[ tstyle ].axisHeight( m_axisHeight ) );
    return static_cast<luPixel>( m_sizeFactor
                                 * textStyleValues[ tstyle ].axisHeight( m_axisHeight ) 
                                 * factor );
}

luPt ContextStyle::getBaseSize() const
{
    return static_cast<luPt>( ptToLayoutUnitPt( m_sizeFactor*m_baseSize ) );
}

void ContextStyle::setBaseSize( int size )
{
    //kdDebug( 40000 ) << "ContextStyle::setBaseSize" << endl;
    if ( size != m_baseSize ) {
        m_baseSize = size;
        setup();
    }
}

void ContextStyle::setSizeFactor( double factor )
{
    m_sizeFactor = factor;
}


luPixel ContextStyle::getLineWidth( double factor ) const
{
    return ptToLayoutUnitPixX( m_sizeFactor*lineWidth*factor );
}

luPixel ContextStyle::getEmptyRectWidth( double factor ) const
{
    return ptToLayoutUnitPixX( m_sizeFactor*m_baseSize*factor/1.8 );
}

luPixel ContextStyle::getEmptyRectHeight( double factor ) const
{
    return ptToLayoutUnitPixX( m_sizeFactor*m_baseSize*factor/1.8 );
}


ContextStyle::TextStyle ContextStyle::convertTextStyleFraction( TextStyle tstyle ) const
{
    TextStyle result;

    switch ( tstyle ){
    case displayStyle:
	result = textStyle;
	break;
    case textStyle:
	result = scriptStyle;
	break;
    default:
	result = scriptScriptStyle;
	break;
    }

    return result;
}


ContextStyle::TextStyle ContextStyle::convertTextStyleIndex( TextStyle tstyle ) const
{
    TextStyle result;

    switch ( tstyle ){
    case displayStyle:
	result = scriptStyle;
	break;
    case textStyle:
	result = scriptStyle;
	break;
    default:
	result = scriptScriptStyle;
	break;
    }

    return result;
}


void ContextStyle::setup()
{
    luPt size = static_cast<luPt>( m_baseSize );
    QFont font = symbolFont;
    font.setPointSize( size );
    QFontMetrics fm( font );

    // Or better the real space required? ( boundingRect )
    quad = ptToLayoutUnitPt( fm.width( 'M' ) );

    font = QFont(defaultFont);
    font.setPointSize( size );
    QFontMetrics fm2( font );
    //m_axisHeight = ptToLayoutUnitPt( fm2.strikeOutPos() );
    //ptToLayoutUnitPixY
    //m_axisHeight = ptToLayoutUnitPt( pixelYToPt( fm2.strikeOutPos() ) );
    m_axisHeight = ptToLayoutUnitPixY( pixelYToPt( fm2.strikeOutPos() ) );
}


double StyleAttributes::getSizeFactor()
{
    if ( factor_stack.empty() ) {
//        kdWarning( DEBUGID ) << "SizeFactor stack is empty.\n";
        return 1.0;
    }
    return factor_stack.top();
}

CharStyle StyleAttributes::getCharStyle()
{
    if ( style_stack.empty() ) {
//        kdWarning( DEBUGID ) << "CharStyle stack is empty.\n";
        return anyChar;
    }
    return style_stack.top();
}

CharFamily StyleAttributes::getCharFamily()
{
    if ( family_stack.empty() ) {
//        kdWarning( DEBUGID ) << "CharFamily stack is empty.\n";
        return anyFamily;
    }
    return family_stack.top();
}

QColor StyleAttributes::getColor()
{
    if ( color_stack.empty() ) {
//        kdWarning( DEBUGID ) << "Color stack is empty.\n";
        return QColor( Qt::black );
        //return getDefaultColor();
    }
    return color_stack.top();
}

QColor StyleAttributes::getBackground()
{
    if ( background_stack.empty() ) {
//        kdWarning( DEBUGID ) << "Background stack is empty.\n";
        return QColor( Qt::white );
    }
    return background_stack.top();
}

void StyleAttributes::setSizeFactor( double f )
{
    factor_stack.push( f );
}

void StyleAttributes::setCharStyle( CharStyle cs ) 
{
    style_stack.push( cs ); 
}

void StyleAttributes::setCharFamily( CharFamily cf ) 
{
    family_stack.push( cf );
}

void StyleAttributes::setColor( const QColor& c )
{
    color_stack.push( c );
}

void StyleAttributes::setBackground( const QColor& bg )
{
    background_stack.push( bg );
}

bool StyleAttributes::derivedColor()
{
    return ! color_stack.empty();
}

void StyleAttributes::resetSizeFactor()
{
    factor_stack.pop();
}

void StyleAttributes::reset()
{
    factor_stack.pop();
    style_stack.pop();
    family_stack.pop();
    color_stack.pop();
    background_stack.pop();
}



KFORMULA_NAMESPACE_END
