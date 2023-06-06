/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToActuator.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImport.hpp"

#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_To_Actuator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExternalInterfaceFunctionalMockupUnitImportToActuator(
    ExternalInterfaceFunctionalMockupUnitImportToActuator& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Actuator);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    const boost::optional<ModelObject> m = modelObject.actuatedComponentUnique();
    if (m.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentUniqueName, m.get().nameString());
    }

    s = modelObject.actuatedComponentType();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentType, s.get());
    }

    s = modelObject.actuatedComponentControlType();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentControlType, s.get());
    }

    boost::optional<ExternalInterfaceFunctionalMockupUnitImport> file = modelObject.fMUFile();
    if (file.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUFileName, file.get().fMUFileName());
    }

    s = modelObject.fMUInstanceName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUInstanceName, s.get());
    }

    s = modelObject.fMUVariableName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUVariableName, s.get());
    }

    d = modelObject.initialValue();
    if (d.is_initialized()) {
      idfObject.setDouble(ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::InitialValue, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
