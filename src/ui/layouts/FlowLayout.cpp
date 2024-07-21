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

#include "FlowLayout.hpp"

#include <QWidget>

namespace capy::ui
{
FlowLayout::FlowLayout(QWidget* parent, const int margin, const int hSpacing, const int vSpacing) :
    QLayout(parent),
    _hSpace(hSpacing),
    _vSpace(vSpacing)
{
  setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(const int margin, const int hSpacing, const int vSpacing) :
    _hSpace(hSpacing),
    _vSpace(vSpacing)
{
  setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
  while (const QLayoutItem* item = FlowLayout::takeAt(0))
  {
    delete item;
  }
}

void FlowLayout::addItem(QLayoutItem* item)
{
  _itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
  if (_hSpace >= 0)
  {
    return _hSpace;
  }

  return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const
{
  if (_vSpace >= 0)
  {
    return _vSpace;
  }

  return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::count() const
{
  return static_cast<int>(_itemList.size());
}

QLayoutItem* FlowLayout::itemAt(const int index) const
{
  return _itemList.value(index);
}

QLayoutItem* FlowLayout::takeAt(const int index)
{
  if (index >= 0 && index < _itemList.size())
  {
    return _itemList.takeAt(index);
  }

  return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
  return {};
}

bool FlowLayout::hasHeightForWidth() const
{
  return true;
}

int FlowLayout::heightForWidth(const int width) const
{
  return doLayout(QRect(0, 0, width, 0), true);
}

void FlowLayout::setGeometry(const QRect& rect)
{
  QLayout::setGeometry(rect);
  doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
  return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
  QSize size;
  for (const QLayoutItem* item: std::as_const(_itemList))
  {
    size = size.expandedTo(item->minimumSize());
  }

  const QMargins margins = contentsMargins();
  size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());

  return size;
}

int FlowLayout::doLayout(const QRect& rect, const bool testOnly) const
{
  // TODO: Refactor
  int left = 0;
  int top = 0;
  int right = 0;
  int bottom = 0;
  getContentsMargins(&left, &top, &right, &bottom);
  const QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
  int x = effectiveRect.x();
  int y = effectiveRect.y();
  int lineHeight = 0;

  for (QLayoutItem* item: std::as_const(_itemList))
  {
    const QWidget* wid = item->widget();
    int spaceX = horizontalSpacing();
    if (spaceX == -1)
    {
      spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton,
                                           Qt::Horizontal);
    }
    int spaceY = verticalSpacing();
    if (spaceY == -1)
    {
      spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton,
                                           Qt::Vertical);
    }

    int nextX = x + item->sizeHint().width() + spaceX;
    if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
    {
      x = effectiveRect.x();
      y = y + lineHeight + spaceY;
      nextX = x + item->sizeHint().width() + spaceX;
      lineHeight = 0;
    }

    if (!testOnly)
    {
      item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
    }

    x = nextX;
    lineHeight = qMax(lineHeight, item->sizeHint().height());
  }

  return y + lineHeight - rect.y() + bottom;
}

int FlowLayout::smartSpacing(const QStyle::PixelMetric pm) const
{
  QObject* parent = this->parent();
  if (parent == nullptr)
  {
    return -1;
  }

  if (parent->isWidgetType())
  {
    auto* const pw = dynamic_cast<QWidget*>(parent);
    return pw->style()->pixelMetric(pm, nullptr, pw);
  }

  return dynamic_cast<QLayout*>(parent)->spacing();
}
} // namespace capy::ui
