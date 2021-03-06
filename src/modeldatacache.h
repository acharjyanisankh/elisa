/*
 * Copyright 2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

#ifndef MODELDATACACHE_H
#define MODELDATACACHE_H

#include "elisaLib_export.h"

#include "datatype.h"
#include "elisautils.h"

#include <QObject>

#include <memory>

class ModelDataCachePrivate;
class DatabaseInterface;
class MusicAlbum;

class ELISALIB_EXPORT ModelDataCache : public QObject
{

    Q_OBJECT

    Q_PROPERTY(DataUtils::DataType dataType READ dataType WRITE setDataType NOTIFY dataTypeChanged)

    Q_PROPERTY(DatabaseInterface* database READ database WRITE setDatabase NOTIFY databaseChanged)

public:

    static const int CACHE_SIZE = 3000;

    enum CallMode {
        CannotBlock,
        CanBlock,
    };

    explicit ModelDataCache(QObject *parent = nullptr);

    ~ModelDataCache();

    DataUtils::DataType dataType() const;

    int dataCount() const;

    QVariant data(int row, ElisaUtils::ColumnsRoles role) const;

    DatabaseInterface* database() const;

Q_SIGNALS:

    void dataTypeChanged(DataUtils::DataType dataType);

    void receiveData(int row);

    void databaseChanged(DatabaseInterface* database);

    void dataChanged(int lowerBound, int upperBound);

public Q_SLOTS:

    void neededData();

    void setDataType(DataUtils::DataType dataType);

    void setDatabase(DatabaseInterface* database);

private Q_SLOTS:

    void databaseContentChanged();

private:

    void fetchPartialData();

    void connectDatabase();

    std::unique_ptr<ModelDataCachePrivate> d;

};

#endif // MODELDATACACHE_H
