#include "AlfalfaPoint.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaPoint::AlfalfaPoint() {}

  AlfalfaPoint::AlfalfaPoint(std::string display_name)
    : m_display_name(display_name) {}

  AlfalfaPoint::AlfalfaPoint(std::string display_name, std::string id)
    : m_display_name(display_name),
      m_id(id) {}

  void AlfalfaPoint::setInput(AlfalfaComponent &component) {
    m_input = &component;
  }

  void AlfalfaPoint::setOutput(AlfalfaComponent &component) {
    m_output = &component;
  }

  std::string AlfalfaPoint::id() const {
    return m_id;
  }

  Json::Value AlfalfaPoint::toJSON() const {
    Json::Value point;

    point["id"] = m_id;
    point["name"] = m_display_name;
    if ( m_input != NULL ) {
      point["input"]["type"] = m_input->type();
      point["input"]["parameters"] = m_input->toJSON();
    }

    if ( m_output != NULL ) {
      point["output"]["type"] = m_output->type();
      point["output"]["parameters"] = m_output->toJSON();
    }

    return point;
  }

}
}
