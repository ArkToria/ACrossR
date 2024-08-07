cmake_minimum_required(VERSION 3.21.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



project(across LANGUAGES CXX)
set(TRAY_APP_NAME ${PROJECT_NAME})
set(APP_NAME ${PROJECT_NAME}-qml)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/qrc.cmake")
include(FetchContent)

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG master
)
FetchContent_MakeAvailable(Corrosion)

find_package(QT NAMES Qt6 COMPONENTS Widgets Core Gui Qml Quick Core5Compat QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Qml Quick Core5Compat QuickControls2)

find_program(CXXBRIDGE cxxbridge PATHS "$ENV{HOME}/.cargo/bin/")
if (CXXBRIDGE STREQUAL "CXXBRIDGE-NOTFOUND")
    message("Could not find cxxbridge, trying to install with `cargo install cxxbridge-cmd'")
    find_program(CARGO cargo PATHS "$ENV{HOME}/.cargo/bin/")
    if (CARGO STREQUAL "CARGO-NOTFOUND")
        message(FATAL_ERROR "Requires cargo available in path, install via rustup https://rustup.rs/")
    endif()
    execute_process(COMMAND ${CARGO} install cxxbridge-cmd)
    find_program(CXXBRIDGE cxxbridge PATHS "$ENV{HOME}/.cargo/bin/")
endif()


# To build this application you need Qt 6.2.0 or higher
if (Qt6_VERSION VERSION_LESS 6.2.0)
message(FATAL_ERROR "You need Qt 6.2.0 or newer to build the application.")
endif()


corrosion_import_crate(MANIFEST_PATH src/rust/Cargo.toml)
corrosion_add_cxxbridge(rusty_bridge
    CRATE 
    across-core
    MANIFEST_PATH
    src/rust
    FILES
    ffi/init.rs
    ffi/profile.rs
    ffi/acolors.rs
    ffi/qml_process.rs
)

corrosion_import_crate(
    MANIFEST_PATH submodules/AColoRS/Cargo.toml 
    CRATE_TYPES bin
)
if ( MSVC )
    set_target_properties(acolors PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>)
endif ( MSVC )
IF (WIN32)
    corrosion_add_target_rustflags(acolors "-C strip=none")
ENDIF()



aux_source_directory(src/tray/ TRAY_CPP_FILES)

qt_add_executable(${TRAY_APP_NAME}
    MANUAL_FINALIZATION
    ${TRAY_CPP_FILES}
    misc/across.rc
)

# qt_standard_project_setup() requires Qt 6.3 or higher. See https://doc.qt.io/qt-6/qt-standard-project-setup.html for details.
if (${QT_VERSION_MINOR} GREATER_EQUAL 3)
qt6_standard_project_setup()
endif()

qt6_policy(
    SET QTP0001 NEW
)

target_compile_definitions(${TRAY_APP_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

set(LIBS
    rusty_bridge
    Qt${QT_VERSION_MAJOR}::Widgets
)
if (WIN32)
    list(APPEND LIBS ntdll)
endif()
target_link_libraries(${TRAY_APP_NAME} PRIVATE
    ${LIBS}
)

qt_add_resources(${TRAY_APP_NAME} misc
    PREFIX "/qt/qml/misc/"
    BASE "${CMAKE_CURRENT_LIST_DIR}/misc"
    FILES
    ${ICON_FILES}
)
aux_source_directory(src/qml/ QML_CPP_FILES)

qt_add_executable(${APP_NAME}
    MANUAL_FINALIZATION
    ${QML_CPP_FILES}
    misc/across.rc
)


qt_add_resources(${APP_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_compile_definitions(${APP_NAME}
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

set(QML_LIBS 
    rusty_bridge
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Core5Compat
)
if (WIN32)
    list(APPEND LIBS ntdll)
endif()
target_link_libraries(${APP_NAME} PRIVATE
    ${QML_LIBS}
)


qt_add_resources(${APP_NAME} misc
    PREFIX "/qt/qml/misc/"
    BASE "${CMAKE_CURRENT_LIST_DIR}/misc"
    FILES
    ${ICON_FILES}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)


#########################
######## INSTALL ########
#########################
option(ENABLE_DEPLOYMENT "Enable fixup_bundle and windeployqt when installing." ON)

if(WIN32 AND ENABLE_DEPLOYMENT)
    get_target_property(ACOLORS_OUTPUT_DIRECTORY acolors RUNTIME_OUTPUT_DIRECTORY)
    
    install(PROGRAMS "${ACOLORS_OUTPUT_DIRECTORY}/acolors.exe" DESTINATION .)
    
    install(TARGETS across RUNTIME DESTINATION .)
    install(TARGETS across-qml RUNTIME DESTINATION .)

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

    set(APP "${CMAKE_INSTALL_PREFIX}/across-qml.exe")
    install(CODE "include(BundleUtilities)")
    install(CODE "fixup_bundle(\"${APP}\"   \"\"   \"${DIRS}\")")
    install(CODE "message(\"Now running windeployqt\")")
    install(CODE "execute_process(COMMAND \"$<TARGET_FILE:Qt::windeployqt>\" \"${APP}\" -- release--no-compiler-runtime --no-opengl-sw -qmldir=${CMAKE_CURRENT_SOURCE_DIR} -qmlimport=.)")
    install(CODE "message(\"Completed\")")

    set(APP "${CMAKE_INSTALL_PREFIX}/across.exe")
    install(CODE "include(BundleUtilities)")
    install(CODE "fixup_bundle(\"${APP}\"   \"\"   \"${DIRS}\")")
    install(CODE "message(\"Now running windeployqt\")")
    install(CODE "execute_process(COMMAND \"$<TARGET_FILE:Qt::windeployqt>\" \"${APP}\" --release --no-compiler-runtime --no-opengl-sw -qmldir=${CMAKE_CURRENT_SOURCE_DIR} -qmlimport=.)")
    install(CODE "message(\"Completed\")")
else()
    get_target_property(ACOLORS_OUTPUT_DIRECTORY acolors RUNTIME_OUTPUT_DIRECTORY)
    install(PROGRAMS "${ACOLORS_OUTPUT_DIRECTORY}/acolors" DESTINATION .)
    install(TARGETS across RUNTIME DESTINATION bin)
    install(TARGETS across-qml RUNTIME DESTINATION .)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/org.arktoria.across.desktop DESTINATION share/applications)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/design/logo.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.running.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/light/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.light.stop.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_running.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.running.svg)
    install(FILES ${CMAKE_SOURCE_DIR}/misc/icons/dark/tray_logo_stop.svg DESTINATION share/icons/hicolor/scalable/apps/ RENAME org.arktoria.across.dark.stop.svg)
endif()
