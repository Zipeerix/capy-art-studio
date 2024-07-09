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

#ifndef JSONSERIALIZABLE_HPP
#define JSONSERIALIZABLE_HPP

#include <rapidjson/document.h>
#include "utils/ErrorHandling.hpp"

namespace capy {
template <class Derived>
class JsonSerializable {
public:
  virtual ~JsonSerializable() = default;

  static Result<Derived, std::string> createFromJson(const std::string& path);
  // TODO: Only save when _wasEdited=true
  PotentialError<std::string> saveToJson(std::optional<std::string> path);

  std::optional<std::string> getPath() const;
  bool wasEditedFromLastSave() const;

protected:
  void markAsEdited();

private:
  std::optional<std::string> _path;
  bool _wasEdited = false;

  virtual PotentialError<std::string> importValuesFromJson(const rapidjson::Document& root) = 0;
  virtual rapidjson::Document exportValuesToJson() const = 0;
};
} // capy

#include "JsonSerializable.tpp"

#endif //JSONSERIALIZABLE_HPP
