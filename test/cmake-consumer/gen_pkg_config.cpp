// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// SPDX-License-Identifier: MIT
// Copyright (c) 2018 - 2026 Daniil Goncharov <neargye@gmail.com>.

#ifdef GEN_PKG_CONFIG_LIBRARY

int gen_pkg_config_answer() {
  return 42;
}

#else

int gen_pkg_config_answer();

int main() {
  return gen_pkg_config_answer() == 42 ? 0 : 1;
}

#endif
