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

#include "PalettesManager.hpp"

#include "io/ApplicationFilesystem.hpp"
#include "io/ConsoleLogger.hpp"

namespace capy {
PalettesManager::PalettesManager(QObject* parent)
    : QObject(parent), _paletteModel(this), _colorTableModel(this) {}

models::PaletteModel* PalettesManager::getPaletteModel() { return &_paletteModel; }

models::PaletteColorTableModel* PalettesManager::getPaletteColorTableModel() {
  return &_colorTableModel;
}

void PalettesManager::createPalette(const std::string& name,
                                    const ManagerErrorHandler& errorHandler) {
  if (_paletteModel.doesPaletteExist(name)) {
    return errorHandler("Palette with this name already exists");
  }

  auto newPalette = Palette(name);

  const auto palettesPath = std::filesystem::path(getFilesystemPath(FilesystemPath::Palettes));
  const auto newPaletteFilePath = palettesPath / (newPalette.getName() + ".json");

  const auto saveError = newPalette.saveToJson(newPaletteFilePath.string());
  if (saveError.has_value()) {
    return errorHandler("Unable to save new palette, check filesystem permissions");
  }

  logger::info(fmt::format("Created new palette: {}", newPalette.getName()));
  loadPalettesFromFilesystem();
}

void PalettesManager::removePalette(int index, const ManagerErrorHandler& errorHandler) {
  logger::info(fmt::format("Attempting to remove palette at index: {}", index));

  const auto& paletteToDelete = _paletteModel.getPalette(index);
  std::optional<std::string> pathToDeleteOpt = paletteToDelete.getPath();
  if (!pathToDeleteOpt.has_value()) {
    logger::error(fmt::format("Internal error: unable to remove palette: {} due to missing path",
                              paletteToDelete.getName()),
                  logger::Severity::Severe);
    return errorHandler("Unable to remove palette due to internal error");
  }

  const std::string pathToDelete = pathToDeleteOpt.value();
  if (!std::filesystem::remove(pathToDelete)) {
    return errorHandler("Unable to remove the palette, check filesystem permissions");
  }

  logger::info(fmt::format("Deleted palette: {}", paletteToDelete.getName()));
  loadPalettesFromFilesystem();
}

void PalettesManager::removeColorFromPalette(const int paletteIndex, const int colorIndex,
                                             const ManagerErrorHandler& errorHandler) {
  const auto removeError = _paletteModel.removeColorFromPalette(paletteIndex, colorIndex);
  // TODO: Below -> maybe add a signal from colorTableModel to paletteModel or other way around to
  // signal this without calling this method
  _colorTableModel.notifyThatColorWasRemovedFromThePalette(colorIndex);
  if (removeError.has_value()) {
    return errorHandler("Error when updating palette after removing color: " + removeError.value());
    // TODO: Re add colors? So there is no discrepancy between ui and file
  }

  loadPalettesFromFilesystem();  // TODO: or just emit?
}

void PalettesManager::addColorToPalette(int paletteIndex, QColor color,
                                        const std::optional<std::string>& hint,
                                        const ManagerErrorHandler& errorHandler) {
  const auto colorAddingError = _paletteModel.addColorToPalette(paletteIndex, color, hint);
  if (colorAddingError.has_value()) {
    return errorHandler("Error when adding color to palette: " + colorAddingError.value());
  }

  // TODO: Why no notifyThatColorWasRemovedFromThePalette but for adding?

  loadPalettesFromFilesystem();  // TODO: or just emit?
}

void PalettesManager::setTableColorsFromPalette(const int paletteIndex) {
  const auto colorsOfCurrentPalette = _paletteModel.getColors(paletteIndex);
  _colorTableModel.setColors(colorsOfCurrentPalette);
}

PaletteColor PalettesManager::getColorFromPalette(const int paletteIndex,
                                                  const int colorIndex) const {
  return _paletteModel.getColor(paletteIndex, colorIndex);
}

void PalettesManager::loadPalettesFromFilesystem() {
  // TODO: only load json files and validate them
  // TODO: load and _paleteModel.setPalettes() and if error then show error box
  const auto paletteFileList = listFilesInPath(FilesystemPath::Palettes);

  std::vector<Palette> palettes;
  for (const auto& paletteFilePath : paletteFileList) {
    const auto loadedPaletteOpt = Palette::createFromJson(paletteFilePath);
    if (!loadedPaletteOpt.has_value()) {
      logger::error(fmt::format("Error when loading palette: {}", loadedPaletteOpt.error()),
                    logger::Severity::Mild);
      continue;
    }

    const auto& loadedPalette = loadedPaletteOpt.value();
    logger::info(fmt::format("Loaded new palette: {}", loadedPalette.getName()));
    palettes.push_back(loadedPalette);
  }

  _paletteModel.setPalettes(std::move(palettes));

  emit palettesUpdated();
}
}  // namespace capy
