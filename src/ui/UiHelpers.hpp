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

#ifndef UIHELPERS_HPP
#define UIHELPERS_HPP

#include <QFont>
#include <QLayout>
#include <QString>

// TODO: Move to ui/utils

namespace capy
{
QString elideText(const QString& string, const QFont& font, int width,
                  Qt::TextElideMode elideMode = Qt::ElideRight);

void clearLayout(QLayout* layout);
} // namespace capy

#endif // UIHELPERS_HPP
