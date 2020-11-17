/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    path filePath = modelObject.externalFile().filePath();
    if (!exists(filePath)) {
      LOG(Warn, "Cannot find file \"" << filePath << "\"");
    } else {
      // make the path correct for this system
      filePath = system_complete(filePath);
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

    m_idfObjects.push_back(idfObject);
    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
