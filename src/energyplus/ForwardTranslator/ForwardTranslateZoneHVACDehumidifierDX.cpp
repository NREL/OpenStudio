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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/ZoneHVACDehumidifierDX.hpp"
#include "../../model/ZoneHVACDehumidifierDX_Impl.hpp"
#include <utilities/idd/ZoneHVAC_Dehumidifier_DX_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACDehumidifierDX(ZoneHVACDehumidifierDX& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_Dehumidifier_DX, modelObject);

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName
    if (auto node = modelObject.inletNode()) {
      idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::AirInletNodeName, node->name().get());
    }

    // AirOutletNodeName
    if (auto node = modelObject.outletNode()) {
      idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::AirOutletNodeName, node->name().get());
    }

    // RatedWaterRemoval
    if ((value = modelObject.ratedWaterRemoval())) {
      idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::RatedWaterRemoval, value.get());
    }

    // RatedEnergyFactor
    if ((value = modelObject.ratedEnergyFactor())) {
      idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::RatedEnergyFactor, value.get());
    }

    // RatedAirFlowRate
    if ((value = modelObject.ratedAirFlowRate())) {
      idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::RatedAirFlowRate, value.get());
    }

    // WaterRemovalCurveName
    {
      auto curve = modelObject.waterRemovalCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::WaterRemovalCurveName, _curve->name().get());
      }
    }

    // EnergyFactorCurveName
    {
      auto curve = modelObject.energyFactorCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::EnergyFactorCurveName, _curve->name().get());
      }
    }

    // PartLoadFractionCorrelationCurveName
    {
      auto curve = modelObject.partLoadFractionCorrelationCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(ZoneHVAC_Dehumidifier_DXFields::PartLoadFractionCorrelationCurveName, _curve->name().get());
      }
    }

    // MinimumDry-BulbTemperatureforDehumidifierOperation
    if ((value = modelObject.minimumDryBulbTemperatureforDehumidifierOperation())) {
      idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::MinimumDryBulbTemperatureforDehumidifierOperation, value.get());
    }

    // MaximumDry-BulbTemperatureforDehumidifierOperation
    if ((value = modelObject.maximumDryBulbTemperatureforDehumidifierOperation())) {
      idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::MaximumDryBulbTemperatureforDehumidifierOperation, value.get());
    }

    // Off-CycleParasiticElectricLoad
    if ((value = modelObject.offCycleParasiticElectricLoad())) {
      idfObject.setDouble(ZoneHVAC_Dehumidifier_DXFields::OffCycleParasiticElectricLoad, value.get());
    }

    // CondensateCollectionWaterStorageTankName

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
