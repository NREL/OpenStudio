/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportFromVariable.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl.hpp"

#include <utilities/idd/ExternalInterface_FunctionalMockupUnitExport_From_Variable_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExternalInterfaceFunctionalMockupUnitExportFromVariable(
    ExternalInterfaceFunctionalMockupUnitExportFromVariable& modelObject) {
    boost::optional<std::string> s;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface_FunctionalMockupUnitExport_From_Variable);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    s = modelObject.outputVariableIndexKeyName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableIndexKeyName, s.get());
    }
    s = modelObject.outputVariableName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableName, s.get());
    }
    s = modelObject.fMUVariableName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::FMUVariableName, s.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
