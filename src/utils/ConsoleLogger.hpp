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

namespace capy::logger {
enum class Severity {
  Default,
  Expected,
  Mild,
  Severe,
  Fatal,
};

void init();
void cleanup();
void showConsoleWindow();

void debug(const std::string& message, const std::string& module);
void info(const std::string& message);
void warning(const std::string& message, Severity severity);
void error(const std::string& message, Severity severity);
}  // namespace capy::logger

#endif  // CONSOLEMANAGER_HPP
