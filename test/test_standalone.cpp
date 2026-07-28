// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2018 - 2026 Daniil Goncharov <neargye@gmail.com>.
//
// Permission is hereby  granted, free of charge, to any  person obtaining a copy
// of this software and associated  documentation files (the "Software"), to deal
// in the Software  without restriction, including without  limitation the rights
// to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
// copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
// IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
// FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
// AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
// LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <nameof.hpp>

enum class DefaultRangeBoundary {
  lower = -128,
  upper = 127,
  outside = 128,
};

#if defined(NAMEOF_ENUM_SUPPORTED)
static_assert(nameof::nameof_enum(DefaultRangeBoundary::lower) == "lower");
static_assert(nameof::nameof_enum(DefaultRangeBoundary::upper) == "upper");
static_assert(nameof::nameof_enum(DefaultRangeBoundary::outside).empty());
#endif

struct StandaloneRttiBase {
  virtual ~StandaloneRttiBase() = default;
};

struct StandaloneRttiDerived : StandaloneRttiBase {};

int main() {
#if defined(NAMEOF_TYPE_RTTI_SUPPORTED)
  StandaloneRttiDerived derived;
  StandaloneRttiBase& base = derived;
  return NAMEOF_SHORT_TYPE_RTTI(base) == "StandaloneRttiDerived" ? 0 : 1;
#else
  return 0;
#endif
}
