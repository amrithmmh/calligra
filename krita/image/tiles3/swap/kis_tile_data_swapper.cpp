/*
 *  Copyright (c) 2010 Dmitry Kazakov <dimula73@gmail.com>
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

#include <QSemaphore>

#include "tiles3/swap/kis_tile_data_swapper.h"
#include "tiles3/swap/kis_tile_data_swapper_p.h"
#include "tiles3/kis_tile_data.h"
#include "tiles3/kis_tile_data_store.h"
#include "tiles3/kis_tile_data_store_iterators.h"
#include "kis_debug.h"

#define SEC 1000

const qint32 KisTileDataSwapper::TIMEOUT = -1;
const qint32 KisTileDataSwapper::DELAY = 0.7 * SEC;

//#define DEBUG_SWAPPER

#ifdef DEBUG_SWAPPER
#define DEBUG_ACTION(action) qDebug() << action
#define DEBUG_VALUE(value) qDebug() << "\t" << ppVar(value)
#else
#define DEBUG_ACTION(action)
#define DEBUG_VALUE(value)
#endif

class SoftSwapStrategy;
class AggressiveSwapStrategy;


struct KisTileDataSwapper::Private
{
public:
    QSemaphore semaphore;
    QAtomicInt shouldExitFlag;
    KisTileDataStore *store;
    KisStoreLimits limits;
    QMutex cycleLock;
};

KisTileDataSwapper::KisTileDataSwapper(KisTileDataStore *store)
    : QThread(),
      m_d(new Private())
{
    m_d->shouldExitFlag = 0;
    m_d->store = store;
}

KisTileDataSwapper::~KisTileDataSwapper()
{
    delete m_d;
}

void KisTileDataSwapper::kick()
{
    m_d->semaphore.release();
}

void KisTileDataSwapper::terminateSwapper()
{
    unsigned long exitTimeout = 100;
    do {
        m_d->shouldExitFlag = true;
        kick();
    } while(!wait(exitTimeout));
}

void KisTileDataSwapper::waitForWork()
{
    m_d->semaphore.tryAcquire(1, TIMEOUT);
}

void KisTileDataSwapper::run()
{
    while (1) {
        waitForWork();

        if (m_d->shouldExitFlag)
            return;

        QThread::msleep(DELAY);

        doJob();
    }
}

void KisTileDataSwapper::checkFreeMemory()
{
//    qDebug() <<"check memory: high limit -" << m_d->limits.emergencyThreshold() <<"in mem -" << m_d->store->numTilesInMemory();
    if(m_d->store->memoryMetric() > m_d->limits.emergencyThreshold())
        doJob();
}

void KisTileDataSwapper::doJob()
{
    /**
     * In emergency case usual threads have access
     * to this function as well
     */
    QMutexLocker locker(&m_d->cycleLock);

    qint32 memoryMetric = m_d->store->memoryMetric();

    DEBUG_ACTION("Started swap cycle");
    DEBUG_VALUE(m_d->store->numTiles());
    DEBUG_VALUE(m_d->store->numTilesInMemory());
    DEBUG_VALUE(memoryMetric);

    DEBUG_VALUE(m_d->limits.softLimitThreshold());
    DEBUG_VALUE(m_d->limits.hardLimitThreshold());


    if(memoryMetric > m_d->limits.softLimitThreshold()) {
        qint32 softFree =  memoryMetric - m_d->limits.softLimit();
        DEBUG_VALUE(softFree);
        DEBUG_ACTION("\t pass0");
        memoryMetric -= pass<SoftSwapStrategy>(softFree);
        DEBUG_VALUE(memoryMetric);

        if(memoryMetric > m_d->limits.hardLimitThreshold()) {
            qint32 hardFree =  memoryMetric - m_d->limits.hardLimit();
            DEBUG_VALUE(hardFree);
            DEBUG_ACTION("\t pass1");
            memoryMetric -= pass<AggressiveSwapStrategy>(hardFree);
            DEBUG_VALUE(memoryMetric);
        }
    }
}


class SoftSwapStrategy
{
public:
    typedef KisTileDataStoreIterator iterator;

    static inline iterator* beginIteration(KisTileDataStore *store) {
        return store->beginIteration();
    }

    static inline void endIteration(KisTileDataStore *store, iterator *iter) {
        store->endIteration(iter);
    }

    static inline bool isInteresting(KisTileData *td) {
        // We are working with mementoed tiles only...
        return td->mementoed() && td->numUsers() <= 1;
    }

    static inline bool swapOutFirst(KisTileData *td) {
        return td->age() > 0;
    }
};

class AggressiveSwapStrategy
{
public:
    typedef KisTileDataStoreClockIterator iterator;

    static inline iterator* beginIteration(KisTileDataStore *store) {
        return store->beginClockIteration();
    }

    static inline void endIteration(KisTileDataStore *store, iterator *iter) {
        store->endIteration(iter);
    }

    static inline bool isInteresting(KisTileData *td) {
        // Add some aggression...
        Q_UNUSED(td);
        return true; // >:)
    }

    static inline bool swapOutFirst(KisTileData *td) {
        return td->age() > 0;
    }
};


template<class strategy>
qint64 KisTileDataSwapper::pass(qint64 needToFreeMetric)
{
    qint64 freedMetric = 0;
    QList<KisTileData*> additionalCandidates;

    typename strategy::iterator *iter =
        strategy::beginIteration(m_d->store);

    KisTileData *item;

    while(iter->hasNext()) {
        item = iter->next();

        if(freedMetric >= needToFreeMetric) break;


        if(!strategy::isInteresting(item)) continue;

        if(strategy::swapOutFirst(item)) {
            if(iter->trySwapOut(item)) {
                freedMetric += item->pixelSize();
            }
        }
        else {
            item->markOld();
            additionalCandidates.append(item);
        }

    }

    foreach(item, additionalCandidates) {
        if(freedMetric >= needToFreeMetric) break;

        if(iter->trySwapOut(item)) {
            freedMetric += item->pixelSize();
        }
    }

    strategy::endIteration(m_d->store, iter);

    return freedMetric;
}

void KisTileDataSwapper::testingRereadConfig()
{
    m_d->limits = KisStoreLimits();
}
