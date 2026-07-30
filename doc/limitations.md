# Limitations

Type, enum, member, and pointer reflection use compiler-specific function signature strings (`__PRETTY_FUNCTION__` / `__FUNCSIG__`). See the [compiler compatibility](reference.md#compiler-compatibility) matrix.

## Nameof

* If argument to `NAMEOF` or `NAMEOF_FULL` has no name, compilation fails: `"Expression does not have a name."` `NAMEOF_RAW` returns raw expression text for any valid expression.

## Nameof Type

* Type-name results are compiler-specific unless customized.

* `nameof::nameof_short_type<T>()`, `NAMEOF_SHORT_TYPE`, `NAMEOF_SHORT_TYPE_EXPR`, and `NAMEOF_SHORT_TYPE_RTTI` do not accept array or pointer types.

* RTTI-based APIs follow `typeid` rules: the dynamic type is reported only for polymorphic glvalues.

* `nameof_member` does not support overloaded operator names.

## Nameof Enum

* Runtime reflection of ordinary enum values is limited to `[NAMEOF_ENUM_RANGE_MIN, NAMEOF_ENUM_RANGE_MAX]`. `NAMEOF_ENUM_CONST`, `nameof::nameof_enum<V>()`, `NAMEOF_ENUM_FLAG`, and `nameof::nameof_enum_flag()` are not restricted by this range.

  * By default, `NAMEOF_ENUM_RANGE_MIN = -128`, `NAMEOF_ENUM_RANGE_MAX = 127`.

  * The effective range is also clamped to the limits of the enum's underlying type.

  * `NAMEOF_ENUM_RANGE_MIN` must be less than or equal to `0` and must be greater than `INT16_MIN`.

  * `NAMEOF_ENUM_RANGE_MAX` must be greater than `0` and must be less than `INT16_MAX`.

  * If another range is needed for all enum types by default, redefine the macro `NAMEOF_ENUM_RANGE_MIN` and `NAMEOF_ENUM_RANGE_MAX`.

    ```cpp
    #define NAMEOF_ENUM_RANGE_MIN 0
    #define NAMEOF_ENUM_RANGE_MAX 256
    #include <nameof.hpp>
    ```

  * If another range is needed for a specific enum type, specialize `enum_range` for that type in `namespace nameof::customize`.

    ```cpp
    #include <nameof.hpp>

    enum class number { one = 100, two = 200, three = 300 };

    template <>
    struct nameof::customize::enum_range<number> {
      static constexpr int min = 100;
      static constexpr int max = 300;
    };
    ```

  * Enum ranges are limited to fewer than `UINT16_MAX` values.

* Names of aliased enum values are compiler-dependent.

* Forward-declared enums are not supported.

* Visual Studio IntelliSense may have problems analyzing some `nameof` expressions.
