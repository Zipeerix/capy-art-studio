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

#include "MessageBoxUtils.hpp"

#include <QMessageBox>

#include "io/ConsoleLogger.hpp"

namespace capy::ui
{
namespace
{
QString getMessageBoxTitleBasedOnType(const QMessageBox::Icon type)
{
  switch (type)
  {
    case QMessageBox::NoIcon:
      logger::warning(
              "Created messagebox using helper but with NoIcon type - this is probably a bug",
              logger::Severity::Mild);
      return "MessageBox";

    case QMessageBox::Information:
      return "Information";

    case QMessageBox::Warning:
      return "Warning";

    case QMessageBox::Critical:
      return "Error";

    case QMessageBox::Question:
      return "Question";

    default:
      throw std::logic_error("Invalid QMessageBox type provided to helper");
  }
}
} // namespace

bool showConfirmationDialog(QWidget* parent, const std::string& message)
{
  const QMessageBox::StandardButton reply =
          QMessageBox::question(parent, "Confirmation", QString::fromStdString(message),
                                QMessageBox::Yes | QMessageBox::No);
  return reply == QMessageBox::Yes;
}

void execMessageBox(QWidget* parent, const QMessageBox::Icon type, const QString& text)
{
  QMessageBox messageBox(parent);
  messageBox.setIcon(type);
  messageBox.setWindowTitle(getMessageBoxTitleBasedOnType(type));
  messageBox.setText(text);
  messageBox.exec();
}
} // namespace capy::ui