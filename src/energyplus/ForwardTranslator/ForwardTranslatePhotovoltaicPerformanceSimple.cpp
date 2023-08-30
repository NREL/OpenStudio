/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PhotovoltaicPerformanceSimple.hpp"
#include "../../model/PhotovoltaicPerformanceSimple_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/PhotovoltaicPerformance_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePhotovoltaicPerformanceSimple(model::PhotovoltaicPerformanceSimple& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PhotovoltaicPerformance_Simple, modelObject);

    if (modelObject.name()) {
      idfObject.setString(PhotovoltaicPerformance_SimpleFields::Name, modelObject.name().get());
    }

    idfObject.setString(PhotovoltaicPerformance_SimpleFields::ConversionEfficiencyInputMode, modelObject.conversionEfficiencyInputMode());

    idfObject.setDouble(PhotovoltaicPerformance_SimpleFields::FractionofSurfaceAreawithActiveSolarCells,
                        modelObject.fractionOfSurfaceAreaWithActiveSolarCells());

    if (modelObject.fixedEfficiency()) {
      idfObject.setDouble(PhotovoltaicPerformance_SimpleFields::ValueforCellEfficiencyifFixed, modelObject.fixedEfficiency().get());
    }

    if (modelObject.efficiencySchedule() && modelObject.efficiencySchedule().get().name()) {
      idfObject.setString(PhotovoltaicPerformance_SimpleFields::EfficiencyScheduleName, modelObject.efficiencySchedule().get().name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
