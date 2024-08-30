#ifndef ALFALFA_COMPONENT_METER_HPP
#define ALFALFA_COMPONENT_METER_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/idf/IdfObject.hpp"
#include "utilities/core/Logger.hpp"

namespace openstudio {
namespace alfalfa {
  class AlfalfaMeter : public AlfalfaComponent
  {
   public:
    AlfalfaMeter(const std::string& meter_name);

    AlfalfaMeter(const IdfObject& output_meter);

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.AlfalfaMeter");
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
