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

QString ConfigurationManager::getDebugSettingPath(const DebugSetting setting) {
  switch (setting) {
    case DebugSetting::ShowConsole:
      return "Debug/ShowConsole";

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

QString ConfigurationManager::getGraphicsSettingPath(const GraphicsSetting setting) {
  switch (setting) {
    case GraphicsSetting::DrawGrid:
      return "Graphics/DrawGrid";

    case GraphicsSetting::GridWidth:
      return "Graphics/GridWidth";

    default:
      throw std::logic_error("Invalid setting requested");
  }
}
}  // namespace capy