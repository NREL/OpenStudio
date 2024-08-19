#ifndef ALFALFA_GLOBALVARIABLE_HPP
#define ALFALFA_GLOBALVARIABLE_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaGlobalVariable : public AlfalfaComponent
  {
   public:
    AlfalfaGlobalVariable(const std::string& variable_name);

    ComponentCapabilities capabilities() const;

    Json::Value toJSON() const;

    std::string type() const;

   private:
    std::string m_variable_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
