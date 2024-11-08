#ifndef UTILITIES_ALFALFA_ALFALFAPOINT_IMPL_HPP
#define UTILITIES_ALFALFA_ALFALFAPOINT_IMPL_HPP

#include "AlfalfaAPI.hpp"
#include "AlfalfaComponent.hpp"

#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"

#include <string>

namespace openstudio {
namespace alfalfa {
  class AlfalfaPoint;
  namespace detail {
    class ALFALFA_API AlfalfaPoint_Impl
    {
     public:
      explicit AlfalfaPoint_Impl(const std::string& display_name);

      Json::Value toJSON() const;

      void setInput(const AlfalfaComponent& component);

      boost::optional<AlfalfaComponent> input() const;

      void setOutput(const AlfalfaComponent& component);

      boost::optional<AlfalfaComponent> output() const;

      void setUnits(const std::string& units);

      boost::optional<std::string> units() const;

      std::string id() const;

      void setId(const std::string& id);

      std::string displayName() const;

      void setDisplayName(const std::string& display_name);

      bool isOptional() const;

      void setOptional(bool optional);

     private:
      static std::string toIdString(const std::string& str);

      boost::optional<AlfalfaComponent> m_input;
      boost::optional<AlfalfaComponent> m_output;
      std::string m_display_name;
      boost::optional<std::string> m_units;
      std::string m_id;
      bool m_optional = true;

      // configure logging
      REGISTER_LOGGER("openstudio.AlfalfaPoint")
    };
  }  // namespace detail
}  // namespace alfalfa
}  // namespace openstudio

#endif
