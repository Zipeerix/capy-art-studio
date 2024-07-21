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

#ifndef JSONSERIALIZABLE_TPP
#define JSONSERIALIZABLE_TPP

#include <fmt/format.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fstream>
#include <sstream>

#include "JsonSerializable.hpp"

namespace capy
{
template<class Derived>
void JsonSerializable<Derived>::markAsEdited()
{
  _wasEdited = true;
}

template<class Derived>
std::optional<std::string> JsonSerializable<Derived>::getPath() const
{
  return _path;
}

template<class Derived>
void JsonSerializable<Derived>::setPath(std::string path)
{
  _path = std::move(path);
}

template<class Derived>
bool JsonSerializable<Derived>::wasEditedFromLastSave() const
{
  return _wasEdited;
}

template<class Derived>
Result<Derived, std::string> JsonSerializable<Derived>::createFromJson(const std::string& path)
{
  using namespace rapidjson;

  static_assert(std::is_base_of_v<JsonSerializable, Derived>,
                "Derived must be a subclass of JsonSerializable");

  std::ifstream file(path);
  if (!file.is_open())
  {
    return std::unexpected(fmt::format("Unable to open JSON file at path: {}", path));
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string jsonContent = buffer.str();
  file.close();

  Document root;
  if (root.Parse(jsonContent.c_str()).HasParseError())
  {
    return std::unexpected(fmt::format("Invalid JSON file at path: {}", path));
  }

  Derived object;
  object._path = path;

  JsonSerializable& objectAsBase = object;
  const auto jsonParseError = objectAsBase.importValuesFromJson(root);
  if (jsonParseError.has_value())
  {
    return std::unexpected(fmt::format("Error when parsing JSON: {}", jsonParseError.value()));
  }

  return object;
}

template<class Derived>
PotentialError<std::string> JsonSerializable<Derived>::saveToJson(std::optional<std::string> path)
{
  using namespace rapidjson;

  static_assert(std::is_base_of_v<JsonSerializable, Derived>,
                "Derived must be a subclass of JsonSerializable");

  Document root = exportValuesToJson();

  StringBuffer buffer;
  Writer writer(buffer);
  root.Accept(writer);

  const std::string finalPath = path.has_value() ? path.value() : _path.value_or("");
  if (finalPath.empty())
  {
    return fmt::format("Unable to save JSON as no output path is given");
  }

  std::ofstream file(finalPath);
  if (!file.is_open())
  {
    return fmt::format("Unable to open JSON file for writing");
  }

  file << buffer.GetString();
  file.close();

  _wasEdited = false;
  return std::nullopt;
}
} // namespace capy

#endif // JSONSERIALIZABLE_TPP
