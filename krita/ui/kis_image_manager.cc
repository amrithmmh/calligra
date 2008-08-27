/* This file is part of the KDE project
 * Copyright (C) Boudewijn Rempt <boud@valdyas.org>, (C) 2006
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

#include "kis_image_manager.h"


#include <QString>

#include <kstandardaction.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kurl.h>
#include <ktoggleaction.h>
#include <kactioncollection.h>

#include <KoFilterManager.h>

#include <kis_types.h>
#include <kis_image.h>
#include <kis_filter_strategy.h>
#include <kis_shear_visitor.h>
#include <kis_transform_worker.h>
#include <kis_paint_device.h>

#include "kis_layer_manager.h"
#include "kis_statusbar.h"
#include "kis_import_catcher.h"
#include "kis_view2.h"
#include "kis_doc2.h"
#include "dialogs/kis_dlg_image_properties.h"
#include "commands/kis_image_commands.h"


KisImageManager::KisImageManager(KisView2 * view)
        : m_view(view)
{
}

void KisImageManager::setup(KActionCollection * actionCollection)
{
    KAction *action  = new KAction(i18n("I&nsert Image as Layer..."), this);
    actionCollection->addAction("insert_image_as_layer", action);
    connect(action, SIGNAL(triggered()), this, SLOT(slotInsertImageAsLayer()));

    action  = new KAction(i18n("Properties..."), this);
    actionCollection->addAction("img_properties", action);
    connect(action, SIGNAL(triggered()), this, SLOT(slotImageProperties()));
}


void KisImageManager::updateGUI()
{
    //bool enabled = m_view->image();
}


void KisImageManager::slotInsertImageAsLayer()
{
    if (importImage() > 0)
        m_view->image()->setModified();

}

qint32 KisImageManager::importImage(const KUrl& urlArg)
{
    KisImageSP currentImage = m_view->image();

    if (!currentImage) {
        return 0;
    }

    KUrl::List urls;
    qint32 rc = 0;

    if (urlArg.isEmpty()) {
        QString mimelist = KoFilterManager::mimeFilter("application/x-krita", KoFilterManager::Import).join(" ");
        urls = KFileDialog::getOpenUrls(KUrl(QString()), mimelist, 0, i18n("Import Image"));
    } else {
        urls.push_back(urlArg);
    }

    if (urls.empty())
        return 0;

    for (KUrl::List::iterator it = urls.begin(); it != urls.end(); ++it) {
        new KisImportCatcher(*it, m_view);
    }

    m_view->canvas()->update();

    return rc;
}

void KisImageManager::resizeCurrentImage(qint32 w, qint32 h, bool cropLayers)
{
    if (!m_view->image()) return;

    m_view->image()->resize(w, h, cropLayers);
    m_view->image()->setModified();
    m_view->layerManager()->layersUpdated();
}

void KisImageManager::scaleCurrentImage(double sx, double sy, KisFilterStrategy *filterStrategy)
{
    if (!m_view->image()) return;
    // XXX_PROGRESS
    m_view->image()->scale(sx, sy, 0, filterStrategy);
    m_view->image()->setModified();
    m_view->layerManager()->layersUpdated();
}

void KisImageManager::rotateCurrentImage(double radians)
{
    if (!m_view->image()) return;
    // XXX_PROGRESS
    m_view->image()->rotate(radians, 0);
    m_view->image()->setModified();
    m_view->layerManager()->layersUpdated();
}

void KisImageManager::shearCurrentImage(double angleX, double angleY)
{
    if (!m_view->image()) return;
    // XXX_PROGRESS
    m_view->image()->shear(angleX, angleY, 0);
    m_view->image()->setModified();
    m_view->layerManager()->layersUpdated();
}


void KisImageManager::slotImageProperties()
{
    KisImageSP img = m_view->image();

    if (!img) return;

    KisDlgImageProperties dlg(img, m_view);

    if (dlg.exec() == QDialog::Accepted) {
        QUndoCommand* cmd = new KisImagePropsCommand(img, dlg.colorSpace(), dlg.profile());
        m_view->document()->addCommand(cmd);
    }
}


#include "kis_image_manager.moc"
