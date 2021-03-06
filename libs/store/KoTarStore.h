/* This file is part of the KDE project
   Copyright (C) 2002 David Faure <faure@kde.org>

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

#ifndef koTarStore_h
#define koTarStore_h

#include "KoStore.h"

#include <QByteArray>

class KTar;
class KArchiveDirectory;
class QUrl;

class KoTarStore : public KoStore
{
public:
    KoTarStore(const QString & _filename, Mode _mode, const QByteArray & appIdentification,
               bool writeMimetype);
    KoTarStore(QIODevice *dev, Mode mode, const QByteArray & appIdentification,
               bool writeMimetype);
    /**
     * QUrl-constructor
     * @todo saving not completely implemented (fixed temporary file)
     */
    KoTarStore(QWidget* window, const QUrl &url, const QString & _filename, Mode _mode,
               const QByteArray & appIdentification, bool writeMimetype = true);
    ~KoTarStore() override;

    QStringList directoryList() const override;

protected:
    void init(const QByteArray &appIdentification);
    bool doFinalize() override;
    bool openWrite(const QString& name) override;
    bool openRead(const QString& name) override;
    bool closeWrite() override;
    bool closeRead() override {
        return true;
    }
    bool enterRelativeDirectory(const QString& dirName) override;
    bool enterAbsoluteDirectory(const QString& path) override;
    bool fileExists(const QString& absPath) const override;

    static QByteArray completeMagic(const QByteArray& appMimetype);

    /// The tar archive
    KTar * m_pTar;

    /** In "Read" mode this pointer is pointing to the
    current directory in the archive to speed up the verification process */
    const KArchiveDirectory* m_currentDir;

    /// Buffer used when writing
    QByteArray m_byteArray;
private:
    Q_DECLARE_PRIVATE(KoStore)
};

#endif
