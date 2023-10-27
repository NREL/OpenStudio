/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleFile.hpp"
#include "../../model/ScheduleFile_Impl.hpp"
#include "../../model/ExternalFile.hpp"
#include "../../model/ExternalFile_Impl.hpp"

#include "../../utilities/data/TimeSeries.hpp"

#include <utilities/idd/Schedule_File_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScheduleFile(ScheduleFile& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Schedule_File);

    idfObject.setName(modelObject.name().get());

    boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
    if (scheduleTypeLimits) {
      boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
      if (idfScheduleTypeLimits) {
        idfObject.setString(openstudio::Schedule_FileFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
      }
    }

    path filePath;
    if (modelObject.externalFile().translateFileName()) {
      filePath = toPath(modelObject.externalFile().fileName());
    } else {
      filePath = modelObject.externalFile().filePath();
      if (!exists(filePath)) {
        LOG(Warn, "Cannot find file \"" << filePath << "\"");
      } else {
        // make the path correct for this system
        filePath = system_complete(filePath);
      }
    }

    // DLM: this path is going to be in the temp dir, might want to fix it up when saving model temp dir
    idfObject.setString(openstudio::Schedule_FileFields::FileName, toString(filePath));

    idfObject.setInt(openstudio::Schedule_FileFields::ColumnNumber, modelObject.columnNumber());
    idfObject.setInt(openstudio::Schedule_FileFields::RowstoSkipatTop, modelObject.rowstoSkipatTop());

    if (!modelObject.isNumberofHoursofDataDefaulted()) {
      idfObject.setInt(openstudio::Schedule_FileFields::NumberofHoursofData, modelObject.numberofHoursofData().get());
    }

    std::string columnSeparator = modelObject.columnSeparator();
    if (istringEqual("Fixed", columnSeparator)) {
      columnSeparator = "Space";
    }
    idfObject.setString(openstudio::Schedule_FileFields::ColumnSeparator, columnSeparator);

    if (modelObject.interpolatetoTimestep()) {
      idfObject.setString(openstudio::Schedule_FileFields::InterpolatetoTimestep, "Yes");
    } else {
      idfObject.setString(openstudio::Schedule_FileFields::InterpolatetoTimestep, "No");
    }

    if (!modelObject.isMinutesperItemDefaulted()) {
      idfObject.setString(openstudio::Schedule_FileFields::MinutesperItem, modelObject.minutesperItem().get());
    }

    if (modelObject.adjustScheduleforDaylightSavings()) {
      idfObject.setString(openstudio::Schedule_FileFields::AdjustScheduleforDaylightSavings, "Yes");
    } else {
      idfObject.setString(openstudio::Schedule_FileFields::AdjustScheduleforDaylightSavings, "No");
    }

    m_idfObjects.push_back(idfObject);
    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
