/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/DistrictCooling.hpp"
#include "../../model/DistrictCooling_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/DistrictCooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDistrictCooling(DistrictCooling& modelObject) {

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::DistrictCooling, modelObject);

    // Inlet Node Name
    if (auto node_ = modelObject.inletModelObject()) {
      idfObject.setString(DistrictCoolingFields::ChilledWaterInletNodeName, node_->nameString());
    }

    // Outlet Node Name
    if (auto node_ = modelObject.outletModelObject()) {
      idfObject.setString(DistrictCoolingFields::ChilledWaterOutletNodeName, node_->nameString());
    }

    // Nominal Capacity
    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(DistrictCoolingFields::NominalCapacity, "Autosize");
    } else if (auto value_ = modelObject.nominalCapacity()) {
      idfObject.setDouble(DistrictCoolingFields::NominalCapacity, *value_);
    }

    // Capacity Fraction Schedule Name
    {
      Schedule capacityFractionSchedule = modelObject.capacityFractionSchedule();
      if (auto sch_ = translateAndMapModelObject(capacityFractionSchedule)) {
        idfObject.setString(DistrictCoolingFields::CapacityFractionScheduleName, sch_->nameString());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
