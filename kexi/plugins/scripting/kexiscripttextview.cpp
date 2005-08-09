/* This file is part of the KDE project
   Copyright (C) 2005 Sebastian Sauer <mail@dipe.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include "kexiscripttextview.h"
#include "kexiscriptmanager.h"

#include <kexidialogbase.h>

#include <qstringlist.h>
#include <qlayout.h>
//#include <kdebug.h>
#include <ktextbrowser.h>

/// @internal
class KexiScriptTextViewPrivate
{
    public:
        /**
         * The \a KexiScriptManager instance used to handle
         * the different \a KexiScriptContainer instances.
         */
        KexiScriptManager* manager;

        /**
         * The \a KexiScriptContainer instance is used to
         * wrap the \a Kross::Api::ScriptContainer functionality
         * to work with scripting code.
         */
        KexiScriptContainer* scriptcontainer;

        /**
         * The textbrowser used to display feedback like the
         * stdOut and stdErr messages of the scripting code.
         */
        KTextBrowser* browser;
};

KexiScriptTextView::KexiScriptTextView(KexiScriptManager* manager, KexiMainWindow *mainWin, QWidget *parent, const char *name)
    : KexiViewBase(mainWin, parent, name)
    , d( new KexiScriptTextViewPrivate() )
{
    d->manager = manager;

    d->browser = new KTextBrowser(this, "KexiScriptTextViewEditor");
    d->browser->setReadOnly(true);
    //d->browser->setFocusPolicy(QTextBrowser::ClickFocus);
    d->browser->setTextFormat(QTextBrowser::RichText);
    //d->browser->setLinkUnderline(true);
    //d->browser->setWordWrap(QTextEdit::WidgetWidth);

    QBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(d->browser);

    d->scriptcontainer = manager->getScriptContainer( parentDialog()->partItem()->name() );
    //plugSharedAction( "script_execute", scriptcontainer, SLOT(execute()) );

    QStringList output = d->scriptcontainer->getOutput();
    for(uint i = 0; i < output.size(); i++)
        d->browser->append( output[i] );

    connect(d->scriptcontainer, SIGNAL(clear()), this, SLOT(clearLog()));
    connect(d->scriptcontainer, SIGNAL(log(const QString&)), this, SLOT(addLog(const QString&)));
}

KexiScriptTextView::~KexiScriptTextView()
{
    /*
    disconnect(d->scriptcontainer, SIGNAL(stdOut(const QString&)),
            this, SLOT(addStdOut(const QString&)));
    disconnect(d->scriptcontainer, SIGNAL(stdErr(const QString&)),
            this, SLOT(addStdErr(const QString&)));
    */
}

void KexiScriptTextView::clearLog()
{
    d->browser->clear();
}

void KexiScriptTextView::addLog(const QString& message)
{
    d->browser->append( message );
    //if(! hasFocus()) setFocus();
}

#include "kexiscripttextview.moc"
