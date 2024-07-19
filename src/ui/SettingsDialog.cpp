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

#include "graphics/GraphicalBackend.hpp"
#include "ui_SettingsDialog.h"

namespace capy::ui {
SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent),
      AutoSizeSavingItem(this, "SettingsDialog"),
      ui(new Ui::SettingsDialog),
      _configurationManager(ConfigurationManager::createInstance()) {
  ui->setupUi(this);
  setupComboBoxes();

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

  ui->showStatusBackCheckbox->setChecked(_configurationManager->getApplicationSetting<bool>(
      ConfigurationManager::ApplicationSettings::ShowStatusBar));
  connect(ui->showStatusBackCheckbox, &QCheckBox::checkStateChanged, this,
          [&](const Qt::CheckState status) {
            _configurationManager->setApplicationSetting(
                ConfigurationManager::ApplicationSettings::ShowStatusBar, status == Qt::Checked);
          });

  ui->statusBarUpdateIntervalSpinbox->setValue(_configurationManager->getApplicationSetting<int>(
      ConfigurationManager::ApplicationSettings::StatusBarUpdateInterval));
  connect(ui->statusBarUpdateIntervalSpinbox, &QSpinBox::valueChanged, this,
          [&](const int newValue) {
            _configurationManager->setApplicationSetting(
                ConfigurationManager::ApplicationSettings::StatusBarUpdateInterval, newValue);
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

  // TODO: this should be GraphicsBackend in templates after CAS-169
  ui->backendComboBox->setCurrentIndex(_configurationManager->getGraphicsSetting<int>(
      ConfigurationManager::GraphicsSetting::GraphicalBackend));
  connect(ui->backendComboBox, &QComboBox::currentIndexChanged, this, [&](const int newIndex) {
    if (newIndex > 0 && newIndex < static_cast<int>(GraphicalBackend::Count)) {
      _configurationManager->setGraphicsSetting(
          ConfigurationManager::GraphicsSetting::GraphicalBackend, newIndex);
    }
  });
}

void SettingsDialog::setupComboBoxes() const {
  for (int i = 0; i < static_cast<int>(GraphicalBackend::Count); i++) {
    const QString backendName =
        QString::fromStdString(getNameOfGraphicalBackend(static_cast<GraphicalBackend>(i)));
    ui->backendComboBox->addItem(backendName, i);
  }
}
}  // namespace capy::ui
