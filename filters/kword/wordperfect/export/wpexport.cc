/* This file is part of the KDE project
   Copyright (C) 2002 Ariya Hidayat <ariya@kde.org>

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

#include <KWEFBaseWorker.h>
#include <KWEFKWordLeader.h>

#include <wpexport.h>
#include <wp5.h>
#include <wp6.h>

typedef KGenericFactory<WPExport, KoFilter> WPExportFactory;
K_EXPORT_COMPONENT_FACTORY( libwpexport, WPExportFactory( "wpexport" ) )

WPExport::WPExport( KoFilter *, const char *, const QStringList& ):
                     KoFilter()
{
}

KoFilter::ConversionStatus 
WPExport::convert( const QCString& from, 
  const QCString& to )
{
  // check for proper conversion
  if( to!= "application/wordperfect" || from != "application/x-kword" )
     return KoFilter::NotImplemented;

  // document version is determined using file extension, 
  // "WP" is for WP 5.x, "WPD" or else is for WP 6/7
  // e.g /home/ariya/test.wp is a WP 5 document
  // TODO ask user with a dialog box
  QString outfile = m_chain->outputFile();
  QString extension = QFileInfo( outfile ).extension().lower();
  int version = ( extension == "wp" ) ? 5 : 6 ;

  KWEFBaseWorker* worker;
  if( version == 5 ) worker = new WPFiveWorker();
  else worker = new WPSixWorker();

  KWEFKWordLeader* leader = new KWEFKWordLeader( worker );

  KoFilter::ConversionStatus result;
  result = leader->convert( m_chain, from, to );

  delete worker;
  delete leader;

  return result; 
}

#include "wpexport.moc"
