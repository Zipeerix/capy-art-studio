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

#ifndef PALETTESMANAGER_HPP
#define PALETTESMANAGER_HPP

#include "Manager.hpp"
#include "models/PaletteColorTableModel.hpp"
#include "models/PaletteModel.hpp"

namespace capy
{
class PalettesManager final : public QObject
{
  Q_OBJECT
public:
  explicit PalettesManager(QObject* parent);

  models::PaletteModel* getPaletteModel();
  models::PaletteColorTableModel* getPaletteColorTableModel();

  void loadPalettesFromFilesystem();

  void createPalette(const std::string& name, const ManagerErrorHandler& errorHandler);
  void removePalette(int index, const ManagerErrorHandler& errorHandler);

  void removeColorFromPalette(int paletteIndex, int colorIndex,
                              const ManagerErrorHandler& errorHandler);
  void addColorToPalette(int paletteIndex, QColor color, const std::optional<std::string>& hint,
                         const ManagerErrorHandler& errorHandler);

  PaletteColor getColorFromPalette(int paletteIndex, int colorIndex) const;

  void setTableColorsFromPalette(int paletteIndex);

signals:
  void palettesUpdated();

private:
  models::PaletteModel _paletteModel;

  // TODO: Move to its own manager?
  models::PaletteColorTableModel _colorTableModel;
};
} // namespace capy

#endif // PALETTESMANAGER_HPP
