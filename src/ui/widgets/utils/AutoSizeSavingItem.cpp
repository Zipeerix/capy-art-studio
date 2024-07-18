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

#include "AutoSizeSavingItem.hpp"

#include <fmt/format.h>

#include "io/ConsoleLogger.hpp"

namespace capy {
AutoSizeSavingItem::AutoSizeSavingItem(QWidget* widget, std::string name)
    : _configurationManager(ConfigurationManager::createInstance()),
      _widget(widget),
      _name(std::move(name)) {
  if (_widget == nullptr) {
    throw std::logic_error("Widget cannot be null");
  }

  const std::optional<QByteArray> geometry = _configurationManager->getWindowGeometry(_name);
  if (geometry.has_value()) {
    _widget->restoreGeometry(geometry.value());
  }
}

AutoSizeSavingItem::~AutoSizeSavingItem() {
  _configurationManager->setWindowGeometry(_name, _widget->saveGeometry());
}
}  // namespace capy
