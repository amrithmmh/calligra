/* This file is part of the KDE project
   Copyright (C) 2002 Ariya Hidayat <ariyahidayat@yahoo.de>

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

#include <config.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <qtextcodec.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>

#include <kdebug.h>
#include <koFilterChain.h>
#include <kgenericfactory.h>

#include <KWEFStructures.h>
#include <KWEFBaseWorker.h>
#include <KWEFKWordLeader.h>
#include <KWEFUtil.h>

#include <amiproexport.h>

typedef KGenericFactory<AmiProExport, KoFilter> AmiProExportFactory;
K_EXPORT_COMPONENT_FACTORY( libamiproexport, AmiProExportFactory( "amiproexport" ) );
class AmiProWorker : public KWEFBaseWorker
{
  public:
    AmiProWorker(void)  { }
    virtual ~AmiProWorker(void) { }
    virtual bool doOpenFile(const QString& filenameOut, const QString& to);
    virtual bool doCloseFile(void);
    virtual bool doOpenDocument(void);
    virtual bool doCloseDocument(void);
    virtual bool doFullParagraph(const QString& paraText, const LayoutData& layout,
        const ValueListFormatData& paraFormatDataList);
  private:
    QString filename;
    QString result;
    bool m_bold, m_italic, m_underline;
};

bool AmiProWorker::doOpenFile(const QString& filenameOut, const QString& /*to*/)
{
  filename = filenameOut;

  return TRUE;
}

bool AmiProWorker::doCloseFile(void)
{
  QFile out( filename );
  if( !out.open( IO_WriteOnly ) )
    return FALSE;
  QTextStream stream;
  stream.setDevice( &out );
  stream << result; 
  return TRUE;
}

bool AmiProWorker::doOpenDocument(void)
{
  result = "[ver]\n\t4\n";
  result += "[sty]\n\t\n";
  result += "[lay]\n";
  result += "\tStandard\n";
  result += "\t516\n";
  result += "\t[rght]\n";

  // don't ask me, I have no idea what these numbers are
  int magic[] = { 16833, 11908, 1, 1440, 1440, 1, 1440, 1440,
   0, 1, 0, 1, 0, 2, 1, 1440, 10465, 12, 1, 720, 1, 1440,
   1, 2160, 1, 2880, 1, 3600, 1, 4320, 1, 5040, 1, 5760,
   1, 6480, 1, 7200, 1, 7920, 1, 8640 };
  for( int i=0; i<sizeof(magic)/sizeof(magic[0]); i++ )
    result += "\t\t" + QString::number(magic[i]) + "\n";

  result += ">\n";
  result += "[elay]\n";
  result += "[edoc]\n";
  result += ">\n";

  m_bold = m_italic = m_underline = FALSE;

  return TRUE;
}

bool AmiProWorker::doCloseDocument(void)
{
  result += ">\n\n";
  return TRUE;
}

bool AmiProWorker::doFullParagraph(const QString& paraText, 
  const LayoutData& layout, const ValueListFormatData& paraFormatDataList)
{
  QString amiproText = "";
  QString text = paraText;

  ValueListFormatData::ConstIterator it;  
  for( it = paraFormatDataList.begin(); it!=paraFormatDataList.end(); ++it )
  {
    const FormatData& formatData = *it;

    // only if the format is for text (id==1)
    if( formatData.id == 1 )
    {
      QString partialText;
      partialText = text.mid( formatData.pos, formatData.len );
  
      // apply formatting
      m_bold = formatData.text.weight >= 75;
      m_italic = formatData.text.italic;
      m_underline = formatData.text.underline;

      if( m_bold ) partialText = "<+!>" + partialText + "<-!>";
      if( m_italic ) partialText = "<+\">" + partialText + "<-\">";
      if( m_underline ) partialText = "<+#>" + partialText + "<-#>";

      amiproText += partialText; 
    }
  }

  result += amiproText + "\n\n";

  return TRUE;
}

AmiProExport::AmiProExport( KoFilter *, const char *, const QStringList& ):
                     KoFilter()
{
}

KoFilter::ConversionStatus 
AmiProExport::convert( const QCString& from, 
  const QCString& to )
{
  // check for proper conversion
  if( to!= "application/x-amipro" || from != "application/x-kword" )
     return KoFilter::NotImplemented;

  AmiProWorker* worker = new AmiProWorker();
  KWEFKWordLeader* leader = new KWEFKWordLeader( worker );

  KoFilter::ConversionStatus result;
  result = leader->convert( m_chain, from, to );

  delete worker;
  delete leader;

  return result; 
}

#include "amiproexport.moc"
