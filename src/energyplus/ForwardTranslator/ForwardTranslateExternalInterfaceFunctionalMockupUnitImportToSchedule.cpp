/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToSchedule.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../../model/ScheduleTypeLimits.hpp"

#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_To_Schedule_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExternalInterfaceFunctionalMockupUnitImportToSchedule(
    ExternalInterfaceFunctionalMockupUnitImportToSchedule& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface_FunctionalMockupUnitImport_To_Schedule);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    boost::optional<ScheduleTypeLimits> schedule = modelObject.scheduleTypeLimits();
    if (schedule.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::ScheduleTypeLimitsNames, schedule.get().nameString());
    }

    boost::optional<ExternalInterfaceFunctionalMockupUnitImport> file = modelObject.fMUFile();
    if (file.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUFileName, file.get().fMUFileName());
    }

    s = modelObject.fMUInstanceName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUInstanceName, s.get());
    }

    s = modelObject.fMUVariableName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::FMUVariableName, s.get());
    }

    d = modelObject.initialValue();
    if (d.is_initialized()) {
      idfObject.setDouble(ExternalInterface_FunctionalMockupUnitImport_To_ScheduleFields::InitialValue, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
