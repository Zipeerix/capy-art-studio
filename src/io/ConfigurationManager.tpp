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

#ifndef CONFIGURATIONMANAGER_TPP
#define CONFIGURATIONMANAGER_TPP

#include "ConfigurationManager.hpp"

namespace capy {
// TODO: Move to utils
template <typename T, typename U>
static void compileTimeTypeCheck() {
  // TODO: This probably doesn't work, check, maybe don't do any checks
  if (!std::is_same<T, U>::value) {
    throw std::logic_error("Compile type type check failed");
  }
}

template <typename SettingValueType>
SettingValueType ConfigurationManager::getDebugSetting(DebugSetting setting) const {
  switch (setting) {
    case DebugSetting::ShowConsole:
      compileTimeTypeCheck<SettingValueType, bool>();
      return _settings.value(getDebugSettingPath(setting), true).toBool();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template <typename SettingValueType>
void ConfigurationManager::setDebugSetting(DebugSetting setting, SettingValueType value) {
  switch (setting) {
    case DebugSetting::ShowConsole:
      compileTimeTypeCheck<SettingValueType, bool>();
      _settings.setValue(getDebugSettingPath(setting), value);
      break;

    default:
      throw std::logic_error("Invalid setting requested");
  }

  _settings.sync();
}

template <typename SettingValueType>
SettingValueType ConfigurationManager::getGraphicsSetting(GraphicsSetting setting) const {
  switch (setting) {
    case GraphicsSetting::DrawGrid:
      compileTimeTypeCheck<SettingValueType, bool>();
      return _settings.value(getGraphicsSettingPath(setting), true).toBool();

    case GraphicsSetting::GridWidth:
      compileTimeTypeCheck<SettingValueType, double>();
      return _settings.value(getGraphicsSettingPath(setting), 0.3).toDouble();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template <typename SettingValueType>
void ConfigurationManager::setGraphicsSetting(GraphicsSetting setting, SettingValueType value) {
  switch (setting) {
    case GraphicsSetting::DrawGrid:
      compileTimeTypeCheck<SettingValueType, bool>();
      _settings.setValue(getGraphicsSettingPath(setting), value);
      break;

    case GraphicsSetting::GridWidth:
      compileTimeTypeCheck<SettingValueType, double>();
      _settings.setValue(getGraphicsSettingPath(setting), value);
      break;

    default:
      throw std::logic_error("Invalid setting requested");
  }

  _settings.sync();
}
}  // namespace capy

#endif  // CONFIGURATIONMANAGER_TPP
