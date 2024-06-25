cmake_minimum_required(VERSION 3.16)

project(CapyArtStudio VERSION 1.0 LANGUAGES CXX)

set(APPLICATION_SOURCE_FILES
        src/Application.cpp
        src/Application.hpp
        src/graphics/Layer.hpp
        src/graphics/Layer.cpp
        src/graphics/Drawing.hpp
        src/graphics/Drawing.cpp
        src/graphics/DrawingTools.hpp
        src/graphics/Pixel.hpp
        src/graphics/Pixel.cpp
        src/ui/MainWindow.hpp
        src/ui/MainWindow.cpp
        src/ui/DrawingWidget.hpp
        src/ui/DrawingWidget.cpp
        src/ui/dialogs/NewFileDialog.cpp
        src/ui/dialogs/NewFileDialog.hpp
        src/utils/TomlWrapper.hpp
        src/utils/ConfigurationManager.hpp
        src/utils/ConfigurationManager.cpp
        src/io/CapyFileManager.cpp
        src/io/CapyFileManager.hpp
)

set(APPLICATION_UI_FILES
        src/ui/MainWindow.ui
        src/ui/dialogs/NewFileDialog.ui
)

set(TEST_FILES
        tests/placeholder.cpp
)

set(LIBS_TO_LINK
        Qt6::Widgets
        spdlog::spdlog
        fmt::fmt
        tomlplusplus::tomlplusplus
        rapidjson
)

set(Qt6_DIR /opt/homebrew/Cellar/qt/6.7.0_1/lib/cmake/Qt6)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(RapidJSON REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(CapyArtStudio
        src/main.cpp
        ${APPLICATION_SOURCE_FILES}
        ${APPLICATION_UI_FILES}
)

set_target_properties(CapyArtStudio PROPERTIES
        # C++ Settings
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        # Qt application settings
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.capyartstudio
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(CapyArtStudio
        PRIVATE
        ${LIBS_TO_LINK}
)

target_include_directories(CapyArtStudio PRIVATE src)

include(GNUInstallDirs)
install(TARGETS CapyArtStudio
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (SKIP_TESTS)
    message("Skipping tests")
else ()
    find_package(GTest REQUIRED)
    if (NOT GTest_FOUND)
        message(FATAL_ERROR "No GTest Found")
    endif ()

    add_executable(CapyArtStudio_Tests
            tests/main.cpp
            ${TEST_FILES}
            ${APPLICATION_SOURCE_FILES}
    )

    target_link_libraries(CapyArtStudio_Tests
            PRIVATE
            GTest::GTest
            ${LIBS_TO_LINK})

    if (GMock_FOUND)
        target_link_libraries(CapyArtStudio_Tests INTERFACE GTest::GMock)
    endif ()

    target_include_directories(CapyArtStudio_Tests PRIVATE src)
    target_include_directories(CapyArtStudio_Tests PRIVATE tests)
endif ()
