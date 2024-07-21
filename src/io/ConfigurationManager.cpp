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

#include <fmt/format.h>

#include <QColor>

#include "ConsoleLogger.hpp"

namespace capy
{
std::shared_ptr<ConfigurationManager> ConfigurationManager::createInstance()
{
  static std::shared_ptr<ConfigurationManager> singletonEntity(new ConfigurationManager());
  return singletonEntity;
}

QString ConfigurationManager::getInternalValuePath(const InternalValues value)
{
  switch (value)
  {
    case InternalValues::ProjectPaths:
      return "Internal/ProjectPaths";

    case InternalValues::WindowGeometry:
      return "Internal/WindowGeometry";

    default:
      throw std::logic_error("Invalid value requested");
  }
}

QString ConfigurationManager::getApplicationSettingPath(const ApplicationSettings setting)
{
  switch (setting)
  {
    case ApplicationSettings::ShowWelcomeScreen:
      return "Application/ShowWelcomeScreen";

    case ApplicationSettings::ShowStatusBar:
      return "Application/ShowStatusBar";

    case ApplicationSettings::StatusBarUpdateInterval:
      return "Application/StatusBarUpdateInterval";

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

QString ConfigurationManager::getDebugSettingPath(const DebugSetting setting)
{
  switch (setting)
  {
    case DebugSetting::ShowConsole:
      return "Debug/ShowConsole";

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

QString ConfigurationManager::getGraphicsSettingPath(const GraphicsSetting setting)
{
  switch (setting)
  {
    case GraphicsSetting::GraphicalBackend:
      return "Graphics/Backend";

    case GraphicsSetting::DrawGrid:
      return "Graphics/DrawGrid";

    case GraphicsSetting::GridWidth:
      return "Graphics/GridWidth";

    case GraphicsSetting::GridDrawingZoomThreshold:
      return "Graphics/GridDrawingZoomThreshold";

    default:
      throw std::logic_error("Invalid setting requested");
  }
}

void ConfigurationManager::addAdditionalProjectPath(const std::string& path)
{
  const auto valuePath = getInternalValuePath(InternalValues::ProjectPaths);
  auto list = _settings.value(valuePath, {}).toStringList();

  if (doesAdditionalProjectExist(path))
  {
    logger::error(fmt::format("There is a duplicate project with path: {}", path),
                  logger::Severity::Mild);
    return;
  }

  list.push_back(QString::fromStdString(path));

  _settings.setValue(valuePath, list);
  _settings.sync();
}

void ConfigurationManager::removeAdditionalProjectPath(const std::string& pathToDelete)
{
  const auto valuePath = getInternalValuePath(InternalValues::ProjectPaths);
  auto list = _settings.value(valuePath, {}).toStringList();

  list.erase(std::remove_if(list.begin(), list.end(),
                            [pathToDelete](const QString& path) {
                              return path.toStdString() == pathToDelete;
                            }),
             list.cend());

  _settings.setValue(valuePath, list);
  _settings.sync();
}

std::vector<std::string> ConfigurationManager::getAdditionalProjectsPaths() const
{
  const auto internalList =
          _settings.value(getInternalValuePath(InternalValues::ProjectPaths), {}).toStringList();

  std::vector<std::string> listToReturn{};
  listToReturn.reserve(internalList.size());

  for (const auto& path: internalList)
  {
    const bool isDuplicate =
            std::ranges::any_of(listToReturn, [path](const std::string& alreadyAddedPath) {
              return alreadyAddedPath == path.toStdString();
            });

    if (isDuplicate)
    {
      logger::error(fmt::format("There is a duplicate project with path: {}", path.toStdString()),
                    logger::Severity::Mild);
      continue;
    }

    listToReturn.push_back(path.toStdString());
  }

  return listToReturn;
}

bool ConfigurationManager::doesAdditionalProjectExist(const std::string& path) const
{
  const auto projects = getAdditionalProjectsPaths();
  return std::ranges::any_of(projects, [path](const std::string& alreadyAddedPath) {
    return alreadyAddedPath == path;
  });
}

std::optional<QByteArray> ConfigurationManager::getWindowGeometry(const std::string& name) const
{
  const auto path =
          getInternalValuePath(InternalValues::WindowGeometry) + "/" + QString::fromStdString(name);
  QByteArray value = _settings.value(path, {}).toByteArray();

  return value.isEmpty() ? std::nullopt : std::optional(value);
}

void ConfigurationManager::setWindowGeometry(const std::string& name, const QByteArray& geometry)
{
  const auto path =
          getInternalValuePath(InternalValues::WindowGeometry) + "/" + QString::fromStdString(name);
  _settings.setValue(path, geometry);
}
} // namespace capy