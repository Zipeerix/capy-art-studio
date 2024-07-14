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

#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <QPixmap>
#include <string>

#include "utils/ErrorHandling.hpp"

namespace capy {
class Project {
 public:
  static Result<Project, std::string> createFromFile(const std::string& path);

  std::string getPath() const;
  std::string getName() const;

  QPixmap getMiniature() const;

 private:
  Project(std::string path, QPixmap miniature);

  std::string _path;
  QPixmap _miniature;
};
}  // namespace capy

#endif  // PROJECT_HPP
