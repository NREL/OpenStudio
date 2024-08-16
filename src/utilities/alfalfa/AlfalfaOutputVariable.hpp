#ifndef ALFALFA_OUTPUTVARIABLE_HPP
#define ALFALFA_OUTPUTVARIABLE_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaOutputVariable : public AlfalfaComponent {
    public:
      AlfalfaOutputVariable(std::string variable_name, std::string variable_key);

      ComponentCapabilities capabilities() const;

      Json::Value toJSON() const;

      std::string type() const;

    private:
      std::string m_variable_name, m_variable_key;
  };
}
}

#endif
