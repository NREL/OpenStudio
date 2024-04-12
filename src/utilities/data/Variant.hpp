/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_VARIANT_HPP
#define UTILITIES_DATA_VARIANT_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Enum.hpp"
#include "../core/Logger.hpp"

#include <optional>
#include <variant>

namespace Json {
class Value;
}

namespace openstudio {

// clang-format off

/** \class VariantType
 *  \brief A listing of data types that can be held in a Variant.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *  \code
OPENSTUDIO_ENUM(VariantType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Integer)(Integer)(2))
  ((String)(String)(3))
);
 *  \endcode */
OPENSTUDIO_ENUM(VariantType,
  ((Boolean)(Boolean)(0))
  ((Double)(Double)(1))
  ((Integer)(Integer)(2))
  ((String)(String)(3))
);

// clang-format on

/** A Variant is a flexible type that can hold several types of values.
 */
class UTILITIES_API Variant
{
 public:
  /// constructors
  explicit Variant();
  explicit Variant(bool value);
  explicit Variant(double value);
  explicit Variant(int value);
  explicit Variant(const char* value);
  explicit Variant(const std::string& value);

  /// get the variant value type
  VariantType variantType() const;

  /// get value as a bool
  bool valueAsBoolean() const;

  /// get value as int
  int valueAsInteger() const;

  /// get value as double
  double valueAsDouble() const;

  /// get value as string
  std::string valueAsString() const;

  Json::Value valueAsJSON() const;

  // No matter the underlying type, will check if it's true-ish
  // Used for skipping a measure. eg true, "true", 1, 1.0 are all trueish
  bool isTrueish() const;

 private:
  REGISTER_LOGGER("openstudio.Variant");

  VariantType m_type;
  std::variant<bool, int, double, std::string> m_value;
};

/** \relates Variant */
using OptionalVariant = std::optional<Variant>;

/** \relates Variant */
using VariantVector = std::vector<Variant>;

UTILITIES_API std::ostream& operator<<(std::ostream& os, const Variant& variant);

}  // namespace openstudio

#endif  // UTILITIES_DATA_VARIANT_HPP
