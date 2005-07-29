/*
 *  dlg_separate.cc - part of KimageShop^WKrayon^WKrita
 *
 *  Copyright (c) 2004 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qcombobox.h>

#include <klocale.h>
#include <knuminput.h>
#include <kdebug.h>

#include "dlg_separate.h"
#include "wdg_separations.h"

// XXX: I'm really real bad at arithmetic, let alone math. Here
// be rounding errors. (Boudewijn)
DlgSeparate::DlgSeparate( QWidget *  parent,
			  const char * name)
	: super (parent, name, true, i18n("Separate Image"), Ok | Cancel, Ok)
{
	m_page = new WdgSeparations(this, "separate_image");
	Q_CHECK_PTR(m_page);
	setMainWidget(m_page);
	resize(m_page -> sizeHint());


	connect(this, SIGNAL(okClicked()),
		this, SLOT(okClicked()));
}

DlgSeparate::~DlgSeparate()
{
	delete m_page;
}

// SLOTS

void DlgSeparate::okClicked()
{
	accept();
}


#include "dlg_separate.moc"
