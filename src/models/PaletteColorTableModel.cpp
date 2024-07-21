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

#include <fmt/format.h>

#include "io/ConsoleLogger.hpp"

namespace capy::models
{
PaletteColorTableModel::PaletteColorTableModel(QObject* parent) :
    QAbstractTableModel(parent)
{
}

void PaletteColorTableModel::setColors(std::vector<PaletteColor> colors)
{
  beginResetModel();
  _colors = std::move(colors);
  endResetModel();
}

void PaletteColorTableModel::notifyThatColorWasRemovedFromThePalette(const int colorIndex)
{
  beginRemoveRows(QModelIndex(), colorIndex, colorIndex);
  _colors.erase(_colors.begin() + colorIndex);
  endRemoveRows();
}

int PaletteColorTableModel::rowCount([[maybe_unused]] const QModelIndex& parent) const
{
  return _colors.size();
}

int PaletteColorTableModel::columnCount([[maybe_unused]] const QModelIndex& parent) const
{
  return static_cast<int>(ColumnName::ColumnCount);
}

QVariant PaletteColorTableModel::data(const QModelIndex& index, const int role) const
{
  if (!index.isValid() || isRowOutsideModel(index))
  {
    return QVariant();
  }

  const auto& paletteColor = _colors.at(index.row());
  const auto color = paletteColor.color;
  switch (role)
  {
    case Qt::DisplayRole: {
      switch (index.column())
      {
        case static_cast<int>(ColumnName::Color):
          return color;

        case static_cast<int>(ColumnName::Hex):
          return color.name();

        case static_cast<int>(ColumnName::Hint):
          return QString::fromStdString(paletteColor.hint.value_or(""));

        default:
          return QVariant();
      }
    }

    default:
      return QVariant();
  }
}

QVariant PaletteColorTableModel::headerData(const int section,
                                            [[maybe_unused]] const Qt::Orientation orientation,
                                            [[maybe_unused]] const int role) const
{
  switch (section)
  {
    case static_cast<int>(ColumnName::Color):
      return "Color";

    case static_cast<int>(ColumnName::Hex):
      return "Hex";

    case static_cast<int>(ColumnName::Hint):
      return "Hint";

    default:
      return QVariant();
  }
}

bool PaletteColorTableModel::isRowOutsideModel(const QModelIndex& index) const
{
  return isRowOutsideModel(index.row());
}

bool PaletteColorTableModel::isRowOutsideModel(const int index) const
{
  return index < 0 || static_cast<size_t>(index) >= _colors.size();
}
} // namespace capy::models