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

#include "SettingsDialog.hpp"

#include "ui_SettingsDialog.h"

namespace capy::ui {
SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent),
      AutoSizeSavingItem(this, "SettingsDialog"),
      ui(new Ui::SettingsDialog),
      _configurationManager(ConfigurationManager::createInstance()) {
  ui->setupUi(this);

  // TODO: Connect signals for updating settings such as drawing grid when app is running

  // TODO: maybe keep in constructor like in other classes
  // TODO: Or if adding ok/cancel then just fill data and then on ok do all sets
  // TODO: OR even better: dont .sync() qSettings and restore if cancel or sync() on ok
  setupConnectionsForApplicationTab();
  setupConnectionsForDebugTab();
  setupConnectionsForGraphicsTab();
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::setupConnectionsForApplicationTab() {
  ui->showWelcomeScreenCheckbox->setChecked(_configurationManager->getApplicationSetting<bool>(
      ConfigurationManager::ApplicationSettings::ShowWelcomeScreen));
  connect(ui->showWelcomeScreenCheckbox, &QCheckBox::checkStateChanged, this,
          [&](const Qt::CheckState status) {
            _configurationManager->setApplicationSetting(
                ConfigurationManager::ApplicationSettings::ShowWelcomeScreen,
                status == Qt::Checked);
          });
}

void SettingsDialog::setupConnectionsForDebugTab() {
  // TODO: Maybe in settings manager have connectToUi, this will based on type create following two
  // TODO things for it
  ui->showConsoleCheckbox->setChecked(_configurationManager->getDebugSetting<bool>(
      ConfigurationManager::DebugSetting::ShowConsole));
  connect(ui->showConsoleCheckbox, &QCheckBox::checkStateChanged, this,
          [&](const Qt::CheckState status) {
            _configurationManager->setDebugSetting(ConfigurationManager::DebugSetting::ShowConsole,
                                                   status == Qt::Checked);
          });
}

void SettingsDialog::setupConnectionsForGraphicsTab() {
  ui->drawPixelGridCheckbox->setChecked(_configurationManager->getGraphicsSetting<bool>(
      ConfigurationManager::GraphicsSetting::DrawGrid));
  connect(ui->drawPixelGridCheckbox, &QCheckBox::checkStateChanged, this,
          [&](const Qt::CheckState status) {
            _configurationManager->setGraphicsSetting(
                ConfigurationManager::GraphicsSetting::DrawGrid, status == Qt::Checked);
          });

  ui->pixelGridWidthSpinBox->setValue(_configurationManager->getGraphicsSetting<double>(
      ConfigurationManager::GraphicsSetting::GridWidth));
  connect(ui->pixelGridWidthSpinBox, &QDoubleSpinBox::valueChanged, this,
          [&](const double newValue) {
            _configurationManager->setGraphicsSetting(
                ConfigurationManager::GraphicsSetting::GridWidth, newValue);
          });

  ui->pixelGridZoomThresholdSpinbox->setValue(_configurationManager->getGraphicsSetting<double>(
      ConfigurationManager::GraphicsSetting::GridDrawingZoomThreshold));
  connect(ui->pixelGridZoomThresholdSpinbox, &QDoubleSpinBox::valueChanged, this,
          [&](const double newValue) {
            _configurationManager->setGraphicsSetting(
                ConfigurationManager::GraphicsSetting::GridDrawingZoomThreshold, newValue);
          });
}
}  // namespace capy::ui
