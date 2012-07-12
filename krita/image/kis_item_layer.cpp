/*
 *  Copyright (c) 2005 C. Boemann <cbo@boemann.dk>
 *  Copyright (c) 2006 Bart Coppens <kde@bartcoppens.be>
 *  Copyright (c) 2007 Boudewijn Rempt <boud@valdyas.org>
 *  Copyright (c) 2009 Dmitry Kazakov <dimula73@gmail.com>
 *  Copyright (c) 2012 Francisco Fernandes <francisco.fernandes.j@gmail.com>
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

#include "kis_item_layer.h"

#include <kis_debug.h>
#include <klocale.h>

#include <KoColorSpace.h>
#include <KoColorProfile.h>
#include <KoCompositeOp.h>

#include "kis_image.h"
#include "kis_painter.h"
#include "kis_paint_device.h"
#include "kis_node_visitor.h"
#include "kis_processing_visitor.h"
#include "kis_default_bounds.h"

struct KisItemLayer::Private
{
public:
    KisPaintDeviceSP paintDevice;
    QBitArray        paintChannelFlags;
};

KisItemLayer::KisItemLayer(KisImageWSP image, const QString& name, quint8 opacity, KisPaintDeviceSP dev)
        : KisLayer(image, name, opacity)
        , m_d(new Private())
{

    Q_ASSERT(dev);
    m_d->paintDevice = dev;
    m_d->paintDevice->setParentNode(this);

    // fixme: overwriting the default bounds is unexpected behaviour.
    // maybe something like if(dynamic_cast<KisDefaultBounds*>(dev.defaultBounds())) {..} is better.
    m_d->paintDevice->setDefaultBounds(new KisDefaultBounds(image));
}


KisItemLayer::KisItemLayer(KisImageWSP image, const QString& name, quint8 opacity)
        : KisLayer(image, name, opacity)
        , m_d(new Private())
{
    Q_ASSERT(image);
    m_d->paintDevice = new KisPaintDevice(this, image->colorSpace(), new KisDefaultBounds(image));

}

KisItemLayer::KisItemLayer(KisImageWSP image, const QString& name, quint8 opacity, const KoColorSpace * colorSpace)
        : KisLayer(image, name, opacity)
        , m_d(new Private())
{
    if (!colorSpace) {
        Q_ASSERT(image);
        colorSpace = image->colorSpace();
    }
    Q_ASSERT(colorSpace);
    m_d->paintDevice = new KisPaintDevice(this, colorSpace, new KisDefaultBounds(image));

}

KisItemLayer::KisItemLayer(const KisItemLayer& rhs)
        : KisLayer(rhs)
        , KisIndirectPaintingSupport()
        , m_d(new Private)
{
    m_d->paintChannelFlags = rhs.m_d->paintChannelFlags;
    m_d->paintDevice = new KisPaintDevice(*rhs.m_d->paintDevice.data());
    m_d->paintDevice->setParentNode(this);

}

KisItemLayer::~KisItemLayer()
{
    delete m_d;
}

bool KisItemLayer::allowAsChild(KisNodeSP node) const
{
    return node->inherits("KisMask");
}

KisPaintDeviceSP KisItemLayer::original() const
{
    return m_d->paintDevice;
}

KisPaintDeviceSP KisItemLayer::paintDevice() const
{
    return m_d->paintDevice;
}

bool KisItemLayer::needProjection() const
{
    return hasTemporaryTarget();
}

void KisItemLayer::copyOriginalToProjection(const KisPaintDeviceSP original,
        KisPaintDeviceSP projection,
        const QRect& rect) const
{
    lockTemporaryTarget();

    KisPainter gc(projection);
    gc.setCompositeOp(projection->colorSpace()->compositeOp(COMPOSITE_COPY));
    gc.bitBlt(rect.topLeft(), original, rect);

    if (hasTemporaryTarget()) {
        gc.setOpacity(temporaryOpacity());
        gc.setCompositeOp(temporaryCompositeOp());
        gc.setChannelFlags(temporaryChannelFlags());
        gc.bitBlt(rect.topLeft(), temporaryTarget(), rect);
    }

    unlockTemporaryTarget();
}

void KisItemLayer::setDirty(const QRect & rect)
{
     KisIndirectPaintingSupport::setDirty(rect);
     KisLayer::setDirty(rect);
}

QIcon KisItemLayer::icon() const
{
    return QIcon();
}

void KisItemLayer::setImage(KisImageWSP image)
{
    m_d->paintDevice->setDefaultBounds(new KisDefaultBounds(image));
    KisLayer::setImage(image);
}

KoDocumentSectionModel::PropertyList KisItemLayer::sectionModelProperties() const
{
    KoDocumentSectionModel::PropertyList l = KisLayer::sectionModelProperties();

    // XXX: get right icons
    l << KoDocumentSectionModel::Property(i18n("Alpha Channel Locked"), KIcon("transparency-locked"), KIcon("transparency-unlocked"), alphaLocked());
    l << KoDocumentSectionModel::Property(i18n("Alpha Channel Disabled"), KIcon("transparency-disabled"), KIcon("transparency-enabled"), alphaChannelDisabled());

    return l;
}

void KisItemLayer::setSectionModelProperties(const KoDocumentSectionModel::PropertyList &properties)
{
    foreach (KoDocumentSectionModel::Property property, properties) {
        if (property.name == i18n("Alpha Channel Locked")) {
            setAlphaLocked(property.state.toBool());
        }
        else if (property.name == i18n("Alpha Channel Disabled")) {
            disableAlphaChannel(property.state.toBool());
        }
    }

    KisLayer::setSectionModelProperties(properties);
}

const KoColorSpace * KisItemLayer::colorSpace() const
{
    return m_d->paintDevice->colorSpace();
}

bool KisItemLayer::accept(KisNodeVisitor &v)
{
    return v.visit(this);
}

void KisItemLayer::accept(KisProcessingVisitor &visitor, KisUndoAdapter *undoAdapter)
{
    return visitor.visit(this, undoAdapter);
}

void KisItemLayer::setChannelLockFlags(const QBitArray& channelFlags)
{
    Q_ASSERT(((quint32)channelFlags.count() == colorSpace()->channelCount() || channelFlags.isEmpty()));
    m_d->paintChannelFlags = channelFlags;
}

const QBitArray& KisItemLayer::channelLockFlags() const
{
    return m_d->paintChannelFlags;
}

QRect KisItemLayer::extent() const
{
    QRect rect = temporaryTarget() ? temporaryTarget()->extent() : QRect();
    return rect | KisLayer::extent();
}

QRect KisItemLayer::exactBounds() const
{
    QRect rect = temporaryTarget() ? temporaryTarget()->exactBounds() : QRect();
    return rect | KisLayer::exactBounds();
}

bool KisItemLayer::alphaLocked() const
{
    QBitArray flags = colorSpace()->channelFlags(false, true) & m_d->paintChannelFlags; 
    return flags.count(true) == 0 && !m_d->paintChannelFlags.isEmpty();
}

void KisItemLayer::setAlphaLocked(bool lock)
{
    if(m_d->paintChannelFlags.isEmpty())
        m_d->paintChannelFlags = colorSpace()->channelFlags(true, true);

    if(lock)
        m_d->paintChannelFlags &= colorSpace()->channelFlags(true, false);
    else
        m_d->paintChannelFlags |= colorSpace()->channelFlags(false, true);
}

#include "kis_item_layer.moc"