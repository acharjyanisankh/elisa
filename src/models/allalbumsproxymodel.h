/*
 * Copyright 2016-2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
 * Copyright 2017 Alexander Stippich <a.stippich@gmx.net>
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

#ifndef ALLALBUMSPROXYMODEL_H
#define ALLALBUMSPROXYMODEL_H

#include "elisaLib_export.h"

#include "abstractmediaproxymodel.h"
#include "elisautils.h"

class MusicAlbum;

class ELISALIB_EXPORT AllAlbumsProxyModel : public AbstractMediaProxyModel
{

    Q_OBJECT

public:

    explicit AllAlbumsProxyModel(QObject *parent = nullptr);

    ~AllAlbumsProxyModel() override;

Q_SIGNALS:

    void albumToEnqueue(QList<MusicAlbum> newAlbums,
                        ElisaUtils::PlayListEnqueueMode enqueueMode,
                        ElisaUtils::PlayListEnqueueTriggerPlay triggerPlay);

public Q_SLOTS:

    void enqueueToPlayList();

    void replaceAndPlayOfPlayList();

protected:

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

};

#endif // ALLALBUMSPROXYMODEL_H
