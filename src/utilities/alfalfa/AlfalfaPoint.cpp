#include "AlfalfaPoint.hpp"
#include "AlfalfaPoint_Impl.hpp"

#include <boost/regex.hpp>

namespace openstudio {
namespace alfalfa {
  namespace detail {
    AlfalfaPoint_Impl::AlfalfaPoint_Impl() {}
  }  // namespace detail

  AlfalfaPoint::AlfalfaPoint() : m_impl(std::shared_ptr<detail::AlfalfaPoint_Impl>(new detail::AlfalfaPoint_Impl())) {}

  AlfalfaPoint::AlfalfaPoint(const std::string& display_name)
    : m_impl(std::shared_ptr<detail::AlfalfaPoint_Impl>(new detail::AlfalfaPoint_Impl())) {
      setDisplayName(display_name);
    }


  void AlfalfaPoint::setInput(const AlfalfaComponent& component) {
    if(component.canInput()) {
      m_impl->m_input = component;
    } else {
      throw std::runtime_error("Component of type: " + component.type + " cannot be used as an input.");
    }
  }

  boost::optional<AlfalfaComponent> AlfalfaPoint::input() const {
    return m_impl->m_input;
  }

  void AlfalfaPoint::setOutput(const AlfalfaComponent& component) {
    if(component.canOutput()) {
      m_impl->m_output = component;
    } else {
      throw std::runtime_error("Component of type: " + component.type + " cannot be used as an output.");
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
        LOG(Warn, "Display name does not produce a valid point ID. Manually set a valid ID or export will fail.");
      }
    }
    return result;
  }

  void AlfalfaPoint::setId(const std::string& id) {
    if(isValidId(id)){
      m_impl->m_id = id;
    } else {
      throw std::runtime_error(ID_VALID_CHARS_MSG);
    }
  }

  std::string AlfalfaPoint::displayName() const{
    return m_impl->m_display_name;
  }

  void AlfalfaPoint::setDisplayName(const std::string& display_name) {
    m_impl->m_display_name = display_name;
    if(!m_impl->m_id.is_initialized()) {
      std::string id = toIdString(display_name);
      if(!isValidId(id)) {
        LOG(Warn, "Display name does not produce a valid point ID. Manually set a valid ID or export will fail.");
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

    if (id().is_initialized()) {
      point["id"] = id().get();
    } else {
      throw std::runtime_error("Point requires a valid ID for export. " + ID_VALID_CHARS_MSG);
    }
    point["name"] = displayName();
    if (input().is_initialized()) {
      point["input"]["type"] = input().get().type;
      point["input"]["parameters"] = input().get().parameters;
    }

    if (output().is_initialized()) {
      point["output"]["type"] = output().get().type;
      point["output"]["parameters"] = output().get().parameters;
    }

    if (units().is_initialized()) {
      point["units"] = units().get();
    }

    point["optional"] = optional();

    return point;
  }

  bool AlfalfaPoint::isValidId(const std::string& id) const{
    return boost::regex_match(id, boost::regex("^[A-Za-z0-9_\\-\\[\\]:()]*$"));
  }

  std::string AlfalfaPoint::toIdString(const std::string& str) const{
    return boost::regex_replace(str, boost::regex(" "), "_");
  }

  bool AlfalfaPoint::operator==(const AlfalfaPoint& rhs) const {
    return toJSON() == rhs.toJSON();
  }
}  // namespace alfalfa
}  // namespace openstudio
