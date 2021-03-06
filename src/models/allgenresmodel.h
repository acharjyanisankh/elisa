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

#ifndef ALLGENRESMODEL_H
#define ALLGENRESMODEL_H

#include "elisaLib_export.h"

#include <QAbstractItemModel>
#include <QVector>
#include <QHash>
#include <QString>

#include <memory>

class DatabaseInterface;
class AllGenresModelPrivate;
class MusicAudioGenre;

class ELISALIB_EXPORT AllGenresModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    explicit AllGenresModel(QObject *parent = nullptr);

    ~AllGenresModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

Q_SIGNALS:

    void allAlbumsChanged();

public Q_SLOTS:

    void genresAdded(const QList<MusicAudioGenre> &newGenres);

    void genreRemoved(const MusicAudioGenre &removedGenre);

    void genreModified(const MusicAudioGenre &modifiedGenre);

private:

    std::unique_ptr<AllGenresModelPrivate> d;

};

#endif // ALLGENRESMODEL_H
