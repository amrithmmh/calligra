/* This file is part of the KDE project
   Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>

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

#include "koApplication.h"
#include "koQueryTypes.h"
#include "koDocument.h"
#include "koMainWindow.h"
#include <klocale.h>
#include <kimgio.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <qstringlist.h>
#include <kcmdlineargs.h>
#include <qdir.h>

KoApplication::KoApplication(int &argc, char **argv, const QCString& rAppName )
    : KApplication(argc, argv, rAppName)
{ // Depricated thanks to KAboutData
    KGlobal::locale()->insertCatalogue("koffice");
    KGlobal::dirs()->addResourceType("toolbar", KStandardDirs::kde_default("data") + "/koffice/toolbar/");
    KGlobal::dirs()->addResourceType("toolbar", KStandardDirs::kde_default("data") + "/koffice/pics/");

    kimgioRegister();

    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
}

KoApplication::KoApplication()
	: KApplication()
{
    KGlobal::locale()->insertCatalogue("koffice");
    KGlobal::dirs()->addResourceType("toolbar", KStandardDirs::kde_default("data") + "/koffice/toolbar/");
    KGlobal::dirs()->addResourceType("toolbar", KStandardDirs::kde_default("data") + "/koffice/pics/");

    kimgioRegister();

    connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
}

void KoApplication::start()
{
    KoDocumentEntry entry = KoDocumentEntry::queryByMimeType( KoDocument::nativeFormatMimeType() );

    ASSERT( !entry.isEmpty() );

    
    KCmdLineArgs *args= KCmdLineArgs::parsedArgs();

    int argsCount=args->count();
    
    // No argument
    if (!argsCount) {
        KoDocument* doc = entry.createDoc( 0, "Document" );
        if ( doc && ( doc->initDoc() ) )
        {
          KoMainWindow* shell = doc->createShell();
          shell->show();
          // setMainWidget( shell ); // probably bad idea, says Torben...
        }
        else
          ::exit(1);
    } else {
        // Loop through arguments
        
        short int n=0;
        for(int it=0; it < argsCount; it++ )
        {
            KoDocument* doc = entry.createDoc( 0 );
            KURL url( QDir::currentDirPath()+"/", args->arg(0) ); // allow URLs relative to current dir
            if ( doc->loadFromURL( url ) )
            {
              KoMainWindow* shell = doc->createShell();
              shell->show();
              //setMainWidget( shell );
              n++;
            }
        }
        if (n == 0) // no doc, all URLs were malformed
          ::exit(1);
    }
    
    args->clear();
    // not calling this before since the program will quit there.
}

KoApplication::~KoApplication()
{
}

#include "koApplication.moc"
