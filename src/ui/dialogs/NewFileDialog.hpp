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

#ifndef NEWFILEDIALOG_HPP
#define NEWFILEDIALOG_HPP

#include <QDialog>

namespace capy::ui {
// TODO: This is a placeholder, use capy manager later
struct NewFileDialogResult {
  int width;
  int height;
};

namespace Ui {
class NewFileDialog;
}

class NewFileDialog final : public QDialog {
  Q_OBJECT

public:
  explicit NewFileDialog(QWidget* parent = nullptr);
  ~NewFileDialog() override;

  // TODO: This is a placeholder
  [[nodiscard]] std::optional<NewFileDialogResult> getResult() const;

public slots:
  void okClicked();

private:
  Ui::NewFileDialog* ui;
  std::optional<NewFileDialogResult> _result;
};
}

#endif // NEWFILEDIALOG_HPP
