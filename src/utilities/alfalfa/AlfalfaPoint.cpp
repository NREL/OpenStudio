#include "AlfalfaPoint.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaPoint::AlfalfaPoint() {}

  AlfalfaPoint::AlfalfaPoint(const std::string& display_name) : m_display_name(display_name) {}

  AlfalfaPoint::AlfalfaPoint(const std::string& display_name, const std::string& id) : m_display_name(display_name), m_id(id) {}

  void AlfalfaPoint::setInput(AlfalfaComponent& component) {
    m_input = &component;
  }

  boost::optional<AlfalfaComponent*> AlfalfaPoint::getInput() {
    if (m_input) {
      boost::optional<AlfalfaComponent*> result = m_input;
      return result;
    }
    return boost::none;
  }

  void AlfalfaPoint::setOutput(AlfalfaComponent& component) {
    m_output = &component;
  }

  boost::optional<AlfalfaComponent*> AlfalfaPoint::getOutput() {
    if (m_output) {
      boost::optional<AlfalfaComponent*> result = m_output;
      return result;
    }
    return boost::none;
  }

  void AlfalfaPoint::setUnits(const std::string& units) {
    m_units = units;
  }

  boost::optional<std::string> AlfalfaPoint::getUnits() {
    if (m_units.length() != 0) {
      boost::optional<std::string> result = m_units;
      return result;
    }
    return boost::none;
  }

  std::string AlfalfaPoint::id() const {
    return m_id;
  }

  Json::Value AlfalfaPoint::toJSON() const {
    Json::Value point;

    point["id"] = m_id;
    point["name"] = m_display_name;
    if (m_input != NULL) {
      point["input"]["type"] = m_input->type();
      point["input"]["parameters"] = m_input->toJSON();
    }

    if (m_output != NULL) {
      point["output"]["type"] = m_output->type();
      point["output"]["parameters"] = m_output->toJSON();
    }

    return point;
  }

}  // namespace alfalfa
}  // namespace openstudio
