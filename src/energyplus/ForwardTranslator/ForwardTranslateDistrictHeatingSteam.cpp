/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/DistrictHeatingSteam.hpp"
#include "../../model/DistrictHeatingSteam_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/DistrictHeating_Steam_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDistrictHeatingSteam(DistrictHeatingSteam& modelObject) {

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::DistrictHeating_Steam, modelObject);

    // Inlet Node Name
    if (auto node_ = modelObject.inletModelObject()) {
      idfObject.setString(DistrictHeating_SteamFields::SteamInletNodeName, node_->nameString());
    }

    // Outlet Node Name
    if (auto node_ = modelObject.outletModelObject()) {
      idfObject.setString(DistrictHeating_SteamFields::SteamOutletNodeName, node_->nameString());
    }

    // Nominal Capacity
    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(DistrictHeating_SteamFields::NominalCapacity, "Autosize");
    } else if (auto value_ = modelObject.nominalCapacity()) {
      idfObject.setDouble(DistrictHeating_SteamFields::NominalCapacity, *value_);
    }

    // Capacity Fraction Schedule Name
    if (auto capacityFractionSchedule_ = modelObject.capacityFractionSchedule()) {
      if (auto sch_ = translateAndMapModelObject(*capacityFractionSchedule_)) {
        idfObject.setString(DistrictHeating_SteamFields::CapacityFractionScheduleName, sch_->nameString());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
