/*
 * Copyright 2015-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "musicalbum.h"
#include "musicaudiotrack.h"
#include "databaseinterface.h"
#include "models/allartistsmodel.h"
#include "qabstractitemmodeltester.h"

#include <QObject>
#include <QUrl>
#include <QString>
#include <QHash>
#include <QVector>
#include <QThread>
#include <QMetaObject>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include <QDebug>

#include <QtTest>

class AllArtistsModelTests: public QObject
{
    Q_OBJECT

private:

    QList<MusicAudioTrack> mNewTracks = {
        {true, QStringLiteral("$1"), QStringLiteral("0"), QStringLiteral("track1"),
         QStringLiteral("artist1"), QStringLiteral("album1"), QStringLiteral("Various Artists"),
         1, 1, QTime::fromMSecsSinceStartOfDay(1), {QUrl::fromLocalFile(QStringLiteral("/$1"))}, QDateTime::fromMSecsSinceEpoch(1),
         QUrl::fromLocalFile(QStringLiteral("album1")), 1, false,
         QStringLiteral("genre1"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$2"), QStringLiteral("0"), QStringLiteral("track2"),
         QStringLiteral("artist2"), QStringLiteral("album1"), QStringLiteral("Various Artists"),
         2, 2, QTime::fromMSecsSinceStartOfDay(2), {QUrl::fromLocalFile(QStringLiteral("/$2"))}, QDateTime::fromMSecsSinceEpoch(2),
         QUrl::fromLocalFile(QStringLiteral("album1")), 2, false,
         QStringLiteral("genre2"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$3"), QStringLiteral("0"), QStringLiteral("track3"),
         QStringLiteral("artist3"), QStringLiteral("album1"), QStringLiteral("Various Artists"),
         3, 3, QTime::fromMSecsSinceStartOfDay(3), {QUrl::fromLocalFile(QStringLiteral("/$3"))}, QDateTime::fromMSecsSinceEpoch(3),
         QUrl::fromLocalFile(QStringLiteral("album1")), 3, false,
         QStringLiteral("genre3"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$4"), QStringLiteral("0"), QStringLiteral("track4"),
         QStringLiteral("artist4"), QStringLiteral("album1"), QStringLiteral("Various Artists"),
         4, 4, QTime::fromMSecsSinceStartOfDay(4), {QUrl::fromLocalFile(QStringLiteral("/$4"))}, QDateTime::fromMSecsSinceEpoch(4),
         QUrl::fromLocalFile(QStringLiteral("album1")), 4, false,
         QStringLiteral("genre4"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$4"), QStringLiteral("0"), QStringLiteral("track4"),
         QStringLiteral("artist4"), QStringLiteral("album1"), QStringLiteral("Various Artists"),
         4, 4, QTime::fromMSecsSinceStartOfDay(4), {QUrl::fromLocalFile(QStringLiteral("/$4Bis"))}, QDateTime::fromMSecsSinceEpoch(4),
         QUrl::fromLocalFile(QStringLiteral("album1")), 4, false,
         QStringLiteral("genre4"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$5"), QStringLiteral("0"), QStringLiteral("track1"),
         QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"),
         1, 1, QTime::fromMSecsSinceStartOfDay(5), {QUrl::fromLocalFile(QStringLiteral("/$5"))}, QDateTime::fromMSecsSinceEpoch(5),
         QUrl::fromLocalFile(QStringLiteral("album2")), 4, true,
         QStringLiteral("genre1"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$6"), QStringLiteral("0"), QStringLiteral("track2"),
         QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"),
         2, 1, QTime::fromMSecsSinceStartOfDay(6), {QUrl::fromLocalFile(QStringLiteral("/$6"))}, QDateTime::fromMSecsSinceEpoch(6),
         QUrl::fromLocalFile(QStringLiteral("album2")), 1, true,
         QStringLiteral("genre1"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$7"), QStringLiteral("0"), QStringLiteral("track3"),
         QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"),
         3, 1, QTime::fromMSecsSinceStartOfDay(7), {QUrl::fromLocalFile(QStringLiteral("/$7"))}, QDateTime::fromMSecsSinceEpoch(7),
         QUrl::fromLocalFile(QStringLiteral("album2")), 5, true,
         QStringLiteral("genre2"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$8"), QStringLiteral("0"), QStringLiteral("track4"),
         QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"),
         4, 1, QTime::fromMSecsSinceStartOfDay(8), {QUrl::fromLocalFile(QStringLiteral("/$8"))}, QDateTime::fromMSecsSinceEpoch(8),
         QUrl::fromLocalFile(QStringLiteral("album2")), 2, true,
         QStringLiteral("genre2"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$9"), QStringLiteral("0"), QStringLiteral("track5"),
         QStringLiteral("artist1"), QStringLiteral("album2"), QStringLiteral("artist1"),
         5, 1, QTime::fromMSecsSinceStartOfDay(9), {QUrl::fromLocalFile(QStringLiteral("/$9"))}, QDateTime::fromMSecsSinceEpoch(9),
         QUrl::fromLocalFile(QStringLiteral("album2")), 3, true,
         QStringLiteral("genre2"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$10"), QStringLiteral("0"), QStringLiteral("track6"),
         QStringLiteral("artist1 and artist2"), QStringLiteral("album2"), QStringLiteral("artist1"),
         6, 1, QTime::fromMSecsSinceStartOfDay(10), {QUrl::fromLocalFile(QStringLiteral("/$10"))}, QDateTime::fromMSecsSinceEpoch(10),
         QUrl::fromLocalFile(QStringLiteral("album2")), 5, true,
         QStringLiteral("genre2"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$11"), QStringLiteral("0"), QStringLiteral("track1"),
         QStringLiteral("artist2"), QStringLiteral("album3"), QStringLiteral("artist2"),
         1, 1, QTime::fromMSecsSinceStartOfDay(11), {QUrl::fromLocalFile(QStringLiteral("/$11"))}, QDateTime::fromMSecsSinceEpoch(11),
         QUrl::fromLocalFile(QStringLiteral("album3")), 1, true,
         QStringLiteral("genre3"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$12"), QStringLiteral("0"), QStringLiteral("track2"),
         QStringLiteral("artist2"), QStringLiteral("album3"), QStringLiteral("artist2"),
         2, 1, QTime::fromMSecsSinceStartOfDay(12), {QUrl::fromLocalFile(QStringLiteral("/$12"))}, QDateTime::fromMSecsSinceEpoch(12),
         QUrl::fromLocalFile(QStringLiteral("album3")), 2, true,
         QStringLiteral("genre3"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$13"), QStringLiteral("0"), QStringLiteral("track3"),
         QStringLiteral("artist2"), QStringLiteral("album3"), QStringLiteral("artist2"),
         3, 1, QTime::fromMSecsSinceStartOfDay(13), {QUrl::fromLocalFile(QStringLiteral("/$13"))}, QDateTime::fromMSecsSinceEpoch(13),
         QUrl::fromLocalFile(QStringLiteral("album3")), 3, true,
         QStringLiteral("genre3"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$14"), QStringLiteral("0"), QStringLiteral("track1"),
         QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"),
         1, 1, QTime::fromMSecsSinceStartOfDay(14), {QUrl::fromLocalFile(QStringLiteral("/$14"))}, QDateTime::fromMSecsSinceEpoch(14),
         QUrl::fromLocalFile(QStringLiteral("album4")), 4, true,
         QStringLiteral("genre4"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$15"), QStringLiteral("0"), QStringLiteral("track2"),
         QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"),
         2, 1, QTime::fromMSecsSinceStartOfDay(15), {QUrl::fromLocalFile(QStringLiteral("/$15"))}, QDateTime::fromMSecsSinceEpoch(15),
         QUrl::fromLocalFile(QStringLiteral("album4")), 5, true,
         QStringLiteral("genre4"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$16"), QStringLiteral("0"), QStringLiteral("track3"),
         QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"),
         3, 1, QTime::fromMSecsSinceStartOfDay(16), {QUrl::fromLocalFile(QStringLiteral("/$16"))}, QDateTime::fromMSecsSinceEpoch(16),
         QUrl::fromLocalFile(QStringLiteral("album4")), 1, true,
         QStringLiteral("genre4"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$17"), QStringLiteral("0"), QStringLiteral("track4"),
         QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"),
         4, 1, QTime::fromMSecsSinceStartOfDay(17), {QUrl::fromLocalFile(QStringLiteral("/$17"))}, QDateTime::fromMSecsSinceEpoch(17),
         QUrl::fromLocalFile(QStringLiteral("album4")), 2, true,
         QStringLiteral("genre4"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$18"), QStringLiteral("0"), QStringLiteral("track5"),
         QStringLiteral("artist2"), QStringLiteral("album4"), QStringLiteral("artist2"),
         5, 1, QTime::fromMSecsSinceStartOfDay(18), {QUrl::fromLocalFile(QStringLiteral("/$18"))}, QDateTime::fromMSecsSinceEpoch(18),
         QUrl::fromLocalFile(QStringLiteral("album4")), 3, true,
         QStringLiteral("genre3"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$19"), QStringLiteral("0"), QStringLiteral("track1"),
         QStringLiteral("artist7"), QStringLiteral("album3"), QStringLiteral("artist7"),
         1, 1, QTime::fromMSecsSinceStartOfDay(19), {QUrl::fromLocalFile(QStringLiteral("/$19"))}, QDateTime::fromMSecsSinceEpoch(19),
         QUrl::fromLocalFile(QStringLiteral("album3")), 1, true,
         QStringLiteral("genre2"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$20"), QStringLiteral("0"), QStringLiteral("track2"),
         QStringLiteral("artist7"), QStringLiteral("album3"), QStringLiteral("artist7"),
         2, 1, QTime::fromMSecsSinceStartOfDay(20), {QUrl::fromLocalFile(QStringLiteral("/$20"))}, QDateTime::fromMSecsSinceEpoch(20),
         QUrl::fromLocalFile(QStringLiteral("album3")), 2, true,
         QStringLiteral("genre1"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$21"), QStringLiteral("0"), QStringLiteral("track3"),
         QStringLiteral("artist7"), QStringLiteral("album3"), QStringLiteral("artist7"),
         3, 1, QTime::fromMSecsSinceStartOfDay(21), {QUrl::fromLocalFile(QStringLiteral("/$21"))}, QDateTime::fromMSecsSinceEpoch(21),
         QUrl::fromLocalFile(QStringLiteral("album3")), 3, true,
         QStringLiteral("genre1"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
        {true, QStringLiteral("$22"), QStringLiteral("0"), QStringLiteral("track9"),
         QStringLiteral("artist2"), QStringLiteral("album3"), QStringLiteral("artist7"),
         9, 1, QTime::fromMSecsSinceStartOfDay(22), {QUrl::fromLocalFile(QStringLiteral("/$22"))}, QDateTime::fromMSecsSinceEpoch(22),
         QUrl::fromLocalFile(QStringLiteral("album3")), 9, true,
         QStringLiteral("genre1"), QStringLiteral("composer1"), QStringLiteral("lyricist1")},
    };

    QHash<QString, QUrl> mNewCovers = {
        {QStringLiteral("file:///$1"), QUrl::fromLocalFile(QStringLiteral("album1"))},
        {QStringLiteral("file:///$2"), QUrl::fromLocalFile(QStringLiteral("album1"))},
        {QStringLiteral("file:///$3"), QUrl::fromLocalFile(QStringLiteral("album1"))},
        {QStringLiteral("file:///$4"), QUrl::fromLocalFile(QStringLiteral("album1"))},
        {QStringLiteral("file:///$4Bis"), QUrl::fromLocalFile(QStringLiteral("album1"))},
        {QStringLiteral("file:///$5"), QUrl::fromLocalFile(QStringLiteral("album2"))},
        {QStringLiteral("file:///$6"), QUrl::fromLocalFile(QStringLiteral("album2"))},
        {QStringLiteral("file:///$7"), QUrl::fromLocalFile(QStringLiteral("album2"))},
        {QStringLiteral("file:///$8"), QUrl::fromLocalFile(QStringLiteral("album2"))},
        {QStringLiteral("file:///$9"), QUrl::fromLocalFile(QStringLiteral("album2"))},
        {QStringLiteral("file:///$0"), QUrl::fromLocalFile(QStringLiteral("album2"))},
        {QStringLiteral("file:///$11"), QUrl::fromLocalFile(QStringLiteral("album3"))},
        {QStringLiteral("file:///$12"), QUrl::fromLocalFile(QStringLiteral("album3"))},
        {QStringLiteral("file:///$13"), QUrl::fromLocalFile(QStringLiteral("album3"))},
        {QStringLiteral("file:///$14"), QUrl::fromLocalFile(QStringLiteral("album4"))},
        {QStringLiteral("file:///$15"), QUrl::fromLocalFile(QStringLiteral("album4"))},
        {QStringLiteral("file:///$16"), QUrl::fromLocalFile(QStringLiteral("album4"))},
        {QStringLiteral("file:///$17"), QUrl::fromLocalFile(QStringLiteral("album4"))},
        {QStringLiteral("file:///$18"), QUrl::fromLocalFile(QStringLiteral("album4"))},
        {QStringLiteral("file:///$19"), QUrl::fromLocalFile(QStringLiteral("album3"))},
        {QStringLiteral("file:///$20"), QUrl::fromLocalFile(QStringLiteral("album3"))},
        {QStringLiteral("file:///$21"), QUrl::fromLocalFile(QStringLiteral("album3"))},
        {QStringLiteral("file:///$22"), QUrl::fromLocalFile(QStringLiteral("album3"))},
    };

private Q_SLOTS:

    void initTestCase()
    {
        qRegisterMetaType<QHash<qulonglong,int>>("QHash<qulonglong,int>");
        qRegisterMetaType<QHash<QString,QUrl>>("QHash<QString,QUrl>");
        qRegisterMetaType<QHash<QString,QVector<MusicAudioTrack>>>("QHash<QString,QVector<MusicAudioTrack>>");
        qRegisterMetaType<QVector<qlonglong>>("QVector<qlonglong>");
        qRegisterMetaType<QHash<qlonglong,int>>("QHash<qlonglong,int>");
        qRegisterMetaType<MusicArtist>("MusicArtist");
    }

    void removeOneArtist()
    {
        DatabaseInterface musicDb;
        AllArtistsModel artistsModel;
        QAbstractItemModelTester testModel(&artistsModel);

        connect(&musicDb, &DatabaseInterface::artistsAdded,
                &artistsModel, &AllArtistsModel::artistsAdded);
        connect(&musicDb, &DatabaseInterface::artistModified,
                &artistsModel, &AllArtistsModel::artistModified);
        connect(&musicDb, &DatabaseInterface::artistRemoved,
                &artistsModel, &AllArtistsModel::artistRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&artistsModel, &AllArtistsModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&artistsModel, &AllArtistsModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&artistsModel, &AllArtistsModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&artistsModel, &AllArtistsModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&artistsModel, &AllArtistsModel::dataChanged);

        QCOMPARE(artistsModel.rowCount(), 0);
        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        auto newFiles = QList<QUrl>();
        const auto &constNewTracks = mNewTracks;
        for (const auto &oneTrack : constNewTracks) {
            newFiles.push_back(oneTrack.resourceURI());
        }

        musicDb.insertTracksList(mNewTracks, mNewCovers, QStringLiteral("autoTest"));

        QCOMPARE(artistsModel.rowCount(), 7);
        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        auto trackId = musicDb.trackIdFromTitleAlbumTrackDiscNumber(QStringLiteral("track6"), QStringLiteral("artist1 and artist2"), QStringLiteral("album2"), 6, 1);

        auto firstTrack = musicDb.trackFromDatabaseId(trackId);

        musicDb.removeTracksList({firstTrack.resourceURI()});

        QCOMPARE(artistsModel.rowCount(), 6);
        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 1);
        QCOMPARE(endRemoveRowsSpy.count(), 1);
        QCOMPARE(dataChangedSpy.count(), 0);
    }

    void addOneArtist()
    {
        DatabaseInterface musicDb;
        AllArtistsModel artistsModel;
        QAbstractItemModelTester testModel(&artistsModel);

        connect(&musicDb, &DatabaseInterface::artistsAdded,
                &artistsModel, &AllArtistsModel::artistsAdded);
        connect(&musicDb, &DatabaseInterface::artistModified,
                &artistsModel, &AllArtistsModel::artistModified);
        connect(&musicDb, &DatabaseInterface::artistRemoved,
                &artistsModel, &AllArtistsModel::artistRemoved);

        musicDb.init(QStringLiteral("testDb"));

        QSignalSpy beginInsertRowsSpy(&artistsModel, &AllArtistsModel::rowsAboutToBeInserted);
        QSignalSpy endInsertRowsSpy(&artistsModel, &AllArtistsModel::rowsInserted);
        QSignalSpy beginRemoveRowsSpy(&artistsModel, &AllArtistsModel::rowsAboutToBeRemoved);
        QSignalSpy endRemoveRowsSpy(&artistsModel, &AllArtistsModel::rowsRemoved);
        QSignalSpy dataChangedSpy(&artistsModel, &AllArtistsModel::dataChanged);

        QCOMPARE(artistsModel.rowCount(), 0);
        QCOMPARE(beginInsertRowsSpy.count(), 0);
        QCOMPARE(endInsertRowsSpy.count(), 0);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        auto newFiles = QList<QUrl>();
        const auto &constNewTracks = mNewTracks;
        for (const auto &oneTrack : constNewTracks) {
            newFiles.push_back(oneTrack.resourceURI());
        }

        musicDb.insertTracksList(mNewTracks, mNewCovers, QStringLiteral("autoTest"));

        QCOMPARE(artistsModel.rowCount(), 7);
        QCOMPARE(beginInsertRowsSpy.count(), 1);
        QCOMPARE(endInsertRowsSpy.count(), 1);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);

        auto newTrack = MusicAudioTrack{true, QStringLiteral("$23"), QStringLiteral("0"), QStringLiteral("track23"),
                QStringLiteral("artist6"), QStringLiteral("album4"), QStringLiteral("Various Artists"), 23, 1, QTime::fromMSecsSinceStartOfDay(23),
        {QUrl::fromLocalFile(QStringLiteral("/$23"))},
                QDateTime::fromMSecsSinceEpoch(23),
        {QUrl::fromLocalFile(QStringLiteral("file://image$23"))}, 5, true,
        {}, QStringLiteral("composer1"), QStringLiteral("lyricist1")};
        auto newTracks = QList<MusicAudioTrack>();
        newTracks.push_back(newTrack);

        auto newFiles2 = QList<QUrl>();
        for (const auto &oneTrack : newTracks) {
            newFiles2.push_back(oneTrack.resourceURI());
        }

        musicDb.insertTracksList(newTracks, mNewCovers, QStringLiteral("autoTest"));

        QCOMPARE(artistsModel.rowCount(), 8);
        QCOMPARE(beginInsertRowsSpy.count(), 2);
        QCOMPARE(endInsertRowsSpy.count(), 2);
        QCOMPARE(beginRemoveRowsSpy.count(), 0);
        QCOMPARE(endRemoveRowsSpy.count(), 0);
        QCOMPARE(dataChangedSpy.count(), 0);
    }
};

QTEST_GUILESS_MAIN(AllArtistsModelTests)


#include "allartistsmodeltest.moc"
