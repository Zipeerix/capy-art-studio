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

#include "ConsoleLogger.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>

#include "ui/ConsoleWindow.hpp"

namespace capy::logger
{
namespace
{
// TODO: clang-tidy disabled until changing the way the logger works
// NOLINTBEGIN
bool isEnabled = false;
ui::ConsoleWindow* consoleWindow = nullptr;
// NOLINTEND

std::string getDateTimeString()
{
  const auto localTime = std::chrono::system_clock::now();
  return fmt::format("{:%F %T}", localTime);
}

std::string severityToString(Severity severity)
{
  switch (severity)
  {
    case Severity::Default:
      return "";

    case Severity::Expected:
      return "Expected";

    case Severity::Mild:
      return "Mild";

    case Severity::Severe:
      return "Severe";

    case Severity::Fatal:
      return "Fatal";

    default:
      throw std::runtime_error("Unhandled severity->string conversion");
  }
}
} // namespace

void log(const std::string& message, const std::string& extraInfo)
{
  if (!isEnabled)
  {
    return;
  }

  const auto logMessage =
          fmt::format("[{}] <b>{}</b>: {}", getDateTimeString(), extraInfo, message);
  consoleWindow->log(QString::fromStdString(logMessage));
}

void init()
{
  isEnabled = true;
  consoleWindow = new ui::ConsoleWindow();
}

void cleanup()
{
  if (!isEnabled)
  {
    return;
  }

  isEnabled = false;
  delete consoleWindow;
}

void showConsoleWindow()
{
  if (!isEnabled)
  {
    return;
  }

  consoleWindow->show();
}

void hideConsoleWindow()
{
  if (!isEnabled)
  {
    return;
  }

  consoleWindow->hide();
}

void debug(const std::string& message, const std::string& module)
{
  log(message, fmt::format("<font color=\"blue\">DEBUG<{}></font>", module));
}

void info(const std::string& message)
{
  log(message, "INFO");
}

void warning(const std::string& message, const Severity severity)
{
  log(message,
      fmt::format("<font color=\"yellow\">WARNING<{}></font>", severityToString(severity)));
}

void error(const std::string& message, const Severity severity)
{
  log(message, fmt::format("<font color=\"red\">ERROR<{}></font>", severityToString(severity)));
}
} // namespace capy::logger