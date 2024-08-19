#ifndef ALFALFA_OUTPUTVARIABLE_HPP
#define ALFALFA_OUTPUTVARIABLE_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaOutputVariable : public AlfalfaComponent
  {
   public:
    AlfalfaOutputVariable(const std::string& variable_key, const std::string& variable_name);

    ComponentCapabilities capabilities() const;

    Json::Value toJSON() const;

    std::string type() const;

   private:
    std::string m_variable_key, m_variable_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
