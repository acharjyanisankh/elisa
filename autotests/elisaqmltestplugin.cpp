/*
 * Copyright 2018 Matthieu Gallien <matthieu_gallien@yahoo.fr>
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

#include "elisaqmltestplugin.h"

#include <QQmlExtensionPlugin>
#include <QQmlEngine>

#include "trackdatahelper.h"
#include "mediaplaylist.h"

void ElisaQmlTestPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

void ElisaQmlTestPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<TrackDataHelper>(uri, 1, 0, "TrackDataHelper");
    qmlRegisterType<MediaPlayList>(uri, 1, 0, "MediaPlayList");
}