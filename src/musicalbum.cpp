/*
 * Copyright 2016-2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

#include <algorithm>

#include <QList>
#include <QString>
#include <QUrl>
#include <QMap>
#include <QSharedData>

#include <QDebug>

class MusicAlbumPrivate : public QSharedData
{
public:

    qulonglong mDatabaseId = 0;

    QString mId;

    QString mParentId;

    QString mTitle;

    QString mArtist;

    QUrl mAlbumPath;

    QUrl mAlbumArtURI;

    QUrl mResourceURI;

    QList<MusicAudioTrack> mTracks;

    QStringList mGenres;

    int mTracksCount = 0;

    bool mIsValid = false;

    bool mIsSingleDiscAlbum = true;

};

MusicAlbum::MusicAlbum() : d(new MusicAlbumPrivate())
{
}

MusicAlbum::MusicAlbum(MusicAlbum &&other) noexcept = default;

MusicAlbum::MusicAlbum(const MusicAlbum &other) = default;

MusicAlbum& MusicAlbum::operator=(MusicAlbum &&other) noexcept = default;

MusicAlbum& MusicAlbum::operator=(const MusicAlbum &other) = default;

MusicAlbum::~MusicAlbum()
= default;

void MusicAlbum::setValid(bool value)
{
    d->mIsValid = value;
}

bool MusicAlbum::isValid() const
{
    return d->mIsValid;
}

void MusicAlbum::setDatabaseId(qulonglong value)
{
    d->mDatabaseId = value;
}

qulonglong MusicAlbum::databaseId() const
{
    return d->mDatabaseId;
}

void MusicAlbum::setId(const QString &value)
{
    d->mId = value;
}

QString MusicAlbum::id() const
{
    return d->mId;
}

void MusicAlbum::setParentId(const QString &value)
{
    d->mParentId = value;
}

QString MusicAlbum::parentId() const
{
    return d->mParentId;
}

void MusicAlbum::setTracksCount(int count)
{
    d->mTracksCount = count;
}

int MusicAlbum::tracksCount() const
{
    return d->mTracks.size();
}

void MusicAlbum::setTitle(const QString &value)
{
    d->mTitle = value;
}

QString MusicAlbum::title() const
{
    return d->mTitle;
}

void MusicAlbum::setArtist(const QString &value)
{
    d->mArtist = value;
}

QString MusicAlbum::artist() const
{
    QString result;

    if (!d->mArtist.isEmpty()) {
        result = d->mArtist;
    } else {
        QString possibleAlbumArtist;
        for (const auto &oneTrack : qAsConst(d->mTracks)) {
            if (oneTrack.isValidAlbumArtist()) {
                result = oneTrack.albumArtist();
                break;
            }
            if (possibleAlbumArtist.isEmpty()) {
                possibleAlbumArtist = oneTrack.artist();
            } else {
                if (possibleAlbumArtist != oneTrack.artist()) {
                    possibleAlbumArtist.clear();
                    break;
                }
            }
        }

        if (result.isEmpty() && d->mTracks.size() > 1) {
            if (possibleAlbumArtist.isEmpty()) {
                result = QStringLiteral("Various Artists");
            } else {
                result = possibleAlbumArtist;
            }
        } else if (result.isEmpty() && d->mTracks.size() == 1) {
            result = d->mTracks.constFirst().albumArtist();
        }
    }

    return result;
}

void MusicAlbum::setAlbumArtURI(const QUrl &value)
{
    d->mAlbumArtURI = value;
}

QUrl MusicAlbum::albumArtURI() const
{
    return d->mAlbumArtURI;
}

void MusicAlbum::setResourceURI(const QUrl &value)
{
    d->mResourceURI = value;
}

QUrl MusicAlbum::resourceURI() const
{
    return d->mResourceURI;
}

void MusicAlbum::setIsSingleDiscAlbum(bool value)
{
    d->mIsSingleDiscAlbum = value;
}

bool MusicAlbum::isSingleDiscAlbum() const
{
    return d->mIsSingleDiscAlbum;
}

void MusicAlbum::setGenres(const QStringList &aGenres)
{
    d->mGenres = aGenres;
}

QStringList MusicAlbum::genres() const
{
    return d->mGenres;
}

void MusicAlbum::setTracks(const QList<MusicAudioTrack> &allTracks)
{
    d->mTracks = allTracks;
}

const MusicAudioTrack& MusicAlbum::trackFromIndex(int index) const
{
    return d->mTracks[index];
}

qulonglong MusicAlbum::trackIdFromIndex(int index) const
{
    return d->mTracks[index].databaseId();
}

int MusicAlbum::trackIndexFromId(qulonglong id) const
{
    int result = -1;

    for (result = 0; result < d->mTracks.size(); ++result) {
        if (d->mTracks[result].databaseId() == id) {
            return result;
        }
    }

    result = -1;

    return result;
}

QStringList MusicAlbum::allArtists() const
{
    auto result = QList<QString>();

    result.reserve(d->mTracks.size() + 1);

    result.push_back(d->mArtist);

    const auto &allTracks = d->mTracks;
    for (const auto &oneTrack : allTracks) {
        result.push_back(oneTrack.artist());
    }

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

QStringList MusicAlbum::allTracksTitle() const
{
    auto result = QList<QString>();

    result.reserve(d->mTracks.size());

    const auto &allTracks = d->mTracks;
    for (const auto &oneTrack : allTracks) {
        result.push_back(oneTrack.title());
    }

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

bool MusicAlbum::isEmpty() const
{
    return d->mTracks.isEmpty();
}

void MusicAlbum::removeTrackFromIndex(int index)
{
    if (index < 0 || index >= tracksCount()) {
        return;
    }

    --d->mTracksCount;
    d->mTracks.removeAt(index);
}

void MusicAlbum::insertTrack(const MusicAudioTrack &newTrack, int index)
{
    d->mTracks.insert(index, newTrack);
    ++d->mTracksCount;
}

void MusicAlbum::updateTrack(const MusicAudioTrack &modifiedTrack, int index)
{
    d->mTracks[index] = modifiedTrack;
}

ELISALIB_EXPORT QDebug operator<<(QDebug stream, const MusicAlbum &data)
{
    stream << "title:" << data.title()
           << "databaseId:" << data.databaseId()
           << "id:" << data.id()
           << "parentId:" << data.parentId()
           << "artist:" << data.artist()
           << "albumPath:" << data.albumPath()
           << "albumArtURI:" << data.albumArtURI()
           << "tracks:" << data.tracksCount()
           << "isSingleDiscAlbum:" << (data.isSingleDiscAlbum() ? "true": "false")
           << "isValidArtist:" << (data.isValidArtist() ? "true": "false");

    return stream;
}

ELISALIB_EXPORT bool operator==(const MusicAlbum &album1, const MusicAlbum &album2)
{
    return album1.title() == album2.title() && (
            (album1.isValidArtist() && album2.isValidArtist() && album1.artist() == album2.artist()) ||
             !album1.isValidArtist() || !album2.isValidArtist());
}

int MusicAlbum::highestTrackRating() const
{
    int result = 0;

    const auto &allTracks = d->mTracks;
    for (const auto &oneTrack : allTracks) {
        result = std::max(result, oneTrack.rating());
    }

    return result;
}

bool MusicAlbum::isValidArtist() const
{
    return !d->mArtist.isEmpty();
}

bool MusicAlbum::canUpdateArtist(const MusicAudioTrack &currentTrack) const
{
    bool result = false;

    if (currentTrack.isValidAlbumArtist() && currentTrack.albumArtist() != artist()) {
        result = true;
    }

    return result;
}

void MusicAlbum::setAlbumPath(const QUrl &value)
{
    d->mAlbumPath = value;
}

QUrl MusicAlbum::albumPath() const
{
    return d->mAlbumPath;
}
