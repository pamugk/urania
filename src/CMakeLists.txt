add_executable(urania)

target_sources(urania
    PRIVATE
    main.cpp
    mapwidget.h mapwidget.cpp
)

target_link_libraries(urania
    PRIVATE
    OSMScout::OSMScout
    OSMScout::Map
    OSMScout::MapQt
    OSMScout::ClientQt

    Qt6::Widgets

    KF6::IconThemes
)

install(TARGETS urania ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
