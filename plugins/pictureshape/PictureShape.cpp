/* This file is part of the KDE project
 * Copyright (C) 2006-2007, 2009 Thomas Zander <zander@kde.org>
 * Copyright (C) 2007 Jan Hambrecht <jaham@gmx.net>
 * Copyright (C) 2008 Thorsten Zachmann <zachmann@kde.org>
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

#include "PictureShape.h"
#include "filters/GreyscaleFilterEffect.h"
#include "filters/MonoFilterEffect.h"
#include "filters/WatermarkFilterEffect.h"

#include <KoViewConverter.h>
#include <KoImageCollection.h>
#include <KoImageData.h>
#include <KoShapeLoadingContext.h>
#include <KoOdfLoadingContext.h>
#include <KoShapeSavingContext.h>
#include <KoXmlWriter.h>
#include <KoXmlNS.h>
#include <KoStoreDevice.h>
#include <KoUnit.h>
#include <KoGenStyle.h>
#include <KoFilterEffectStack.h>
#include <SvgSavingContext.h>
#include <SvgLoadingContext.h>
#include <SvgUtil.h>

#include <KDebug>
#include <KJob>
#include <KIO/Job>

#include <QPainter>
#include <QTimer>
#include <QPixmapCache>
#include <QtCore/QBuffer>

QString generate_key(qint64 key, const QSize & size)
{
    return QString("%1-%2-%3").arg(key).arg(size.width()).arg(size.height());
}

void LoadWaiter::setImageData(KJob *job)
{
    if (m_pictureShape == 0)
        return; // ugh, the shape got deleted meanwhile
    KIO::StoredTransferJob *transferJob = qobject_cast<KIO::StoredTransferJob*>(job);
    Q_ASSERT(transferJob);

    if (m_pictureShape->imageCollection()) {
        KoImageData *data = m_pictureShape->imageCollection()->createImageData(transferJob->data());
        if (data) {
            m_pictureShape->setUserData(data);
            m_pictureShape->setSize(data->imageSize());
        }
    }
    deleteLater();
}

void RenderQueue::renderImage()
{
    KoImageData *imageData = qobject_cast<KoImageData*>(m_pictureShape->userData());
    if (m_wantedImageSize.isEmpty() || imageData == 0) {
        return;
    }
    QSize size = m_wantedImageSize.takeFirst();
    QString key(generate_key(imageData->key(), size));
    if (QPixmapCache::find(key) == 0) {
        QPixmap pixmap = imageData->pixmap(size);
        QPixmapCache::insert(key, pixmap);
        m_pictureShape->update();
    }
    if (! m_wantedImageSize.isEmpty()) {
        QTimer::singleShot(0, this, SLOT(renderImage()));
    }
}

void RenderQueue::updateShape()
{
    m_pictureShape->update();
}

//////////////
PictureShape::PictureShape()
    : KoFrameShape(KoXmlNS::draw, "image"),
    m_imageCollection(0),
    m_renderQueue(new RenderQueue(this)),
    m_mode(Standard),
    m_cropRect(0, 0, 1, 1)
{
    setKeepAspectRatio(true);
    KoFilterEffectStack * effectStack = new KoFilterEffectStack();
    effectStack->setClipRect(QRectF(0, 0, 1, 1));
    setFilterEffectStack(effectStack);
}

PictureShape::~PictureShape()
{
    delete m_renderQueue;
}

KoImageData* PictureShape::imageData() const
{
    return qobject_cast<KoImageData*>(userData());
}

QRectF PictureShape::cropRect() const
{
    return m_cropRect;
}

void PictureShape::setCropRect(const QRectF& rect)
{
    m_cropRect = rect;
    update();
}

void PictureShape::paint(QPainter &painter, const KoViewConverter &converter, KoShapePaintingContext &)
{
    QRectF       pixelsF   = converter.documentToView(QRectF(QPointF(0,0), size()));
    KoImageData *imageData = qobject_cast<KoImageData*>(userData());
    
    if (imageData == 0) {
        painter.fillRect(pixelsF, QColor(Qt::gray));
        return;
    }
    QRect pixels     = pixelsF.toRect();
    QSize pixmapSize = pixelsF.size().toSize();

    QString key(generate_key(imageData->key(), pixmapSize));
    QPixmap pixmap;
    
#if QT_VERSION  >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) { // first check cache.
#else
    if (!QPixmapCache::find(key, pixmap)) { // first check cache.
#endif
        // no? Does the imageData have it then?
        if (!(imageData->hasCachedPixmap() && imageData->pixmap().size() == pixmapSize)) {
            // ok, not what we want.
            // before asking to render it, make sure the image doesn't get too big
            QSize imageSize = imageData->image().size();
            if (imageSize.width() < pixmapSize.width() || imageSize.height() < pixmapSize.height()) {
                // kDebug() << "clipping size to orig image size" << imageSize;
                pixmapSize.setWidth(imageSize.width());
                pixmapSize.setHeight(imageSize.height());
            }

            if (m_printQualityImage.isNull()) {
                const int MaxSize = 1000; // TODO set the number as a KoImageCollection size
                // make sure our pixmap doesn't get too slow.
                // In future we may want to make this action cause a multi-threaded rescale of the pixmap.
                if (pixmapSize.width() > MaxSize) { // resize to max size.
                    pixmapSize.setHeight(qRound(pixelsF.height() / pixelsF.width() * MaxSize));
                    pixmapSize.setWidth(MaxSize);
                }
                if (pixmapSize.height() > MaxSize) {
                    pixmapSize.setWidth(qRound(pixelsF.width() / pixelsF.height() * MaxSize));
                    pixmapSize.setHeight(MaxSize);
                }
            }
            
            key = generate_key(imageData->key(), pixmapSize);
        }
    }

    if (!m_printQualityImage.isNull() && pixmapSize == m_printQualityImage.size()) { // painting the image as prepared in waitUntilReady()
        painter.drawImage(pixels, m_printQualityImage, QRect(0, 0, pixmapSize.width(), pixmapSize.height()));
        m_printQualityImage = QImage(); // free memory
        return;
    }

#if QT_VERSION  >= 0x040600
    if (!QPixmapCache::find(key, &pixmap)) {
#else
    if (!QPixmapCache::find(key, pixmap)) {
#endif
        m_renderQueue->addSize(pixmapSize);
        QTimer::singleShot(0, m_renderQueue, SLOT(renderImage()));
        if (!imageData->hasCachedPixmap() || imageData->pixmap().size().width() > pixmapSize.width()) { // don't scale down
            QTimer::singleShot(0, m_renderQueue, SLOT(updateShape()));
            return;
        }
        pixmap = imageData->pixmap();
    }
    
//     painter.drawPixmap(pixels, pixmap, QRect(100, 0, pixmap.width(), pixmap.height()));

    QSizeF size = imageData->imageSize();

    QRectF cropRect(
        size.width()  * m_cropRect.x(),
        size.height() * m_cropRect.y(),
        size.width()  * m_cropRect.width(),
        size.height() * m_cropRect.height()
    );
    
    painter.drawImage(pixels, imageData->image(), cropRect);
}

void PictureShape::waitUntilReady(const KoViewConverter &converter, bool asynchronous) const
{
    KoImageData *imageData = qobject_cast<KoImageData*>(userData());
    if (imageData == 0) {
        return;
    }

    if (asynchronous) {
        // get pixmap and schedule it if not
        QSize pixels = converter.documentToView(QRectF(QPointF(0,0), size())).size().toSize();
        QImage image = imageData->image();
        if (image.isNull()) {
            return;
        }
        if (image.size().width() < pixels.width()) { // don't scale up.
            pixels = image.size();
        }
        m_printQualityImage = image.scaled(pixels, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else {
        QSize pixels = converter.documentToView(QRectF(QPointF(0,0), size())).size().toSize();
        QString key(generate_key(imageData->key(), pixels));
        if (QPixmapCache::find(key) == 0) {
            QPixmap pixmap = imageData->pixmap(pixels);
            QPixmapCache::insert(key, pixmap);
        }
    }
}

void PictureShape::saveOdf(KoShapeSavingContext &context) const
{
    // make sure we have a valid image data pointer before saving
    KoImageData *imageData = qobject_cast<KoImageData*>(userData());
    if (imageData == 0) {
        return;
    }

    KoXmlWriter &writer = context.xmlWriter();

    writer.startElement("draw:frame");
    saveOdfAttributes(context, OdfAllAttributes);
    writer.startElement("draw:image");
    // In the spec, only the xlink:href attribute is marked as mandatory, cool :)
    QString name = context.imageHref(imageData);
    writer.addAttribute("xlink:type", "simple");
    writer.addAttribute("xlink:show", "embed");
    writer.addAttribute("xlink:actuate", "onLoad");
    writer.addAttribute("xlink:href", name);
    saveText(context);
    writer.endElement(); // draw:image
    writer.endElement(); // draw:frame

    context.addDataCenter(m_imageCollection);
}

bool PictureShape::loadOdf(const KoXmlElement &element, KoShapeLoadingContext &context)
{
    loadOdfAttributes(element, context, OdfAllAttributes);
    return loadOdfFrame(element, context);
}

bool PictureShape::loadOdfFrameElement(const KoXmlElement &element, KoShapeLoadingContext &context)
{
    if (m_imageCollection) {
        const QString href = element.attribute("href");
        // this can happen in case it is a presentation:placeholder
        if (!href.isEmpty()) {
            KoStore *store = context.odfLoadingContext().store();
            KoImageData *data = m_imageCollection->createImageData(href, store);
            setUserData(data);
        } else {
            // check if we have an office:binary data element containing the image data
            const KoXmlElement &binaryData(KoXml::namedItemNS(element, KoXmlNS::office, "binary-data"));
            if (!binaryData.isNull()) {
                QImage image;
                if (image.loadFromData(QByteArray::fromBase64(binaryData.text().toLatin1()))) {
                    KoImageData *data = m_imageCollection->createImageData(image);
                    setUserData(data);
                }
            }
        }
    }

    loadText(element, context);

    return true;
}

KoImageCollection *PictureShape::imageCollection() const
{
    return m_imageCollection;
}

QString PictureShape::saveStyle(KoGenStyle& style, KoShapeSavingContext& context) const
{
    if(transparency() > 0.0) {
        style.addProperty("draw:image-opacity", QString("%1%").arg((1.0 - transparency()) * 100.0));
    }
    
    return KoShape::saveStyle(style, context);
}

void PictureShape::loadStyle(const KoXmlElement& element, KoShapeLoadingContext& context)
{
    KoShape::loadStyle(element, context);
    KoStyleStack &styleStack = context.odfLoadingContext().styleStack();
    styleStack.setTypeProperties("graphic");

    //FIXME: are there other applicable properties?
    if (styleStack.hasProperty(KoXmlNS::draw, "color-mode")) {
        QString colorMode = styleStack.property(KoXmlNS::draw, "color-mode");
        if (colorMode == "greyscale") {
            setMode(Greyscale);
        }
        else if (colorMode == "mono") {
            setMode(Mono);
        }
        else if (colorMode == "watermark") {
            setMode(Watermark);
        }
    }
    
    const QString opacity(styleStack.property(KoXmlNS::draw, "image-opacity"));
    
    if (! opacity.isEmpty() && opacity.right(1) == "%") {
        setTransparency(1.0 - (opacity.left(opacity.length() - 1).toFloat() / 100.0));
    }
}

PictureShape::PictureMode PictureShape::mode() const
{
    return m_mode;
}

void PictureShape::setMode(PictureShape::PictureMode mode)
{
    if( mode != m_mode ) {
        m_mode = mode;
        KoFilterEffect* filterMode = filterEffectStack()->takeFilterEffect(0);
        delete filterMode;
        switch( mode ) {
            case Greyscale:
                filterMode = new GreyscaleFilterEffect();
                break;
            case Mono:
                filterMode = new MonoFilterEffect();
                break;
            default:
                filterMode = new WatermarkFilterEffect();
                break;
        }
        if( filterMode )
            filterEffectStack()->appendFilterEffect(filterMode);
        update();
    }
}

bool PictureShape::saveSvg(SvgSavingContext &context)
{
    KoImageData *imageData = qobject_cast<KoImageData*>(userData());
    if (!imageData) {
        qWarning() << "Picture has no image data. Omitting.";
        return false;
    }

    context.shapeWriter().startElement("image");
    context.shapeWriter().addAttribute("id", context.getID(this));

    QTransform m = transformation();
    if (m.type() == QTransform::TxTranslate) {
        const QPointF pos = position();
        context.shapeWriter().addAttributePt("x", pos.x());
        context.shapeWriter().addAttributePt("y", pos.y());
    } else {
        context.shapeWriter().addAttribute("transform", SvgUtil::transformToString(m));
    }

    const QSizeF s = size();
    context.shapeWriter().addAttributePt("width", s.width());
    context.shapeWriter().addAttributePt("height", s.height());
    context.shapeWriter().addAttribute("xlink:href", context.saveImage(imageData));
    context.shapeWriter().endElement();

    return true;
}

bool PictureShape::loadSvg(const KoXmlElement &element, SvgLoadingContext &context)
{
    const qreal x = SvgUtil::parseUnitX(context.currentGC(), element.attribute("x", "0"));
    const qreal y = SvgUtil::parseUnitY(context.currentGC(), element.attribute("y", "0"));
    const qreal w = SvgUtil::parseUnitX(context.currentGC(), element.attribute("width", "0"));
    const qreal h = SvgUtil::parseUnitY(context.currentGC(), element.attribute("height", "0"));

    // zero width of height disables rendering this image (see svg spec)
    if (w == 0.0 || h == 0.0)
        return 0;

    const QString href = element.attribute("xlink:href");

    QImage image;

    if (href.startsWith(QLatin1String("data:"))) {
        int start = href.indexOf("base64,");
        if (start <= 0)
            return false;
        if(!image.loadFromData(QByteArray::fromBase64(href.mid(start + 7).toLatin1())))
            return false;
    } else if (!image.load(context.absoluteFilePath(href))) {
        return false;
    }

    KoImageCollection *imageCollection = context.imageCollection();
    if (!imageCollection)
        return false;

    // TODO use it already for loading
    KoImageData *data = imageCollection->createImageData(image);

    setUserData(data);
    setSize(QSizeF(w, h));
    setPosition(QPointF(x, y));
    return true;
}
