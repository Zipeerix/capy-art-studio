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

#include "ColorPicker.hpp"

#include <fmt/format.h>

#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>

#include "ColorPicker.hpp"
#include "ColorPickerSlider.hpp"
#include "utils/ConsoleLogger.hpp"

// TODO: Rewrite constructor, break it down

namespace capy::ui {
ColorPicker::ColorPicker(QWidget* parent) : QDialog(parent) {
  auto* mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  auto* colorShowcase = new QLabel(this);
  colorShowcase->setMinimumSize(100, 100);
  mainLayout->addWidget(colorShowcase);

  auto* subLayout = new QVBoxLayout(this);
  subLayout->setContentsMargins(10, 10, 10, 10);
  mainLayout->addLayout(subLayout);

  auto* controls = new QHBoxLayout();
  subLayout->addLayout(controls);

  auto* hexLabel = new QLabel;
  controls->addWidget(hexLabel);

  auto* colorSliders = new QGridLayout;

  auto* hueSlider = new ColorPickerSlider(Qt::Horizontal);
  auto* saturationSlider = new ColorPickerSlider(Qt::Horizontal);
  auto* brightnessSlider = new ColorPickerSlider(Qt::Horizontal);
  auto* alphaSlider = new ColorPickerSlider(Qt::Horizontal);
  auto* hueLabel = new QLabel;
  auto* saturationLabel = new QLabel;
  auto* brightnessLabel = new QLabel;
  auto* alphaLabel = new QLabel;

  auto updateColor = [=]() {
    QColor color;
    color.setHsv(hueSlider->value(), saturationSlider->value(),
                 brightnessSlider->value(), alphaSlider->value());
    colorShowcase->setStyleSheet("background-color: " + color.name());
    hexLabel->setText(color.name());
    _selectedColor = color;
  };

  connect(hueSlider, &QSlider::valueChanged, this, [=](int value) {
    hueLabel->setText(QString::number(value));
    saturationSlider->setGradientStops(
        {{0.0 / 255.0, QColor::fromHsv(value, 0, 255)},
         {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}});
    brightnessSlider->setGradientStops(
        {{0.0 / 255.0, QColor::fromHsv(value, 255, 0)},
         {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}});
    alphaSlider->setGradientStops(
        {{0.0 / 255.0, QColor::fromHsv(value, 255, 255, 0)},
         {255.0 / 255.0, QColor::fromHsv(value, 255, 255, 255)}});
    updateColor();
  });
  connect(saturationSlider, &QSlider::valueChanged, this, [=](int value) {
    saturationLabel->setText(QString::number(value));
    updateColor();
  });
  connect(brightnessSlider, &QSlider::valueChanged, this, [=](int value) {
    brightnessLabel->setText(QString::number(value));
    updateColor();
  });
  connect(alphaSlider, &QSlider::valueChanged, this, [=](int value) {
    alphaLabel->setText(QString::number(value));
    updateColor();
  });

  hueLabel->setText(QString::number(hueSlider->value()));
  saturationLabel->setText(QString::number(saturationSlider->value()));
  brightnessLabel->setText(QString::number(brightnessSlider->value()));
  alphaLabel->setText(QString::number(alphaSlider->value()));

  hueLabel->setMinimumSize(hueLabel->fontMetrics().boundingRect("000").size());
  saturationLabel->setMinimumSize(
      saturationLabel->fontMetrics().boundingRect("000").size());
  brightnessLabel->setMinimumSize(
      brightnessLabel->fontMetrics().boundingRect("000").size());
  alphaLabel->setMinimumSize(
      alphaLabel->fontMetrics().boundingRect("000").size());

  hueLabel->setAlignment(Qt::AlignRight);
  saturationLabel->setAlignment(Qt::AlignRight);
  brightnessLabel->setAlignment(Qt::AlignRight);
  alphaLabel->setAlignment(Qt::AlignRight);

  hueSlider->setRange(0, 359);
  hueSlider->setGradientStops({{0.0 / 360.0, Qt::red},
                               {60.0 / 360.0, Qt::yellow},
                               {120.0 / 360.0, Qt::green},
                               {180.0 / 360.0, Qt::cyan},
                               {240.0 / 360.0, Qt::blue},
                               {300.0 / 360.0, Qt::magenta},
                               {359.0 / 360.0, Qt::red}});
  saturationSlider->setRange(0, 255);
  brightnessSlider->setRange(0, 255);
  alphaSlider->setRange(0, 255);

  saturationSlider->setGradientStops(
      {{0.0 / 255.0, QColor::fromHsv(hueSlider->value(), 0, 255)},
       {255.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255)}});
  brightnessSlider->setGradientStops(
      {{0.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 0)},
       {255.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255)}});
  alphaSlider->setRenderCheckerboard(true);
  alphaSlider->setGradientStops(
      {{0.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255, 0)},
       {255.0 / 255.0, QColor::fromHsv(hueSlider->value(), 255, 255, 255)}});

  hueSlider->setValue(180);
  saturationSlider->setValue(255);
  brightnessSlider->setValue(255);
  alphaSlider->setValue(255);

  colorSliders->addWidget(hueSlider, 0, 0);
  colorSliders->addWidget(hueLabel, 0, 1);
  colorSliders->addWidget(brightnessSlider, 2, 0);
  colorSliders->addWidget(brightnessLabel, 2, 1);
  colorSliders->addWidget(saturationSlider, 1, 0);
  colorSliders->addWidget(saturationLabel, 1, 1);
  colorSliders->addWidget(alphaSlider, 3, 0);
  colorSliders->addWidget(alphaLabel, 3, 1);

  subLayout->addLayout(colorSliders);

  auto* selectButton = new QPushButton("Add to current Palette");

  connect(selectButton, &QPushButton::clicked, this,
          &ColorPicker::addToColorPaletteClicked);

  QBoxLayout* buttons = new QHBoxLayout();
  buttons->addWidget(selectButton);
  subLayout->addLayout(buttons);

  updateColor();
}

ColorPicker::~ColorPicker() = default;

void ColorPicker::addToColorPaletteClicked() {
  logger::info(fmt::format("Attempting to add color to palette: ({}, {}. {})",
                           _selectedColor.red(), _selectedColor.green(),
                           _selectedColor.blue()));
  // TODO: log what color
}

QColor ColorPicker::getSelectedColor() const { return _selectedColor; }
}  // namespace capy::ui