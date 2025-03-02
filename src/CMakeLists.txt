qt_add_executable(urania
    main.cpp
    mapwidget.h mapwidget.cpp
    mainwindow.h mainwindow.cpp mainwindow.ui
    startuppage.h startuppage.cpp startuppage.ui
)

target_link_libraries(urania PRIVATE
    OSMScout::OSMScout
    OSMScout::Map
    OSMScout::MapQt
    OSMScout::ClientQt

    Qt6::Widgets
)

install(TARGETS urania
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET urania
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
