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

#include "../../model/CoilCoolingDX.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDX(model::CoilCoolingDX& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // PerformanceObjectName, is required, so start by that
    CoilCoolingDXCurveFitPerformance performance = modelObject.performanceObject();
    if (boost::optional<IdfObject> _performance = translateAndMapModelObject(performance)) {
      s = _performance->name().get();
    } else {
      LOG(Warn, modelObject.briefDescription() << " cannot be translated as its performance object cannot be translated: "
                                               << performance.briefDescription() << ".");
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Cooling_DX, modelObject);

    // PerformanceObjectName
    idfObject.setString(Coil_Cooling_DXFields::PerformanceObjectName, s.get());

    // Evaporator Nodes are handled in the FT for AirLoopHVACUnitarySystem
    // EvaporatorInletNodeName
    // EvaporatorOutletNodeName

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(Coil_Cooling_DXFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // CondenserZoneName: as of E+ 9.3.0, this appears unused.
    // TODO: eventually handle the condenser inlet/outlet node connections if the Condenser Zone is used?
    if (boost::optional<ThermalZone> thermalZone = modelObject.condenserZone()) {
      if (boost::optional<IdfObject> _thermalZone = translateAndMapModelObject(thermalZone.get())) {
        idfObject.setString(Coil_Cooling_DXFields::CondenserZoneName, _thermalZone->name().get());
      }
    }

    // CondenserInletNodeName
    std::string condenserInletNodeName(modelObject.nameString() + " Condenser Inlet Node");
    // if (auto _s = modelObject.condenserInletNodeName()) {
    //   condenserInletNodeName =  _s.get();
    // } else {
    // Create an OutdoorAir:NodeList for the condenser inlet conditions to be set directly from weather file
    IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
    oaNodeListIdf.setString(0, condenserInletNodeName);
    m_idfObjects.push_back(oaNodeListIdf);
    // }
    idfObject.setString(Coil_Cooling_DXFields::CondenserInletNodeName, condenserInletNodeName);

    // CondenserOutletNodeName
    std::string condenserOutletNodeName(modelObject.nameString() + " Condenser Outlet Node");
    //if (auto _s = modelObject.condenserOutletNodeName()) {
    //condenserOutletNodeName =  _s.get();
    //}
    idfObject.setString(Coil_Cooling_DXFields::CondenserOutletNodeName, condenserOutletNodeName);

    // CondensateCollectionWaterStorageTankName

    // EvaporativeCondenserSupplyWaterStorageTankName

    return boost::optional<IdfObject>(idfObject);
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
