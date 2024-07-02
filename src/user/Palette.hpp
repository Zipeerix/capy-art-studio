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

namespace capy {
struct PaletteColor {
  QColor color;
  std::optional<std::string> hint;
};

class Palette {
 public:
  Palette() = default;
  explicit Palette(std::string name);

  static std::expected<Palette, std::string> fromJson(const std::string& path);
  std::expected<void, std::string> saveToJson(std::optional<std::string> path) const;

  [[nodiscard]] bool wasEditedFromLastLoad() const;

  [[nodiscard]] std::string getName() const;
  void setName(std::string newName);

  [[nodiscard]] std::optional<std::string> getPath() const;

  [[nodiscard]] int colorCount() const;
  [[nodiscard]] PaletteColor getColor(int index) const;
  [[nodiscard]] std::vector<PaletteColor> getAllColors() const;
  void addColor(const QColor& color, const std::optional<std::string>& hint);
  void removeColor(int index);

 private:
  std::string _name;
  std::optional<std::string> _path;
  bool _wasEdited = false;
  std::vector<PaletteColor> _colors;

  std::expected<void, std::string> importValuesFromJson(const rapidjson::Document& root);
  [[nodiscard]] rapidjson::Document exportValuesToJson() const;
};
}  // namespace capy

#endif  // PALETTE_HPP
