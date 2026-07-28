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

#ifndef NAMEOF_COMPILE_FAIL_CASE
#  error NAMEOF_COMPILE_FAIL_CASE must select a compile-fail scenario.
#endif

#if NAMEOF_COMPILE_FAIL_CASE == 1

enum class NoReflectedValues { value = 1000 };

int main() {
  (void)nameof::nameof_enum(NoReflectedValues::value);
}

#elif NAMEOF_COMPILE_FAIL_CASE == 2

enum class NoSingleBitFlags : unsigned { composite = 3 };

int main() {
  (void)nameof::nameof_enum_flag(NoSingleBitFlags::composite);
}

#elif NAMEOF_COMPILE_FAIL_CASE == 3

constexpr auto invalid_name = NAMEOF(42);

int main() {}

#elif NAMEOF_COMPILE_FAIL_CASE == 4

enum class InvertedRange { value = 0 };

template <>
struct nameof::customize::enum_range<InvertedRange> {
  static constexpr int min = 1;
  static constexpr int max = 0;
};

int main() {
  (void)nameof::nameof_enum(InvertedRange::value);
}

#elif NAMEOF_COMPILE_FAIL_CASE == 5

enum class OversizedRange { value = 0 };

template <>
struct nameof::customize::enum_range<OversizedRange> {
  static constexpr int min = 0;
  static constexpr int max = 65534;
};

int main() {
  (void)nameof::nameof_enum(OversizedRange::value);
}

#else

#  error Unknown NAMEOF_COMPILE_FAIL_CASE.

#endif
