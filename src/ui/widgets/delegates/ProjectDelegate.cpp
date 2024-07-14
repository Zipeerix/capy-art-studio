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

#include "ProjectDelegate.hpp"

#include <utility>

#include "ui/widgets/utils/ClickableLabel.hpp"
#include "ui_ProjectDelegate.h"
#include "utils/UiHelpers.hpp"

namespace capy::ui {
ProjectDelegate::ProjectDelegate(Project project, bool isProjectInternal, QWidget *parent)
    : QWidget(parent), ui(new Ui::ProjectDelegate), _project(std::move(project)) {
  ui->setupUi(this);

  if (isProjectInternal) {
    ui->removeButton->hide();
  }

  connect(ui->nameLabel, &ClickableLabel::clicked, this, &ProjectDelegate::imageOrNameClicked);
  connect(ui->imageLabel, &ClickableLabel::clicked, this, &ProjectDelegate::imageOrNameClicked);

  connect(ui->deleteButton, &QPushButton::clicked, this,
          [&]() { emit deleteClicked(_project.getPath()); });

  connect(ui->removeButton, &QPushButton::clicked, this,
          [&]() { emit removeClicked(_project.getPath()); });

  const auto projectName = QString::fromStdString(_project.getName());
  ui->nameLabel->setText(elideText(projectName, ui->nameLabel->font(), ui->nameLabel->width()));
  ui->nameLabel->setToolTip(QString::fromStdString(_project.getPath().value_or("Unknown Path")));

  // TODO: Get real miniature
  QPixmap miniature{QString::fromStdString("THIS_IS_DELIBERATE_TO_FAIL")};
  if (miniature.isNull()) {
    miniature = QPixmap{":/no-image.png"};
  }

  ui->imageLabel->setPixmap(miniature);
}

ProjectDelegate::~ProjectDelegate() { delete ui; }

void ProjectDelegate::imageOrNameClicked() { emit itemClicked(_project.getPath()); }
}  // namespace capy::ui
