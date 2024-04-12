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
  if (m_type != VariantType::Boolean) {
    LOG_AND_THROW("Variant does not hold a boolean");
  }
  return std::get<bool>(m_value);
}

int Variant::valueAsInteger() const {
  if (m_type != VariantType::Integer) {
    LOG_AND_THROW("Variant does not hold an int");
  }
  return std::get<int>(m_value);
}

double Variant::valueAsDouble() const {
  if (m_type == VariantType::Integer) {
    return static_cast<double>(valueAsInteger());
  }

  if (m_type != VariantType::Double) {
    LOG_AND_THROW("Variant does not hold a double");
  }
  return std::get<double>(m_value);
}

std::string Variant::valueAsString() const {
  if (m_type != VariantType::String) {
    LOG_AND_THROW("Variant does not hold a string");
  }
  return std::get<std::string>(m_value);
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

bool Variant::isTrueish() const {
  return std::visit(
    [](auto&& arg) -> bool {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, bool>) {  // NOLINT(bugprone-branch-clone)
        return arg;
      } else if constexpr (std::is_same_v<T, int>) {
        return arg != 0;
      } else if constexpr (std::is_same_v<T, double>) {
        return arg != 0.0;
      } else if constexpr (std::is_same_v<T, std::string>) {
        return arg == "true";
      } else {
        static_assert(always_false_v<T>, "non-exhaustive visitor!");
      }
    },
    m_value);
}

std::ostream& operator<<(std::ostream& os, const Variant& variant) {
  const VariantType variantType = variant.variantType();
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
