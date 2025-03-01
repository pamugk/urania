add_executable(urania)

ecm_add_qml_module(urania
    URI
    com.github.pamugk.urania
)

target_sources(urania
    PRIVATE
    main.cpp
    geoapiclient.h geoapiclient.cpp
)

ecm_target_qml_sources(urania
    SOURCES
    Main.qml
)

target_link_libraries(urania
    PRIVATE
    OSMScout::OSMScout
    OSMScout::Map
    OSMScout::MapQt
    OSMScout::ClientQt

    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Location
    Qt6::Network
    Qt6::Positioning

    KF6::CoreAddons
    KF6::IconThemes
)

install(TARGETS urania ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
