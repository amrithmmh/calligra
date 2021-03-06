/* This file is part of the KDE project
   Copyright (C) 2000 - 2003 David Faure <faure@kde.org>
   Copyright (C) 2003 Norbert Andres <nandres@web.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#ifndef OPENCALCEXPORT_H
#define OPENCALCEXPORT_H

#include "opencalcstyleexport.h"

#include <KoFilter.h>
#include <QByteArray>
#include <QVariantList>

class QDomDocument;
class QDomElement;
class KLocale;
class KoStore;

namespace Calligra
{
namespace Sheets
{
class Doc;
class Sheet;
}
}

class OpenCalcExport : public KoFilter
{
    Q_OBJECT

public:
    OpenCalcExport(QObject* parent, const QVariantList &);
    ~OpenCalcExport() override {}

    KoFilter::ConversionStatus convert(const QByteArray & from,
            const QByteArray & to) override;

private:
    enum files { metaXML = 0x01, contentXML = 0x02, stylesXML = 0x04, settingsXML = 0x08 };
    OpenCalcStyles m_styles;

    bool writeFile(const Calligra::Sheets::Doc * ksdoc);

    bool exportDocInfo(KoStore * store, const Calligra::Sheets::Doc * ksdoc);
    bool exportStyles(KoStore * store, const Calligra::Sheets::Doc * ksdoc);
    bool exportContent(KoStore * store, const Calligra::Sheets::Doc * ksdoc);
    bool exportSettings(KoStore * store, const Calligra::Sheets::Doc * ksdoc);

    bool exportBody(QDomDocument & doc, QDomElement & content, const Calligra::Sheets::Doc * ksdoc);
    void exportSheet(QDomDocument & doc, QDomElement & tabElem,
                     const Calligra::Sheets::Sheet * sheet, int maxCols, int maxRows);
    void exportCells(QDomDocument & doc, QDomElement & rowElem,
                     const Calligra::Sheets::Sheet * sheet, int row, int maxCols);
    void exportDefaultCellStyle(QDomDocument & doc, QDomElement & officeStyles);
    void exportPageAutoStyles(QDomDocument & doc, QDomElement & autoStyles,
                              const Calligra::Sheets::Doc * ksdoc);
    void exportMasterStyles(QDomDocument & doc, QDomElement & masterStyles,
                            const Calligra::Sheets::Doc *ksdoc);

    bool writeMetaFile(KoStore * store, uint filesWritten);

    void convertPart(QString const & part, QDomDocument & doc,
                     QDomElement & parent, const Calligra::Sheets::Doc * ksdoc);
    void addText(QString const & text, QDomDocument & doc,
                 QDomElement & parent);

    void createDefaultStyles();
    QString convertFormula(QString const & formula) const;
private:
    /// Pointer to the Calligra::Sheets locale
    KLocale* m_locale;
};

#endif
