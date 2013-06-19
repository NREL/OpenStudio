#include "AdvancedStatus.hpp"

namespace openstudio {
namespace runmanager {

  AdvancedStatus::AdvancedStatus()
    : m_enum(AdvancedStatusEnum::Idle)
  {
  }

  AdvancedStatus::AdvancedStatus(const AdvancedStatusEnum &t_enum)
    : m_enum(t_enum)
  {
  }

  AdvancedStatus::AdvancedStatus(const AdvancedStatusEnum &t_enum, const std::string &t_description)
    : m_enum(t_enum), m_description(t_description)
  {
  }

  std::string AdvancedStatus::toString() const
  {
    if (m_description.empty())
    {
      return m_enum.valueDescription();
    } else {
      return m_enum.valueDescription() + ": " + m_description;
    }
  }

  AdvancedStatusEnum AdvancedStatus::value() const
  {
    return m_enum;
  }

  std::string AdvancedStatus::description() const
  {
    return m_description;
  }


  bool AdvancedStatus::operator==(const AdvancedStatus &rhs) const
  {
    return m_enum == rhs.m_enum && m_description == rhs.m_description;
  }

  bool AdvancedStatus::operator!=(const AdvancedStatus &rhs) const
  {
    return m_enum != rhs.m_enum || m_description != rhs.m_description;
  }

  bool AdvancedStatus::operator<(const AdvancedStatus &rhs) const
  {
    return m_enum < rhs.m_enum 
       || ( m_enum == rhs.m_enum && m_description < rhs.m_description);
  }



}
}


