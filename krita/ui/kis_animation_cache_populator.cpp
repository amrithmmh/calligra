/*
 *  Copyright (c) 2015 Jouni Pentikäinen <joupent@gmail.com>
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

#include "kis_animation_cache_populator.h"

#include <QTimer>

#include "KisPart.h"
#include "KisDocument.h"
#include "kis_image.h"
#include "kis_image_animation_interface.h"
#include "kis_canvas2.h"
#include "kis_time_range.h"
#include "kis_animation_frame_cache.h"

struct KisAnimationCachePopulator::Private
{
    KisAnimationCachePopulator *q;

    QTimer timer;

    /**
     * Counts up the number of subsequent times Krita has been detected idle.
     */
    int idleCounter;
    static const int IDLE_COUNT_THRESHOLD = 5;
    static const int IDLE_CHECK_INTERVAL = 500;
    static const int GENERATION_TIMEOUT = 1000;

    KisImageWSP requestImage;
    int requestedFrame;

    Private(KisAnimationCachePopulator *q)
        : q(q),
          idleCounter(0),
          requestedFrame(-1)
    {}

    bool checkIdle()
    {
        QList<QPointer<KisDocument> > documents = KisPart::instance()->documents();

        QPointer<KisDocument> document;
        foreach (document, documents) {
            bool lock = document->image()->tryBarrierLock();
            if (lock) {
                document->image()->unlock();
            } else {
                return false;
            }
        }

        return true;
    }

    void generateIfIdle()
    {
        bool waitingForIdle = idleCounter < IDLE_COUNT_THRESHOLD;
        bool idle;

        if (requestImage) {
            disconnectImage();
        }

        if (waitingForIdle) {
            if (checkIdle()) {
                idleCounter++;
            } else {
                idleCounter = 0;
            }

            idle = (idleCounter >= IDLE_COUNT_THRESHOLD);
        } else {
            idle = checkIdle();
        }

        if (idle) {
            bool requested = tryRequestGeneration();
            if (requested) {
                timer.setInterval(GENERATION_TIMEOUT);
                timer.start();
                return;
            }
        }

        if (!waitingForIdle) {
            idleCounter = 0;
            timer.setInterval(IDLE_CHECK_INTERVAL);
            timer.start();
        }
    }


    bool tryRequestGeneration()
    {
        // Prioritize the active document

        KisMainWindow *activeWindow = KisPart::instance()->currentMainwindow();

        if (activeWindow && activeWindow->activeView()) {
            KisCanvas2 *activeCanvas = activeWindow->activeView()->canvasBase();

            if (activeCanvas && activeCanvas->frameCache()) {
                bool requested = tryRequestGeneration(activeCanvas->frameCache());
                if (requested) return true;
            }
        }

        QList<KisAnimationFrameCache*> caches = KisAnimationFrameCache::caches();
        KisAnimationFrameCache *cache;
        foreach (cache, caches) {
            bool requested = tryRequestGeneration(cache);
            if (requested) return true;
        }

        return false;
    }

    bool tryRequestGeneration(KisAnimationFrameCacheSP cache)
    {
        KisImageSP image = cache->image();
        if (!image) return false;

        KisImageAnimationInterface *animation = image->animationInterface();
        KisTimeRange currentRange = animation->currentRange();

        if (currentRange.isValid()) {
            Q_ASSERT(!currentRange.isInfinite());

            // TODO: optimize check for fully-cached case

            for (int t = currentRange.start(); t <= currentRange.end(); t++) {
                if (!cache->frameStatus(t) == KisAnimationFrameCache::Cached) {
                    connect(animation, SIGNAL(sigFrameReady(int)), q, SLOT(slotFrameReady(int)));

                    requestImage = image;
                    requestedFrame = t;

                    animation->requestFrameRegeneration(t, image->bounds());
                    return true;
                }
            }
        }

        return false;
    }

    void disconnectImage()
    {
        KisImageSP image = requestImage;
        if (image) {
            disconnect(requestImage->animationInterface(), 0, q, 0);
        }
        requestImage = 0;
    }
};

KisAnimationCachePopulator::KisAnimationCachePopulator()
    : m_d(new Private(this))
{
    connect(&m_d->timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
}

KisAnimationCachePopulator::~KisAnimationCachePopulator()
{}

void KisAnimationCachePopulator::slotStart()
{
    m_d->timer.setInterval(500);
    m_d->timer.start();
}

void KisAnimationCachePopulator::slotTimer()
{
    m_d->generateIfIdle();
}

void KisAnimationCachePopulator::slotFrameReady(int frame)
{
    if (frame == m_d->requestedFrame) {
        m_d->generateIfIdle();
    }
}

#include "kis_animation_cache_populator.moc"