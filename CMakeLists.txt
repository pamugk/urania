cmake_minimum_required(VERSION 3.16)

project(urania VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependency feature flags
set(OSMSCOUT_BUILD_DEMOS OFF)
set(OSMSCOUT_BUILD_DOC_API OFF)
set(OSMSCOUT_BUILD_CLIENT_QT OFF)
set(OSMSCOUT_BUILD_GPX OFF)
set(OSMSCOUT_BUILD_IMPORT OFF)
set(OSMSCOUT_BUILD_MAP_AGG OFF)
set(OSMSCOUT_BUILD_MAP_CAIRO OFF)
set(OSMSCOUT_BUILD_MAP_DIRECTX OFF)
set(OSMSCOUT_BUILD_MAP_GDI OFF)
set(OSMSCOUT_BUILD_MAP_IOSX OFF)
set(OSMSCOUT_BUILD_MAP_OPENGL OFF)
set(OSMSCOUT_BUILD_MAP_QT OFF)
set(OSMSCOUT_BUILD_MAP_SVG OFF)

find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMFindQmlModule)
include(ECMQmlModule)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    Gui
    QuickControls2
    Location
    Network
    Positioning
)

find_package(KF6 REQUIRED COMPONENTS
    Kirigami
    I18n
    CoreAddons
    QQC2DesktopStyle
    IconThemes
)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)

add_subdirectory(src)
#add_subdirectory(dependencies/libosmscout)

install(PROGRAMS com.github.pamugk.urania.desktop DESTINATION ${KDE_INSTALL_APPDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
