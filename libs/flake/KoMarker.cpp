/* This file is part of the KDE project
   Copyright (C) 2011 Thorsten Zachmann <zachmann@kde.org>

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

#include "KoMarker.h"

#include <KoXmlReader.h>
#include <KoXmlWriter.h>
#include <KoXmlNS.h>
#include <KoGenStyle.h>
#include <KoGenStyles.h>
#include "KoPathShape.h"
#include "KoPathShapeLoader.h"
#include "KoShapeLoadingContext.h"
#include "KoShapeSavingContext.h"

#include <QString>
#include <QUrl>
#include <QPainterPath>

class KoMarker::Private
{
public:
    Private()
    {}

    QString name;
    QString d;
    QPainterPath path;
    QRectF viewBox;
};

KoMarker::KoMarker()
: d(new Private())
{
}

KoMarker::~KoMarker()
{
    delete d;
}

bool KoMarker::loadOdf(const KoXmlElement &element, KoShapeLoadingContext &context)
{
    // A shape uses draw:marker-end="Arrow" draw:marker-end-width="0.686cm" draw:marker-end-center="true" which marker and how the marker is used

    //<draw:marker draw:name="Arrow" svg:viewBox="0 0 20 30" svg:d="m10 0-10 30h20z"/>
    //<draw:marker draw:name="Arrowheads_20_1" draw:display-name="Arrowheads 1" svg:viewBox="0 0 10 10" svg:d="m0 0h10v10h-10z"/>

    d->d =element.attributeNS(KoXmlNS::svg, "d");
    if (d->d.isEmpty()) {
        return false;
    }

    KoPathShape pathShape;
    KoPathShapeLoader loader(&pathShape);
    loader.parseSvg(d->d, true);

    d->path = pathShape.outline();
    d->viewBox = KoPathShape::loadOdfViewbox(element);

    QString displayName(element.attributeNS(KoXmlNS::draw, "display-name"));
    if (displayName.isEmpty()) {
        displayName = element.attributeNS(KoXmlNS::draw, "name");
    }
    d->name = displayName;
    return true;
}

QString KoMarker::saveOdf(KoShapeSavingContext &context) const
{
    KoGenStyle style(KoGenStyle::MarkerStyle);
    style.addAttribute("draw:display-name", d->name);
    style.addAttribute("svg:d", d->d);
    QString viewBox = QString("0 0 %1 %2").arg(qRound(d->viewBox.width())).arg(qRound(d->viewBox.height()));
    style.addAttribute("svg:viewBox", viewBox);
    QString name = QString(QUrl::toPercentEncoding(d->name, "", " ")).replace('%', '_');
    return context.mainStyles().insert(style, name, KoGenStyles::DontAddNumberToName);
}

QString KoMarker::name() const
{
    return d->name;
}

QPainterPath KoMarker::path(qreal width) const
{
    if (!d->viewBox.isValid() || width == 0) {
        return QPainterPath();
    }

    qreal height = width * d->viewBox.height() / d->viewBox.width();

    QTransform transform;
    transform.scale(width / d->viewBox.width(), height / d->viewBox.height());
    return transform.map(d->path);
}
