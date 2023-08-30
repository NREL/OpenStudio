/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalDualDuctConstantVolume.hpp"
#include "../../model/AirTerminalDualDuctConstantVolume_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/AirTerminal_DualDuct_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirTerminalDualDuctConstantVolume(AirTerminalDualDuctConstantVolume& modelObject) {

    std::string baseName = modelObject.name().get();

    IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
    _airDistributionUnit.setName("ADU " + baseName);  //ADU: Air Distribution Unit

    IdfObject idfObject(openstudio::IddObjectType::AirTerminal_DualDuct_ConstantVolume);
    idfObject.setName(baseName);

    m_idfObjects.push_back(_airDistributionUnit);
    m_idfObjects.push_back(idfObject);

    {
      auto schedule = modelObject.availabilitySchedule();
      if (auto idf = translateAndMapModelObject(schedule)) {
        idfObject.setString(AirTerminal_DualDuct_ConstantVolumeFields::AvailabilityScheduleName, idf->name().get());
      }
    }

    if (auto mo = modelObject.outletModelObject()) {
      idfObject.setString(AirTerminal_DualDuct_ConstantVolumeFields::AirOutletNodeName, mo->name().get());
    }

    if (auto mo = modelObject.inletModelObject(0)) {
      idfObject.setString(AirTerminal_DualDuct_ConstantVolumeFields::HotAirInletNodeName, mo->name().get());
    }

    if (auto mo = modelObject.inletModelObject(1)) {
      idfObject.setString(AirTerminal_DualDuct_ConstantVolumeFields::ColdAirInletNodeName, mo->name().get());
    }

    if (modelObject.isMaximumAirFlowRateAutosized()) {
      idfObject.setString(AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate, "Autosize");
    } else if (auto value = modelObject.maximumAirFlowRate()) {
      idfObject.setDouble(AirTerminal_DualDuct_ConstantVolumeFields::MaximumAirFlowRate, value.get());
    }

    // Populate fields for AirDistributionUnit
    if (boost::optional<ModelObject> outletNode = modelObject.outletModelObject()) {
      _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName, outletNode->name().get());
    }
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType, idfObject.iddObject().name());
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName, idfObject.name().get());

    return _airDistributionUnit;
  }

}  // namespace energyplus

}  // namespace openstudio
