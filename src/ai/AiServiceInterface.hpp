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

#ifndef AISERVICEINTERFACE_HPP
#define AISERVICEINTERFACE_HPP

#include <optional>
#include <string>
#include <vector>

#include "graphics/Layer.hpp"

namespace capy {
struct AiPromptData {
  std::string topic;
  std::vector<std::string> features;
  int width;
  int height;
};

// TODO: Maybe make default behavioiur of sending rest request
// TODO: If so then i need requestURL parameter in constructor
class AiServiceInterface {
 public:
  explicit AiServiceInterface(std::optional<std::string> apiKey);
  virtual ~AiServiceInterface() = default;

  virtual Layer query(AiPromptData promptData) = 0;

  // TODO: Export label/line edit list for ui settings?

 protected:
  std::optional<std::string> _apiKey;
};
}  // namespace capy

#endif  // AISERVICEINTERFACE_HPP
