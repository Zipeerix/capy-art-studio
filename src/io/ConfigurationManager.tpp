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

#include "graphics/GraphicalBackend.hpp"
#include "meta/CompileTimeChecks.hpp"

namespace capy
{
namespace constants::defaultValue
{
// Graphics
constexpr auto graphicsBackend = GraphicalBackend::QtSoftware;
constexpr bool drawGrid = true;
constexpr double gridWidth = 0.3;
constexpr double gridDrawingZoomThreshold = 5.0;

// Application
constexpr bool showStatusBar = true;
constexpr bool showWelcomeScreen = true;
constexpr int statusBarUpdateInterval = 5;

// Debug
constexpr bool showConsole = false;
} // namespace constants::defaultValue

template<typename SettingValueType>
SettingValueType ConfigurationManager::getDebugSetting(const DebugSetting setting) const
{
  switch (setting)
  {
    case DebugSetting::ShowConsole:
      compileTimeTypeCheck<SettingValueType, bool>();
      return _settings.value(getDebugSettingPath(setting), constants::defaultValue::showConsole)
              .toBool();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template<typename SettingValueType>
void ConfigurationManager::setDebugSetting(const DebugSetting setting, const SettingValueType value)
{
  switch (setting)
  {
    case DebugSetting::ShowConsole:
      compileTimeTypeCheck<SettingValueType, bool>();
      _settings.setValue(getDebugSettingPath(setting), value);
      break;

    default:
      throw std::logic_error("Invalid setting requested");
  }

  _settings.sync();
}

template<typename SettingValueType>
SettingValueType ConfigurationManager::getGraphicsSetting(const GraphicsSetting setting) const
{
  switch (setting)
  {
    case GraphicsSetting::GraphicalBackend:
      // TODO: When CAS-169 gets done this should not be converted to int, this method should return
      // GraphicsBackend
      // TODO: and for default value use QVariant::fromValue(GraphicalBackend::QtSoftware)
      compileTimeTypeCheck<SettingValueType, int>();
      return _settings
              .value(getGraphicsSettingPath(setting),
                     static_cast<int>(constants::defaultValue::graphicsBackend))
              .toInt();

    case GraphicsSetting::DrawGrid:
      compileTimeTypeCheck<SettingValueType, bool>();
      return _settings.value(getGraphicsSettingPath(setting), constants::defaultValue::drawGrid)
              .toBool();

    case GraphicsSetting::GridWidth:
      compileTimeTypeCheck<SettingValueType, double>();
      return _settings.value(getGraphicsSettingPath(setting), constants::defaultValue::gridWidth)
              .toDouble();

    case GraphicsSetting::GridDrawingZoomThreshold:
      compileTimeTypeCheck<SettingValueType, double>();
      return _settings
              .value(getGraphicsSettingPath(setting),
                     constants::defaultValue::gridDrawingZoomThreshold)
              .toDouble();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template<typename SettingValueType>
void ConfigurationManager::setGraphicsSetting(const GraphicsSetting setting,
                                              const SettingValueType value)
{
  switch (setting)
  {
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

template<typename SettingValueType>
SettingValueType ConfigurationManager::getApplicationSetting(ApplicationSettings setting) const
{
  switch (setting)
  {
    case ApplicationSettings::ShowStatusBar:
    case ApplicationSettings::ShowWelcomeScreen:
      compileTimeTypeCheck<SettingValueType, bool>();
      // TODO: add getter for default values and then all cases can be combined for each type
      static_assert(constants::defaultValue::showStatusBar ==
                    constants::defaultValue::showWelcomeScreen);
      return _settings
              .value(getApplicationSettingPath(setting), constants::defaultValue::showStatusBar)
              .toBool();

    case ApplicationSettings::StatusBarUpdateInterval:
      compileTimeTypeCheck<SettingValueType, int>();
      return _settings
              .value(getApplicationSettingPath(setting),
                     constants::defaultValue::statusBarUpdateInterval)
              .toInt();

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

template<typename SettingValueType>
void ConfigurationManager::setApplicationSetting(ApplicationSettings setting,
                                                 SettingValueType value)
{
  switch (setting)
  {
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
} // namespace capy

#endif // CONFIGURATIONMANAGER_TPP
