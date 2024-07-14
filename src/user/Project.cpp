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

#include "Project.hpp"

#include <fmt/format.h>

#include <QDebug>
#include <fstream>
#include <utility>

#include "io/ApplicationFilesystem.hpp"
#include "io/ChunkFileLoader.hpp"
#include "io/ConsoleLogger.hpp"

namespace capy {
Result<Project, std::string> Project::createFromFile(const std::string& path) {
  if (path.empty()) {
    return std::unexpected("File path is empty");
  }

  // TODO: Break down into static methods, or just statics

  ChunkFileLoader file{path};
  if (!file.isFileValid()) {
    return std::unexpected(fmt::format("Unable to open project file at: {}", path));
  }

  const auto magicStringReadRes = file.readNextString(5, true);
  if (!magicStringReadRes.has_value()) {
    logger::error("Unable to read magic string from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  if (magicStringReadRes.value() != "CAPY") {
    logger::error(
        fmt::format("Project's magic string mismatch {} != CAPY", magicStringReadRes.value()),
        logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  const auto miniatureSizeReadRes = file.readNext32BitInt();
  if (!miniatureSizeReadRes.has_value()) {
    logger::error("Unable to read miniature size from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  std::uint32_t miniatureSize = miniatureSizeReadRes.value();
  if (miniatureSize >= INT32_MAX) {
    logger::error("Miniature size > INT32_MAX", logger::Severity::Severe);
    return std::unexpected("Project file is corrupted");
  }

  const auto miniatureDataReadRes =
      file.readNextBytesToQByteArray(static_cast<int32_t>(miniatureSize));
  if (!miniatureDataReadRes.has_value()) {
    logger::error("Unable to read miniature data from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  QPixmap miniature;
  if (!miniature.loadFromData(miniatureDataReadRes.value())) {
    logger::warning("Error when creating miniature pixmap from data", logger::Severity::Mild);
  }

  // TODO: May

  return Project(path, miniature, file.currentReadingIndex());
}

Project::Project(std::string path, QPixmap miniature, const std::size_t indexOfDataAfterMiniature)
    : _path(std::move(path)),
      _miniature(std::move(miniature)),
      _indexOfDataAfterMiniature(indexOfDataAfterMiniature) {}

std::string Project::getPath() const { return _path; }

std::string Project::getName() const { return getFileNameFromPath(_path); }

QPixmap Project::getMiniature() const { return _miniature; }

Result<Drawing, std::string> Project::readDrawing() const {
  ChunkFileLoader file{_path};
  if (!file.isFileValid()) {
    logger::debug("Project file was opened once successfully but then failed when reading data",
                  "Project");
    return std::unexpected(fmt::format("Unable to open project file at: {}", _path));
  }

  // TODO: Break down into static methods, or just statics

  file.setReadingIndex(_indexOfDataAfterMiniature);

  /* Project Metadata */
  const auto projectWidthRes = file.readNext32BitInt();
  if (!projectWidthRes.has_value()) {
    logger::error("Unable to read project width from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  const auto projectHeightRes = file.readNext32BitInt();
  if (!projectHeightRes.has_value()) {
    logger::error("Unable to read project height from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  const auto layersCountRes = file.readNext32BitInt();
  if (!layersCountRes.has_value()) {
    logger::error("Unable to read project layer count from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  std::uint32_t projectWidth = projectWidthRes.value();
  std::uint32_t projectHeight = projectHeightRes.value();
  std::uint32_t layerCount = layersCountRes.value();

  /* Layer names */
  std::vector<std::string> layerNames;
  for (std::uint32_t i = 0; i < layerCount; i++) {
    const auto layerNameReadingRes = file.readNextVariableLengthString();
    if (!layerNameReadingRes.has_value()) {
      logger::error(fmt::format("Unable to read project layer name at index {} from project file", i), logger::Severity::Mild);
      return std::unexpected("Project file is corrupted or incompatible with this version");
    }

    layerNames.push_back(layerNameReadingRes.value());
  }

  if (layerNames.size() != layerCount) {
    throw std::logic_error("Assertion failed layerNames.size() != layerCount");
  }

  const auto endOfLayerNamesByte = file.readNextByte();
  if (!endOfLayerNamesByte.has_value()) {
    logger::error("Unable to read end of layers names byte from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  if (endOfLayerNamesByte.value() != 0xFF) {
    logger::error("Invalid end of layers name byte read from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  auto drawing = Drawing(projectWidth, projectHeight);

  /* Layer data */
  const auto sizePerLayer = projectHeight * projectWidth * 4;
  for (std::uint32_t layerIndex = 0; layerIndex < layerCount; layerIndex++) {
    const auto layersRawPixelsReadRes = file.readNextBytesToVector(sizePerLayer);
    if (!layersRawPixelsReadRes.has_value()) {
      logger::error(fmt::format("Unable to read layer data with index: {}", layerIndex), logger::Severity::Mild);
      return std::unexpected("Project file is corrupted or incompatible with this version");
    }

    const auto& layerRawPixels = layersRawPixelsReadRes.value();

    std::vector<Pixel> layerPixels;
    layerPixels.reserve(projectHeight * projectWidth);

    for (std::uint32_t j = 0; j < layerRawPixels.size(); j += 4) {
      const auto r = layerRawPixels[j];
      const auto g = layerRawPixels[j + 1];
      const auto b = layerRawPixels[j + 2];
      const auto a = layerRawPixels[j + 3];
      layerPixels.emplace_back(r, g, b, a);
    }

    drawing.insertOrAssignLayerFromRawPixels(layerIndex, layerNames[layerIndex], std::move(layerPixels));
  }

  if (static_cast<std::uint32_t>(drawing.getLayerCount()) != layerCount) {
    throw std::logic_error("drawing.getLayerCount() != layerCount");
  }

  if (file.readNextByte().has_value()) {
    logger::error("Unexpected data at the end of project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  return drawing;
}
}  // namespace capy