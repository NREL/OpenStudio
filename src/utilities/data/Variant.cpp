/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Variant.hpp"
#include <json/value.h>

namespace openstudio {

Variant::Variant() : m_type(VariantType::Boolean), m_value(false) {}

Variant::Variant(bool value) : m_type(VariantType::Boolean), m_value(value) {}

Variant::Variant(double value) : m_type(VariantType::Double), m_value(value) {}

Variant::Variant(int value) : m_type(VariantType::Integer), m_value(value) {}

Variant::Variant(const char* value) : m_type(VariantType::String), m_value(std::string(value)) {}

Variant::Variant(const std::string& value) : m_type(VariantType::String), m_value(value) {}

VariantType Variant::variantType() const {
  return m_type;
}

bool Variant::valueAsBoolean() const {
  // Note JM 2019-05-17: This is functionally equivalent to `std::get<bool>(m_value)` except it doesn't risk throwing
  // std::bad_variant_access which isn't available on mac prior to 10.14
  if (const auto* p = std::get_if<bool>(&m_value)) {
    return *p;
  } else {
    LOG_AND_THROW("Variant does not hold a boolean");
  }
}

int Variant::valueAsInteger() const {
  if (const auto* p = std::get_if<int>(&m_value)) {
    return *p;
  } else {
    LOG_AND_THROW("Variant does not hold an int");
  }
}

double Variant::valueAsDouble() const {
  if (m_type == VariantType::Integer) {
    return (double)valueAsInteger();
  }

  if (const auto* p = std::get_if<double>(&m_value)) {
    return *p;
  } else {
    LOG_AND_THROW("Variant does not hold a double");
  }
}

std::string Variant::valueAsString() const {
  if (const auto* p = std::get_if<std::string>(&m_value)) {
    return *p;
  } else {
    LOG_AND_THROW("Variant does not hold a string");
  }
}

// helper constant for the visitor below so we static assert we didn't miss a type
template <class>
inline constexpr bool always_false_v = false;

Json::Value Variant::valueAsJSON() const {
  return std::visit(
    [](auto&& arg) -> Json::Value {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, bool>) {  // NOLINT(bugprone-branch-clone)
        return arg;
      } else if constexpr (std::is_same_v<T, int>) {
        return arg;
      } else if constexpr (std::is_same_v<T, double>) {
        return arg;
      } else if constexpr (std::is_same_v<T, std::string>) {
        return arg;
      } else {
        static_assert(always_false_v<T>, "non-exhaustive visitor!");
      }
    },
    m_value);
}

std::ostream& operator<<(std::ostream& os, const Variant& variant) {
  VariantType variantType = variant.variantType();
  if (variantType == VariantType::String) {
    os << variant.valueAsString();
  } else if (variantType == VariantType::Double) {
    os << variant.valueAsDouble();
  } else if (variantType == VariantType::Integer) {
    os << variant.valueAsInteger();
  } else if (variantType == VariantType::Boolean) {
    if (variant.valueAsBoolean()) {
      os << "true";
    } else {
      os << "false";
    }
  }
  return os;
}

}  // namespace openstudio
