/*******************************************************************************
** Copyright (C) 2024  CapyArt Studio                                         **
**                                                                            **
** This program is free software: you can redistribute it and/or modify       **
** it under the terms of the GNU General Public License as published by       **
** the Free Software Foundation, either version 3 of the License, or          **
** (at your option) any later version.                                        **
**                                                                            **
** This program is distributed in the hope that it will be useful,            **
** but WITHOUT ANY WARRANTY; without even the implied warranty of             **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              **
** GNU General Public License for more details.                               **
**                                                                            **
** You should have received a copy of the GNU General Public License          **
** along with this program.  If not, see <https://www.gnu.org/licenses/>.     **
*******************************************************************************/

#include "Application.hpp"

#include "io/ApplicationFilesystem.hpp"
#include "io/ConsoleLogger.hpp"
#include "ui/MainWindow.hpp"
#include "ui/WelcomeScreen.hpp"

namespace capy
{
Application::Application() :
    _configurationManager(ConfigurationManager::createInstance()),
    _guiApplication(_cmdArgcOverwrite, nullptr)
{
  QApplication::setAttribute(
          Qt::AA_DontUseNativeMenuBar); // NOLINT(*-static-accessed-through-instance)
}

Application::~Application()
{
  logger::cleanup();
}

int Application::start() const
{
  if (_configurationManager->getDebugSetting<bool>(ConfigurationManager::DebugSetting::ShowConsole))
  {
    logger::init();
    logger::showConsoleWindow();
  }

  initApplicationFilesystem();

  ui::MainWindow mainWindow{};
  QMainWindow* firstWindowToShow = &mainWindow;
  std::unique_ptr<ui::WelcomeScreen> welcomeScreen = nullptr;
  if (_configurationManager->getApplicationSetting<bool>(
              ConfigurationManager::ApplicationSettings::ShowWelcomeScreen))
  {
    welcomeScreen = std::make_unique<ui::WelcomeScreen>(&mainWindow);
    firstWindowToShow = welcomeScreen.get();
  }

  firstWindowToShow->show();

  return _guiApplication.exec(); // NOLINT(*-static-accessed-through-instance)
}

void Application::registerMetadata()
{
  QCoreApplication::setOrganizationName("Zipeerix");
  QCoreApplication::setOrganizationDomain("https://github.com/Zipeerix/capy-art-studio");
  QCoreApplication::setApplicationName("CapyArt Studio");

  qRegisterMetaType<GraphicalBackend>("GraphicalBackend");
}
} // namespace capy