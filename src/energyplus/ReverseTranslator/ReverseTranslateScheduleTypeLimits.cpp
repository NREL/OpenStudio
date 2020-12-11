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

#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/ScheduleTypeLimits_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ScheduleTypeLimits_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateScheduleTypeLimits(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ScheduleTypeLimits) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ScheduleTypeLimits");
      return boost::none;
    }

    openstudio::model::ScheduleTypeLimits scheduleTypeLimits(m_model);
    OptionalString s = workspaceObject.name();
    if (s) {
      if ((workspaceObject.numFields() == 1u) && (istringEqual(*s, "Any Number") || istringEqual(*s, "Number"))) {
        // Do not translate ScheduleTypeLimits called "Any Number" or "Number" and
        // with no other fields specified. Instead, let ModelObjects assign more
        // meaningful limits.
        LOG(Info, "The energyplus::ReverseTranslator throws out all 'Any Number' ScheduleTypeLimits "
                    << "to leave the OpenStudio model free to apply the appropriate units and limits.");
        return boost::none;
      }
      scheduleTypeLimits.setName(*s);
    }

    OptionalDouble d = workspaceObject.getDouble(ScheduleTypeLimitsFields::LowerLimitValue);
    if (d) {
      scheduleTypeLimits.setLowerLimitValue(*d);
    }

    d = workspaceObject.getDouble(ScheduleTypeLimitsFields::UpperLimitValue);
    if (d) {
      scheduleTypeLimits.setUpperLimitValue(*d);
    }

    s = workspaceObject.getString(ScheduleTypeLimitsFields::NumericType);
    if (s) {
      scheduleTypeLimits.setNumericType(*s);
    }

    s = workspaceObject.getString(ScheduleTypeLimitsFields::UnitType);
    if (s) {
      scheduleTypeLimits.setUnitType(*s);
    } else {
      bool test;
      // Attempt to default based on name (many EnergyPlus files do not have this field filled out).
      std::string name = scheduleTypeLimits.name().get();
      if (boost::regex_search(name, boost::regex("[Tt][Ee][Mm][Pp]"))) {
        if (boost::regex_search(name, boost::regex("[Dd][Ee][Ll][Tt][Aa]"))) {
          test = scheduleTypeLimits.setUnitType("DeltaTemperature");
          OS_ASSERT(test);
        } else {
          test = scheduleTypeLimits.setUnitType("Temperature");
          OS_ASSERT(test);
        }
      } else if (boost::regex_search(name, boost::regex("[Oo][Nn]")) && boost::regex_search(name, boost::regex("[Oo][Ff][Ff]"))) {
        test = scheduleTypeLimits.setUnitType("Availability");
        OS_ASSERT(test);
      } else if (boost::regex_search(name, boost::regex("[Cc][Oo][Nn][Tt][Rr][Oo][Ll]"))) {
        test = scheduleTypeLimits.setUnitType("ControlMode");
        OS_ASSERT(test);
      }
    }

    return scheduleTypeLimits;
  }

}  // namespace energyplus

}  // namespace openstudio
