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

namespace capy::models {
class PaletteModel final : public QAbstractListModel {
  Q_OBJECT
 public:
  explicit PaletteModel(QObject* parent = nullptr);

  [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
  [[nodiscard]] QVariant data(const QModelIndex& index, int role) const override;

  // TODO: Should three methdos below be hre?
  [[nodiscard]] std::vector<PaletteColor> getColors(int index) const;
  [[nodiscard]] PaletteColor getColor(int index, int colorIndex) const;
  [[nodiscard]] PotentialError<std::string> removeColorFromPalette(int index, int colorIndex);
  [[nodiscard]] PotentialError<std::string> addColorToPalette(int index, QColor color,
                                                              std::optional<std::string> hint);
  [[nodiscard]] PotentialError<std::string> updatePaletteFile(int index, bool emitDataChanged);

  [[nodiscard]] bool doesPaletteExist(const std::string& name) const;

  void setPalettes(std::vector<Palette> palettes);
  [[nodiscard]] const Palette& getPalette(int index) const;

 private:
  std::vector<Palette> _palettes;
};
}  // namespace capy::models

#endif  // PALETTEMODEL_HPP
