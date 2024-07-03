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

#ifndef COLORAREA_HPP
#define COLORAREA_HPP

#include <QWidget>
#include "models/PaletteModel.hpp"
#include "models/PaletteColorTableModel.hpp"
#include "ui/widgets/color-pickers/DefaultColorPicker.hpp"

namespace capy::ui {
namespace Ui {
class ColorArea;
}

class ColorArea final : public QWidget {
  Q_OBJECT
public:
  explicit ColorArea(QWidget *parent = nullptr);
  ~ColorArea() override;

public slots:
  void currentColorPaletteChanged(int newPaletteIndex);
  void userCurrentColorPaletteChanged(int newPaletteIndex);
  void colorClicked(const QModelIndex& index) const;
  void addColorToPaletteClicked();
  void createPaletteClicked();
  void removePaletteClicked();
  void createColorClicked() const;
  void removeColorClicked();

signals:
    void colorPickerColorChanged(QColor color);

private:
  Ui::ColorArea *ui;
  models::PaletteModel _paletteModel;
  models::PaletteColorTableModel _colorTableModel;
  DefaultColorPicker* _colorPicker;
  int _savedPaletteComboBoxIndex = -1;

  void loadPalettesFromFilesystem();
  bool isPaletteComboBoxIndexRestorable() const;
};
}

#endif // COLORAREA_HPP
