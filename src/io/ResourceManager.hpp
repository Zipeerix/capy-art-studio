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

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <QString>

namespace capy
{
// TODO: Change to namespace just like ConsoleLogger?
class ResourceManager
{
public:
  enum class Icon
  {
    ApplicationIcon,
    CorruptedProjectMiniatureIcon,
  };

  enum class ToolIcon
  {
    Pen,
    Hand
  };

  static QString getIconPath(Icon icon);
  static QString getToolIconPath(ToolIcon toolIcon);

private:
  enum class Prefix
  {
    Root,
    Icons,
    ToolsIcons,
  };

  static QString getPrefixPath(Prefix prefix);
};
} // namespace capy

#endif // RESOURCEMANAGER_HPP
