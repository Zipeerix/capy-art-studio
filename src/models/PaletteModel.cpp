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

#include "PaletteModel.hpp"

#include <fmt/format.h>

#include <graphics/Colors.hpp>
#include <ranges>

#include "io/ConsoleLogger.hpp"

namespace capy::models
{
PaletteModel::PaletteModel(QObject* parent) :
    QAbstractListModel(parent)
{
}

bool PaletteModel::doesPaletteExist(const std::string& name) const
{
  return std::ranges::any_of(_palettes, [name](const auto& palette) {
    return palette.getName() == name;
  });
}

std::vector<PaletteColor> PaletteModel::getColors(const int index) const
{
  if (isRowOutsideModel(index))
  {
    logger::error(
            fmt::format("Attempting to get colors of non-existent palette with index {}", index),
            logger::Severity::Mild);
    return {};
  }

  return _palettes.at(index).getAllColors();
}

PaletteColor PaletteModel::getColor(const int index, const int colorIndex) const
{
  if (isRowOutsideModel(index))
  {
    logger::error(
            fmt::format("Attempting to get colors of non-existent palette with index {}", index),
            logger::Severity::Mild);
    // TODO: Change to optional and return error
    return PaletteColor{QColor(0, 0, 0, constants::alpha::solidColor), "ERROR"};
  }

  const auto& palette = _palettes.at(index);
  return palette.getColor(colorIndex);
}

PotentialError<std::string> PaletteModel::addColorToPalette(const int paletteIndex,
                                                            const QColor color,
                                                            const std::optional<std::string>& hint)
{
  if (isRowOutsideModel(paletteIndex))
  {
    logger::error(fmt::format("Attempting to add color to non-existent palette with index {}",
                              paletteIndex),
                  logger::Severity::Mild);
    return {};
  }

  auto& palette = _palettes.at(paletteIndex);
  palette.addColor(color, hint);
  auto paletteSaveError = updatePaletteFile(paletteIndex, true);
  if (paletteSaveError.has_value())
  {
    return paletteSaveError;
    // TODO: if unable to save then remove the color from model
  }

  return std::nullopt;
}

PotentialError<std::string> PaletteModel::removeColorFromPalette(const int paletteIndex,
                                                                 const int colorIndex)
{
  if (isRowOutsideModel(paletteIndex))
  {
    logger::error(fmt::format("Attempting to remove color from non-existent palette with index {}",
                              paletteIndex),
                  logger::Severity::Mild);
    return {};
  }

  auto& palette = _palettes.at(paletteIndex);
  palette.removeColor(colorIndex);
  auto paletteSaveError = updatePaletteFile(paletteIndex, true);
  if (paletteSaveError.has_value())
  {
    return paletteSaveError;
    // TODO: if unable to save then remove the color from model
  }

  return std::nullopt;
}

PotentialError<std::string> PaletteModel::updatePaletteFile(const int paletteIndex,
                                                            const bool emitDataChanged)
{
  auto& palette = _palettes.at(paletteIndex);
  const auto paletteSaveError = palette.saveToJson(std::nullopt);
  if (paletteSaveError.has_value())
  {
    return "Unable to save palette due to: " + paletteSaveError.value();
  }

  if (emitDataChanged)
  {
    emit dataChanged(index(paletteIndex), index(paletteIndex), {Qt::DisplayRole});
  }

  return std::nullopt;
}

void PaletteModel::setPalettes(std::vector<Palette> palettes)
{
  beginResetModel();
  _palettes = std::move(palettes);
  endResetModel();
}

const Palette& PaletteModel::getPalette(const int index) const
{
  return _palettes.at(index);
}

int PaletteModel::rowCount([[maybe_unused]] const QModelIndex& parent) const
{
  return static_cast<int>(_palettes.size());
}

QVariant PaletteModel::data(const QModelIndex& index, int role) const
{
  // TODO: Maybe do QAbstractItemModel and have Palette's data as columns
  if (!index.isValid() || isRowOutsideModel(index))
  {
    return {};
  }

  const auto& palette = _palettes.at(index.row());
  switch (role)
  {
    case Qt::DisplayRole:
      return QString::fromStdString(palette.getName());

    default:
      return {};
  }
}

bool PaletteModel::isRowOutsideModel(const QModelIndex& index) const
{
  return isRowOutsideModel(index.row());
}

bool PaletteModel::isRowOutsideModel(const int index) const
{
  return index < 0 || static_cast<size_t>(index) >= _palettes.size();
}
} // namespace capy::models
