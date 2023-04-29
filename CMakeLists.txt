cmake_minimum_required(VERSION 3.21.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(BUILD_QDS_COMPONENTS ON CACHE BOOL "Build design studio components")

project(across LANGUAGES CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/qrc.cmake")

find_package(QT NAMES Qt6 COMPONENTS Gui Qml Quick Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Core5Compat)

# To build this application you need Qt 6.2.0 or higher
if (Qt6_VERSION VERSION_LESS 6.2.0)
message(FATAL_ERROR "You need Qt 6.2.0 or newer to build the application.")
endif()

qt_add_executable(${CMAKE_PROJECT_NAME}
    MANUAL_FINALIZATION
    src/main.cpp
    misc/across.rc
)

# qt_standard_project_setup() requires Qt 6.3 or higher. See https://doc.qt.io/qt-6/qt-standard-project-setup.html for details.
if (${QT_VERSION_MINOR} GREATER_EQUAL 3)
qt6_standard_project_setup()
endif()

qt6_policy(
    SET QTP0001 NEW
)

qt_add_resources(${CMAKE_PROJECT_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Core5Compat
)

qt_add_resources(${CMAKE_PROJECT_NAME} misc
    PREFIX "/qt/qml/misc/"
    BASE "${CMAKE_CURRENT_LIST_DIR}/misc"
    FILES
    ${ICON_FILES}
)

if (${BUILD_QDS_COMPONENTS})
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif ()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

#########################
######## INSTALL ########
#########################
option(ENABLE_DEPLOYMENT "Enable fixup_bundle and windeployqt when installing." ON)

if(WIN32 AND ENABLE_DEPLOYMENT)
    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION .)

    # For Global Target
    list(APPEND DIRS "${CMAKE_BINARY_DIR}")
    list(APPEND DIRS "${CMAKE_CURRENT_BINARY_DIR}")
    list(APPEND DIRS "${Qt6Core_DIR}/../../")
    list(APPEND DIRS "${Qt6Core_DIR}/../../../bin/")

    if(CMAKE_PREFIX_PATH)
        foreach(dir ${CMAKE_PREFIX_PATH})
            list(APPEND DIRS "${dir}/bin" "${dir}/lib")
        endforeach()
    endif()

    list(APPEND DIRS "${Qt6Core_DIR}/../..")

    foreach(d ${DIRS})
        string(REPLACE "\\" "/" newd ${d})
        list(APPEND NEW_DIRS ${newd})
    endforeach()
    set(DIRS ${NEW_DIRS})

    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)

    set(APP "${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME}.exe")
    install(CODE "include(BundleUtilities)")
    install(CODE "fixup_bundle(\"${APP}\"   \"\"   \"${DIRS}\")")
    install(CODE "message(\"Now running windeployqt\")")
    install(CODE "execute_process(COMMAND \"$<TARGET_FILE:Qt::windeployqt>\" \"${APP}\" --no-virtualkeyboard --no-compiler-runtime --no-opengl-sw -qmldir=${CMAKE_CURRENT_SOURCE_DIR}/content/ -qmlimport=.)")
    install(CODE "message(\"Completed\")")
else()
    install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/org.arktoria.across.desktop DESTINATION share/applications)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/design/logo.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.running.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.stop.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.running.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.stop.svg)
endif()
