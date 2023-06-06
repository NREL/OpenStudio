/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/OutputEnergyManagementSystem.hpp"
#include "../../model/OutputEnergyManagementSystem_Impl.hpp"

#include <utilities/idd/Output_EnergyManagementSystem_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputEnergyManagementSystem(OutputEnergyManagementSystem& modelObject) {
    boost::optional<std::string> s;

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_EnergyManagementSystem, modelObject);

    s = modelObject.actuatorAvailabilityDictionaryReporting();
    if (s) {
      idfObject.setString(Output_EnergyManagementSystemFields::ActuatorAvailabilityDictionaryReporting, s.get());
    }
    s = modelObject.internalVariableAvailabilityDictionaryReporting();
    if (s.is_initialized()) {
      idfObject.setString(Output_EnergyManagementSystemFields::InternalVariableAvailabilityDictionaryReporting, s.get());
    }
    s = modelObject.eMSRuntimeLanguageDebugOutputLevel();
    if (s.is_initialized()) {
      idfObject.setString(Output_EnergyManagementSystemFields::EMSRuntimeLanguageDebugOutputLevel, s.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
