/* -*- C++ -*-

  $Id$

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as
  published by  
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU Library General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <iostream.h>
#include <strstream.h>
#include <qclipboard.h>
#include "CutCmd.h"
#include <klocale.h>

#include "GDocument.h"
#include "GObject.h"

using namespace std;

CutCmd::CutCmd (GDocument* doc) 
  : Command(i18n("Cut"))
{
  document = doc;
  for (list<GObject*>::iterator it = doc->getSelection ().begin ();
       it != doc->getSelection ().end (); it++) {
    GObject* o = *it;
    o->ref ();
    // store the old position of the object
    int pos = doc->findIndexOfObject (o);
    objects.push_back (pair<int, GObject*> (pos, o));
  }
}

CutCmd::~CutCmd () {
  for (list<pair<int, GObject*> >::iterator it = objects.begin ();
       it != objects.end (); it++)
      it->second->unref ();
}

void CutCmd::execute () {
  ostrstream os;
  XmlWriter xs (os);

  xs.startTag ("doc", false);
  xs.addAttribute ("mime", KILLUSTRATOR_MIMETYPE);
  xs.closeTag ();

  for (list<pair<int, GObject*> >::iterator it = objects.begin ();
       it != objects.end (); it++) {
      it->second->writeToXml (xs);
      document->deleteObject (it->second);
  }

  xs.endTag (); // </doc>

  os << ends;
  QApplication::clipboard ()->setText (os.str ());
}

void CutCmd::unexecute () {
  QApplication::clipboard ()->clear ();
  list<pair<int, GObject*> >::iterator i;
  document->unselectAllObjects ();

  for (i = objects.begin (); i != objects.end (); i++) {
    // insert the object at the old position
    int pos = i->first;
    GObject* obj = i->second;
    obj->ref ();
    document->insertObjectAtIndex (obj, pos);
    document->selectObject (obj);
  }
}

