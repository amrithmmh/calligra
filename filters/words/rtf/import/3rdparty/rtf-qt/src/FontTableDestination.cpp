/*
    Copyright (C)  2010  Brad Hards <bradh@frogmouth.net>

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FontTableDestination.h"

#include "rtfreader.h"
#include "rtfdebug.h"

namespace {
struct CharsetEntry { int id; const char *name; };

static constexpr CharsetEntry charsetToCodec[] = {
	{   0, "Windows-1252" },            // ANSI
	{   1, "Windows-1252" },            // Default
	{   2, nullptr },                   // Symbol,
	{   3, nullptr },                   // Invalid,
	{  77, "Macintosh" },               // Mac
	{ 128, "Shift-JIS" },               // Shift Jis
	{ 129, "CP949" },                   // Hangul
	{ 130, nullptr },                   // Johab
	{ 134, "GB2312" },                  // GB2312
	{ 136, "Big5" },                    // Big5
	{ 161, "Windows-1253" },            // Greek
	{ 162, "Windows-1254" },            // Turkish
	{ 163, "Windows-1258" },            // Vietnamese
	{ 177, "Windows-1255" },            // Hebrew
	{ 178, "Windows-1256" },            // Arabic
	{ 179, nullptr },                   // Arabic Traditional
	{ 180, nullptr },                   // Arabic user
	{ 181, nullptr },                   // Hebrew user
	{ 186, "Windows-1257" },            // Baltic
	{ 204, "Windows-1251" },            // Russian
	{ 222, "CP847" },                   // Thai
	{ 238, "Windows-1250" },            // Eastern European
	{ 254, "IBM437" },                  // PC 437
};

}

namespace RtfReader
{
    FontTableDestination::FontTableDestination( Reader *reader, AbstractRtfOutput *output, const QString &name ) :
      Destination( reader, output, name )
    {
    }

    FontTableDestination::~FontTableDestination()
    {
    }

    void FontTableDestination::handleControlWord( const QByteArray &controlWord, bool hasValue, const int value )
    {
	if ( controlWord == "f" ) {
	  m_currentFontTableIndex = value;
	} else if ( controlWord == "froman" ) {
	  m_fontTableEntry.setFontFamily( Roman );
	} else if ( controlWord == "fswiss" ) {
	  m_fontTableEntry.setFontFamily( Swiss );
	} else if ( controlWord == "fnil" ) {
	  m_fontTableEntry.setFontFamily( Nil );
	} else if ( controlWord == "fmodern" ) {
	  m_fontTableEntry.setFontFamily( Modern );
	} else if ( controlWord == "fscript" ) {
	  m_fontTableEntry.setFontFamily( Script );
	} else if ( controlWord == "fdecor" ) {
	  m_fontTableEntry.setFontFamily( Decor );
	} else if ( controlWord == "ftech" ) {
	  m_fontTableEntry.setFontFamily( Tech );
	} else if ( controlWord == "fbidi" ) {
	  m_fontTableEntry.setFontFamily( Bidi );
	} else if ( controlWord == "fprq" ) {
	  m_fontTableEntry.setFontPitch( static_cast<enum FontPitch>(value) );
	} else if ( controlWord == "fcharset" ) {
	    for (const auto &entry : charsetToCodec) {
	        if (entry.id == value) {
	            m_fontTableEntry.setCodec(QTextCodec::codecForName(entry.name));
	            break;
	        }
	    }
	} else {
          qCDebug(lcRtf) << "unhandled fonttbl control word:" << controlWord << "(" << value << ")";
	}
    }

    void FontTableDestination::handlePlainText( const QByteArray &plainText )
    {
	if ( plainText == ";" ) {
	    m_output->insertFontTableEntry( m_fontTableEntry, m_currentFontTableIndex );
	} else if ( plainText.endsWith( ";" ) ) {
	    // probably a font name with a terminating delimiter
	    int delimiterPosition = plainText.indexOf( ";" );
	    if ( delimiterPosition == ( plainText.length() - 1) ) {
		// It is at the end, chop it off
		QString fontName = plainText.left( delimiterPosition );
		m_fontTableEntry.setFontName( fontName );
		m_output->insertFontTableEntry( m_fontTableEntry, m_currentFontTableIndex );
	    } else {
		// we were not expecting a name with a delimiter other than at the end
                qCDebug(lcRtf) << "Font name with embedded delimiter: " << plainText;
	    }
	} else {
	    // plain font name
	    m_fontTableEntry.setFontName( plainText );
	}
    }

    void FontTableDestination::aboutToEndDestination()
    {
	// TODO
    }
}
