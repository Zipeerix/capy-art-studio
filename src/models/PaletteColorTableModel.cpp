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

#include "PaletteColorTableModel.hpp"

namespace capy::models {
PaletteColorTableModel::PaletteColorTableModel(QObject* parent) :
  QAbstractTableModel(parent) {

}

void PaletteColorTableModel::setColors(std::vector<PaletteColor> colors) {
  beginResetModel();
  _colors = std::move(colors);
  endResetModel();
}

int PaletteColorTableModel::rowCount(const QModelIndex& parent) const {
  return _colors.size();
}

int PaletteColorTableModel::columnCount(const QModelIndex& parent) const {
  return static_cast<int>(ColumnName::ColumnCount);
}

QVariant PaletteColorTableModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (index.row() >= _colors.size() || index.row() < 0)
    return QVariant();

  const auto& color = _colors.at(index.row());
  const auto actualColor = color.color;
  switch (role) {
    case Qt::DisplayRole: {
      switch (index.column()) {
        case static_cast<int>(ColumnName::Color):
          return actualColor;

        case static_cast<int>(ColumnName::Hex):
          return actualColor.name();

        case static_cast<int>(ColumnName::Hint):
          return QString::fromStdString(color.hint.value_or(""));

        default:
          return QVariant();
      }
    }

    default:
      return QVariant();
  }
}

QVariant PaletteColorTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  switch (section) {
    case static_cast<int>(ColumnName::Color):
      return "Color";

    case static_cast<int>(ColumnName::Hex):
      return "Hex";

    default:
      return QVariant();
  }
}
} // capy::models