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

#ifndef CONSOLEMANAGER_HPP
#define CONSOLEMANAGER_HPP

#include <QString>
#include <string>

namespace capy {
// TODO: Change to namespace?
class ConsoleLogger {
 public:
  enum class Severity {
    Default,
    Expected,
    Mild,
    Severe,
    Fatal,
  };

  ConsoleLogger(ConsoleLogger&) = delete;
  void operator=(const ConsoleLogger&) = delete;

  static void init();
  static void cleanup();
  static void showConsoleWindow();

  static void debug(const std::string& message, const std::string& module);
  static void info(const std::string& message);
  static void warning(const std::string& message, Severity severity);
  static void error(const std::string& message, Severity severity);

 protected:
  ConsoleLogger() = default;

  static std::string severityToString(Severity severity);
  static void log(const std::string& message, const std::string& extraInfo);
  static std::string getDateTimeString();
};
}  // namespace capy

#endif  // CONSOLEMANAGER_HPP
