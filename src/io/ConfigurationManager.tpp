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
#include "graphics/GraphicalBackend.hpp"
#include "meta/CompileTimeChecks.hpp"

namespace capy {
template <typename SettingValueType>
SettingValueType ConfigurationManager::getDebugSetting(const DebugSetting setting) const {
  switch (setting) {
    case DebugSetting::ShowConsole:
      compileTimeTypeCheck<SettingValueType, bool>();
      return _settings.value(getDebugSettingPath(setting), true).toBool();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template <typename SettingValueType>
void ConfigurationManager::setDebugSetting(const DebugSetting setting,
                                           const SettingValueType value) {
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
SettingValueType ConfigurationManager::getGraphicsSetting(const GraphicsSetting setting) const {
  switch (setting) {
    case GraphicsSetting::GraphicalBackend:
      // TODO: When CAS-169 gets done this should not be converted to int, this method should return
      // GraphicsBackend
      // TODO: and for default value use QVariant::fromValue(GraphicalBackend::QtSoftware)
      compileTimeTypeCheck<SettingValueType, int>();
      return _settings
          .value(getGraphicsSettingPath(setting), static_cast<int>(GraphicalBackend::QtSoftware))
          .toInt();

    case GraphicsSetting::DrawGrid:
      compileTimeTypeCheck<SettingValueType, bool>();
      return _settings.value(getGraphicsSettingPath(setting), true).toBool();

    case GraphicsSetting::GridWidth:
      compileTimeTypeCheck<SettingValueType, double>();
      return _settings.value(getGraphicsSettingPath(setting), 0.3).toDouble();

    case GraphicsSetting::GridDrawingZoomThreshold:
      compileTimeTypeCheck<SettingValueType, double>();
      return _settings.value(getGraphicsSettingPath(setting), 5.0).toDouble();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template <typename SettingValueType>
void ConfigurationManager::setGraphicsSetting(const GraphicsSetting setting,
                                              const SettingValueType value) {
  switch (setting) {
    case GraphicsSetting::GraphicalBackend:
      compileTimeTypeCheck<SettingValueType, GraphicalBackend>();
      break;

    case GraphicsSetting::DrawGrid:
      compileTimeTypeCheck<SettingValueType, bool>();
      break;

    case GraphicsSetting::GridWidth:
    case GraphicsSetting::GridDrawingZoomThreshold:
      compileTimeTypeCheck<SettingValueType, double>();
      break;

    default:
      throw std::logic_error("Invalid setting requested");
  }

  _settings.setValue(getGraphicsSettingPath(setting), value);

  _settings.sync();
}

template <typename SettingValueType>
SettingValueType ConfigurationManager::getApplicationSetting(ApplicationSettings setting) const {
  switch (setting) {
    case ApplicationSettings::ShowStatusBar:
    case ApplicationSettings::ShowWelcomeScreen:
      compileTimeTypeCheck<SettingValueType, bool>();
      // TODO: add getter for default values and then all cases can be combined for each type
      return _settings.value(getApplicationSettingPath(setting), true).toBool();

    case ApplicationSettings::StatusBarUpdateInterval:
      compileTimeTypeCheck<SettingValueType, int>();
      return _settings.value(getApplicationSettingPath(setting), 3).toInt();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template <typename SettingValueType>
void ConfigurationManager::setApplicationSetting(ApplicationSettings setting,
                                                 SettingValueType value) {
  switch (setting) {
    case ApplicationSettings::ShowStatusBar:
    case ApplicationSettings::ShowWelcomeScreen:
      compileTimeTypeCheck<SettingValueType, bool>();
      _settings.setValue(getApplicationSettingPath(setting), value);
      break;

    case ApplicationSettings::StatusBarUpdateInterval:
      compileTimeTypeCheck<SettingValueType, int>();
      _settings.setValue(getApplicationSettingPath(setting), value);
      break;

    default:
      throw std::logic_error("Invalid setting requested");
  }

  _settings.sync();
}
}  // namespace capy

#endif  // CONFIGURATIONMANAGER_TPP
