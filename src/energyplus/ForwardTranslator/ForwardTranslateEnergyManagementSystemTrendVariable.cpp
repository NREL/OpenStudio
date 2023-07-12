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
#include "../../model/EnergyManagementSystemTrendVariable.hpp"
#include "../../model/EnergyManagementSystemTrendVariable_Impl.hpp"

#include <utilities/idd/EnergyManagementSystem_TrendVariable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateEnergyManagementSystemTrendVariable(EnergyManagementSystemTrendVariable& modelObject) {
    boost::optional<int> i;
    boost::optional<std::string> s;

    IdfObject idfObject(openstudio::IddObjectType::EnergyManagementSystem_TrendVariable);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }
    s = modelObject.emsVariableName();
    if (s) {
      idfObject.setString(EnergyManagementSystem_TrendVariableFields::EMSVariableName, s.get());
    }
    i = modelObject.numberOfTimestepsToBeLogged();
    if (i.is_initialized()) {
      idfObject.setInt(EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged, i.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
