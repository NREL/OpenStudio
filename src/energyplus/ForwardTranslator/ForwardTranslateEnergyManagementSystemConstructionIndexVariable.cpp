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
#include "../../model/EnergyManagementSystemConstructionIndexVariable.hpp"
#include "../../model/EnergyManagementSystemConstructionIndexVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_ConstructionIndexVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateEnergyManagementSystemConstructionIndexVariable(EnergyManagementSystemConstructionIndexVariable& modelObject) {
    boost::optional<std::string> s;

    IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_ConstructionIndexVariable);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    idfObject.setString(EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName,
                        modelObject.constructionObject().nameString());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
