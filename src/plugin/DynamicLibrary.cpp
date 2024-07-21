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

#include "DynamicLibrary.hpp"

#include <utility>

#include "io/ConsoleLogger.hpp"

namespace capy
{
DynamicLibrary::DynamicLibrary(std::string path, HandleType libHandle) :
    _libPath(std::move(path)),
    _libHandle(libHandle)
{
}

DynamicLibrary::~DynamicLibrary()
{
  if (_libHandle != nullptr)
  {
#if defined(__linux__) || defined(__APPLE__)
    const bool close_successful = dlclose(_libHandle) == 0;
#elif defined(_WIN32)
    int close_successful = FreeLibrary(_libHandle) != 0;
#endif
    if (!close_successful)
    {
      // it's a destructor, and it doesn't really matter if it fails to be honest so just log it
      const std::string err_msg = "Error when unloading dynamic library at: '" + _libPath +
                                  "'. Error: " + getErrorMessage();
      logger::error(err_msg, logger::Severity::Mild);
    }
  }
}

bool DynamicLibrary::isValid() const
{
  return _libHandle != nullptr;
}

Result<DynamicLibrary, std::string> DynamicLibrary::fromFile(const std::string& path)
{
#if defined(__linux__) || defined(__APPLE__)
  auto* const libHandle = dlopen(path.c_str(), RTLD_NOW);
#elif defined(_WIN32)
  const auto libHandle = LoadLibraryA(path.c_str());
#endif

  if (libHandle == nullptr)
  {
    return std::unexpected("Unable to open dynamic library at " + path);
  }

  return DynamicLibrary(path, libHandle);
}

std::string DynamicLibrary::getErrorMessage()
{
#if defined(__linux__) || defined(__APPLE__)
  const char* err = dlerror();
  return err == nullptr ? "Unknown error (id=0)" : std::string(err);
#elif defined(_WIN32)
  DWORD error = GetLastError();
  if (error == 0)
  {
    return "Unknown error (id=0)";
  }

  LPSTR buffer = nullptr;

  size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                       FORMAT_MESSAGE_IGNORE_INSERTS,
                               NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                               (LPSTR) &buffer, 0, NULL);

  std::string message(buffer, size);

  LocalFree(buffer);

  return message;
#endif
}
} // namespace capy