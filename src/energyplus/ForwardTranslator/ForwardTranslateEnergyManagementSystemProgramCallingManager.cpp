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
#include "../../model/EnergyManagementSystemProgram.hpp"
#include "../../model/EnergyManagementSystemProgram_Impl.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager.hpp"
#include "../../model/EnergyManagementSystemProgramCallingManager_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateEnergyManagementSystemProgramCallingManager(EnergyManagementSystemProgramCallingManager& modelObject) {
    boost::optional<std::string> s;

    IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::EnergyManagementSystem_ProgramCallingManager, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      pcm.setName(*s);
    }

    //callingpoint
    s = modelObject.callingPoint();
    if (s) {
      pcm.setString(EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, s.get());
    }

    //program names
    for (const IdfExtensibleGroup& eg : modelObject.extensibleGroups()) {
      pcm.pushExtensibleGroup(eg.fields());
    }
    return pcm;
  }

}  // namespace energyplus

}  // namespace openstudio
