/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/SiteWaterMainsTemperature.hpp"
#include "../../model/SiteWaterMainsTemperature_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/Site_WaterMainsTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSiteWaterMainsTemperature(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Site_WaterMainsTemperature) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Site_WaterMainsTemperature");
      return boost::none;
    }

    auto mo = m_model.getUniqueModelObject<SiteWaterMainsTemperature>();

    boost::optional<WorkspaceObject> wo;
    boost::optional<Schedule> schedule;

    wo = workspaceObject.getTarget(Site_WaterMainsTemperatureFields::TemperatureScheduleName);
    if (wo) {
      boost::optional<ModelObject> mo2 = translateAndMapWorkspaceObject(wo.get());
      if (mo2) {
        if ((schedule = mo2->optionalCast<Schedule>())) {
          mo.setTemperatureSchedule(schedule.get());
        }
      }
    }

    boost::optional<double> value = workspaceObject.getDouble(Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature);
    if (value) {
      mo.setAnnualAverageOutdoorAirTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures);
    if (value) {
      mo.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(value.get());
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
