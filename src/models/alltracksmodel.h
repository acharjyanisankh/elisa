/*
 * Copyright 2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

#ifndef ALLTRACKSMODEL_H
#define ALLTRACKSMODEL_H

#include "elisaLib_export.h"

#include <QAbstractItemModel>

#include "musicaudiotrack.h"

#include <memory>

class AllTracksModelPrivate;

class ELISALIB_EXPORT AllTracksModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    explicit AllTracksModel(QObject *parent = nullptr);

    ~AllTracksModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public Q_SLOTS:

    void tracksAdded(const QList<MusicAudioTrack> &allTracks);

    void trackRemoved(qulonglong removedTrackId);

    void trackModified(const MusicAudioTrack &modifiedTrack);

private:

    std::unique_ptr<AllTracksModelPrivate> d;

};

#endif // ALLTRACKSMODEL_H
