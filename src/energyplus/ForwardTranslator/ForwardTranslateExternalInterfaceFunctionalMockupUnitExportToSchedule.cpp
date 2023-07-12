/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToSchedule.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"

#include <utilities/idd/ExternalInterface_FunctionalMockupUnitExport_To_Schedule_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExternalInterfaceFunctionalMockupUnitExportToSchedule(
    ExternalInterfaceFunctionalMockupUnitExportToSchedule& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface_FunctionalMockupUnitExport_To_Schedule);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    const boost::optional<ScheduleTypeLimits> m = modelObject.scheduleTypeLimits();
    if (m.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::ScheduleTypeLimitsNames, m.get().nameString());
    }

    s = modelObject.fMUVariableName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::FMUVariableName, s.get());
    }

    d = modelObject.initialValue();
    if (d.is_initialized()) {
      idfObject.setDouble(ExternalInterface_FunctionalMockupUnitExport_To_ScheduleFields::InitialValue, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
