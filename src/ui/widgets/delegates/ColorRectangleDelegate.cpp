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

#include "ColorRectangleDelegate.hpp"

namespace capy::ui
{
ColorRectangleDelegate::ColorRectangleDelegate(QWidget* parent) :
    QStyledItemDelegate(parent)
{
}

void ColorRectangleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const
{
  painter->save();

  painter->drawTiledPixmap(option.rect, _checkerboardPixmap);

  const auto color = index.data(Qt::DisplayRole).value<QColor>();
  painter->fillRect(option.rect, color);

  const auto alphaText = QString::number(color.alpha());
  painter->setPen(algorithms::blackOrWhiteBasedOnLuminance(color));
  painter->drawText(option.rect, Qt::AlignCenter, alphaText);

  painter->restore();
}
} // namespace capy::ui