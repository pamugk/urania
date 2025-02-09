add_executable(appurania)

ecm_add_qml_module(appurania
    URI
    com.github.pamugk.urania
)

target_sources(appurania
    PRIVATE
    main.cpp
    geoapiclient.h geoapiclient.cpp
)

ecm_target_qml_sources(appurania
    SOURCES
    Main.qml
)

target_link_libraries(appurania
    PRIVATE
    #OSMScout::OSMScout
    #OSMScout::Map

    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Location
    Qt6::Network
    Qt6::Positioning

    KF6::I18n
    KF6::CoreAddons
    KF6::IconThemes
)

install(TARGETS appurania ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
