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

#include <fstream>
#include <graphics/Colors.hpp>

#include "io/ConsoleLogger.hpp"

namespace capy
{
Palette::Palette(std::string name) :
    JsonSerializable(),
    _name(std::move(name))
{
}

PotentialError<std::string> Palette::importValuesFromJson(const rapidjson::Document& root)
{
  if (!root.HasMember("name") || !root["name"].IsString())
  {
    return "No 'name' member";
  }

  _name = root["name"].GetString();

  if (!root.HasMember("colors") || !root["colors"].IsArray())
  {
    return "No 'colors' member";
  }

  for (const auto& colorObject: root["colors"].GetArray())
  {
    PaletteColor paletteColorEntry;
    if (colorObject.HasMember("hint"))
    {
      if (!colorObject["hint"].IsString())
      {
        return "Invalid 'hint' value"; // TOOD: add index value for error
      }

      paletteColorEntry.hint = colorObject["hint"].GetString();
    }

    if (!colorObject.HasMember("color") || !colorObject["color"].IsObject())
    {
      return "No inner color for color object"; // TODO: add index value for error
    }

    const auto innerColor = colorObject["color"].GetObject();
    if (!innerColor.HasMember("r") || !innerColor.HasMember("g") || !innerColor.HasMember("b") ||
        !innerColor.HasMember("alpha"))
    {
      return "Incomplete inner color"; // TODO: add index value for error
    }

    paletteColorEntry.color.setRed(innerColor["r"].GetInt());
    paletteColorEntry.color.setGreen(innerColor["g"].GetInt());
    paletteColorEntry.color.setBlue(innerColor["b"].GetInt());
    paletteColorEntry.color.setAlpha(innerColor["alpha"].GetInt());

    _colors.push_back(paletteColorEntry);
  }

  return std::nullopt;
}

rapidjson::Document Palette::exportValuesToJson() const
{
  rapidjson::Document document;
  document.SetObject();
  auto& allocator = document.GetAllocator();

  document.AddMember("name", rapidjson::Value(_name.c_str(), allocator), allocator);

  rapidjson::Value colorsArray(rapidjson::kArrayType);
  for (const auto& colorEntry: _colors)
  {
    rapidjson::Value colorObject(rapidjson::kObjectType);

    const char* hint = colorEntry.hint.has_value() ? colorEntry.hint->c_str() : "";
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

void Palette::addColor(const QColor& color, const std::optional<std::string>& hint)
{
  _colors.push_back(PaletteColor{color, hint});
}

void Palette::removeColor(int index)
{
  if (isIndexOutsideColors(index))
  {
    logger::error(fmt::format("Attempting to remove non-existent color at "
                              "index {} from palette {}",
                              index, _name),
                  logger::Severity::Mild);
    return;
  }

  _colors.erase(_colors.begin() + index);
  markAsEdited();
}

std::string Palette::getName() const
{
  return _name;
}

void Palette::setName(std::string newName)
{
  _name = std::move(newName);
  markAsEdited();
}

int Palette::colorCount() const
{
  return static_cast<int>(_colors.size());
}

PaletteColor Palette::getColor(const int index) const
{
  if (isIndexOutsideColors(index))
  {
    logger::error(fmt::format("Attempting to get non-existent color at "
                              "index {} from palette {}",
                              index, _name),
                  logger::Severity::Mild);
    // TODO: change return type to optional and return error
    return PaletteColor{QColor(0, 0, 0, constants::alpha::solidColor), "Error"};
  }

  return _colors.at(index);
}

std::vector<PaletteColor> Palette::getAllColors() const
{
  return _colors;
}

bool Palette::isIndexOutsideColors(const int index) const
{
  return index < 0 || static_cast<size_t>(index) >= _colors.size();
}
} // namespace capy