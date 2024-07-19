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

#ifndef COLORRECTANGLEDELEGATE_HPP
#define COLORRECTANGLEDELEGATE_HPP

#include <QLineEdit>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QWidget>

#include "algorithms/Luminance.hpp"
#include "ui/utils/CheckerboardPixmap.hpp"

namespace capy::ui {
class ColorRectangleDelegate final : public QStyledItemDelegate {
  Q_OBJECT
 public:
  explicit ColorRectangleDelegate(QWidget *parent = nullptr);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;

  // TODO: Other methods here i think allow for editing of he data

 private:
  CheckerboardPixmap _checkerboardPixmap{};
};
}  // namespace capy::ui

#endif  // COLORRECTANGLEDELEGATE_HPP
