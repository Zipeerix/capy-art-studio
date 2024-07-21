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

#include <expected>
#include <QColor>

#include "io/JsonSerializable.hpp"
#include "meta/ErrorHandling.hpp"

namespace capy
{
// TODO: Derive from JsonSerializable and append document to other document?
struct PaletteColor {
  QColor color;
  std::optional<std::string> hint;
};

class Palette final : public JsonSerializable<Palette>
{
public:
  Palette() = default;
  explicit Palette(std::string name);

  std::string getName() const;
  void setName(std::string newName);

  int colorCount() const;
  PaletteColor getColor(int index) const;
  std::vector<PaletteColor> getAllColors() const;
  void addColor(const QColor& color, const std::optional<std::string>& hint);
  void removeColor(int index);

private:
  std::string _name;
  std::vector<PaletteColor> _colors;

  [[nodiscard]] PotentialError<std::string>
  importValuesFromJson(const rapidjson::Document& root) override;
  rapidjson::Document exportValuesToJson() const override;

  bool isIndexOutsideColors(int index) const;
};
} // namespace capy

#endif // PALETTE_HPP
