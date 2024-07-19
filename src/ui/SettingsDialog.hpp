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

#ifndef CAPY_UI_SETTINGSWINDOW_HPP
#define CAPY_UI_SETTINGSWINDOW_HPP

#include <QDialog>

#include "io/ConfigurationManager.hpp"
#include "utils/AutoSizeSavingItem.hpp"

namespace capy::ui {
namespace Ui {
class SettingsDialog;
}

class SettingsDialog final : public QDialog, AutoSizeSavingItem {
  Q_OBJECT
 public:
  explicit SettingsDialog(QWidget* parent = nullptr);
  ~SettingsDialog() override;

 private:
  Ui::SettingsDialog* ui;
  std::shared_ptr<ConfigurationManager> _configurationManager;

  void setupConnectionsForApplicationTab();
  void setupConnectionsForDebugTab();
  void setupConnectionsForGraphicsTab();
};
}  // namespace capy::ui

#endif  // CAPY_UI_SETTINGSWINDOW_HPP
