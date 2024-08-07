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

#ifndef PALETTEMODEL_HPP
#define PALETTEMODEL_HPP

#include <QAbstractItemModel>
#include <vector>

#include "user/Palette.hpp"

namespace capy::models
{
class PaletteModel final : public QAbstractListModel
{
  Q_OBJECT
public:
  explicit PaletteModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent) const override;
  QVariant data(const QModelIndex& index, int role) const override;

  // TODO: Should three methdos below be hre?
  std::vector<PaletteColor> getColors(int index) const;
  PaletteColor getColor(int paletteIndex, int colorIndex) const;

  [[nodiscard]] PotentialError<std::string> removeColorFromPalette(int paletteIndex,
                                                                   int colorIndex);
  [[nodiscard]] PotentialError<std::string>
  addColorToPalette(int paletteIndex, QColor color, const std::optional<std::string>& hint);
  [[nodiscard]] PotentialError<std::string> updatePaletteFile(int paletteIndex,
                                                              bool emitDataChanged);

  bool doesPaletteExist(const std::string& name) const;

  void setPalettes(std::vector<Palette> palettes);
  const Palette& getPalette(int index) const;

private:
  std::vector<Palette> _palettes;

  bool isRowOutsideModel(const QModelIndex& index) const;
  bool isRowOutsideModel(int index) const;
};
} // namespace capy::models

#endif // PALETTEMODEL_HPP
