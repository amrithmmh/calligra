/*
 *  Copyright (c) 2007 Boudewijn Rempt boud@valdyas.org
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

#include "kis_crop_visitor_test.h"

#include <qtest_kde.h>
#include "kis_crop_visitor.h"
#include "kis_image.h"
#include "kis_paint_device.h"

void KisCropVisitorTest::testCreation()
{
    QRect rc(0, 0, 100, 100);
    KisCropVisitor test(rc, true);
}


QTEST_KDEMAIN(KisCropVisitorTest, GUI)
#include "kis_crop_visitor_test.moc"
