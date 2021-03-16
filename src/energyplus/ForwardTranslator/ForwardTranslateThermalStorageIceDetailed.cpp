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
#include "../../model/ThermalStorageIceDetailed.hpp"
#include "../../model/ThermalStorageIceDetailed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/ThermalStorage_Ice_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateThermalStorageIceDetailed(ThermalStorageIceDetailed& modelObject) {
    IdfObject idfObject(IddObjectType::ThermalStorage_Ice_Detailed);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(ThermalStorage_Ice_DetailedFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(ThermalStorage_Ice_DetailedFields::OutletNodeName, node->name().get());
      }
    }

    // AvailabilityScheduleName
    if (auto schedule = modelObject.availabilitySchedule()) {
      idfObject.setString(ThermalStorage_Ice_DetailedFields::AvailabilityScheduleName, schedule->name().get());
    }

    // Capacity
    {
      auto value = modelObject.capacity();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::Capacity, value);
    }

    // DischargingCurveObjectType
    // DischargingCurveName
    {
      auto curve = modelObject.dischargingCurve();
      auto idf = translateAndMapModelObject(curve);
      OS_ASSERT(idf);

      idfObject.setString(ThermalStorage_Ice_DetailedFields::DischargingCurveVariableSpecifications,
                          modelObject.dischargingCurveVariableSpecifications());
      idfObject.setString(ThermalStorage_Ice_DetailedFields::DischargingCurveName, idf->name().get());
    }

    // ChargingCurveObjectType
    // ChargingCurveName
    {
      auto curve = modelObject.chargingCurve();
      auto idf = translateAndMapModelObject(curve);
      OS_ASSERT(idf);

      idfObject.setString(ThermalStorage_Ice_DetailedFields::ChargingCurveVariableSpecifications, modelObject.chargingCurveVariableSpecifications());
      idfObject.setString(ThermalStorage_Ice_DetailedFields::ChargingCurveName, idf->name().get());
    }

    // TimestepoftheCurveData
    {
      auto value = modelObject.timestepoftheCurveData();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::TimestepoftheCurveData, value);
    }

    // ParasiticElectricLoadDuringDischarging
    {
      auto value = modelObject.parasiticElectricLoadDuringDischarging();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringDischarging, value);
    }

    // ParasiticElectricLoadDuringCharging
    {
      auto value = modelObject.parasiticElectricLoadDuringCharging();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::ParasiticElectricLoadDuringCharging, value);
    }

    // TankLossCoefficient
    {
      auto value = modelObject.tankLossCoefficient();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::TankLossCoefficient, value);
    }

    // FreezingTemperatureofStorageMedium
    {
      auto value = modelObject.freezingTemperatureofStorageMedium();
      idfObject.setDouble(ThermalStorage_Ice_DetailedFields::FreezingTemperatureofStorageMedium, value);
    }

    // ThawProcessIndicator
    {
      auto value = modelObject.thawProcessIndicator();
      idfObject.setString(ThermalStorage_Ice_DetailedFields::ThawProcessIndicator, value);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
