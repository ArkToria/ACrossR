option(FETCH_MAGIC_ENUM "Download magic_enum" OFF)
if (FETCH_MAGIC_ENUM)
    message("Fetching 'magic_enum'...")

    CPMAddPackage(
        NAME magic_enum
        GITHUB_REPOSITORY Neargye/magic_enum
        GIT_TAG v0.7.3
        )

    message("Fetch 'magic_enum' done.")
else()
    set(MAGIC_ENUM_DIR ${CMAKE_SOURCE_DIR}/3rdpart/magic_enum)

    set(MAGIC_ENUM_INCLUDE_PATH ${MAGIC_ENUM_DIR}/include)

    set(MAGIC_ENUM_LIBRARY magic_enum)

    add_library(${MAGIC_ENUM_LIBRARY} STATIC
        ${MAGIC_ENUM_INCLUDE_PATH}/magic_enum.hpp
        )

    target_include_directories(${MAGIC_ENUM_LIBRARY} INTERFACE
        ${MAGIC_ENUM_INCLUDE_PATH}
        )
endif()
