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

#include <QSettings>
#include <memory>

namespace capy {
class ConfigurationManager {
 public:
  enum class DebugSetting : int {
    ShowConsole,
  };

  enum class GraphicsSetting : int {
    DrawGrid,
    GridWidth,
  };

  ConfigurationManager(ConfigurationManager&) = delete;
  void operator=(const ConfigurationManager&) = delete;

  static std::shared_ptr<ConfigurationManager> createInstance();

  template <typename SettingValueType>
  SettingValueType getDebugSetting(DebugSetting setting) const;

  template <typename SettingValueType>
  void setDebugSetting(DebugSetting setting, SettingValueType value);

  template <typename SettingValueType>
  SettingValueType getGraphicsSetting(GraphicsSetting setting) const;

  template <typename SettingValueType>
  void setGraphicsSetting(GraphicsSetting setting, SettingValueType value);

 protected:
  ConfigurationManager() = default;

 private:
  QSettings _settings{};

  [[nodiscard]] static QString getDebugSettingPath(DebugSetting setting);
  [[nodiscard]] static QString getGraphicsSettingPath(GraphicsSetting setting);
};
}  // namespace capy

#include "ConfigurationManager.tpp"

#endif  // CONFIGURATIONMANAGER_HPP
