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

#ifndef CAPY_UI_COLORPICKERAREA_H
#define CAPY_UI_COLORPICKERAREA_H

#include <QWidget>

#include "ui/widgets/color-pickers/DefaultColorPicker.hpp"

namespace capy::ui {
namespace Ui {
class ColorPickerArea;
}

class ColorPickerArea final : public QWidget {
    Q_OBJECT

public:
    explicit ColorPickerArea(QWidget *parent = nullptr);
    ~ColorPickerArea() override;

signals:
    void colorPickerColorChanged(QColor color);

private:
    Ui::ColorPickerArea* ui;
    DefaultColorPicker* _colorPicker;
};

} // namespace capy::ui

#endif // CAPY_UI_COLORPICKERAREA_H
