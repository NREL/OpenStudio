/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SiteWaterMainsTemperature.hpp"
#include "../../model/SiteWaterMainsTemperature_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Site_WaterMainsTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSiteWaterMainsTemperature(SiteWaterMainsTemperature& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Site_WaterMainsTemperature);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(Site_WaterMainsTemperatureFields::CalculationMethod, modelObject.calculationMethod());

    boost::optional<Schedule> schedule = modelObject.temperatureSchedule();
    if (schedule) {
      idfObject.setString(Site_WaterMainsTemperatureFields::TemperatureScheduleName, schedule->name().get());
    }

    boost::optional<double> d = modelObject.annualAverageOutdoorAirTemperature();
    if (d) {
      idfObject.setDouble(Site_WaterMainsTemperatureFields::AnnualAverageOutdoorAirTemperature, *d);
    }

    d = modelObject.maximumDifferenceInMonthlyAverageOutdoorAirTemperatures();
    if (d) {
      idfObject.setDouble(Site_WaterMainsTemperatureFields::MaximumDifferenceInMonthlyAverageOutdoorAirTemperatures, *d);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
