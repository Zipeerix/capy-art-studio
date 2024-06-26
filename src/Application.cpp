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

namespace capy {
Application::Application(int argc, char** argv)
    : _guiApplication(argc, argv), _configurationManager(ConfigurationManager::createInstance()) {
  _guiApplication.setAttribute(Qt::AA_DontUseNativeMenuBar);
}

Application::~Application() { logger::cleanup(); }

int Application::start() {
  if (_configurationManager->getDebugSetting<bool>(
          ConfigurationManager::DebugSetting::ShowConsole)) {
    logger::init();
    logger::showConsoleWindow();
  }

  initApplicationFilesystem();

  ui::MainWindow mainWindow;
  mainWindow.show();

  return _guiApplication.exec();
}

void Application::registerMetadata() {
  QCoreApplication::setOrganizationName("Zipeerix");
  QCoreApplication::setOrganizationDomain("https://github.com/Zipeerix/capy-art-studio");
  QCoreApplication::setApplicationName("CapyArt Studio");
}
}  // namespace capy