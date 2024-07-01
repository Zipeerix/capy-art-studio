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

#include "Palette.hpp"

#include <fmt/format.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <fstream>
#include <sstream>

#include "../io/ConsoleLogger.hpp"

namespace capy {
Palette::Palette(std::string name) : _name(std::move(name)) {
  _wasEdited = true;  // Created = edited
}

std::expected<Palette, std::string> Palette::fromJson(const std::string& path) {
  using namespace rapidjson;

  std::ifstream file(path);
  if (!file.is_open()) {
    return std::unexpected("Unable to open Palette JSON file at path: " + path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  std::string jsonContent = buffer.str();
  file.close();

  Document root;
  if (root.Parse(jsonContent.c_str()).HasParseError()) {
    return std::unexpected("Invalid Palette JSON file at path: " + path);
  }

  Palette palette;
  palette._path = path;
  const auto jsonParseResult = palette.importValuesFromJson(root);
  if (!jsonParseResult.has_value()) {
    return std::unexpected("Error when parsing Palette JSON: " + jsonParseResult.error());
  }

  return palette;
}

std::expected<void, std::string> Palette::saveToJson(std::optional<std::string> path) const {
  using namespace rapidjson;

  Document root = exportValuesToJson();
  Document::AllocatorType& allocator = root.GetAllocator();

  StringBuffer buffer;
  Writer writer(buffer);
  root.Accept(writer);

  const std::string finalPath = path.has_value() ? path.value() : _path.value_or("");
  if (finalPath.empty()) {
    return std::unexpected("Unable to save Palette JSON as no output path is given");
  }

  std::ofstream file(finalPath);
  if (!file.is_open()) {
    return std::unexpected("Unable to open Palette JSON file to writing");
  }

  file << buffer.GetString();
  file.close();

  return {};
}

std::expected<void, std::string> Palette::importValuesFromJson(const rapidjson::Document& root) {
  if (!root.HasMember("name") || !root["name"].IsString()) {
    return std::unexpected("No 'name' member");
  }

  _name = root["name"].GetString();

  if (!root.HasMember("colors") || !root["colors"].IsArray()) {
    return std::unexpected("No 'colors' member");
  }

  for (const auto& colorObject : root["colors"].GetArray()) {
    PaletteColor paletteColorEntry;
    if (colorObject.HasMember("hint")) {
      if (!colorObject["hint"].IsString()) {
        return std::unexpected("Invalid 'hint' value");  // TOOD: add index value for error
      }

      paletteColorEntry.hint = colorObject["hint"].GetString();
    }

    if (!colorObject.HasMember("color") || !colorObject["color"].IsObject()) {
      return std::unexpected("No inner color for color object");  // TODO: add index value for error
    }

    const auto innerColor = colorObject["color"].GetObject();
    if (!innerColor.HasMember("r") || !innerColor.HasMember("g") || !innerColor.HasMember("b") ||
        !innerColor.HasMember("alpha")) {
      return std::unexpected("Incomplete inner color");  // TODO: add index value for error
    }

    paletteColorEntry.color.setRed(innerColor["r"].GetInt());
    paletteColorEntry.color.setGreen(innerColor["g"].GetInt());
    paletteColorEntry.color.setBlue(innerColor["b"].GetInt());
    paletteColorEntry.color.setAlpha(innerColor["alpha"].GetInt());

    _colors.push_back(paletteColorEntry);
  }

  return {};
}

rapidjson::Document Palette::exportValuesToJson() const {
  rapidjson::Document document;
  document.SetObject();
  auto& allocator = document.GetAllocator();

  document.AddMember("name", rapidjson::Value(_name.c_str(), allocator), allocator);

  rapidjson::Value colorsArray(rapidjson::kArrayType);
  for (const auto& colorEntry : _colors) {
    rapidjson::Value colorObject(rapidjson::kObjectType);

    const auto hint = colorEntry.hint.has_value() ? colorEntry.hint->c_str() : "";
    colorObject.AddMember("hint", rapidjson::Value(hint, allocator), allocator);

    rapidjson::Value colorValues(rapidjson::kObjectType);
    colorValues.AddMember("r", colorEntry.color.red(), allocator);
    colorValues.AddMember("g", colorEntry.color.green(), allocator);
    colorValues.AddMember("b", colorEntry.color.blue(), allocator);
    colorValues.AddMember("alpha", colorEntry.color.alpha(), allocator);

    colorObject.AddMember("color", colorValues, allocator);

    colorsArray.PushBack(colorObject, allocator);
  }

  document.AddMember("colors", colorsArray, allocator);

  return document;
}

bool Palette::wasEditedFromLastLoad() const { return _wasEdited; }

void Palette::addColor(const QColor& color, const std::optional<std::string>& hint) {
  _colors.push_back({color, hint});
}

void Palette::removeColor(int index) {
  if (index >= _colors.size()) {
    logger::error(fmt::format("Attempting to remove non-existent color at "
                              "index {} from palette {}",
                              index, _name),
                  logger::Severity::Mild);
    return;
  }

  _colors.erase(_colors.begin() + index);
}

std::string Palette::getName() const { return _name; }

std::optional<std::string> Palette::getPath() const { return _path; }

void Palette::setName(std::string newName) { _name = std::move(newName); }

int Palette::colorCount() const { return _colors.size(); }

QColor Palette::getColor(int index) const {
  if (index >= _colors.size()) {
    logger::error(fmt::format("Attempting to get non-existent color at "
                              "index {} from palette {}",
                              index, _name),
                  logger::Severity::Mild);
    return QColor(0, 0, 0, 255);
  }

  return _colors.at(index).color;
}

std::vector<PaletteColor> Palette::getAllColors() const { return _colors; }
}  // namespace capy