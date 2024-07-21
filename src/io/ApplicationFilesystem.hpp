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

#ifndef APPLICATIONFILESYSTEM_HPP
#define APPLICATIONFILESYSTEM_HPP

#include <string>
#include <vector>

// TODO: Refactor? Maybe seperate file for listFilesInPath  cuz its utils
// TODO: maybe io namespace

namespace capy
{
enum class FilesystemPath : int
{
  Palettes,
  Projects,
  PathCount
};

void initApplicationFilesystem();
std::string getConfigFilePath();
std::string getFilesystemPath(FilesystemPath of);
std::vector<std::string> getCorrectExtensionsForPath(FilesystemPath applicationPath);
std::vector<std::string> listFilesInPath(FilesystemPath applicationPath);
std::string getFileNameFromPath(const std::string& fullPath);
bool createFileIfItDoesntExist(const std::string& path);
} // namespace capy

#endif // APPLICATIONFILESYSTEM_HPP
