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

#include "AsyncFileDownloader.hpp"

#include <fmt/format.h>

#include <QNetworkRequest>

#include "io/ConsoleLogger.hpp"

namespace capy
{
AsyncFileDownloader::AsyncFileDownloader(QObject* parent) :
    QObject(parent)
{
  connect(&_networkAccessManager, &QNetworkAccessManager::finished, this,
          &AsyncFileDownloader::fileDownloaded);
}

void AsyncFileDownloader::startDownloadOfFile(const std::string& url)
{
  _currentUrl = url;
  _downloadedData.clear();
  const auto networkUrl = QUrl(QString::fromStdString(_currentUrl));
  const auto request = QNetworkRequest(networkUrl);
  _networkAccessManager.get(request);
}

const std::vector<uint8_t>& AsyncFileDownloader::getDownloadedData() const
{
  return _downloadedData;
}

void AsyncFileDownloader::fileDownloaded(QNetworkReply* reply)
{
  if (reply->error() != QNetworkReply::NoError)
  {
    logger::error(fmt::format("Failed to download file from: {} due to error: {}", _currentUrl,
                              reply->errorString().toStdString()),
                  logger::Severity::Mild);
    emit finished(false);
    return;
  }

  const auto downloadedBytes = reply->readAll();
  _downloadedData = std::vector<uint8_t>(downloadedBytes.begin(), downloadedBytes.end());

  reply->deleteLater();

  emit finished(true);
}
} // namespace capy
