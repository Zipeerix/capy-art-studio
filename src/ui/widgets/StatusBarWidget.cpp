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

#include "StatusBarWidget.hpp"

#include "ui_StatusBarWidget.h"
#include "utils/Converters.hpp"

namespace capy::ui
{
namespace
{
uint64_t calculateRamSizeOfLayers(const std::vector<Layer>& layers)
{
  if (layers.empty())
  {
    return 0;
  }

  const auto& exampleLayer = layers.back();
  return layers.size() * exampleLayer.calculateInMemorySize();
}
} // namespace

StatusBarWidget::StatusBarWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::StatusBarWidget)
{
  ui->setupUi(this);
}

StatusBarWidget::~StatusBarWidget()
{
  delete ui;
}

void StatusBarWidget::update(const std::vector<Layer>& layers)
{
  const auto ramSize = convertBytesTo(calculateRamSizeOfLayers(layers),
                                      utils::converters::StorageSize::Megabytes);
  ui->ramLabel->setText(QString::number(ramSize, 'f', 2) + " MB");
  ui->layersLabel->setText(QString::number(layers.size()));
}
} // namespace capy::ui
