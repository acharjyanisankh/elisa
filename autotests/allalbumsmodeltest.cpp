/*
 * Copyright 2015-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
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

#include "musicalbum.h"
#include "musicaudiotrack.h"
#include "databaseinterface.h"
#include "allalbumsmodel.h"

#include <QObject>
#include <QUrl>
#include <QString>
#include <QHash>
#include <QVector>
#include <QThread>
#include <QMetaObject>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include <QDebug>

#include <QtTest/QtTest>

class AllAlbumsModelTests: public QObject
{
    Q_OBJECT

private:

    QHash<QString, QVector<MusicAudioTrack>> mNewTracks;
    QHash<QString, QUrl> mNewCovers;

private Q_SLOTS:

    void initTestCase()
    {
        mNewTracks[QStringLiteral("album1")] = {
            {true, QStringLiteral("$1"), QStringLiteral("0"), QStringLiteral("track1"),
                QStringLiteral("artist1"), QStringLiteral("album1"), QStringLiteral("Various Artists"), 1, 1, QTime::fromMSecsSinceStartOfDay(1), {QUrl::fromLocalFile(QStringLiteral("/$1"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$1"))}},
            {true, QStringLiteral("$2"), QStringLiteral("0"), QStringLiteral("track2"),
                QStringLiteral("artist2"), QStringLiteral("album1"), QStringLiteral("Various Artists"), 2, 2, QTime::fromMSecsSinceStartOfDay(2), {QUrl::fromLocalFile(QStringLiteral("/$2"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$2"))}},
            {true, QStringLiteral("$3"), QStringLiteral("0"), QStringLiteral("track3"),
                QStringLiteral("artist3"), QStringLiteral("album1"), QStringLiteral("Various Artists"), 3, 3, QTime::fromMSecsSinceStartOfDay(3), {QUrl::fromLocalFile(QStringLiteral("/$3"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$3"))}},
            {true, QStringLiteral("$4"), QStringLiteral("0"), QStringLiteral("track4"),
                QStringLiteral("artist4"), QStringLiteral("album1"), QStringLiteral("Various Artists"), 4, 4, QTime::fromMSecsSinceStartOfDay(4), {QUrl::fromLocalFile(QStringLiteral("/$4"))},
                {QUrl::fromLocalFile(QStringLiteral("file://image$4"))}},
        };

        mNewTracks[QStringLiteral("album2")] = {
            {true, QStringLiteral("$5"), QStringLiteral("0"), QStringLiteral("track1"),
                QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"), 1, 1, QTime::fromMSecsSinceStartOfDay(5), {QUrl::fromLocalFile(QStringLiteral("/$5"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$5"))}},
            {true, QStringLiteral("$6"), QStringLiteral("0"), QStringLiteral("track2"),
                QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"), 2, 1, QTime::fromMSecsSinceStartOfDay(6), {QUrl::fromLocalFile(QStringLiteral("/$6"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$6"))}},
            {true, QStringLiteral("$7"), QStringLiteral("0"), QStringLiteral("track3"),
                QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"), 3, 1, QTime::fromMSecsSinceStartOfDay(7), {QUrl::fromLocalFile(QStringLiteral("/$7"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$7"))}},
            {true, QStringLiteral("$8"), QStringLiteral("0"), QStringLiteral("track4"),
                QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"), 4, 1, QTime::fromMSecsSinceStartOfDay(8), {QUrl::fromLocalFile(QStringLiteral("/$8"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$8"))}},
            {true, QStringLiteral("$9"), QStringLiteral("0"), QStringLiteral("track5"),
                QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"), 5, 1, QTime::fromMSecsSinceStartOfDay(9), {QUrl::fromLocalFile(QStringLiteral("/$9"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$9"))}},
            {true, QStringLiteral("$10"), QStringLiteral("0"), QStringLiteral("track6"),
                QStringLiteral("artist1 and artist2"), QStringLiteral("album2"), QStringLiteral("artist1"), 6, 1, QTime::fromMSecsSinceStartOfDay(10), {QUrl::fromLocalFile(QStringLiteral("/$10"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$10"))}}
        };

        mNewTracks[QStringLiteral("album3")] = {
            {true, QStringLiteral("$11"), QStringLiteral("0"), QStringLiteral("track1"),
                QStringLiteral("artist2"), QStringLiteral("album3"), QStringLiteral("artist2"), 1, 1, QTime::fromMSecsSinceStartOfDay(11), {QUrl::fromLocalFile(QStringLiteral("/$11"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$11"))}},
            {true, QStringLiteral("$12"), QStringLiteral("0"), QStringLiteral("track2"),
                QStringLiteral("artist2"), QStringLiteral("album3"), QStringLiteral("artist2"), 2, 1, QTime::fromMSecsSinceStartOfDay(12), {QUrl::fromLocalFile(QStringLiteral("/$12"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$12"))}},
            {true, QStringLiteral("$13"), QStringLiteral("0"), QStringLiteral("track3"),
                QStringLiteral("artist2"), QStringLiteral("album3"), QStringLiteral("artist2"), 3, 1, QTime::fromMSecsSinceStartOfDay(13), {QUrl::fromLocalFile(QStringLiteral("/$13"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$13"))}}
        };

        mNewTracks[QStringLiteral("album4")] = {
            {true, QStringLiteral("$14"), QStringLiteral("0"), QStringLiteral("track1"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral(""), 1, 1, QTime::fromMSecsSinceStartOfDay(14), {QUrl::fromLocalFile(QStringLiteral("/$14"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$14"))}},
            {true, QStringLiteral("$15"), QStringLiteral("0"), QStringLiteral("track2"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral(""), 2, 1, QTime::fromMSecsSinceStartOfDay(15), {QUrl::fromLocalFile(QStringLiteral("/$15"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$15"))}},
            {true, QStringLiteral("$16"), QStringLiteral("0"), QStringLiteral("track3"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral(""), 3, 1, QTime::fromMSecsSinceStartOfDay(16), {QUrl::fromLocalFile(QStringLiteral("/$16"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$16"))}},
            {true, QStringLiteral("$17"), QStringLiteral("0"), QStringLiteral("track4"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral(""), 4, 1, QTime::fromMSecsSinceStartOfDay(17), {QUrl::fromLocalFile(QStringLiteral("/$17"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$17"))}},
            {true, QStringLiteral("$18"), QStringLiteral("0"), QStringLiteral("track5"),
                QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral(""), 5, 1, QTime::fromMSecsSinceStartOfDay(18), {QUrl::fromLocalFile(QStringLiteral("/$18"))},
        {QUrl::fromLocalFile(QStringLiteral("file://image$18"))}}
        };

        mNewCovers[QStringLiteral("album1")] = QUrl::fromLocalFile(QStringLiteral("album1"));
        mNewCovers[QStringLiteral("album2")] = QUrl::fromLocalFile(QStringLiteral("album2"));
        mNewCovers[QStringLiteral("album3")] = QUrl::fromLocalFile(QStringLiteral("album3"));
        mNewCovers[QStringLiteral("album4")] = QUrl::fromLocalFile(QStringLiteral("album4"));

        qRegisterMetaType<QHash<qulonglong,int>>("QHash<qulonglong,int>");
        qRegisterMetaType<QHash<QString,QUrl>>("QHash<QString,QUrl>");
        qRegisterMetaType<QHash<QString,QVector<MusicAudioTrack>>>("QHash<QString,QVector<MusicAudioTrack>>");
        qRegisterMetaType<QVector<qlonglong>>("QVector<qlonglong>");
        qRegisterMetaType<QHash<qlonglong,int>>("QHash<qlonglong,int>");
        qRegisterMetaType<MusicArtist>("MusicArtist");
    }

    void removeOneTrack()
    {
        auto configDirectory = QDir(QStandardPaths::writableLocation(QStandardPaths::QStandardPaths::AppDataLocation));
        auto rootDirectory = QDir::root();
        rootDirectory.mkpath(configDirectory.path());
        auto fileName = configDirectory.filePath(QStringLiteral("elisaMusicDatabase.sqlite"));
        QFile dbFile(fileName);
        auto dbExists = dbFile.exists();

        if (dbExists) {
            QCOMPARE(dbFile.remove(), true);
        }

        DatabaseInterface musicDb;
        AllAlbumsModel albumsModel;

        connect(&musicDb, &DatabaseInterface::albumAdded,
                &albumsModel, &AllAlbumsModel::albumAdded);
        connect(&musicDb, &DatabaseInterface::albumModified,
                &albumsModel, &AllAlbumsModel::albumModified);
        connect(&musicDb, &DatabaseInterface::albumRemoved,
                &albumsModel, &AllAlbumsModel::albumRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&albumsModel, &AllAlbumsModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&albumsModel, &AllAlbumsModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&albumsModel, &AllAlbumsModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&albumsModel, &AllAlbumsModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&albumsModel, &AllAlbumsModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 3);
        QCOMPARE(endInsertRowsSpy.count(), 3);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        auto trackId = musicDb.trackIdFromTitleAlbumArtist(QStringLiteral("track1"), QStringLiteral("album1"), QStringLiteral("artist1"));

        auto firstTrack = musicDb.trackFromDatabaseId(trackId);

        musicDb.removeTracksList({firstTrack.resourceURI()});

        QCOMPARE(beginInsertRowsSpy.count(), 3);
        QCOMPARE(endInsertRowsSpy.count(), 3);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 1);
    }

    void removeOneAlbum()
    {
        auto configDirectory = QDir(QStandardPaths::writableLocation(QStandardPaths::QStandardPaths::AppDataLocation));
        auto rootDirectory = QDir::root();
        rootDirectory.mkpath(configDirectory.path());
        auto fileName = configDirectory.filePath(QStringLiteral("elisaMusicDatabase.sqlite"));
        QFile dbFile(fileName);
        auto dbExists = dbFile.exists();

        if (dbExists) {
            QCOMPARE(dbFile.remove(), true);
        }

        DatabaseInterface musicDb;
        AllAlbumsModel albumsModel;

        connect(&musicDb, &DatabaseInterface::albumAdded,
                &albumsModel, &AllAlbumsModel::albumAdded);
        connect(&musicDb, &DatabaseInterface::albumModified,
                &albumsModel, &AllAlbumsModel::albumModified);
        connect(&musicDb, &DatabaseInterface::albumRemoved,
                &albumsModel, &AllAlbumsModel::albumRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&albumsModel, &AllAlbumsModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&albumsModel, &AllAlbumsModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&albumsModel, &AllAlbumsModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&albumsModel, &AllAlbumsModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&albumsModel, &AllAlbumsModel::dataChanged);

        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        musicDb.insertTracksList(mNewTracks, mNewCovers);

        QCOMPARE(beginInsertRowsSpy.count(), 3);
        QCOMPARE(endInsertRowsSpy.count(), 3);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        auto firstTrackId = musicDb.trackIdFromTitleAlbumArtist(QStringLiteral("track1"), QStringLiteral("album1"), QStringLiteral("artist1"));
        auto firstTrack = musicDb.trackFromDatabaseId(firstTrackId);
        auto secondTrackId = musicDb.trackIdFromTitleAlbumArtist(QStringLiteral("track2"), QStringLiteral("album1"), QStringLiteral("artist2"));
        auto secondTrack = musicDb.trackFromDatabaseId(secondTrackId);
        auto thirdTrackId = musicDb.trackIdFromTitleAlbumArtist(QStringLiteral("track3"), QStringLiteral("album1"), QStringLiteral("artist3"));
        auto thirdTrack = musicDb.trackFromDatabaseId(thirdTrackId);
        auto fourthTrackId = musicDb.trackIdFromTitleAlbumArtist(QStringLiteral("track4"), QStringLiteral("album1"), QStringLiteral("artist4"));
        auto fourthTrack = musicDb.trackFromDatabaseId(fourthTrackId);

        musicDb.removeTracksList({firstTrack.resourceURI(), secondTrack.resourceURI(), thirdTrack.resourceURI(), fourthTrack.resourceURI()});

        QCOMPARE(beginInsertRowsSpy.count(), 3);
        QCOMPARE(endInsertRowsSpy.count(), 3);
        QCOMPARE(beginRemoveRowsSpy.count(), 1);
        QCOMPARE(endRemoveRowsSpy.count(), 1);
        QCOMPARE(dataChangedSpy.count(), 3);
    }
};

QTEST_MAIN(AllAlbumsModelTests)


#include "allalbumsmodeltest.moc"
