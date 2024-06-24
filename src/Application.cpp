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

#include "utils/ConfigurationManager.hpp"
#include "Application.hpp"

namespace capy {
Application::Application(int argc, char** argv) :
  _guiApplication(argc, argv), _guiEngine({}) {
}

int Application::start() {
  registerMetadata();
  registerTypes();

  QObject::connect(
      &_guiEngine,
      &QQmlApplicationEngine::objectCreationFailed,
      &_guiApplication,
      [] { QCoreApplication::exit(-1); },
      Qt::QueuedConnection);
  _guiEngine.loadFromModule("CapyArtStudio", "Main");

  return _guiApplication.exec();
}

void Application::registerMetadata() {
  QCoreApplication::setOrganizationName("Zipeerix");
  QCoreApplication::setOrganizationDomain(
      "https://github.com/Zipeerix/capy-art-studio");
  QCoreApplication::setApplicationName("CapyArt Studio");
}

void Application::registerTypes() {
  qmlRegisterSingletonType<ConfigurationManager>(
      "Capy.ConfigurationManager", 1, 0, "ConfigurationManager",
      &ConfigurationManager::accessInstance);
}
} // capy