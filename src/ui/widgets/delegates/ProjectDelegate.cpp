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

#include "../../UiHelpers.hpp"
#include "ui/widgets/utils/ClickableLabel.hpp"
#include "ui/widgets/utils/ResourceManager.hpp"
#include "ui_ProjectDelegate.h"

namespace capy::ui {
ProjectDelegate::ProjectDelegate(Project project, bool isProjectInternal, QWidget *parent)
    : QWidget(parent), ui(new Ui::ProjectDelegate), _project(std::move(project)) {
  ui->setupUi(this);

  if (isProjectInternal) {
    ui->removeButton->hide();
  }

  connect(ui->nameLabel, &ClickableLabel::clicked, this, &ProjectDelegate::imageOrNameClicked);
  connect(ui->imageLabel, &ClickableLabel::clicked, this, &ProjectDelegate::imageOrNameClicked);

  connect(ui->deleteButton, &QPushButton::clicked, this, [&]() { emit deleteClicked(_project); });

  connect(ui->removeButton, &QPushButton::clicked, this, [&]() { emit removeClicked(_project); });

  const auto projectName = QString::fromStdString(_project.getName());
  ui->nameLabel->setText(elideText(projectName, ui->nameLabel->font(), ui->nameLabel->width()));
  ui->nameLabel->setToolTip(QString::fromStdString(_project.getPath()));

  QPixmap miniature = _project.getMiniature();
  if (miniature.isNull()) {
    miniature =
        QPixmap{ResourceManager::getIconPath(ResourceManager::Icon::CorruptedProjectMiniatureIcon)};
  }

  const auto actualMiniature =
      miniature.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);

  ui->imageLabel->setPixmap(actualMiniature);
}

ProjectDelegate::~ProjectDelegate() { delete ui; }

void ProjectDelegate::imageOrNameClicked() { emit itemClicked(_project); }
}  // namespace capy::ui
