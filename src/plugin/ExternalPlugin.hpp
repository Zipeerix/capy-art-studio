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

#ifndef EXTERNALPLUGIN_HPP
#define EXTERNALPLUGIN_HPP

#include <expected>
#include <string>

#include "DynamicLibrary.hpp"

namespace capy {
class ExternalPlugin {
 public:
  ExternalPlugin() = delete;

  [[nodiscard]] static Result<ExternalPlugin, std::string> fromFile(const std::string& path);

 private:
  explicit ExternalPlugin(const DynamicLibrary& dynLib);

  DynamicLibrary _dynLib;
};
}  // namespace capy

#endif  // EXTERNALPLUGIN_HPP
