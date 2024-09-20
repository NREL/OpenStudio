#include "AlfalfaPoint.hpp"
#include "AlfalfaPoint_Impl.hpp"

#include <boost/regex.hpp>
#include <fmt/format.h>

#include <memory>
#include <string_view>

namespace openstudio {
namespace alfalfa {

  static constexpr std::string_view ID_VALID_CHARS_MSG = "IDs can only contain letters, numbers, and the following special characters _-[]():";

  namespace detail {
    AlfalfaPoint_Impl::AlfalfaPoint_Impl() = default;
  }  // namespace detail

  AlfalfaPoint::AlfalfaPoint() : m_impl(std::make_shared<detail::AlfalfaPoint_Impl>()) {}

  AlfalfaPoint::AlfalfaPoint(const std::string& display_name) : m_impl(std::make_shared<detail::AlfalfaPoint_Impl>()) {
    setDisplayName(display_name);
  }

  void AlfalfaPoint::setInput(const AlfalfaComponent& component) {
    if (component.canInput()) {
      m_impl->m_input = component;
    } else {
      throw std::runtime_error(fmt::format("Component of type: {} cannot be used as an input.", component.type));
    }
  }

  boost::optional<AlfalfaComponent> AlfalfaPoint::input() const {
    return m_impl->m_input;
  }

  void AlfalfaPoint::setOutput(const AlfalfaComponent& component) {
    if (component.canOutput()) {
      m_impl->m_output = component;
    } else {
      throw std::runtime_error(fmt::format("Component of type: {} cannot be used as an output.", component.type));
    }
  }

  boost::optional<AlfalfaComponent> AlfalfaPoint::output() const {
    return m_impl->m_output;
  }

  void AlfalfaPoint::setUnits(const std::string& units) {
    m_impl->m_units = units;
  }

  boost::optional<std::string> AlfalfaPoint::units() const {
    return m_impl->m_units;
  }

  boost::optional<std::string> AlfalfaPoint::id() const {
    boost::optional<std::string> result = m_impl->m_id;
    if (!result.is_initialized()) {
      std::string id = toIdString(displayName());
      if (isValidId(id)) {
        result = id;
      } else {
        LOG(Warn, fmt::format("Display name '{}' does not produce a valid point ID. Manually set a valid ID or export will fail.", displayName()));
      }
    }
    return result;
  }

  void AlfalfaPoint::setId(const std::string& id) {
    if (isValidId(id)) {
      m_impl->m_id = id;
    } else {
      throw std::runtime_error(ID_VALID_CHARS_MSG.data());
    }
  }

  std::string AlfalfaPoint::displayName() const {
    return m_impl->m_display_name;
  }

  void AlfalfaPoint::setDisplayName(const std::string& display_name) {
    if (display_name.empty()) {
      throw std::runtime_error("Display name must have non-zero length");
    }
    m_impl->m_display_name = display_name;
    if (!m_impl->m_id.is_initialized()) {
      std::string id = toIdString(display_name);
      if (!isValidId(id)) {
        LOG(Warn, fmt::format("Display name '{}' does not produce a valid point ID. Manually set a valid ID or export will fail.", display_name));
      }
    }
  }

  bool AlfalfaPoint::optional() const {
    return m_impl->m_optional;
  }

  void AlfalfaPoint::setOptional(bool optional) {
    m_impl->m_optional = optional;
  }

  Json::Value AlfalfaPoint::toJSON() const {
    Json::Value point;

    if (auto id_ = id()) {
      point["id"] = *id_;
    } else {
      throw std::runtime_error(fmt::format("Point requires a valid ID for export. {}", ID_VALID_CHARS_MSG));
    }
    point["name"] = displayName();
    if (auto input_ = input()) {
      point["input"]["type"] = input_->type;
      point["input"]["parameters"] = input_->parameters;
    }

    if (auto output_ = output()) {
      point["output"]["type"] = output_->type;
      point["output"]["parameters"] = output_->parameters;
    }

    if (auto units_ = units()) {
      point["units"] = *units_;
    }

    point["optional"] = optional();

    return point;
  }

  bool AlfalfaPoint::isValidId(const std::string& id) {
    return !id.empty() && boost::regex_match(id, boost::regex(R"(^[A-Za-z0-9_\-\[\]:()]*$)"));
  }

  std::string AlfalfaPoint::toIdString(const std::string& str) {
    return boost::regex_replace(str, boost::regex(" "), "_");
  }

  bool AlfalfaPoint::operator==(const AlfalfaPoint& rhs) const {
    return toJSON() == rhs.toJSON();
  }
}  // namespace alfalfa
}  // namespace openstudio
