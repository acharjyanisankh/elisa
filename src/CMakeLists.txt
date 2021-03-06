include_directories(${elisa_BINARY_DIR})

set(elisaLib_SOURCES
    mediaplaylist.cpp
    musicalbum.cpp
    musicaudiotrack.cpp
    musicartist.cpp
    musicaudiogenre.cpp
    progressindicator.cpp
    databaseinterface.cpp
    musiclistenersmanager.cpp
    managemediaplayercontrol.cpp
    manageheaderbar.cpp
    manageaudioplayer.cpp
    trackslistener.cpp
    elisaapplication.cpp
    audiowrapper.cpp
    notificationitem.cpp
    topnotificationmanager.cpp
    elisautils.cpp
    datatype.cpp
    trackdatahelper.cpp
    modeldatacache.cpp
    abstractfile/abstractfilelistener.cpp
    abstractfile/abstractfilelisting.cpp
    filescanner.cpp
    file/filelistener.cpp
    file/localfilelisting.cpp
    models/albummodel.cpp
    models/allalbumsmodel.cpp
    models/allartistsmodel.cpp
    models/alltracksmodel.cpp
    models/allgenresmodel.cpp
    models/abstractmediaproxymodel.cpp
    models/allalbumsproxymodel.cpp
    models/allartistsproxymodel.cpp
    models/alltracksproxymodel.cpp
    models/singleartistproxymodel.cpp
    models/singlealbumproxymodel.cpp
    models/genericdatamodel.cpp
)

if (KF5KIO_FOUND)
    set(elisaLib_SOURCES
        ${elisaLib_SOURCES}
        models/filebrowsermodel.cpp
        models/filebrowserproxymodel.cpp
    )
endif()

if (KF5Baloo_FOUND)
    if (Qt5DBus_FOUND)
        set(elisaLib_SOURCES
            ${elisaLib_SOURCES}
            baloo/localbaloofilelisting.cpp
            baloo/baloolistener.cpp
            )

        qt5_add_dbus_interface(elisaLib_SOURCES
            ${BALOO_DBUS_INTERFACES_DIR}/org.kde.baloo.main.xml
            baloo/main)

        qt5_add_dbus_interface(elisaLib_SOURCES
            ${BALOO_DBUS_INTERFACES_DIR}/org.kde.baloo.fileindexer.xml
            baloo/fileindexer)

        qt5_add_dbus_interface(elisaLib_SOURCES
            ${BALOO_DBUS_INTERFACES_DIR}/org.kde.baloo.scheduler.xml
            baloo/scheduler)

        qt5_add_dbus_adaptor(elisaLib_SOURCES
            ${BALOO_DBUS_INTERFACES_DIR}/org.kde.BalooWatcherApplication.xml
            baloo/localbaloofilelisting.h LocalBalooFileListing)
    endif()
endif()

if (Qt5DBus_FOUND)
    set(elisaLib_SOURCES
        ${elisaLib_SOURCES}
        mpris2/mpris2.cpp
        mpris2/mediaplayer2.cpp
        mpris2/mediaplayer2player.cpp
    )
endif()

if (UPNPQT_FOUND)
    set(elisaLib_SOURCES
        ${elisaLib_SOURCES}
        upnp/upnpcontrolcontentdirectory.cpp
        upnp/upnpcontentdirectorymodel.cpp
        upnp/upnpcontrolconnectionmanager.cpp
        upnp/upnpcontrolmediaserver.cpp
        upnp/didlparser.cpp
        upnp/upnplistener.cpp
        upnp/upnpdiscoverallmusic.cpp
        )
endif()

if (KF5Baloo_FOUND)
    if (Qt5DBus_FOUND)
        qt5_add_dbus_interface(elisaLib_SOURCES
            ${BALOO_DBUS_INTERFACES_DIR}/org.kde.baloo.fileindexer.xml
            baloo/fileindexer)

        qt5_add_dbus_interface(elisaLib_SOURCES
            ${BALOO_DBUS_INTERFACES_DIR}/org.kde.baloo.scheduler.xml
            baloo/scheduler)

        set(elisaLib_SOURCES
            ${elisaLib_SOURCES}
            ../src/baloo/baloolistener.cpp
            ../src/baloo/localbaloofilelisting.cpp
            )
    endif()
endif()

kconfig_add_kcfg_files(elisaLib_SOURCES ../src/elisa_settings.kcfgc )
set(elisaLib_SOURCES
    ${elisaLib_SOURCES}
    ../src/elisa_core.kcfg
)

add_library(elisaLib ${elisaLib_SOURCES})
target_link_libraries(elisaLib
    LINK_PUBLIC
        Qt5::Multimedia
    LINK_PRIVATE
        Qt5::Core Qt5::Sql Qt5::Widgets Qt5::Concurrent Qt5::Qml
        KF5::I18n KF5::CoreAddons
        KF5::ConfigCore KF5::ConfigGui)

if (KF5FileMetaData_FOUND)
    target_link_libraries(elisaLib
        LINK_PRIVATE
            KF5::FileMetaData
    )
endif()

if (KF5KIO_FOUND)
    target_link_libraries(elisaLib
        LINK_PUBLIC
            KF5::KIOCore KF5::KIOFileWidgets KF5::KIOWidgets
    )
endif()

if (KF5XmlGui_FOUND)
    target_link_libraries(elisaLib
        LINK_PUBLIC
            KF5::XmlGui
    )
endif()

if (KF5ConfigWidgets_FOUND)
    target_link_libraries(elisaLib
        LINK_PUBLIC
            KF5::ConfigWidgets
    )
endif()

if (KF5KCMUtils_FOUND)
    target_link_libraries(elisaLib
        LINK_PUBLIC
            KF5::KCMUtils
    )
endif()

if (KF5Baloo_FOUND)
    if (Qt5DBus_FOUND)
        target_link_libraries(elisaLib
            LINK_PUBLIC
                KF5::Baloo
        )
    endif()
endif()

if (Qt5DBus_FOUND)
    target_link_libraries(elisaLib
        LINK_PUBLIC
            Qt5::DBus
    )

    if (KF5DBusAddons_FOUND)
        target_link_libraries(elisaLib
            LINK_PUBLIC
                KF5::DBusAddons
        )
    endif()
endif()

generate_export_header(elisaLib BASE_NAME ElisaLib EXPORT_FILE_NAME elisaLib_export.h)

set_target_properties(elisaLib PROPERTIES
    VERSION 0.1
    SOVERSION 0
    EXPORT_NAME ElisaLib
)

if (NOT APPLE)
    install(TARGETS elisaLib
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/elisa
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/elisa
        BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/elisa
    )
else()
    install(TARGETS elisaLib ${INSTALL_TARGETS_DEFAULT_ARGS})
endif()

set(elisaqmlplugin_SOURCES
    elisaqmlplugin.cpp
    datatype.cpp
    elisautils.cpp
)

add_library(elisaqmlplugin SHARED ${elisaqmlplugin_SOURCES})
target_link_libraries(elisaqmlplugin
    LINK_PRIVATE
        Qt5::Quick Qt5::Widgets
        KF5::ConfigCore KF5::ConfigGui
        elisaLib
)

if (KF5FileMetaData_FOUND)
    target_link_libraries(elisaqmlplugin
        LINK_PRIVATE
            KF5::FileMetaData
    )
endif()

set_target_properties(elisaqmlplugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/org/kde/elisa
)

if (NOT APPLE)
    set_target_properties(elisaqmlplugin PROPERTIES
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/elisa;${CMAKE_INSTALL_RPATH}"
    )
endif()

install(TARGETS elisaqmlplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/elisa/)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/elisa)

add_custom_target(copy)
add_custom_target(copy2)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/org/kde/elisa)

add_custom_command(TARGET copy PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${CMAKE_CURRENT_SOURCE_DIR}/qmldir ${CMAKE_BINARY_DIR}/bin/org/kde/elisa/)

add_custom_command(TARGET copy2 PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${CMAKE_CURRENT_SOURCE_DIR}/plugins.qmltypes ${CMAKE_BINARY_DIR}/bin/org/kde/elisa/)

add_dependencies(elisaqmlplugin copy copy2)

if (Qt5Quick_FOUND AND Qt5Widgets_FOUND)
    set(elisa_SOURCES
        main.cpp

        windows/WindowsTheme.qml
        windows/PlatformIntegration.qml

        android/ElisaMainWindow.qml
        android/AndroidTheme.qml
        android/PlatformIntegration.qml

        qml/ElisaMainWindow.qml
        qml/ApplicationMenu.qml
        qml/BaseTheme.qml
        qml/Theme.qml
        qml/PlatformIntegration.qml
        qml/LabelWithToolTip.qml

        qml/RatingStar.qml
        qml/PlayListEntry.qml
        qml/MediaBrowser.qml
        qml/DraggableItem.qml
        qml/PassiveNotification.qml
        qml/TopNotification.qml
        qml/TopNotificationItem.qml
        qml/TrackImportNotification.qml

        qml/HeaderBar.qml
        qml/NavigationActionBar.qml
        qml/MediaPlayerControl.qml
        qml/ContextView.qml
        qml/ContentView.qml
        qml/ViewSelector.qml
        qml/ViewManager.qml

        qml/MediaPlayListView.qml
        qml/MediaTrackDelegate.qml
        qml/MediaAlbumTrackDelegate.qml
        qml/MediaTrackMetadataView.qml
        qml/GridBrowserView.qml
        qml/GridBrowserDelegate.qml
        qml/ListBrowserView.qml
        qml/FileBrowserDelegate.qml
        qml/FileBrowserView.qml
    )

    qt5_add_resources(elisa_SOURCES resources.qrc)
     set_property(SOURCE qrc_resources.cpp PROPERTY SKIP_AUTOMOC ON)

    set(elisa_ICONS_PNG
        ../icons/128-apps-elisa.png
        ../icons/64-apps-elisa.png
        ../icons/48-apps-elisa.png
        ../icons/32-apps-elisa.png
        ../icons/22-apps-elisa.png
        ../icons/16-apps-elisa.png
    )

    # add icons to application sources, to have them bundled
    ecm_add_app_icon(elisa_SOURCES ICONS ${elisa_ICONS_PNG})

    add_executable(elisa ${elisa_SOURCES})

    target_include_directories(elisa PRIVATE ${KDSoap_INCLUDE_DIRS})

    target_link_libraries(elisa
        LINK_PRIVATE
            elisaLib
            Qt5::Widgets Qt5::QuickControls2 Qt5::Svg
            KF5::I18n KF5::CoreAddons KF5::ConfigCore KF5::ConfigGui
    )

    if (ANDROID)
        target_link_libraries(elisa
            LINK_PRIVATE
            Qt5::AndroidExtras
        )
    endif()

    if (KF5Crash_FOUND)
        target_link_libraries(elisa
            LINK_PRIVATE
            KF5::Crash
        )
    endif()

    if (KF5Declarative_FOUND)
        target_link_libraries(elisa
            LINK_PRIVATE
                KF5::Declarative
        )
    endif()

    if (NOT APPLE)
        set_target_properties(elisa PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}/elisa;${CMAKE_INSTALL_RPATH}"
        )
    endif()

    install(TARGETS elisa ${INSTALL_TARGETS_DEFAULT_ARGS})
endif()

if (KF5ConfigWidgets_FOUND AND KF5Declarative_FOUND)
    add_subdirectory(localFileConfiguration)
endif()

set(elisaImport_SOURCES
    elisaimport.cpp
    elisaimportapplication.cpp
)

kconfig_add_kcfg_files(elisaImport_SOURCES ../src/elisa_settings.kcfgc )
set(elisaImport_SOURCES
    ${elisaImport_SOURCES}
    ../src/elisa_core.kcfg
)

add_executable(elisaImport ${elisaImport_SOURCES})

target_link_libraries(elisaImport
    LINK_PRIVATE
        KF5::ConfigCore KF5::ConfigGui
        elisaLib
)

if (KF5FileMetaData_FOUND)
    target_link_libraries(elisaImport
        LINK_PRIVATE
            KF5::FileMetaData
    )
endif()

set(QML_IMPORT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "qml import path" FORCE)
