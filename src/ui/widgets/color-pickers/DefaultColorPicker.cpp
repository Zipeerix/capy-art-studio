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

// TODO: Rewrite constructor, break it down

// TODO: This class is due for big refactor, disable clang-tidy for now
// NOLINTBEGIN

namespace capy::ui
{
namespace constants
{
constexpr int colorShowcaseMinimumWidth = 100;
constexpr int colorShowcaseMinimumHeight = 20;

constexpr int defaultMargin = 10;
} // namespace constants

DefaultColorPicker::DefaultColorPicker(QWidget* parent) :
    QWidget(parent),
    _hueSlider(new DefaultColorPickerSlider(Qt::Horizontal)),
    _saturationSlider(new DefaultColorPickerSlider(Qt::Horizontal)),
    _brightnessSlider(new DefaultColorPickerSlider(Qt::Horizontal)),
    _alphaSlider(new DefaultColorPickerSlider(Qt::Horizontal)),
    _hexLabel(new QLabel),
    _colorShowcase(new QLabel)
{
  // TODO: Breakup into methods
  auto* mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  _colorShowcase->setMinimumSize(constants::colorShowcaseMinimumWidth,
                                 constants::colorShowcaseMinimumHeight);
  mainLayout->addWidget(_colorShowcase);

  auto* subLayout = new QVBoxLayout();
  subLayout->setContentsMargins(constants::defaultMargin, constants::defaultMargin,
                                constants::defaultMargin, constants::defaultMargin);
  mainLayout->addLayout(subLayout);

  auto* controls = new QHBoxLayout();
  subLayout->addLayout(controls);

  auto* colorSliders =
          new QGridLayout(); // TODO: This argument? here and in other ui elements in constructor

  controls->addWidget(_hexLabel);

  auto* hueLabel = new QLabel;
  auto* saturationLabel = new QLabel;
  auto* brightnessLabel = new QLabel;
  auto* alphaLabel = new QLabel;

  connect(_hueSlider, &QSlider::valueChanged, this, [hueLabel, this](const int value) {
    hueLabel->setText(QString::number(value));
    _saturationSlider->setGradientStops({{0.0 / 255.0, QColor::fromHsv(value, 0, 255)},
                                         {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}});
    _brightnessSlider->setGradientStops({{0.0 / 255.0, QColor::fromHsv(value, 255, 0)},
                                         {255.0 / 255.0, QColor::fromHsv(value, 255, 255)}});
    _alphaSlider->setGradientStops({{0.0 / 255.0, QColor::fromHsv(value, 255, 255, 0)},
                                    {255.0 / 255.0, QColor::fromHsv(value, 255, 255, 255)}});
    setColor(_hueSlider->value(), _saturationSlider->value(), _brightnessSlider->value(),
             _alphaSlider->value());
  });
  connect(_saturationSlider, &QSlider::valueChanged, this,
          [saturationLabel, this](const int value) {
            saturationLabel->setText(QString::number(value));
            setColor(_hueSlider->value(), _saturationSlider->value(), _brightnessSlider->value(),
                     _alphaSlider->value());
          });
  connect(_brightnessSlider, &QSlider::valueChanged, this,
          [brightnessLabel, this](const int value) {
            brightnessLabel->setText(QString::number(value));
            setColor(_hueSlider->value(), _saturationSlider->value(), _brightnessSlider->value(),
                     _alphaSlider->value());
          });
  connect(_alphaSlider, &QSlider::valueChanged, this, [alphaLabel, this](const int value) {
    alphaLabel->setText(QString::number(value));
    setColor(_hueSlider->value(), _saturationSlider->value(), _brightnessSlider->value(),
             _alphaSlider->value());
  });

  hueLabel->setText(QString::number(_hueSlider->value()));
  saturationLabel->setText(QString::number(_saturationSlider->value()));
  brightnessLabel->setText(QString::number(_brightnessSlider->value()));
  alphaLabel->setText(QString::number(_alphaSlider->value()));

  hueLabel->setMinimumSize(hueLabel->fontMetrics().boundingRect("000").size());
  saturationLabel->setMinimumSize(saturationLabel->fontMetrics().boundingRect("000").size());
  brightnessLabel->setMinimumSize(brightnessLabel->fontMetrics().boundingRect("000").size());
  alphaLabel->setMinimumSize(alphaLabel->fontMetrics().boundingRect("000").size());

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

  setColor(QColor(0, 0, 0, 255)); // TODO: Maybe have a default color somewhere

  updateShownColor();
}

void DefaultColorPicker::updateShownColor() const
{
  const auto hsvColor = _selectedColor.toHsv();
  _hueSlider->setValue(hsvColor.hue());
  _saturationSlider->setValue(hsvColor.saturation());
  _brightnessSlider->setValue(hsvColor.value());
  _alphaSlider->setValue(hsvColor.alpha());

  _colorShowcase->setStyleSheet("background-color: " + _selectedColor.name());
}

void DefaultColorPicker::setColor(const QColor color)
{
  _selectedColor = color;
  updateShownColor();
  emit colorChanged(_selectedColor);
}

void DefaultColorPicker::setColor(const int hue, const int saturation, const int brightness,
                                  const int alpha)
{
  _selectedColor.setHsv(hue, saturation, brightness, alpha);
  updateShownColor();
  emit colorChanged(_selectedColor);
}

QColor DefaultColorPicker::getColor() const
{
  return _selectedColor;
}
} // namespace capy::ui

// NOLINTEND