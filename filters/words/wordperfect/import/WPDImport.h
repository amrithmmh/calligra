/* This file is part of the KDE project
   Copyright (C) 2012 Yue Liu <yue.liu@mail.com>

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

#ifndef WPDIMPORT_H
#define WPDIMPORT_H

#include <KoFilter.h>
#include <QVariantList>

class WPDImport : public KoFilter
{

    Q_OBJECT

public:

    WPDImport(QObject *parent, const QVariantList&);
    virtual ~WPDImport();

    virtual KoFilter::ConversionStatus convert(const QByteArray& from, const QByteArray& to);

private:
    const char *m_inputFile;
    const char *m_outputFile;
};

#endif // WPDIMPORT_H
