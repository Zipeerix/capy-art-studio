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

#include "ExternalPlugin.hpp"

namespace capy
{
ExternalPlugin::ExternalPlugin(const DynamicLibrary& dynLib) :
    _dynLib(dynLib)
{
  if (!_dynLib.isValid())
  {
    throw std::logic_error(
            "Attempting to create plugin with invalid dynamib library, check error handling");
  }
}

Result<ExternalPlugin, std::string> ExternalPlugin::fromFile(const std::string& path)
{
  const auto dynLibLoadRes = DynamicLibrary::fromFile(path);
  if (!dynLibLoadRes.has_value())
  {
    return std::unexpected("Unable to load plugin as its file is invalid/corrupted");
  }

  const DynamicLibrary& dynLib = dynLibLoadRes.value();

  // TODO: Validte dynlib has all the needed functions in order to be a plugin in helper fns when
  // API decided

  return ExternalPlugin(dynLib);
}
} // namespace capy