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

#include "ConfigurationManager.hpp"

#include <QColor>

namespace capy {
std::shared_ptr<ConfigurationManager> ConfigurationManager::createInstance() {
  static std::shared_ptr<ConfigurationManager> singletonEntity(new ConfigurationManager());
  return singletonEntity;
}

bool ConfigurationManager::getShouldDrawGrid() const {
  return _settings.value("general/drawGrid", true).toBool();
}

double ConfigurationManager::getGridWidth() const {
  // also do color
  return _settings.value("general/gridWidth", 0.3).toDouble();
}

bool ConfigurationManager::getEnableConsole() const {
  return _settings.value("general/showConsole", true).toBool();
}
}  // namespace capy