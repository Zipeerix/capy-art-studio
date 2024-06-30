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

namespace capy::models {
PaletteModel::PaletteModel(QObject* parent) : QAbstractItemModel(parent) {}

QModelIndex PaletteModel::index(int row, int column,
                                const QModelIndex& parent) const {
  throw;
}
QModelIndex PaletteModel::parent(const QModelIndex& child) const { throw; }
int PaletteModel::rowCount(const QModelIndex& parent) const { throw; }
int PaletteModel::columnCount(const QModelIndex& parent) const { throw; }
QVariant PaletteModel::data(const QModelIndex& index, int role) const { throw; }
}  // namespace capy::models
