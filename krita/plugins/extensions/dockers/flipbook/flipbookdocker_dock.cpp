/*
 *  Copyright (c) 2012 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "flipbookdocker_dock.h"

#include <QListWidget>

#include <klocale.h>
#include <kactioncollection.h>

#include <KoIcon.h>
#include <KoCanvasBase.h>

#include <kis_view2.h>
#include <kis_canvas2.h>


FlipbookDockerDock::FlipbookDockerDock( )
    : QDockWidget(i18n("Flipbook"))
    , m_canvas(0)
{
    QWidget* widget = new QWidget(this);
    setupUi(widget);
    setWidget(widget);

    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(updateLayout(Qt::DockWidgetArea)));

    bnNewFlipbook->setIcon(koIcon("document-new"));
    bnNewFlipbook->setToolTip(i18n("Create a new flipbook"));
    connect(bnNewFlipbook, SIGNAL(clicked()), SLOT(newFlipbook()));

    bnLoadFlipbook->setIcon(koIcon("document-open"));
    bnLoadFlipbook->setToolTip(i18n("Open a flipbook file"));
    connect(bnLoadFlipbook, SIGNAL(clicked()), SLOT(openFlipbook()));

    connect(cmbFlipbooks, SIGNAL(activated(int)), SLOT(selectFlipbook(int)));

    bnDeleteFlipbook->setIcon(koIcon("edit-delete"));
    bnDeleteFlipbook->setToolTip(i18n("Delete current flipbook from disk"));
    connect(bnDeleteFlipbook, SIGNAL(clicked()), SLOT(deleteFlipbook()));

    bnFirstItem->setIcon(koIcon("arrow-up-double"));
    bnFirstItem->setToolTip(i18n("Go to the first image in the current flipbook"));
    connect(bnFirstItem, SIGNAL(clicked()), SLOT(goFirst()));

    bnPreviousItem->setIcon(koIcon("arrow-up"));
    bnPreviousItem->setToolTip(i18n("Show previous image"));
    connect(bnPreviousItem, SIGNAL(clicked()), SLOT(goPrevious()));

    bnNextItem->setIcon(koIcon("arrow-down"));
    bnNextItem->setToolTip(i18n("Show next image"));
    connect(bnNextItem, SIGNAL(clicked()), SLOT(goNext()));

    bnLastItem->setIcon(koIcon("arrow-down-double"));
    bnLastItem->setToolTip(i18n("Go to the last image in the current flipblook"));
    connect(bnLastItem, SIGNAL(clicked()), SLOT(goLast()));

    bnAddItem->setIcon(koIcon("list-add"));
    bnAddItem->setToolTip(i18n("Add one or more images to the current flipbook"));
    connect(bnAddItem, SIGNAL(clicked()), SLOT(addImage()));

    bnDeleteItem->setIcon(koIcon("list-remove"));
    bnDeleteItem->setToolTip(i18n("Remove selected images from the current flipbook"));
    connect(bnDeleteItem, SIGNAL(clicked()), SLOT(removeImage()));

    connect(listFlipbook, SIGNAL(itemActivated(QListWidgetItem *)), SLOT(selectImage(QListWidgetItem*)));
}

FlipbookDockerDock::~FlipbookDockerDock()
{
}

void FlipbookDockerDock::setCanvas(KoCanvasBase * canvas)
{
    if (m_canvas && m_canvas->view()) {
         m_canvas->view()->actionCollection()->disconnect(this);
         foreach(KXMLGUIClient* client, m_canvas->view()->childClients()) {
            client->actionCollection()->disconnect(this);
        }
    }
    m_canvas = dynamic_cast<KisCanvas2*>(canvas);
}

void FlipbookDockerDock::unsetCanvas()
{
    m_canvas = 0;
}

void FlipbookDockerDock::updateLayout(Qt::DockWidgetArea area)
{
    if (area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea) {
        listFlipbook->setFlow(QListView::TopToBottom);
        bnFirstItem->setIcon(koIcon("arrow-up-double"));
        bnPreviousItem->setIcon(koIcon("arrow-up"));
        bnNextItem->setIcon(koIcon("arrow-down"));
        bnLastItem->setIcon(koIcon("arrow-down-double"));
    }
    else {
        listFlipbook->setFlow(QListView::LeftToRight);
        bnFirstItem->setIcon(koIcon("arrow-left-double"));
        bnPreviousItem->setIcon(koIcon("arrow-left"));
        bnNextItem->setIcon(koIcon("arrow-right"));
        bnLastItem->setIcon(koIcon("arrow-left-double"));
    }
}


void FlipbookDockerDock::newFlipbook()
{

}

void FlipbookDockerDock::openFlipbook()
{

}

void FlipbookDockerDock::selectFlipbook(int index)
{

}

void FlipbookDockerDock::deleteFlipbook()
{

}

void FlipbookDockerDock::addImage()
{

}

void FlipbookDockerDock::removeImage()
{

}

void FlipbookDockerDock::goFirst()
{

}

void FlipbookDockerDock::goPrevious()
{

}

void FlipbookDockerDock::goNext()
{

}

void FlipbookDockerDock::goLast()
{

}

void FlipbookDockerDock::selectImage(QListWidgetItem *item)
{

}


#include "flipbookdocker_dock.moc"
