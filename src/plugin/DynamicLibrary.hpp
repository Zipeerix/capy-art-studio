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

#ifndef DYNAMICLIBRARY_HPP
#define DYNAMICLIBRARY_HPP

#if defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#elif defined(_WIN32)
// clang-format off
#include <windows.h>
#include <libloaderapi.h>
#include <errhandlingapi.h>
#include <winnt.rh>
// clang-format on
#endif

#include <expected>
#include <functional>
#include <string>

#include "meta/ErrorHandling.hpp"

namespace capy {
class DynamicLibrary {
 public:
#if defined(__linux__) || defined(__APPLE__)
  using HandleType = void*;
#elif defined(_WIN32)
  using HandleType = HMODULE;
#endif

  ~DynamicLibrary();

  [[nodiscard]] static Result<DynamicLibrary, std::string> fromFile(const std::string& path);

  bool isValid() const;

  template <typename SymbolType>
  [[nodiscard]] Result<SymbolType, std::string> getSymbol(const std::string& name);

 private:
  std::string _libPath;
  HandleType _libHandle;

  DynamicLibrary(std::string path, HandleType libHandle);

  static std::string getErrorMessage();
};
}  // namespace capy

#include "DynamicLibrary.tpp"

#endif  // DYNAMICLIBRARY_HPP
