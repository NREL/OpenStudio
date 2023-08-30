/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingWater(CoilCoolingWater& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_Cooling_Water);

    m_idfObjects.push_back(idfObject);

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    Schedule sched = modelObject.availableSchedule();
    boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
    if (_sched) {
      idfObject.setString(Coil_Cooling_WaterFields::AvailabilityScheduleName, _sched->name().get());
    }

    // DesignWaterFlowRate

    if (modelObject.isDesignWaterFlowRateAutosized()) {
      idfObject.setString(Coil_Cooling_WaterFields::DesignWaterFlowRate, "Autosize");
    } else if ((value = modelObject.designWaterFlowRate())) {
      idfObject.setDouble(Coil_Cooling_WaterFields::DesignWaterFlowRate, value.get());
    }

    // DesignAirFlowRate

    if (modelObject.isDesignAirFlowRateAutosized()) {
      idfObject.setString(Coil_Cooling_WaterFields::DesignAirFlowRate, "Autosize");
    } else if ((value = modelObject.designAirFlowRate())) {
      idfObject.setDouble(Coil_Cooling_WaterFields::DesignAirFlowRate, value.get());
    }

    // DesignInletWaterTemperature

    if (modelObject.isDesignInletWaterTemperatureAutosized()) {
      idfObject.setString(Coil_Cooling_WaterFields::DesignInletWaterTemperature, "Autosize");
    } else if ((value = modelObject.designInletWaterTemperature())) {
      idfObject.setDouble(Coil_Cooling_WaterFields::DesignInletWaterTemperature, value.get());
    }

    // DesignInletAirTemperature

    if (modelObject.isDesignInletAirTemperatureAutosized()) {
      idfObject.setString(Coil_Cooling_WaterFields::DesignInletAirTemperature, "Autosize");
    } else if ((value = modelObject.designInletAirTemperature())) {
      idfObject.setDouble(Coil_Cooling_WaterFields::DesignInletAirTemperature, value.get());
    }

    // DesignOutletAirTemperature

    if (modelObject.isDesignOutletAirTemperatureAutosized()) {
      idfObject.setString(Coil_Cooling_WaterFields::DesignOutletAirTemperature, "Autosize");
    } else if ((value = modelObject.designOutletAirTemperature())) {
      idfObject.setDouble(Coil_Cooling_WaterFields::DesignOutletAirTemperature, value.get());
    }

    // DesignInletAirHumidityRatio

    if (modelObject.isDesignInletAirHumidityRatioAutosized()) {
      idfObject.setString(Coil_Cooling_WaterFields::DesignInletAirHumidityRatio, "Autosize");
    } else if ((value = modelObject.designInletAirHumidityRatio())) {
      idfObject.setDouble(Coil_Cooling_WaterFields::DesignInletAirHumidityRatio, value.get());
    }

    // DesignOutletAirHumidityRatio

    if (modelObject.isDesignOutletAirHumidityRatioAutosized()) {
      idfObject.setString(Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio, "Autosize");
    } else if ((value = modelObject.designOutletAirHumidityRatio())) {
      idfObject.setDouble(Coil_Cooling_WaterFields::DesignOutletAirHumidityRatio, value.get());
    }

    // WaterInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterFields::WaterInletNodeName, node->name().get());
      }
    }

    // WaterOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterFields::WaterOutletNodeName, node->name().get());
      }
    }

    // AirInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterFields::AirInletNodeName, node->name().get());
      }
    }

    // AirOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterFields::AirOutletNodeName, node->name().get());
      }
    }

    // TypeofAnalysis

    s = modelObject.typeOfAnalysis();
    if (s) {
      idfObject.setString(Coil_Cooling_WaterFields::TypeofAnalysis, s.get());
    }

    // HeatExchangerConfiguration

    s = modelObject.heatExchangerConfiguration();
    if (s) {
      idfObject.setString(Coil_Cooling_WaterFields::HeatExchangerConfiguration, s.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

  //((Name)(Name))
  //((AvailabilityScheduleName)(Availability Schedule Name))

}  // namespace energyplus

}  // namespace openstudio
