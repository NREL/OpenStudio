/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/DistrictHeatingWater.hpp"
#include "../../model/DistrictHeatingWater_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/DistrictHeating_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDistrictHeatingWater(DistrictHeatingWater& modelObject) {

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::DistrictHeating_Water, modelObject);

    // Inlet Node Name
    if (auto node_ = modelObject.inletModelObject()) {
      idfObject.setString(DistrictHeating_WaterFields::HotWaterInletNodeName, node_->nameString());
    }

    // Outlet Node Name
    if (auto node_ = modelObject.outletModelObject()) {
      idfObject.setString(DistrictHeating_WaterFields::HotWaterOutletNodeName, node_->nameString());
    }

    // Nominal Capacity
    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(DistrictHeating_WaterFields::NominalCapacity, "Autosize");
    } else if (auto value_ = modelObject.nominalCapacity()) {
      idfObject.setDouble(DistrictHeating_WaterFields::NominalCapacity, *value_);
    }

    // Capacity Fraction Schedule Name
    if (auto capacityFractionSchedule_ = modelObject.capacityFractionSchedule()) {
      if (auto sch_ = translateAndMapModelObject(*capacityFractionSchedule_)) {
        idfObject.setString(DistrictHeating_WaterFields::CapacityFractionScheduleName, sch_->nameString());
      }
    }

    return idfObject;
  }
}  // namespace energyplus

}  // namespace openstudio
