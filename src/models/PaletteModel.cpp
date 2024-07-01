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

#include "../io/ConsoleLogger.hpp"

namespace capy::models {
PaletteModel::PaletteModel(QObject* parent) : QAbstractListModel(parent) {}

std::vector<PaletteColor> PaletteModel::getColors(int index) const {
  if (index >= _palettes.size()) {
    logger::error(
        fmt::format("Attempting to get colors of non-existent palette with index {}", index),
        logger::Severity::Mild);
    return {};
  }

  return _palettes.at(index).getAllColors();
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

QVariant PaletteModel::headerData(int section, Qt::Orientation orientation, int role) const {
  throw;
}
}  // namespace capy::models
