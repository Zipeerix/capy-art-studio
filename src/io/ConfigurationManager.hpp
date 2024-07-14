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

namespace capy {
class ConfigurationManager {
 public:
  enum class InternalValues : int { ProjectPaths };

  enum class ApplicationSettings : int { ShowWelcomeScreen };

  enum class DebugSetting : int {
    ShowConsole,
  };

  enum class GraphicsSetting : int {
    DrawGrid,
    GridWidth,
    GridDrawingZoomThreshold
  };

  ConfigurationManager(ConfigurationManager&) = delete;
  void operator=(const ConfigurationManager&) = delete;

  static std::shared_ptr<ConfigurationManager> createInstance();

  // TODO: Maybe don't seperate into categories, a lot of syf especialyl for ui linking

  template <typename SettingValueType>
  SettingValueType getApplicationSetting(ApplicationSettings setting) const;

  template <typename SettingValueType>
  void setApplicationSetting(ApplicationSettings setting, SettingValueType value);

  template <typename SettingValueType>
  SettingValueType getDebugSetting(DebugSetting setting) const;

  template <typename SettingValueType>
  void setDebugSetting(DebugSetting setting, SettingValueType value);

  template <typename SettingValueType>
  SettingValueType getGraphicsSetting(GraphicsSetting setting) const;

  template <typename SettingValueType>
  void setGraphicsSetting(GraphicsSetting setting, SettingValueType value);

  void addAdditionalProjectPath(const std::string& path);
  void removeAdditionalProjectPath(const std::string& pathToDelete);
  bool doesAdditionalProjectExist(const std::string& path) const;
  std::vector<std::string> getAdditionalProjectsPaths() const;

 protected:
  ConfigurationManager() = default;

 private:
  QSettings _settings{};

  static QString getInternalValuePath(InternalValues value);
  static QString getApplicationSettingPath(ApplicationSettings setting);
  static QString getDebugSettingPath(DebugSetting setting);
  static QString getGraphicsSettingPath(GraphicsSetting setting);
};
}  // namespace capy

#include "ConfigurationManager.tpp"

#endif  // CONFIGURATIONMANAGER_HPP
