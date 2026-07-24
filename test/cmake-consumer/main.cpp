#include <nameof.hpp>

enum class color { red, green };

static_assert(NAMEOF_ENUM_CONST(color::green) == "green");
static_assert(!NAMEOF_TYPE(color).empty());

int main() {
  const auto value = color::red;
  return NAMEOF_ENUM(value) == "red" ? 0 : 1;
}
