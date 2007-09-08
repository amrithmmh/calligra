/*
 * This file is part of Krita
 *
 * Copyright (c) 2004 Cyrille Berger <cberger@cberger.net>
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

#ifndef CONVOLUTIONFILTERS_H
#define CONVOLUTIONFILTERS_H

#include <kparts/plugin.h>
#include "kis_convolution_filter.h"

class KisGaussianBlurFilter : public KisConvolutionFilter {
public:
    KisGaussianBlurFilter();
public:
    static inline KoID id() { return KoID("gaussian blur", i18n("Gaussian Blur")); }
    virtual bool supportsPainting() const { return true; }
    virtual bool supportsIncrementalPainting() const { return false; }
};

class KisSharpenFilter : public KisConvolutionFilter {
public:
    KisSharpenFilter();
public:
    static inline KoID id() { return KoID("sharpen", i18n("Sharpen")); }
    virtual bool supportsPainting() const { return true; }
    virtual bool supportsIncrementalPainting() const { return false; }
};

class KisMeanRemovalFilter : public KisConvolutionFilter {
public:
    KisMeanRemovalFilter();
public:
    static inline KoID id() { return KoID("mean removal", i18n("Mean Removal")); }
    virtual bool supportsPainting() const { return false; }

};

class KisEmbossLaplascianFilter : public KisConvolutionFilter {
public:
    KisEmbossLaplascianFilter();
public:
    static inline KoID id() { return KoID("emboss laplascian", i18n("Emboss Laplascian")); }
    virtual bool supportsPainting() const { return false; }

};

class KisEmbossInAllDirectionsFilter : public KisConvolutionFilter {
public:
    KisEmbossInAllDirectionsFilter();
public:
    static inline KoID id() { return KoID("emboss all directions", i18n("Emboss in All Directions")); }
    virtual bool supportsPainting() const { return false; }

};

class KisEmbossHorizontalVerticalFilter : public KisConvolutionFilter {
public:
    KisEmbossHorizontalVerticalFilter();
public:
    static inline KoID id() { return KoID("", i18n("Emboss Horizontal & Vertical")); }
    virtual bool supportsPainting() const { return false; }

};

class KisEmbossVerticalFilter : public KisConvolutionFilter {
public:
    KisEmbossVerticalFilter();
public:
    static inline KoID id() { return KoID("emboss vertical only", i18n("Emboss Vertical Only")); }
    virtual bool supportsPainting() const { return false; }

};

class KisEmbossHorizontalFilter : public KisConvolutionFilter {
public:
    KisEmbossHorizontalFilter();
public:
    static inline KoID id() { return KoID("emboss horizontal only", i18n("Emboss Horizontal Only")); }
    virtual bool supportsPainting() const { return false; }

};

class KisEmbossDiagonalFilter : public KisConvolutionFilter {
public:
    KisEmbossDiagonalFilter();
public:
    static inline KoID id() { return KoID("emboss diagonal", i18n("Emboss Diagonal")); }
    virtual bool supportsPainting() const { return false; }

};

class KisTopEdgeDetectionFilter : public KisConvolutionFilter {
public:
    KisTopEdgeDetectionFilter();
public:
    static inline KoID id() { return KoID("top edge detections", i18n("Top Edge Detection")); }
    virtual bool supportsPainting() const { return false; }

};

class KisRightEdgeDetectionFilter : public KisConvolutionFilter {
public:
    KisRightEdgeDetectionFilter();
public:
    static inline KoID id() { return KoID("right edge detections", i18n("Right Edge Detection")); }
    virtual bool supportsPainting() const { return false; }

};

class KisBottomEdgeDetectionFilter : public KisConvolutionFilter {
public:
    KisBottomEdgeDetectionFilter();
public:
    static inline KoID id() { return KoID("bottom edge detections", i18n("Bottom Edge Detection")); }
    virtual bool supportsPainting() const { return false; }

};

class KisLeftEdgeDetectionFilter : public KisConvolutionFilter {
public:
    KisLeftEdgeDetectionFilter();
public:
    static inline KoID id() { return KoID("left edge detections", i18n("Left Edge Detection")); }
    virtual bool supportsPainting() const { return false; }

};


class KritaConvolutionFilters : public KParts::Plugin
{
public:
    KritaConvolutionFilters(QObject *parent, const QStringList &);
    virtual ~KritaConvolutionFilters();
};

#endif
