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

#include "NewFileDialog.hpp"
#include "ui_NewFileDialog.h"

namespace capy::ui {
NewFileDialog::NewFileDialog(QWidget* parent) :
  QDialog(parent), ui(new Ui::NewFileDialog) {
  ui->setupUi(this);

  connect(ui->buttonBox, SIGNAL(accepted()), this,
          SLOT(okClicked()));
}

NewFileDialog::~NewFileDialog() {
  delete ui;
}

std::optional<NewFileDialogResult> NewFileDialog::getResult() const {
  return _result;
}

void NewFileDialog::okClicked() {
  // TODO: This is a plceholder
  _result = NewFileDialogResult{ui->widthLineEdit->text().toInt(),
                                ui->heightLineEdit->text().toInt()};
  accept();
}
}