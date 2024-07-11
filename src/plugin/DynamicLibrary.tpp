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

#ifndef DYNAMICLIBRARY_TPP
#define DYNAMICLIBRARY_TPP

#include "DynamicLibrary.hpp"
#include <stdexcept>

namespace capy {
template <typename SymbolType>
Result<SymbolType, std::string> DynamicLibrary::getSymbol(const std::string& name) {
  if (_libHandle == nullptr || _libPath.empty()) {
    throw std::logic_error("Attempting to find symbol '" + name +
                           "' from dynamic library that was not loaded");
  }

#if defined(__linux__) || defined(__APPLE__)
  void* symbol = dlsym(_libHandle, name.c_str());
#elif defined(_WIN32)
  void* symbol = (void*)GetProcAddress(_libHandle, name.c_str());
#endif
  if (symbol == nullptr) {
    const std::string errorMessage = "Unable to find symbol '" + name +
                                     "' in dynamic library at: " + _libPath +
                                     ". Error: " + getErrorMessage();
    return std::unexpected(errorMessage);
  }

  return (SymbolType)symbol;
}
}  // namespace capy

#endif  // DYNAMICLIBRARY_TPP
