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

#ifndef ELISAAPPLICATION_H
#define ELISAAPPLICATION_H

#include "elisaLib_export.h"

#include "config-upnp-qt.h"

#include <QObject>
#include <QString>

#include <memory>

class QIcon;
class QAction;
class MusicListenersManager;
class QSortFilterProxyModel;
class MediaPlayList;
class AudioWrapper;
class ManageAudioPlayer;
class ManageMediaPlayerControl;
class ManageHeaderBar;
class ElisaApplicationPrivate;

class ELISALIB_EXPORT ElisaApplication : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QStringList arguments
               READ arguments
               WRITE setArguments
               NOTIFY argumentsChanged)

    Q_PROPERTY(MusicListenersManager *musicManager
               READ musicManager
               NOTIFY musicManagerChanged)

    Q_PROPERTY(QSortFilterProxyModel* allAlbumsProxyModel
               READ allAlbumsProxyModel
               NOTIFY allAlbumsProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* allArtistsProxyModel
               READ allArtistsProxyModel
               NOTIFY allArtistsProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* allTracksProxyModel
               READ allTracksProxyModel
               NOTIFY allTracksProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* allGenresProxyModel
               READ allGenresProxyModel
               NOTIFY allGenresProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* allComposersProxyModel
               READ allComposersProxyModel
               NOTIFY allComposersProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* allLyricistsProxyModel
               READ allLyricistsProxyModel
               NOTIFY allLyricistsProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* singleArtistProxyModel
               READ singleArtistProxyModel
               NOTIFY singleArtistProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* singleAlbumProxyModel
               READ singleAlbumProxyModel
               NOTIFY singleAlbumProxyModelChanged)

    Q_PROPERTY(QSortFilterProxyModel* fileBrowserProxyModel
               READ fileBrowserProxyModel
               NOTIFY fileBrowserProxyModelChanged)

    Q_PROPERTY(MediaPlayList *mediaPlayList
               READ mediaPlayList
               NOTIFY mediaPlayListChanged)

    Q_PROPERTY(AudioWrapper *audioPlayer
               READ audioPlayer
               NOTIFY audioPlayerChanged)

    Q_PROPERTY(ManageAudioPlayer *audioControl
               READ audioControl
               NOTIFY audioControlChanged)

    Q_PROPERTY(ManageMediaPlayerControl *playerControl
               READ playerControl
               NOTIFY playerControlChanged)

    Q_PROPERTY(ManageHeaderBar *manageHeaderBar
               READ manageHeaderBar
               NOTIFY manageHeaderBarChanged)

public:
    explicit ElisaApplication(QObject *parent = nullptr);

    ~ElisaApplication() override;

    Q_INVOKABLE QAction* action(const QString& name);

    Q_INVOKABLE QString iconName(const QIcon& icon);

    const QStringList &arguments() const;

    MusicListenersManager *musicManager() const;

    QSortFilterProxyModel *allAlbumsProxyModel() const;

    QSortFilterProxyModel *allArtistsProxyModel() const;

    QSortFilterProxyModel *allGenresProxyModel() const;

    QSortFilterProxyModel *allComposersProxyModel() const;

    QSortFilterProxyModel *allLyricistsProxyModel() const;

    QSortFilterProxyModel *allTracksProxyModel() const;

    QSortFilterProxyModel *singleArtistProxyModel() const;

    QSortFilterProxyModel *singleAlbumProxyModel() const;

    QSortFilterProxyModel *fileBrowserProxyModel() const;

    MediaPlayList *mediaPlayList() const;

    AudioWrapper *audioPlayer() const;

    ManageAudioPlayer *audioControl() const;

    ManageMediaPlayerControl *playerControl() const;

    ManageHeaderBar *manageHeaderBar() const;

Q_SIGNALS:

    void argumentsChanged();

    void musicManagerChanged();

    void allAlbumsProxyModelChanged();

    void allArtistsProxyModelChanged();

    void allGenresProxyModelChanged();

    void allComposersProxyModelChanged();

    void allLyricistsProxyModelChanged();

    void allTracksProxyModelChanged();

    void singleArtistProxyModelChanged();

    void singleAlbumProxyModelChanged();

    void fileBrowserProxyModelChanged();

    void mediaPlayListChanged();

    void audioPlayerChanged();

    void audioControlChanged();

    void playerControlChanged();

    void manageHeaderBarChanged();

    void enqueue(const QStringList &files);

    void initializationDone();

public Q_SLOTS:

    void appHelpActivated();

    void aboutApplication();

    void reportBug();

    void configureShortcuts();

    void configureElisa();

    void setArguments(const QStringList &newArguments);

    void activateActionRequested(const QString &actionName, const QVariant &parameter);

    void activateRequested(const QStringList &arguments, const QString &workingDirectory);

    void openRequested(const QList< QUrl > &uris);

    void initialize();

private Q_SLOTS:

    void goBack();

    void find();

    void togglePlaylist();

private:

    void initializeModels();

    void initializePlayer();

    void setupActions(const QString &actionName);

    QStringList checkFileListAndMakeAbsolute(const QStringList &filesList, const QString &workingDirectory) const;

    std::unique_ptr<ElisaApplicationPrivate> d;

};

#endif // ELISAAPPLICATION_H
