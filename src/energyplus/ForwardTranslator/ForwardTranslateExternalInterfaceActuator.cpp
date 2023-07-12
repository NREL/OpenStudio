/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterfaceActuator.hpp"
#include "../../model/ExternalInterfaceActuator_Impl.hpp"

#include <utilities/idd/ExternalInterface_Actuator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExternalInterfaceActuator(ExternalInterfaceActuator& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface_Actuator);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    const boost::optional<ModelObject> m = modelObject.actuatedComponentUnique();
    if (m.is_initialized()) {
      idfObject.setString(ExternalInterface_ActuatorFields::ActuatedComponentUniqueName, m.get().nameString());
    }

    s = modelObject.actuatedComponentType();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_ActuatorFields::ActuatedComponentType, s.get());
    }

    s = modelObject.actuatedComponentControlType();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_ActuatorFields::ActuatedComponentControlType, s.get());
    }

    d = modelObject.optionalInitialValue();
    if (d.is_initialized()) {
      idfObject.setDouble(ExternalInterface_ActuatorFields::OptionalInitialValue, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
