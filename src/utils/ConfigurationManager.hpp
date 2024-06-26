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

#ifndef CONFIGURATIONMANAGER_HPP
#define CONFIGURATIONMANAGER_HPP

#include <memory>
#include <QSettings>

namespace capy {
class ConfigurationManager {
public:
  ConfigurationManager(ConfigurationManager&) = delete;
  void operator=(const ConfigurationManager&) = delete;

  static std::shared_ptr<ConfigurationManager> createInstance();

  // TODO This is a placeholder, think of a way how to get/set settings
  [[nodiscard]] bool getShouldDrawGrid() const;
  [[nodiscard]] double getGridWidth() const;

protected:
  ConfigurationManager() = default;

private:
  QSettings _settings{};
};
} // capy

#endif //CONFIGURATIONMANAGER_HPP
