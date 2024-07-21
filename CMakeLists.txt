cmake_minimum_required(VERSION 3.16)

project(
        CapyArtStudio
        VERSION 1.0
        LANGUAGES CXX)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy; -format-style='file';)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(APPLICATION_SOURCE_FILES
        src/Application.cpp
        src/Application.hpp
        src/models/PaletteModel.hpp
        src/models/PaletteModel.cpp
        src/models/PaletteColorTableModel.hpp
        src/models/PaletteColorTableModel.cpp
        src/models/ProjectsModel.hpp
        src/models/ProjectsModel.cpp
        src/ai/AiServiceInterface.cpp
        src/ai/AiServiceInterface.hpp
        src/algorithms/Bresenham.hpp
        src/algorithms/Bresenham.cpp
        src/algorithms/AlphaBlending.hpp
        src/algorithms/AlphaBlending.cpp
        src/algorithms/Luminance.cpp
        src/algorithms/Luminance.hpp
        src/graphics/Layer.hpp
        src/graphics/Layer.cpp
        src/graphics/GraphicalBackend.hpp
        src/graphics/GraphicalBackend.cpp
        src/graphics/drawing-tools/IDrawingTool.hpp
        src/graphics/drawing-tools/IDrawingTool.cpp
        src/graphics/drawing-tools/PenTool.cpp
        src/graphics/drawing-tools/PenTool.hpp
        src/graphics/drawing-tools/HandTool.hpp
        src/graphics/drawing-tools/HandTool.cpp
        src/graphics/drawing-tools/Toolbox.hpp
        src/graphics/drawing-tools/Toolbox.cpp
        src/managers/ProjectsManager.hpp
        src/managers/ProjectsManager.cpp
        src/managers/PalettesManager.hpp
        src/managers/PalettesManager.cpp
        src/managers/Manager.hpp
        src/user/Palette.cpp
        src/user/Palette.hpp
        src/user/Project.hpp
        src/user/Project.cpp
        src/graphics/Drawing.hpp
        src/graphics/Drawing.cpp
        src/graphics/Pixel.hpp
        src/graphics/Pixel.cpp
        src/ui/ConsoleWindow.cpp
        src/ui/ConsoleWindow.hpp
        src/ui/MainWindow.hpp
        src/ui/MainWindow.cpp
        src/ui/WelcomeScreen.hpp
        src/ui/WelcomeScreen.cpp
        src/ui/utils/AutoSizeSavingItem.hpp
        src/ui/utils/AutoSizeSavingItem.cpp
        src/ui/SettingsDialog.hpp
        src/ui/SettingsDialog.cpp
        src/ui/widgets/color-pickers/DefaultColorPicker.cpp
        src/ui/widgets/color-pickers/DefaultColorPicker.hpp
        src/ui/widgets/color-pickers/DefaultColorPickerSlider.cpp
        src/ui/widgets/color-pickers/DefaultColorPickerSlider.hpp
        src/ui/widgets/delegates/ColorRectangleDelegate.hpp
        src/ui/widgets/delegates/ColorRectangleDelegate.cpp
        src/ui/widgets/delegates/ProjectDelegate.hpp
        src/ui/widgets/delegates/ProjectDelegate.cpp
        src/ui/widgets/delegates/ToolButton.hpp
        src/ui/widgets/delegates/ToolButton.cpp
        src/ui/utils/CheckerboardPixmap.hpp
        src/ui/utils/CheckerboardPixmap.cpp
        src/ui/utils/MessageBoxUtils.cpp
        src/ui/utils/MessageBoxUtils.hpp
        src/io/ResourceManager.hpp
        src/io/ResourceManager.cpp
        src/utils/Dimensions.cpp
        src/utils/Dimensions.hpp
        src/ui/layouts/FlowLayout.cpp
        src/ui/layouts/FlowLayout.hpp
        src/ui/docks/ColorArea.hpp
        src/ui/docks/ColorArea.cpp
        src/ui/docks/ToolsArea.cpp
        src/ui/docks/ToolsArea.hpp
        src/ui/docks/LayersArea.cpp
        src/ui/docks/LayersArea.hpp
        src/ui/utils/ClickableLabel.cpp
        src/ui/utils/ClickableLabel.hpp
        src/ui/widgets/DrawingCanvasItem.hpp
        src/ui/widgets/DrawingCanvasItem.cpp
        src/ui/widgets/DrawingWidget.hpp
        src/ui/widgets/DrawingWidget.cpp
        src/ui/widgets/DockableWidget.hpp
        src/ui/widgets/DockableWidget.cpp
        src/ui/widgets/StatusBarWidget.hpp
        src/ui/widgets/StatusBarWidget.cpp
        src/ui/utils/SpacerUtils.hpp
        src/ui/utils/SpacerUtils.cpp
        src/ui/dialogs/NewFileDialog.cpp
        src/ui/dialogs/NewFileDialog.hpp
        src/io/ConfigurationManager.hpp
        src/io/ConfigurationManager.tpp
        src/io/ConfigurationManager.cpp
        src/io/ConsoleLogger.hpp
        src/io/ConsoleLogger.cpp
        src/io/ChunkFileReader.hpp
        src/io/ChunkFileReader.cpp
        src/io/ChunkFileWriter.hpp
        src/io/ChunkFileWriter.cpp
        src/io/ApplicationFilesystem.hpp
        src/io/ApplicationFilesystem.cpp
        src/io/JsonSerializable.hpp
        src/io/JsonSerializable.tpp
        src/utils/Converters.hpp
        src/utils/Converters.cpp
        src/utils/General.hpp
        src/utils/General.cpp
        src/meta/CompileTimeChecks.hpp
        src/meta/CompileTimeChecks.tpp
        src/meta/ErrorHandling.hpp
        src/meta/ErrorHandling.cpp
        src/meta/UnimplementedException.cpp
        src/meta/UnimplementedException.hpp
        src/ui/UiHelpers.cpp
        src/ui/UiHelpers.hpp
        src/plugin/DynamicLibrary.cpp
        src/plugin/DynamicLibrary.hpp
        src/plugin/DynamicLibrary.tpp
        src/plugin/ExternalPlugin.hpp
        src/plugin/ExternalPlugin.cpp
        src/network/AsyncFileDownloader.hpp
        src/network/AsyncFileDownloader.cpp)

set(APPLICATION_UI_FILES
        src/ui/MainWindow.ui
        src/ui/widgets/StatusBarWidget.ui
        src/ui/WelcomeScreen.ui
        src/ui/ConsoleWindow.ui
        src/ui/SettingsDialog.ui
        src/ui/dialogs/NewFileDialog.ui
        src/ui/widgets/delegates/ProjectDelegate.ui
        src/ui/docks/ToolsArea.ui
        src/ui/docks/LayersArea.ui
        src/ui/docks/ColorArea.ui)

set(TEST_FILES
        tests/algorithms/BresenhamTests.cpp
        tests/algorithms/AlphaBlendingTests.cpp
        tests/algorithms/LuminanceTests.cpp
        tests/user/PaletteTests.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests/user/resources/TestPalette.json
        ${CMAKE_CURRENT_BINARY_DIR}/tests/user/resources/TestPalette.json COPYONLY)

set(LIBS_TO_LINK
        Qt6::Core
        Qt6::Network
        Qt6::Widgets
        fmt::fmt
        rapidjson)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
find_package(fmt REQUIRED)
find_package(RapidJSON REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(
        CapyArtStudio
        src/main.cpp
        ${APPLICATION_SOURCE_FILES}
        ${APPLICATION_UI_FILES}
        resources/resources.qrc)

if (SKIP_COMPILER_FLAGS)
    message("Skipping compiler flags")
else ()
    if (MSVC)
        target_compile_options(CapyArtStudio PUBLIC "/W4" "$<$<CONFIG:RELEASE>:/O2>")
    else ()
        target_compile_options(
                CapyArtStudio
                PUBLIC "-Wall"
                "-Wextra"
                "-Werror"
                "-Wshadow"
                "-Wundef"
                "-Wcast-align"
                "-Wpointer-arith"
                "-Wwrite-strings"
                "-Wcast-qual"
                "-Wswitch-enum"
                "$<$<CONFIG:RELEASE>:-O3>")
    endif ()
endif ()

set_target_properties(
        CapyArtStudio
        PROPERTIES # C++ Settings
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        # Qt application settings
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.capyartstudio
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE)

target_link_libraries(CapyArtStudio PRIVATE ${LIBS_TO_LINK})

target_include_directories(CapyArtStudio PRIVATE src)

include(GNUInstallDirs)
install(
        TARGETS CapyArtStudio
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if (SKIP_TESTS)
    message("Skipping tests")
else ()
    find_package(GTest REQUIRED)
    if (NOT GTest_FOUND)
        message(FATAL_ERROR "No GTest Found")
    endif ()

    add_executable(CapyArtStudio_Tests tests/main.cpp ${TEST_FILES} ${APPLICATION_SOURCE_FILES})

    target_link_libraries(CapyArtStudio_Tests PRIVATE GTest::GTest ${LIBS_TO_LINK})

    set_target_properties(CapyArtStudio_Tests PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED YES)

    if (GMock_FOUND)
        target_link_libraries(CapyArtStudio_Tests INTERFACE GTest::GMock)
    endif ()

    target_include_directories(CapyArtStudio_Tests PRIVATE src)
    target_include_directories(CapyArtStudio_Tests PRIVATE tests)
endif ()
