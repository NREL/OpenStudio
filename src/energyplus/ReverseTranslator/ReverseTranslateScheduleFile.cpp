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

    OptionalString os = workspaceObject.getString(Schedule_FileFields::InterpolatetoTimestep);
    if (os) {
      std::string temp = *os;
      boost::to_lower(temp);
      if (temp == "yes") {
        scheduleFile.setInterpolatetoTimestep(true);
      }
    }

    if (OptionalInt oi = workspaceObject.getInt(Schedule_FileFields::MinutesperItem)) {
      double result = 60.0 / (double)oi.get();
      if (trunc(result) == result) {
        scheduleFile.setMinutesperItem(std::to_string((int)result));
      }
      // Throw?
    }

    return scheduleFile;
  }

}  // namespace energyplus

}  // namespace openstudio
