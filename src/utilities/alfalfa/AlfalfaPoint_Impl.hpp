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

     protected:
      friend class openstudio::alfalfa::AlfalfaPoint;
      boost::optional<AlfalfaComponent> m_input, m_output;
      std::string m_display_name;
      boost::optional<std::string> m_units, m_id;
      bool m_optional = true;
    };
  }  // namespace detail
}  // namespace alfalfa
}  // namespace openstudio

#endif
