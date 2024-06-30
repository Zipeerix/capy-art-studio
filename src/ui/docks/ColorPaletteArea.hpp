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

#ifndef CAPY_UI_COLORPALETTEAREA_HPP
#define CAPY_UI_COLORPALETTEAREA_HPP

#include <QWidget>
#include "models/PaletteColorTableModel.hpp"
#include "models/PaletteModel.hpp"

namespace capy::ui {
namespace Ui {
class ColorPaletteArea;
}

class ColorPaletteArea final : public QWidget {
    Q_OBJECT
public:
    explicit ColorPaletteArea(QWidget *parent = nullptr);
    ~ColorPaletteArea() override;

public slots:
  void currentColorPaletteChanged(int newPaletteIndex);

private:
    Ui::ColorPaletteArea* ui;
    models::PaletteModel _paletteModel;
    models::PaletteColorTableModel _colorTableModel;
};

} // namespace capy::ui

#endif // CAPY_UI_COLORPALETTEAREA_HPP