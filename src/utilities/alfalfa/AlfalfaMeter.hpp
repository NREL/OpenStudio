#ifndef ALFALFA_COMPONENT_METER_HPP
#define ALFALFA_COMPONENT_METER_HPP

#include "AlfalfaComponent.hpp"
#include "../idf/IdfObject.hpp"

#include "../UtilitiesAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class UTILITIES_API AlfalfaMeter : public AlfalfaComponent
  {
   public:
    /**
       * Create an AlfalfaMeter Component from a meter_name which reflects the Output:Meter EneregyPlus IDD
       */
    AlfalfaMeter(const std::string& meter_name);

    /**
       * Create an AlfalfaMeter Component from an IdfObject.
       * Valid idf_object types are OutputMeter and IdfObject(IddObjectType::Output_Meter).
       */
    AlfalfaMeter(const IdfObject& output_meter);
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
