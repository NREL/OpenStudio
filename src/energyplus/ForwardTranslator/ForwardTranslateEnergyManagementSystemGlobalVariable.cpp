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
#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemGlobalVariable(EnergyManagementSystemGlobalVariable& modelObject) {
    boost::optional<std::string> s;
    //TODO add all the global variables to one object in the IDF
    IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_GlobalVariable);
    m_idfObjects.push_back(idfObject);
    //m_map.insert(std::make_pair(modelObject.handle(), idfObject));

    //AddErlVariable
    s = modelObject.name();
    if (s) {
      idfObject.setString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, s.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
