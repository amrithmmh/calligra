/*
 *  Copyright (c) 2004 Adrian Page <adrian@pagenet.plus.com>
 *  Copyright (c) 2004 Bart Coppens <kde@bartcoppens.be>
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
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdlib.h>
#include <string.h>
#include <cfloat>
#include <stack>

#include "qbrush.h"
#include "qcolor.h"
#include "qfontinfo.h"
#include "qfontmetrics.h"
#include "qpen.h"
#include "qregion.h"
#include "qwmatrix.h"
#include <qimage.h>
#include <qmap.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpointarray.h>
#include <qrect.h>
#include <qstring.h>

#include <kdebug.h>
#include <kcommand.h>
#include <klocale.h>

#include <qcolor.h>

#include "kis_brush.h"
#include "kis_global.h"
#include "kis_image.h"
#include "kis_layer.h"
#include "kis_paint_device.h"
#include "kis_painter.h"
#include "kis_pattern.h"
#include "kis_rect.h"
#include "kis_strategy_colorspace.h"
#include "kis_transaction.h"
#include "kis_types.h"
#include "kis_vec.h"
#include "kis_selection.h"
#include "kis_fill_painter.h"
#include "kis_pixel.h"
#include "kis_iterators_pixel.h"
#include "kis_iterator.h"

namespace {
}

KisFillPainter::KisFillPainter()
	: super()
{
    m_width = m_height = -1;
}

KisFillPainter::KisFillPainter(KisPaintDeviceSP device) : super(device)
{
    m_width = m_height = -1;
}

// 'regular' filling
// XXX: This needs to be optimized.
// XXX: This also needs renaming, since filling ought to keep the opacity and the composite op in mind,
//      this is more eraseToColor.
void KisFillPainter::fillRect(Q_INT32 x1, Q_INT32 y1, Q_INT32 w, Q_INT32 h, const QColor& c, QUANTUM opacity)
{

	Q_INT32 y;
        Q_UINT8 src[m_device->pixelSize()]; // XXX: Change QColor to KisColor, then use channelsize from color space
	Q_UINT32 depth = m_device->pixelSize();
        m_device->colorStrategy()->nativeColor(c, opacity, src, 0);

	for (y = y1; y < y1 + h; y++)
	{
		KisHLineIterator hiter = m_device->createHLineIterator(x1, y, w, true);
		while( ! hiter.isDone())
		{
			memcpy(hiter.rawData(), src, depth);
			++hiter;
		}
	}
}

void KisFillPainter::fillRect(Q_INT32 x1, Q_INT32 y1, Q_INT32 w, Q_INT32 h, KisPattern * pattern) {
	if (!pattern) return;
	if (!pattern -> valid()) return;
	if (!m_device) return;


	KisLayerSP patternLayer = pattern -> image(m_device->colorStrategy());

	int sx, sy, sw, sh;

	int y = y1;
	sy = y % pattern -> height();
	while (y < y1 + h) {
		int x = x1;
		sx = x % pattern -> width();
		sh = QMIN(y + pattern -> height() - (y1 + h), pattern -> height());
		if (sh <= 0)
			sh = pattern -> height();

		while (x < x1 + w) {
			sw = QMIN(x + pattern -> width() - (x1 + w), pattern -> width());
			if (sw <= 0)
				sw = pattern -> width();
			bitBlt(x, y, m_compositeOp, patternLayer.data(), m_opacity, sx, sy, sw, sh);
			x += sw; sx = 0;
		}

		y+=sh; sy = 0;
	}
}

// flood filling

void KisFillPainter::fillColor(int startX, int startY) {
	genericFillStart(startX, startY);

	// Now create a layer and fill it
	KisPaintDeviceSP filled = new KisPaintDevice(m_device->colorStrategy(), "Fill Temporary Layer");
	Q_CHECK_PTR(filled);
	KisFillPainter painter(filled.data());
	painter.fillRect(0, 0, m_width, m_height, m_paintColor);
	painter.end();

	genericFillEnd(filled);
}

void KisFillPainter::fillPattern(int startX, int startY) {
	genericFillStart(startX, startY);

	// Now create a layer and fill it
	KisPaintDeviceSP filled = new KisPaintDevice(m_device->colorStrategy(), "Fill Temporary Layer");
	Q_CHECK_PTR(filled);
	KisFillPainter painter(filled.data());
	painter.fillRect(0, 0, m_width, m_height, m_pattern);
	painter.end();

	genericFillEnd(filled);
}

void KisFillPainter::genericFillStart(int startX, int startY) {
	m_cancelRequested = false;

	if (m_width < 0 || m_height < 0) {
		if (m_device->image()) {
			m_width = m_device->image()->width();
			m_height = m_device->image()->height();
		} else {
			kdDebug() << "KisFillPainter::genericFillStart: no size set, assuming 500x500"
					<< endl;
			m_width = m_height = 500;
		}
	}

	m_size = m_width * m_height;

	if (m_device -> hasSelection()) {
		m_selection = m_device -> selection();
	} else {
		// Create a selection from the surrounding area
		m_selection = createFloodSelection(startX, startY);
	}
}

void KisFillPainter::genericFillEnd(KisPaintDeviceSP filled) {
    if (m_cancelRequested) {
        m_width = m_height = -1;
		return;
    }

	if (! m_device -> hasSelection() ) {
		bltSelectionExt(0, 0, m_compositeOp, filled, m_selection, m_opacity,
					 0, 0, m_width, m_height);
	} else {
		bltSelection(0, 0, m_compositeOp, filled.data(), m_opacity,
					 0, 0, m_width, m_height);
	}

    emit notifyProgressDone(this);

    m_width = m_height = -1;
}

struct FillSegment {
	FillSegment(int x, int y/*, FillSegment* parent*/) : x(x), y(y)/*, parent(parent)*/ {}
	int x;
	int y;
//	FillSegment* parent;
};

typedef enum { None = 0, Added = 1, Checked = 2 } Status;

KisSelectionSP KisFillPainter::createFloodSelection(int startX, int startY) {
	if (m_width < 0 || m_height < 0) {
		if (m_device->image()) {
			m_width = m_device->image()->width();
			m_height = m_device->image()->height();
		} else {
			kdDebug() << "KisFillPainter::genericFillStart: no size set, assuming 500x500"
					<< endl;
			m_width = m_height = 500;
		}
	}

	m_size = m_width * m_height;

	KisSelectionSP selection = new KisSelection(m_device, "Fill Temporary Selection");
	KisStrategyColorSpaceSP colorStrategy = selection -> colorStrategy();
	KisStrategyColorSpaceSP devColorStrategy = m_device -> colorStrategy();
	
	QUANTUM* source = new QUANTUM[m_device->pixelSize()];
	KisHLineIteratorPixel pixelIt = m_device->createHLineIterator(startX, startY, startX+1, false);
	KisPixel pixel = pixelIt.rawData();

	for (int i = 0; i < m_device -> pixelSize(); i++) {
		source[i] = pixel[i];
	}
	
	std::stack<FillSegment*> stack;

	stack.push(new FillSegment(startX, startY/*, 0*/));

	Status* map = new Status[m_size];

	memset(map, None, m_size * sizeof(Status));

	int progressPercent = 0; int pixelsDone = 0; int currentPercent = 0;
	emit notifyProgressStage(this, i18n("Making fill outline..."), 0);

	while(!stack.empty()) {
		FillSegment* segment = stack.top();
		stack.pop();
		if (map[m_width * segment->y + segment->x] == Checked) {
			delete segment;
			continue;
		}
		map[m_width * segment->y + segment->x] = Checked;
		
		int x = segment->x;
		int y = segment->y;

		/* We need an iterator that is valid in the range (0,y) - (width,y). Therefore,
		it is needed to start the iterator at the first position, and then skip to (x,y). */
		pixelIt = m_device->createHLineIterator(0, y, m_width, false);
		pixelIt += x;
		QUANTUM diff = devColorStrategy -> difference(source, pixelIt.rawData());

		if (diff >= m_threshold) {
			delete segment;
			continue;
		}

		// Here as well: start the iterator at (0,y)
		KisHLineIteratorPixel selIt = selection -> createHLineIterator(0, y, m_width, true);
		selIt += x;
		colorStrategy -> nativeColor(Qt::white, MAX_SELECTED /* - diff*/ , selIt.rawData(), 0); // ### diff for fuzzyness

		if (y > 0 && (map[m_width * (y - 1) + x] == None)) {
			map[m_width * (y - 1) + x] = Added;
			stack.push(new FillSegment(x, y-1));
		}
		if (y < (m_height - 1) && (map[m_width * (y + 1) + x] == None)) {
			map[m_width * (y + 1) + x] = Added;
			stack.push(new FillSegment(x, y+1));
		}

		++pixelsDone;
		
		bool stop = false;

		--pixelIt;
		--selIt;
		--x;

		// go to the left
		while(!stop && x >= 0 && (map[m_width * y + x] != Checked) ) { // FIXME optimizeable?
			map[m_width * y + x] = Checked;
			diff = devColorStrategy -> difference(source, pixelIt.rawData());
			if (diff >= m_threshold) {
				stop = true;
				continue;
			}
			colorStrategy -> nativeColor(Qt::white, MAX_SELECTED /*- diff*/, selIt.rawData(), 0); // Qt::white?? ### diff for fuzzy
			if (y > 0 && (map[m_width * (y - 1) + x] == None)) {
				map[m_width * (y - 1) + x] = Added;
				stack.push(new FillSegment(x, y-1));
			}
			if (y < (m_height - 1) && (map[m_width * (y + 1) + x] == None)) {
				map[m_width * (y + 1) + x] = Added;
				stack.push(new FillSegment(x, y+1));
			}
			++pixelsDone;
			--pixelIt;
			--selIt;
			--x;
		}

		x = segment -> x + 1;
		delete segment;

		if (map[m_width * y + x] == Checked)
			continue;

		// and go to the right
		pixelIt = m_device -> createHLineIterator(x, y, m_width, false);
		selIt = selection -> createHLineIterator(x, y, m_width, true);

		stop = false;
		while(!stop && x < m_width && (map[m_width * y + x] != Checked) ) {
			diff = devColorStrategy -> difference(source, pixelIt.rawData());
			map[m_width * y + x] = Checked;

			if (diff >= m_threshold) {
				stop = true;
				continue;
			}

			colorStrategy -> nativeColor(Qt::white, MAX_SELECTED /* -diff*/, selIt.rawData(), 0); // Qt::white?? ### fuzzy
			if (y > 0 && (map[m_width * (y - 1) + x] == None)) {
				map[m_width * (y - 1) + x] = Added;
				stack.push(new FillSegment(x, y-1));
			}
			if (y < (m_height - 1) && (map[m_width * (y + 1) + x] == None)) {
				map[m_width * (y + 1) + x] = Added;
				stack.push(new FillSegment(x, y+1));
			}
			++pixelsDone;
			++pixelIt;
			++selIt;
			++x;
		}
		
		if (m_size > 0) {
			progressPercent = (pixelsDone * 100) / m_size;
			if (progressPercent > currentPercent) {
				emit notifyProgress(this, progressPercent);
				currentPercent = progressPercent;
			}
		}
	}


	delete map;
	delete source;

	return selection;
}
