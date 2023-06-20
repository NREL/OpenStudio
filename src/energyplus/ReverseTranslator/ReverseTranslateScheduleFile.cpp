/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ScheduleFile.hpp"
#include "../../model/ScheduleFile_Impl.hpp"
#include "../../model/ExternalFile.hpp"
#include "../../model/ExternalFile_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Schedule_File_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_File_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateScheduleFile(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_File) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:File");
      return boost::none;
    }

    std::string fileName = workspaceObject.getString(Schedule_FileFields::FileName).get();
    boost::optional<ExternalFile> extfile = ExternalFile::getExternalFile(m_model, fileName);

    if (!extfile) {
      LOG(Error, "Could not translate Schedule:File, cannot find file \"" << fileName << "\"");
      return boost::none;
    }

    ScheduleFile scheduleFile(extfile.get(), workspaceObject.getInt(Schedule_FileFields::ColumnNumber).get(),
                              workspaceObject.getInt(Schedule_FileFields::RowstoSkipatTop).get());

    OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_FileFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleFile.setPointer(OS_Schedule_FileFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }

    if (OptionalString os = workspaceObject.name()) {
      scheduleFile.setName(*os);
    }

    if (OptionalInt oi = workspaceObject.getInt(Schedule_FileFields::NumberofHoursofData)) {
      scheduleFile.setNumberofHoursofData(*oi);
    }

    if (OptionalString os = workspaceObject.getString(Schedule_FileFields::ColumnSeparator)) {
      scheduleFile.setColumnSeparator(*os);
    }

    if (OptionalString os = workspaceObject.getString(Schedule_FileFields::InterpolatetoTimestep)) {
      if (openstudio::istringEqual("Yes", *os)) {
        scheduleFile.setInterpolatetoTimestep(true);
      }
    }

    if (OptionalInt oi = workspaceObject.getInt(Schedule_FileFields::MinutesperItem)) {
      scheduleFile.setMinutesperItem(*oi);
    }

    if (OptionalString os = workspaceObject.getString(Schedule_FileFields::AdjustScheduleforDaylightSavings)) {
      if (openstudio::istringEqual("Yes", *os)) {
        scheduleFile.setAdjustScheduleforDaylightSavings(true);
      }
    }

    return scheduleFile;
  }

}  // namespace energyplus

}  // namespace openstudio
