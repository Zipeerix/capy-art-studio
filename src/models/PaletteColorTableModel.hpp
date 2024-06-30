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

#ifndef PALETTECOLORTABLEMODEL_HPP
#define PALETTECOLORTABLEMODEL_HPP

#include <QAbstractTableModel>

#include "user/Palette.hpp"

namespace capy::models {
class PaletteColorTableModel final : public QAbstractTableModel {
  Q_OBJECT
public:
  enum class ColumnName: int {
    Color,
    Hex,
    ColumnCount
  };

  explicit PaletteColorTableModel(QObject* parent);

  [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
  [[nodiscard]] int columnCount(const QModelIndex& parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;
  [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  void setColors(std::vector<PaletteColor> colors);

private:
  std::vector<PaletteColor> _colors;
};
} // capy

#endif //PALETTECOLORTABLEMODEL_HPP
