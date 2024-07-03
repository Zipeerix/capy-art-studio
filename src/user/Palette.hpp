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

#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <rapidjson/document.h>

#include <QColor>
#include <expected>

#include "utils/ErrorHandling.hpp"

namespace capy {
struct PaletteColor {
  QColor color;
  std::optional<std::string> hint;
};

class Palette {
 public:
  Palette() = default;
  explicit Palette(std::string name);

  [[nodiscard]] static Result<Palette, std::string> fromJson(const std::string& path);
  [[nodiscard]] PotentialError<std::string> saveToJson(std::optional<std::string> path) const;

  bool wasEditedFromLastLoad() const;

  std::string getName() const;
  void setName(std::string newName);

  std::optional<std::string> getPath() const;

  int colorCount() const;
  PaletteColor getColor(int index) const;
  std::vector<PaletteColor> getAllColors() const;
  void addColor(const QColor& color, const std::optional<std::string>& hint);
  void removeColor(int index);

 private:
  std::string _name;
  std::optional<std::string> _path;
  bool _wasEdited = false;
  std::vector<PaletteColor> _colors;

  [[nodiscard]] PotentialError<std::string> importValuesFromJson(const rapidjson::Document& root);
  rapidjson::Document exportValuesToJson() const;

  bool isIndexOutsideColors(int index) const;
};
}  // namespace capy

#endif  // PALETTE_HPP
