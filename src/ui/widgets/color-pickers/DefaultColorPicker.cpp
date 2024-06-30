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

#include "DefaultColorPicker.hpp"

#include <fmt/format.h>

#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPushButton>

#include "DefaultColorPickerSlider.hpp"
#include "utils/ConsoleLogger.hpp"

// TODO: Rewrite constructor, break it down

namespace capy::ui {
DefaultColorPicker::DefaultColorPicker(QWidget* parent) : QWidget(parent) {
  auto* mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  _colorShowcase = new QLabel;
  _colorShowcase->setMinimumSize(100, 20);
  mainLayout->addWidget(_colorShowcase);

  auto* subLayout = new QVBoxLayout();
  subLayout->setContentsMargins(10, 10, 10, 10);
  mainLayout->addLayout(subLayout);

  auto* controls = new QHBoxLayout();
  subLayout->addLayout(controls);

  auto* colorSliders = new QGridLayout;

  _hexLabel = new QLabel;
  controls->addWidget(_hexLabel);

  _hueSlider = new DefaultColorPickerSlider(Qt::Horizontal);
  _saturationSlider = new DefaultColorPickerSlider(Qt::Horizontal);
  _brightnessSlider = new DefaultColorPickerSlider(Qt::Horizontal);
  _alphaSlider = new DefaultColorPickerSlider(Qt::Horizontal);

  auto* hueLabel = new QLabel;
  auto* saturationLabel = new QLabel;
  auto* brightnessLabel = new QLabel;
  auto* alphaLabel = new QLabel;

  connect(_hueSlider, &QSlider::valueChanged, this, [=](int value) {
    hueLabel->setText(QString::number(value));
    _saturationSlider->setGradientStops(
        {{0.0 / 255.0, QColor::fromHsv(value, 0, 255)},
         {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}});
    _brightnessSlider->setGradientStops(
        {{0.0 / 255.0, QColor::fromHsv(value, 255, 0)},
         {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}});
    _alphaSlider->setGradientStops(
        {{0.0 / 255.0, QColor::fromHsv(value, 255, 255, 0)},
         {255.0 / 255.0, QColor::fromHsv(value, 255, 255, 255)}});
    updateShownColor();
  });
  connect(_saturationSlider, &QSlider::valueChanged, this, [=](int value) {
    saturationLabel->setText(QString::number(value));
    updateShownColor();
  });
  connect(_brightnessSlider, &QSlider::valueChanged, this, [=](int value) {
    brightnessLabel->setText(QString::number(value));
    updateShownColor();
  });
  connect(_alphaSlider, &QSlider::valueChanged, this, [=](int value) {
    alphaLabel->setText(QString::number(value));
    updateShownColor();
  });

  hueLabel->setText(QString::number(_hueSlider->value()));
  saturationLabel->setText(QString::number(_saturationSlider->value()));
  brightnessLabel->setText(QString::number(_brightnessSlider->value()));
  alphaLabel->setText(QString::number(_alphaSlider->value()));

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

  _hueSlider->setRange(0, 359);
  _hueSlider->setGradientStops({{0.0 / 360.0, Qt::red},
                                {60.0 / 360.0, Qt::yellow},
                                {120.0 / 360.0, Qt::green},
                                {180.0 / 360.0, Qt::cyan},
                                {240.0 / 360.0, Qt::blue},
                                {300.0 / 360.0, Qt::magenta},
                                {359.0 / 360.0, Qt::red}});
  _saturationSlider->setRange(0, 255);
  _brightnessSlider->setRange(0, 255);
  _alphaSlider->setRange(0, 255);

  _saturationSlider->setGradientStops(
      {{0.0 / 255.0, QColor::fromHsv(_hueSlider->value(), 0, 255)},
       {255.0 / 255.0, QColor::fromHsv(_hueSlider->value(), 255, 255)}});
  _brightnessSlider->setGradientStops(
      {{0.0 / 255.0, QColor::fromHsv(_hueSlider->value(), 255, 0)},
       {255.0 / 255.0, QColor::fromHsv(_hueSlider->value(), 255, 255)}});
  _alphaSlider->setRenderCheckerboard(true);
  _alphaSlider->setGradientStops(
      {{0.0 / 255.0, QColor::fromHsv(_hueSlider->value(), 255, 255, 0)},
       {255.0 / 255.0, QColor::fromHsv(_hueSlider->value(), 255, 255, 255)}});

  _hueSlider->setValue(180);
  _saturationSlider->setValue(255);
  _brightnessSlider->setValue(255);
  _alphaSlider->setValue(255);

  colorSliders->addWidget(_hueSlider, 0, 0);
  colorSliders->addWidget(hueLabel, 0, 1);
  colorSliders->addWidget(_brightnessSlider, 2, 0);
  colorSliders->addWidget(brightnessLabel, 2, 1);
  colorSliders->addWidget(_saturationSlider, 1, 0);
  colorSliders->addWidget(saturationLabel, 1, 1);
  colorSliders->addWidget(_alphaSlider, 3, 0);
  colorSliders->addWidget(alphaLabel, 3, 1);

  subLayout->addLayout(colorSliders);

  auto* selectButton = new QPushButton("Add to current Palette");

  connect(selectButton, &QPushButton::clicked, this,
          &DefaultColorPicker::addToColorPaletteClicked);

  QBoxLayout* buttons = new QHBoxLayout();
  buttons->addWidget(selectButton);
  subLayout->addLayout(buttons);

  setColor(
      QColor(0, 0, 0, 255));  // TODO: Maybe have a default color somewhere,
                              // this is copied from canvas widget
  updateShownColor();
}

DefaultColorPicker::~DefaultColorPicker() = default;

void DefaultColorPicker::updateShownColor() {
  QColor color;
  color.setHsv(_hueSlider->value(), _saturationSlider->value(),
               _brightnessSlider->value(), _alphaSlider->value());
  _colorShowcase->setStyleSheet("background-color: " + color.name());
  _hexLabel->setText(color.name());
  _selectedColor = color;
  emit colorChanged(_selectedColor);
}

void DefaultColorPicker::addToColorPaletteClicked() {
  logger::info(fmt::format("Attempting to add color to palette: ({}, {}. {})",
                           _selectedColor.red(), _selectedColor.green(),
                           _selectedColor.blue()));
  // TODO
}

void DefaultColorPicker::setColor(QColor color) {
  _selectedColor = color;
  updateShownColor();
}
}  // namespace capy::ui