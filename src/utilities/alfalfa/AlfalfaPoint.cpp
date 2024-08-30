#include "AlfalfaPoint.hpp"
#include "AlfalfaPoint_Impl.hpp"

namespace openstudio {
namespace alfalfa {
  namespace detail {
    AlfalfaPoint_Impl::AlfalfaPoint_Impl() {}

    AlfalfaPoint_Impl::AlfalfaPoint_Impl(const std::string& display_name) : m_display_name(display_name) {}

    AlfalfaPoint_Impl::AlfalfaPoint_Impl(const std::string& display_name, const std::string& id) : m_display_name(display_name), m_id(id) {}

    Json::Value AlfalfaPoint_Impl::toJSON() const {
      Json::Value point;

      point["id"] = m_id;
      point["name"] = m_display_name;
      if (m_input.is_initialized()) {
        point["input"]["type"] = m_input.get().type;
        point["input"]["parameters"] = m_input.get().parameters;
      }

      if (m_output.is_initialized()) {
        point["output"]["type"] = m_output.get().type;
        point["output"]["parameters"] = m_output.get().parameters;
      }

      return point;
    }
  }  // namespace detail

  AlfalfaPoint::AlfalfaPoint() : m_impl(std::shared_ptr<detail::AlfalfaPoint_Impl>(new detail::AlfalfaPoint_Impl())) {}

  AlfalfaPoint::AlfalfaPoint(const std::string& display_name)
    : m_impl(std::shared_ptr<detail::AlfalfaPoint_Impl>(new detail::AlfalfaPoint_Impl(display_name))) {}

  AlfalfaPoint::AlfalfaPoint(const std::string& display_name, const std::string& id)
    : m_impl(std::shared_ptr<detail::AlfalfaPoint_Impl>(new detail::AlfalfaPoint_Impl(display_name, id))) {}

  void AlfalfaPoint::setInput(const AlfalfaComponent& component) {
    m_impl->m_input = component;
  }

  boost::optional<AlfalfaComponent> AlfalfaPoint::getInput() {
    return m_impl->m_input;
  }

  void AlfalfaPoint::setOutput(const AlfalfaComponent& component) {
    m_impl->m_output = component;
  }

  boost::optional<AlfalfaComponent> AlfalfaPoint::getOutput() {
    return m_impl->m_output;
  }

  void AlfalfaPoint::setUnits(const std::string& units) {
    m_impl->m_units = units;
  }

  boost::optional<std::string> AlfalfaPoint::getUnits() {
    return m_impl->m_units;
  }

  std::string AlfalfaPoint::id() const {
    return m_impl->m_id;
  }

  Json::Value AlfalfaPoint::toJSON() const {
    return m_impl->toJSON();
  }

  bool AlfalfaPoint::operator==(const AlfalfaPoint& rhs) const {
    return toJSON() == rhs.toJSON();
  }
}  // namespace alfalfa
}  // namespace openstudio
