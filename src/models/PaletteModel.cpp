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

#include <ranges>

#include "io/ConsoleLogger.hpp"

namespace capy::models {
PaletteModel::PaletteModel(QObject* parent) : QAbstractListModel(parent) {}

bool PaletteModel::doesPaletteExist(const std::string& name) const {
  return std::ranges::any_of(_palettes,
                             [name](const auto& palette) { return palette.getName() == name; });
}

std::vector<PaletteColor> PaletteModel::getColors(int index) const {
  if (index >= _palettes.size()) {
    logger::error(
        fmt::format("Attempting to get colors of non-existent palette with index {}", index),
        logger::Severity::Mild);
    return {};
  }

  return _palettes.at(index).getAllColors();
}

PaletteColor PaletteModel::getColor(int index, int colorIndex) const {
  if (index >= _palettes.size()) {
    logger::error(
        fmt::format("Attempting to get colors of non-existent palette with index {}", index),
        logger::Severity::Mild);
    return PaletteColor{QColor(0, 0, 0, 255), "ERROR"};
  }

  const auto& palette = _palettes.at(index);
  return palette.getColor(colorIndex);
}

std::expected<void, std::string> PaletteModel::addColorToPalette(int paletteIndex, QColor color,
                                                                 std::optional<std::string> hint) {
  if (paletteIndex >= _palettes.size()) {
    logger::error(
        fmt::format("Attempting to add color to non-existent palette with index {}", paletteIndex),
        logger::Severity::Mild);
    return {};
  }

  auto& palette = _palettes.at(paletteIndex);
  palette.addColor(color, hint);
  // TODO: use updatePaletteFile method instead as it does the same
  const auto paletteSaveRes = palette.saveToJson(std::nullopt);
  if (!paletteSaveRes.has_value()) {
    return std::unexpected("Unable to save palette due to: " + paletteSaveRes.error());
    // TODO: if unable to save then remove the color from model
  } else {
    emit dataChanged(index(paletteIndex), index(paletteIndex), {Qt::DisplayRole});
  }

  return {};
}

std::expected<void, std::string> PaletteModel::removeColorFromPalette(int paletteIndex,
                                                                      int colorIndex) {
  if (paletteIndex >= _palettes.size()) {
    logger::error(fmt::format("Attempting to remove color from non-existent palette with index {}",
                              paletteIndex),
                  logger::Severity::Mild);
    return {};
  }

  auto& palette = _palettes.at(paletteIndex);
  palette.removeColor(colorIndex);
  // TODO: use updatePaletteFile method instead as it does the same
  const auto paletteSaveRes = palette.saveToJson(std::nullopt);
  if (!paletteSaveRes.has_value()) {
    return std::unexpected("Unable to save palette due to: " + paletteSaveRes.error());
    // TODO: if unable to save then remove the color from model
  } else {
    emit dataChanged(index(paletteIndex), index(paletteIndex), {Qt::DisplayRole});
  }

  return {};
}

std::expected<void, std::string> PaletteModel::updatePaletteFile(int paletteIndex,
                                                                 bool emitDataChanged) {
  auto& palette = _palettes.at(paletteIndex);
  const auto paletteSaveRes = palette.saveToJson(std::nullopt);
  if (!paletteSaveRes.has_value()) {
    return std::unexpected("Unable to save palette due to: " + paletteSaveRes.error());
  } else {
    if (emitDataChanged) {
      emit dataChanged(index(paletteIndex), index(paletteIndex), {Qt::DisplayRole});
    }
  }

  return {};
}

void PaletteModel::setPalettes(std::vector<Palette> palettes) {
  beginResetModel();
  _palettes = std::move(palettes);
  endResetModel();
}

const Palette& PaletteModel::getPalette(int index) const { return _palettes.at(index); }

int PaletteModel::rowCount(const QModelIndex& parent) const { return _palettes.size(); }

QVariant PaletteModel::data(const QModelIndex& index, int role) const {
  // TODO: Maybe do QAbstractItemModel and have Palette's data as columns
  if (!index.isValid()) return QVariant();

  if (index.row() >= _palettes.size() || index.row() < 0) return QVariant();

  const auto& palette = _palettes.at(index.row());
  switch (role) {
    case Qt::DisplayRole:
      return QString::fromStdString(palette.getName());

    default:
      return QVariant();
  }
}
}  // namespace capy::models
