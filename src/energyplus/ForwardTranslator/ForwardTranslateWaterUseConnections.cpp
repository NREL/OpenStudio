/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/WaterUseConnections.hpp"
#include "../../model/WaterUseConnections_Impl.hpp"
#include "../../model/WaterUseEquipment.hpp"
#include "../../model/WaterUseEquipment_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WaterUse_Connections_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateWaterUseConnections(WaterUseConnections& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    OptionalSchedule schedule;

    IdfObject idfObject(IddObjectType::WaterUse_Connections);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // InletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(WaterUse_ConnectionsFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(WaterUse_ConnectionsFields::OutletNodeName, node->name().get());
      }
    }

    // Hot Water Supply Temperature Schedule Name

    if (boost::optional<Schedule> s = modelObject.hotWaterSupplyTemperatureSchedule()) {
      translateAndMapModelObject(s.get());

      idfObject.setString(WaterUse_ConnectionsFields::HotWaterSupplyTemperatureScheduleName, s->name().get());
    }

    // Cold Water Supply Temperature Schedule Name

    if (boost::optional<Schedule> s = modelObject.coldWaterSupplyTemperatureSchedule()) {
      translateAndMapModelObject(s.get());

      idfObject.setString(WaterUse_ConnectionsFields::ColdWaterSupplyTemperatureScheduleName, s->name().get());
    }

    // Drain Water Heat Exchanger Type

    s = modelObject.drainWaterHeatExchangerType();
    if (s) {
      idfObject.setString(WaterUse_ConnectionsFields::DrainWaterHeatExchangerType, *s);
    }

    // Drain Water Heat Exchanger Destination

    s = modelObject.drainWaterHeatExchangerDestination();
    if (s) {
      idfObject.setString(WaterUse_ConnectionsFields::DrainWaterHeatExchangerDestination, *s);
    }

    // Drain Water Heat Exchanger U-Factor Times Area

    value = modelObject.drainWaterHeatExchangerUFactorTimesArea();
    if (value) {
      idfObject.setDouble(WaterUse_ConnectionsFields::DrainWaterHeatExchangerUFactorTimesArea, value.get());
    }

    //  Water Use Equipment 1 Name

    std::vector<WaterUseEquipment> equipment = modelObject.waterUseEquipment();

    for (auto& elem : equipment) {
      boost::optional<IdfObject> _equipment = translateAndMapModelObject(elem);

      if (_equipment) {
        IdfExtensibleGroup group = idfObject.pushExtensibleGroup();

        group.setString(WaterUse_ConnectionsExtensibleFields::WaterUseEquipmentName, _equipment->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
