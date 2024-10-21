#include "AlfalfaPoint.hpp"
#include "AlfalfaPoint_Impl.hpp"
#include "AlfalfaComponent.hpp"

#include <boost/regex.hpp>
#include <fmt/format.h>

#include <memory>
#include <string_view>

namespace openstudio {
namespace alfalfa {

  static constexpr std::string_view ID_VALID_CHARS_MSG = "IDs can only contain letters, numbers, and the following special characters _-[]():";

  namespace detail {

    AlfalfaPoint_Impl::AlfalfaPoint_Impl(const std::string& display_name) {
      setDisplayName(display_name);
    }

    Json::Value AlfalfaPoint_Impl::toJSON() const {
      Json::Value point;

      if (auto id_ = id()) {
        point["id"] = *id_;
      } else {
        throw std::runtime_error(fmt::format("Point requires a valid ID for export. {}", ID_VALID_CHARS_MSG));
      }
      point["name"] = displayName();
      if (auto input_ = input()) {
        point["input"]["type"] = input_->typeName();
        point["input"]["parameters"] = input_->toJSON();
      }

      if (auto output_ = output()) {
        point["output"]["type"] = output_->typeName();
        point["output"]["parameters"] = output_->toJSON();
      }

      if (auto units_ = units()) {
        point["units"] = *units_;
      }

      point["optional"] = isOptional();

      return point;
    }

    void AlfalfaPoint_Impl::setInput(const AlfalfaComponent& component) {
      if (component.canInput()) {
        m_input = component;
      } else {
        throw std::runtime_error(fmt::format("Component of type: {} cannot be used as an input.", component.typeName()));
      }
    }

    boost::optional<AlfalfaComponent> AlfalfaPoint_Impl::input() const {
      return m_input;
    }

    void AlfalfaPoint_Impl::setOutput(const AlfalfaComponent& component) {
      if (component.canOutput()) {
        m_output = component;
      } else {
        throw std::runtime_error(fmt::format("Component of type: {} cannot be used as an output.", component.typeName()));
      }
    }

    boost::optional<AlfalfaComponent> AlfalfaPoint_Impl::output() const {
      return m_output;
    }

    void AlfalfaPoint_Impl::setUnits(const std::string& units) {
      m_units = units;
    }

    boost::optional<std::string> AlfalfaPoint_Impl::units() const {
      return m_units;
    }

    void AlfalfaPoint_Impl::setId(const std::string& id) {
      if (isValidId(id)) {
        m_id = id;
      } else {
        throw std::runtime_error(ID_VALID_CHARS_MSG.data());
      }
    }

    boost::optional<std::string> AlfalfaPoint_Impl::id() const {
      boost::optional<std::string> result = m_id;
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

    void AlfalfaPoint_Impl::setDisplayName(const std::string& display_name) {
      if (display_name.empty()) {
        throw std::runtime_error("Display name must have non-zero length");
      }
      m_display_name = display_name;
      if (!m_id.is_initialized()) {
        const std::string id = toIdString(display_name);
        if (!isValidId(id)) {
          LOG(Warn, fmt::format("Display name '{}' does not produce a valid point ID. Manually set a valid ID or export will fail.", display_name));
        }
      }
    }

    std::string AlfalfaPoint_Impl::displayName() const {
      return m_display_name;
    }

    void AlfalfaPoint_Impl::setOptional(bool optional) {
      m_optional = optional;
    }

    bool AlfalfaPoint_Impl::isOptional() const {
      return m_optional;
    }

    bool AlfalfaPoint_Impl::isValidId(const std::string& id) {
      return !id.empty() && boost::regex_match(id, boost::regex(R"(^[A-Za-z0-9_\-\[\]:()]*$)"));
    }

    std::string AlfalfaPoint_Impl::toIdString(const std::string& str) {
      return boost::regex_replace(str, boost::regex(" "), "_");
    }
  }  // namespace detail

  // AlfalfaPoint::AlfalfaPoint(const AlfalfaPoint& point) : m_impl(point.m_impl) {}

  AlfalfaPoint::AlfalfaPoint(const std::string& display_name) : m_impl(std::make_shared<detail::AlfalfaPoint_Impl>(display_name)) {}

  void AlfalfaPoint::setInput(const AlfalfaComponent& component) {
    m_impl->setInput(component);
  }

  boost::optional<AlfalfaComponent> AlfalfaPoint::input() const {
    return m_impl->input();
  }

  void AlfalfaPoint::setOutput(const AlfalfaComponent& component) {
    m_impl->setOutput(component);
  }

  boost::optional<AlfalfaComponent> AlfalfaPoint::output() const {
    return m_impl->output();
  }

  void AlfalfaPoint::setUnits(const std::string& units) {
    m_impl->setUnits(units);
  }

  boost::optional<std::string> AlfalfaPoint::units() const {
    return m_impl->units();
  }

  boost::optional<std::string> AlfalfaPoint::id() const {
    return m_impl->id();
  }

  void AlfalfaPoint::setId(const std::string& id) {
    m_impl->setId(id);
  }

  std::string AlfalfaPoint::displayName() const {
    return m_impl->displayName();
  }

  void AlfalfaPoint::setDisplayName(const std::string& display_name) {
    m_impl->setDisplayName(display_name);
  }

  bool AlfalfaPoint::isOptional() const {
    return m_impl->isOptional();
  }

  void AlfalfaPoint::setOptional(bool optional) {
    m_impl->setOptional(optional);
  }

  Json::Value AlfalfaPoint::toJSON() const {
    return m_impl->toJSON();
  }

  bool AlfalfaPoint::operator==(const AlfalfaPoint& rhs) const {
    return toJSON() == rhs.toJSON();
  }
}  // namespace alfalfa
}  // namespace openstudio
