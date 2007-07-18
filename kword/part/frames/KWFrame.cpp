/* This file is part of the KDE project
 * Copyright (C) 2000-2006 David Faure <faure@kde.org>
 * Copyright (C) 2005-2006 Thomas Zander <zander@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "KWFrame.h"
#include "KWFrameSet.h"
#include "KWCopyShape.h"

#include <KoXmlWriter.h>

KWFrame::KWFrame(KoShape *shape, KWFrameSet *parent)
      // Initialize member vars here. This ensures they are all initialized, since it's
      // easier to compare this list with the member vars list (compiler ensures order).
    : m_shape(shape),
    m_frameBehavior( KWord::AutoExtendFrameBehavior ),
    m_copyToEverySheet( true ),
    m_newFrameBehavior( KWord::NoFollowupFrame ),
    m_runAroundSide( KWord::BiggestRunAroundSide ),
    m_runAround( KWord::RunAround ),
    m_runAroundDistance( 1.0 ),
    m_frameSet( parent )
{
    Q_ASSERT(shape);
    shape->setApplicationData(this);
    if(parent)
        parent->addFrame(this);
}

KWFrame::~KWFrame() {
    m_shape = 0; // no delete is needed as the shape deletes us.
    if(m_frameSet) {
        bool justMe = m_frameSet->frameCount() == 1;
        m_frameSet->removeFrame(this); // first remove me so we won't get double deleted.
        if(justMe)
            delete m_frameSet;
        m_frameSet = 0;
    }
}

void KWFrame::setFrameSet(KWFrameSet *fs) {
    if(fs == m_frameSet)
        return;
    if(m_frameSet)
        m_frameSet->removeFrame(this);
    m_frameSet = fs;
    if(fs)
        fs->addFrame(this);
}

void KWFrame::copySettings(const KWFrame *frame) {
    setFrameBehavior(frame->frameBehavior());
    setNewFrameBehavior(frame->newFrameBehavior());
    setFrameOnBothSheets(frame->frameOnBothSheets());
    setRunAroundDistance(frame->runAroundDistance());
    setRunAroundSide(frame->runAroundSide());
    setTextRunAround(frame->textRunAround());
    shape()->copySettings(frame->shape());
    // TODO copy-shape
}

void KWFrame::saveOdf(KoShapeSavingContext & context) {
    KoXmlWriter &writer = context.xmlWriter();
    writer.startElement("draw:frame");
    /*
    ODF: 9.3 Frames
    The attributes that may be associated with the <draw:frame> element are:
       Position, Size (relative sizes, see below), Style, Layer, Z-Index, ID, Caption ID and
       Transformation – see section 9.2.15.
       Text anchor, table background, draw end position – see section 9.2.16
       Presentation class – see section 9.6.1
       Copy frames
    */

    shape()->saveOdfFrameAttributes(context);
    shape()->saveOdf(context);
    writer.endElement();
}

void KWFrame::setShape(KoShape *shape) {
    if(m_shape == shape) return;
    shape->copySettings(m_shape);
    m_shape->setApplicationData(0);
    delete m_shape;
    m_shape = shape;
    m_shape->setApplicationData(this);
    emit m_frameSet->frameAdded(this);
}

bool KWFrame::isCopy() const {
    return dynamic_cast<KWCopyShape*> (shape());
}

void KWFrame::makeCopyFrame() {
    if(isCopy())
        return;
    KWFrame *prev = 0;
    foreach(KWFrame* frame, m_frameSet->frames()) {
        if(frame == this)
            break;
        prev = frame;
    }
    if(prev == 0)
        return;
    KWCopyShape *copyShape = new KWCopyShape(prev->shape());
    setShape(copyShape);
}

