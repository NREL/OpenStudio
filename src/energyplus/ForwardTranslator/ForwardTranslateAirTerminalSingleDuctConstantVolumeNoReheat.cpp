/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_NoReheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAirTerminalSingleDuctConstantVolumeNoReheat(AirTerminalSingleDuctConstantVolumeNoReheat& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<double> value;
    std::string s;

    std::string baseName = modelObject.name().get();

    IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
    _airDistributionUnit.setName("ADU " + baseName);  //ADU: Air Distribution Unit

    IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_NoReheat);
    idfObject.setName(baseName);

    // hook up required objects
    try {
      idfObject.setString(openstudio::AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AvailabilityScheduleName,
                          modelObject.availabilitySchedule().name().get());
    } catch (std::exception& e) {
      LOG(Error, "Could not translate " << modelObject.briefDescription() << ", because " << e.what() << ".");
      return boost::none;
    }

    // Register the ATU and the ADU
    m_idfObjects.push_back(idfObject);
    m_idfObjects.push_back(_airDistributionUnit);

    boost::optional<std::string> inletNodeName;
    boost::optional<std::string> outletNodeName;

    if (boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject()) {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>()) {
        inletNodeName = inletNode->name().get();
      }
    }

    if (boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject()) {
      if (boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>()) {
        outletNodeName = outletNode->name().get();
      }
    }

    if (inletNodeName && outletNodeName) {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AirInletNodeName, inletNodeName.get());
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::AirOutletNodeName, outletNodeName.get());
    }

    if (modelObject.isMaximumAirFlowRateAutosized()) {
      idfObject.setString(openstudio::AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate, "AutoSize");
    } else if ((value = modelObject.maximumAirFlowRate())) {
      idfObject.setDouble(openstudio::AirTerminal_SingleDuct_ConstantVolume_NoReheatFields::MaximumAirFlowRate, value.get());
    }

    // Populate fields for AirDistributionUnit
    if (outletNodeName) {
      _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName, outletNodeName.get());
    }
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType, idfObject.iddObject().name());
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName, idfObject.name().get());

    // We return the ADU
    return _airDistributionUnit;
  }

}  // namespace energyplus

}  // namespace openstudio
