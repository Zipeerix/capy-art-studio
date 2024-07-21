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

#include <fstream>
#include <utility>

#include "io/ApplicationFilesystem.hpp"
#include "io/ChunkFileReader.hpp"
#include "io/ChunkFileWriter.hpp"
#include "io/ConsoleLogger.hpp"

namespace capy
{
Result<Project, std::string> Project::createFromFile(const std::string& path)
{
  if (path.empty())
  {
    return std::unexpected("File path is empty");
  }

  // TODO: Break down into static methods, or just statics

  ChunkFileReader file{path};
  if (!file.isFileValid())
  {
    return std::unexpected(fmt::format("Unable to open project file at: {}", path));
  }

  const auto magicStringReadRes = file.readNextString(5, true);
  if (!magicStringReadRes.has_value())
  {
    logger::error("Unable to read magic string from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  if (magicStringReadRes.value() != "CAPY")
  {
    logger::error(
            fmt::format("Project's magic string mismatch {} != CAPY", magicStringReadRes.value()),
            logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  const auto miniatureSizeReadRes = file.readNext32BitInt();
  if (!miniatureSizeReadRes.has_value())
  {
    logger::error("Unable to read miniature size from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  std::uint32_t miniatureSize = miniatureSizeReadRes.value();
  if (miniatureSize >= INT32_MAX)
  {
    logger::error("Miniature size > INT32_MAX", logger::Severity::Severe);
    return std::unexpected("Project file is corrupted");
  }

  const auto miniatureDataReadRes =
          file.readNextBytesToQByteArray(static_cast<int32_t>(miniatureSize));
  if (!miniatureDataReadRes.has_value())
  {
    logger::error("Unable to read miniature data from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  QPixmap miniature;
  if (!miniature.loadFromData(miniatureDataReadRes.value()))
  {
    logger::warning("Error when creating miniature pixmap from data", logger::Severity::Mild);
  }

  // TODO: May

  return Project(path, miniature, file.currentReadingIndex());
}

Project::Project(std::string path, QPixmap miniature, const std::size_t indexOfDataAfterMiniature) :
    _path(std::move(path)),
    _miniature(std::move(miniature)),
    _indexOfDataAfterMiniature(indexOfDataAfterMiniature)
{
}

std::string Project::getPath() const
{
  return _path;
}

std::string Project::getName() const
{
  return getFileNameFromPath(_path);
}

QPixmap Project::getMiniature() const
{
  return _miniature;
}

Result<Drawing, std::string> Project::readDrawing() const
{
  ChunkFileReader file{_path};
  if (!file.isFileValid())
  {
    logger::debug("Project file was opened once successfully but then failed when reading data",
                  "Project");
    return std::unexpected(fmt::format("Unable to open project file at: {}", _path));
  }

  // TODO: Break down into static methods, or just statics
  // TODO: Clean ups casts here and above file reading

  file.setReadingIndex(_indexOfDataAfterMiniature);

  /* Project Metadata */
  const auto projectWidthRes = file.readNext32BitInt();
  if (!projectWidthRes.has_value())
  {
    logger::error("Unable to read project width from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  const auto projectHeightRes = file.readNext32BitInt();
  if (!projectHeightRes.has_value())
  {
    logger::error("Unable to read project height from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  const auto layersCountRes = file.readNext32BitInt();
  if (!layersCountRes.has_value())
  {
    logger::error("Unable to read project layer count from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  std::uint32_t projectWidth = projectWidthRes.value();
  std::uint32_t projectHeight = projectHeightRes.value();
  std::uint32_t layerCount = layersCountRes.value();

  /* Layer names */
  std::vector<std::string> layerNames;
  for (std::uint32_t i = 0; i < layerCount; i++)
  {
    const auto layerNameReadingRes = file.readNextVariableLengthString();
    if (!layerNameReadingRes.has_value())
    {
      logger::error(
              fmt::format("Unable to read project layer name at index {} from project file", i),
              logger::Severity::Mild);
      return std::unexpected("Project file is corrupted or incompatible with this version");
    }

    layerNames.push_back(layerNameReadingRes.value());
  }

  if (layerNames.size() != layerCount)
  {
    throw std::logic_error("Assertion failed layerNames.size() != layerCount");
  }

  const auto endOfLayerNamesByte = file.readNextByte();
  if (!endOfLayerNamesByte.has_value())
  {
    logger::error("Unable to read end of layers names byte from project file",
                  logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  if (endOfLayerNamesByte.value() != 0xFF)
  {
    logger::error("Invalid end of layers name byte read from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  auto drawing = Drawing(static_cast<int>(projectWidth), static_cast<int>(projectHeight));

  /* Layer data */
  const auto sizePerLayer = projectHeight * projectWidth * 4;
  for (std::uint32_t layerIndex = 0; layerIndex < layerCount; layerIndex++)
  {
    // TODO: Bottleneck?
    const auto layersRawPixelsReadRes = file.readNextBytesToVector(static_cast<int>(sizePerLayer));
    if (!layersRawPixelsReadRes.has_value())
    {
      logger::error(fmt::format("Unable to read layer data with index: {}", layerIndex),
                    logger::Severity::Mild);
      return std::unexpected("Project file is corrupted or incompatible with this version");
    }

    const auto& layerRawPixels = layersRawPixelsReadRes.value();

    std::vector<Pixel> layerPixels;
    layerPixels.reserve(projectHeight * projectWidth);

    for (std::uint32_t j = 0; j < layerRawPixels.size(); j += 4)
    {
      const auto r = layerRawPixels[j];
      const auto g = layerRawPixels[j + 1];
      const auto b = layerRawPixels[j + 2];
      const auto a = layerRawPixels[j + 3];
      layerPixels.emplace_back(r, g, b, a);
    }

    drawing.insertOrAssignLayerFromRawPixels(static_cast<int>(layerIndex), layerNames[layerIndex],
                                             std::move(layerPixels));
  }

  if (static_cast<std::uint32_t>(drawing.getLayerCount()) != layerCount)
  {
    throw std::logic_error("drawing.getLayerCount() != layerCount");
  }

  if (file.readNextByte().has_value())
  {
    logger::error("Unexpected data at the end of project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  return drawing;
}

PotentialError<std::string> Project::save(const QByteArray& miniatureBytes,
                                          const std::vector<Layer>& layers,
                                          const std::optional<std::string>& pathOverride)
{
  // TODO: When optimizing should probably use replace or somehow do it in a different way that
  // doesnt hammer the disk

  // TODO: Remember to setPath(pathOverride.value()) if its not optional on success

  const auto path = pathOverride.has_value() ? pathOverride.value() : _path;

  const auto existingFileStrategyCheckError = ChunkFileWriter::existingFileStrategyCheck(
          path, ChunkFileWriter::ExistingFileStrategy::Error);
  if (existingFileStrategyCheckError.has_value())
  {
    return fmt::format("Error when saving project: {}", existingFileStrategyCheckError.value());
  }

  if (layers.empty())
  {
    return "Unable to save project file as there are no layers";
  }

  const auto fileWriter = ChunkFileWriter(path);
  if (!fileWriter.isFileValid())
  {
    logger::error("Bad file in chunk file writer", logger::Severity::Mild);
    return "Unable to save project file";
  }

  /* Project Metadata */
  const auto magicNumberWriteError = fileWriter.writeString("CAPY", true);
  if (magicNumberWriteError.has_value())
  {
    logger::error("Unable to write magic number", logger::Severity::Mild);
    return "Unable to save project file";
  }

  const auto miniatureSizeWriteError = fileWriter.write32BitInt(miniatureBytes.size());
  if (miniatureSizeWriteError.has_value())
  {
    logger::error("Unable to write miniature size", logger::Severity::Mild);
    return "Unable to save project file";
  }

  const auto miniatureDataWriteError = fileWriter.writeQByteArray(miniatureBytes);
  if (miniatureDataWriteError.has_value())
  {
    logger::error("Unable to write miniature data", logger::Severity::Mild);
    return "Unable to save project file";
  }

  // TODO: take width and height as args?
  const auto layerDimensions = layers.back().getDimensions();
  const auto projectWidthWriteError = fileWriter.write32BitInt(layerDimensions.getWidth());
  if (projectWidthWriteError.has_value())
  {
    logger::error("Unable to write project width", logger::Severity::Mild);
    return "Unable to save project file";
  }

  const auto projectHeightWriteError = fileWriter.write32BitInt(layerDimensions.getHeight());
  if (projectHeightWriteError.has_value())
  {
    logger::error("Unable to write project height", logger::Severity::Mild);
    return "Unable to save project file";
  }

  const auto projectLayerCountWriteError = fileWriter.write32BitInt(layers.size());
  if (projectLayerCountWriteError.has_value())
  {
    logger::error("Unable to write layers count", logger::Severity::Mild);
    return "Unable to save project file";
  }

  /* Layer names */
  for (const auto& layer: layers)
  {
    const auto layerNameWriteError = fileWriter.writeString(layer.getName(), true);
    if (layerNameWriteError.has_value())
    {
      logger::error(fmt::format("Unable to write layer name for layer: {}", layer.getName()),
                    logger::Severity::Mild);
      return "Unable to save project file";
    }
  }

  const auto layerNameEndingByteWriteError = fileWriter.writeByte(0xFF);
  if (layerNameEndingByteWriteError.has_value())
  {
    logger::error("Unable to write layer names ending byte", logger::Severity::Mild);
    return "Unable to save project file";
  }

  /* Layer Data */
  for (const auto& layer: layers)
  {
    const auto& layerPixels = layer.getPixels();
    // TODO: bottleneck?
    for (const auto& pixel: layerPixels)
    {
      const std::vector<uint8_t> bytesToWrite = {pixel.getRed(), pixel.getGreen(), pixel.getBlue(),
                                                 pixel.getAlpha()};

      const auto pixelWriteError = fileWriter.writeVector(bytesToWrite);
      if (pixelWriteError.has_value())
      {
        // TODO: get exact byte index etc
        logger::error("Unable to write layer data", logger::Severity::Mild);
        return "Unable to save project file";
      }
    }
  }

  return std::nullopt;
}
} // namespace capy