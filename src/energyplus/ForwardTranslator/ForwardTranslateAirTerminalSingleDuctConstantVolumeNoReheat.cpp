/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
