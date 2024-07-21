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

#ifndef FILEDOWNLOADERS_HPP
#define FILEDOWNLOADERS_HPP

#include <cstdint>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

namespace capy
{
class AsyncFileDownloader final : public QObject
{
  Q_OBJECT
public:
  explicit AsyncFileDownloader(QObject* parent = nullptr);
  ~AsyncFileDownloader() override = default;

  void startDownloadOfFile(const std::string& url);

  const std::vector<uint8_t>& getDownloadedData() const;

signals:
  void finished(bool success);

private slots:
  void fileDownloaded(QNetworkReply* reply);

private:
  std::string _currentUrl;
  QNetworkAccessManager _networkAccessManager;
  std::vector<uint8_t> _downloadedData;
};
} // namespace capy

#endif // FILEDOWNLOADERS_HPP
