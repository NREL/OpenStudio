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
#include "../../model/CoilHeatingSteam.hpp"
#include "../../model/CoilHeatingSteam_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Coil_Heating_Steam_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingSteam(CoilHeatingSteam& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Coil_Heating_Steam);

    m_idfObjects.push_back(idfObject);

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    Schedule sched = modelObject.availabilitySchedule();
    boost::optional<IdfObject> _sched = translateAndMapModelObject(sched);
    if (_sched) {
      idfObject.setString(Coil_Heating_SteamFields::AvailabilityScheduleName, _sched->name().get());
    }

    // MaximumSteamFlowRate

    if (modelObject.isMaximumSteamFlowRateAutosized()) {
      idfObject.setString(Coil_Heating_SteamFields::MaximumSteamFlowRate, "Autosize");
    } else if ((value = modelObject.maximumSteamFlowRate())) {
      idfObject.setDouble(Coil_Heating_SteamFields::MaximumSteamFlowRate, value.get());
    }

    // DegreeofSubCooling

    if (value = modelObject.degreeofSubCooling()) {
      idfObject.setDouble(Coil_Heating_SteamFields::DegreeofSubCooling, value.get());
    }

    // DegreeofLoopSubCooling

    if (value = modelObject.degreeofLoopSubCooling()) {
      idfObject.setDouble(Coil_Heating_SteamFields::DegreeofLoopSubCooling, value.get());
    }

    // WaterInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_SteamFields::WaterInletNodeName, node->name().get());
      }
    }

    // WaterOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_SteamFields::WaterOutletNodeName, node->name().get());
      }
    }

    // AirInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_SteamFields::AirInletNodeName, node->name().get());
      }
    }

    // AirOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Heating_SteamFields::AirOutletNodeName, node->name().get());
      }
    }

    // CoilControlType

    if (s = modelObject.coilControlType()) {
      idfObject.setString(Coil_Heating_SteamFields::CoilControlType, s.get());
    }

    // TemperatureSetpointNodeName
    // If it was hardset we actually use that, otherwise keep above default (coil outlet)
    if (boost::optional<Node> node = modelObject.temperatureSetpointNode()) {
      idfObject.setString(Coil_Heating_SteamFields::TemperatureSetpointNodeName, node->name().get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

  //((Name)(Name))
  //((AvailabilityScheduleName)(Availability Schedule Name))

}  // namespace energyplus

}  // namespace openstudio
