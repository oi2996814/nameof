// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2018 - 2026 Daniil Goncharov <neargye@gmail.com>.

#include <nameof.hpp>

enum class color { red, green };

static_assert(NAMEOF_ENUM_CONST(color::green) == "green");
static_assert(!NAMEOF_TYPE(color).empty());

int main() {
  const auto value = color::red;
  return NAMEOF_ENUM(value) == "red" ? 0 : 1;
}
