#ifndef ALFALFA_METER_HPP
#define ALFALFA_METER_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaMeter : public AlfalfaComponent
  {
   public:
    AlfalfaMeter(const std::string& meter_name);

    ComponentCapabilities capabilities() const;

    Json::Value toJSON() const;

    std::string type() const;

   private:
    std::string m_meter_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
