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

#include "utils/ConsoleLogger.hpp"

namespace capy::models {
PaletteModel::PaletteModel(QObject* parent) : QAbstractListModel(parent) {
  // TODO: Here for testing
  Palette firstPallette;
  firstPallette.setName("First Palette");
  firstPallette.addColor(QColor(128, 255, 44, 255), "FirstFirst Color Hint");
  firstPallette.addColor(QColor(255, 255, 0, 255), "FirstSecond Color Hint");

  Palette secondPalette;
  secondPalette.setName("Second Palette");
  secondPalette.addColor(QColor(255, 0, 0, 255), "SecondFirst Color Hint");
  secondPalette.addColor(QColor(0, 255, 0, 64), "SecondSecond Color Hint");

  _palettes.push_back(firstPallette);
  _palettes.push_back(secondPalette);
}

std::vector<PaletteColor> PaletteModel::getColors(int index) const {
  if (index >= _palettes.size()) {
    logger::error(
        fmt::format(
            "Attempting to get colors of non-existent palette with index {}",
            index),
        logger::Severity::Mild);
    return {};
  }

  return _palettes.at(index).getAllColors();
}

int PaletteModel::rowCount(const QModelIndex& parent) const {
  return _palettes.size();
}

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

QVariant PaletteModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const {
  throw;
}
}  // namespace capy::models
