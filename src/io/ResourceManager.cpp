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

#include "ResourceManager.hpp"

#include <stdexcept>

namespace capy
{
QString ResourceManager::getPrefixPath(const Prefix prefix)
{
  switch (prefix)
  {
    case Prefix::Root:
      return ":/";

    case Prefix::Icons:
      return ":/icons/";

    case Prefix::ToolsIcons:
      return ":/icons/tools/";

    default:
      throw std::logic_error("Invalid prefix path requested");
  }
}

QString ResourceManager::getIconPath(const Icon icon)
{
  const auto rootPath = getPrefixPath(Prefix::Icons);
  switch (icon)
  {
    case Icon::ApplicationIcon:
      return rootPath + "icon.png";

    case Icon::CorruptedProjectMiniatureIcon:
      return rootPath + "no-image.png";

    default:
      throw std::logic_error("Invalid resource requested");
  }
}

QString ResourceManager::getToolIconPath(const ToolIcon toolIcon)
{
  const auto rootPath = getPrefixPath(Prefix::ToolsIcons);
  switch (toolIcon)
  {
    case ToolIcon::Pen:
      return rootPath + "pencil.png";

    case ToolIcon::Hand:
      return rootPath + "hand.png";

    default:
      throw std::logic_error("Invalid resource requested");
  }
}
} // namespace capy