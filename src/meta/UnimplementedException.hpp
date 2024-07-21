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

#ifndef UNIMPLEMENTEDEXCEPTION_HPP
#define UNIMPLEMENTEDEXCEPTION_HPP

#include <stdexcept>
#include <string>

#define UNIMPLEMENTED throw capy::UnimplementedException();
#define TODO throw capy::UnimplementedException();

namespace capy
{
class UnimplementedException final : std::exception
{
public:
  UnimplementedException() = default;
  explicit UnimplementedException(std::string extraInfo);

  const char* what() const noexcept override;

private:
  std::string _extraInfo = "This code path is unimplemented";
};
} // namespace capy

#endif // UNIMPLEMENTEDEXCEPTION_HPP
