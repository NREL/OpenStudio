/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/PlantComponentTemperatureSource.hpp"
#include "../../model/PlantComponentTemperatureSource_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/PlantComponent_TemperatureSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePlantComponentTemperatureSource(PlantComponentTemperatureSource& modelObject) {
    OptionalString s;
    OptionalDouble value;
    OptionalModelObject temp;

    //Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PlantComponent_TemperatureSource, modelObject);

    // InletNode
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(PlantComponent_TemperatureSourceFields::InletNode, node->name().get());
    }

    // OutletNode
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(PlantComponent_TemperatureSourceFields::OutletNode, node->name().get());
    }

    // DesignVolumeFlowRate
    if (modelObject.isDesignVolumeFlowRateAutosized()) {
      idfObject.setString(PlantComponent_TemperatureSourceFields::DesignVolumeFlowRate, "Autosize");
    } else if ((value = modelObject.designVolumeFlowRate())) {
      idfObject.setDouble(PlantComponent_TemperatureSourceFields::DesignVolumeFlowRate, value.get());
    }

    // TemperatureSpecificationType
    if ((s = modelObject.temperatureSpecificationType())) {
      idfObject.setString(PlantComponent_TemperatureSourceFields::TemperatureSpecificationType, s.get());
    }

    // SourceTemperature
    if ((value = modelObject.sourceTemperature())) {
      idfObject.setDouble(PlantComponent_TemperatureSourceFields::SourceTemperature, value.get());
    }

    // SourceTemperatureScheduleName
    if (auto schedule = modelObject.sourceTemperatureSchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(PlantComponent_TemperatureSourceFields::SourceTemperatureScheduleName, _schedule->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
