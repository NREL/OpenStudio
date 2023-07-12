/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
