#ifndef UTILITIES_ALFALFA_ALFALFAPOINT_IMPL_HPP
#define UTILITIES_ALFALFA_ALFALFAPOINT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "AlfalfaComponent.hpp"

#include "../core/Optional.hpp"
#include <string>

namespace openstudio {
namespace alfalfa {
  class AlfalfaPoint;
  namespace detail {
    class UTILITIES_API AlfalfaPoint_Impl
    {
     public:
      AlfalfaPoint_Impl();

      AlfalfaPoint_Impl(const std::string& display_name);

      AlfalfaPoint_Impl(const std::string& display_name, const std::string& id);

      Json::Value toJSON() const;

     protected:
      friend class openstudio::alfalfa::AlfalfaPoint;
      boost::optional<AlfalfaComponent> m_input, m_output;
      std::string m_display_name, m_id;
      boost::optional<std::string> m_units;
    };
  }  // namespace detail
}  // namespace alfalfa
}  // namespace openstudio

#endif
