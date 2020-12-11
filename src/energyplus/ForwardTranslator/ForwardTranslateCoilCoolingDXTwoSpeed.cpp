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
#include "../../model/CoilCoolingDXTwoSpeed.hpp"
#include "../../model/CoilCoolingDXTwoSpeed_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXTwoSpeedWithoutUnitary(model::CoilCoolingDXTwoSpeed& modelObject) {
    //setup two boost optionals to use to store get method returns
    boost::optional<std::string> s;
    boost::optional<double> d;

    //create the IdfObject that will be the coil
    IdfObject idfObject(IddObjectType::Coil_Cooling_DX_TwoSpeed);

    //Name
    m_idfObjects.push_back(idfObject);
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    //  A2 , \field Availability Schedule Name
    Schedule sched = modelObject.getAvailabilitySchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::AvailabilityScheduleName, sched.name().get());

    //  N1 , \field Rated High Speed Total Cooling Capacity
    d = modelObject.getRatedHighSpeedTotalCoolingCapacity();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedTotalCoolingCapacity, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedTotalCoolingCapacity, "Autosize");
    }

    //  N2 , \field Rated High Speed Sensible Heat Ratio
    d = modelObject.getRatedHighSpeedSensibleHeatRatio();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedSensibleHeatRatio, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedSensibleHeatRatio, "Autosize");
    }

    //  N3 , \field Rated High Speed COP
    d = modelObject.getRatedHighSpeedCOP();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedCoolingCOP, *d);
    }

    //  N4 , \field Rated High Speed Air Flow Rate
    d = modelObject.getRatedHighSpeedAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedAirFlowRate, "Autosize");
    }

    // Unit Internal Static Air Pressure
    {
      auto value = modelObject.unitInternalStaticAirPressure();
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::UnitInternalStaticAirPressure, value);
    }

    //A3 , \field Air Inlet Node Name
    OptionalModelObject omo = modelObject.inletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName, *s);
      }
    }

    //A4 , \field Air Outlet Node Name
    omo = modelObject.outletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName, *s);
      }
    }

    //  A5 , \field Total Cooling Capacity Function of Temperature Curve Name
    Curve cb = modelObject.getTotalCoolingCapacityFunctionOfTemperatureCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName, cb.name().get());

    //  A6 , \field Total Cooling Capacity Function of Flow Fraction Curve Name
    cb = modelObject.getTotalCoolingCapacityFunctionOfFlowFractionCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName, cb.name().get());

    //  A7 , \field Energy Input Ratio Function of Temperature Curve Name
    cb = modelObject.getEnergyInputRatioFunctionOfTemperatureCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName, cb.name().get());

    //  A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
    Curve cq = modelObject.getEnergyInputRatioFunctionOfFlowFractionCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName, cq.name().get());

    //  A9 , \field Part Load Fraction Correlation Curve Name
    cq = modelObject.getPartLoadFractionCorrelationCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::PartLoadFractionCorrelationCurveName, cq.name().get());

    //  N5 , \field Rated Low Speed Total Cooling Capacity
    d = modelObject.getRatedLowSpeedTotalCoolingCapacity();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedTotalCoolingCapacity, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedTotalCoolingCapacity, "Autosize");
    }

    //  N6 , \field Rated Low Speed Sensible Heat Ratio
    d = modelObject.getRatedLowSpeedSensibleHeatRatio();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedSensibleHeatRatio, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedSensibleHeatRatio, "Autosize");
    }

    //  N7 , \field Rated Low Speed COP
    d = modelObject.getRatedLowSpeedCOP();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedCoolingCOP, *d);
    }

    //  N8 , \field Rated Low Speed Air Flow Rate
    d = modelObject.getRatedLowSpeedAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedRatedAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedRatedAirFlowRate, "Autosize");
    }

    //  A10, \field Low Speed Total Cooling Capacity Function of Temperature Curve Name
    cq = modelObject.getLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedTotalCoolingCapacityFunctionofTemperatureCurveName, cq.name().get());

    //  A11, \field Low Speed Energy Input Ratio Function of Temperature Curve Name
    cq = modelObject.getLowSpeedEnergyInputRatioFunctionOfTemperatureCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEnergyInputRatioFunctionofTemperatureCurveName, cq.name().get());

    //  A12, \field Condenser Air Inlet Node Name
    s = modelObject.getCondenserAirInletNodeName();
    if (s) {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::CondenserAirInletNodeName, *s);
    }

    //  A13, \field Condenser Type
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::CondenserType, modelObject.getCondenserType());

    // Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
    {
      auto value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation();
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, value);
    }

    //   N9, \field High Speed Evaporative Condenser Effectiveness
    d = modelObject.getHighSpeedEvaporativeCondenserEffectiveness();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserEffectiveness, *d);
    }

    //  N10, \field High Speed Evaporative Condenser Air Flow Rate
    d = modelObject.getHighSpeedEvaporativeCondenserAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate, "Autosize");
    }

    //  N11, \field High Speed Evaporative Condenser Pump Rated Power Consumption
    d = modelObject.getHighSpeedEvaporativeCondenserPumpRatedPowerConsumption();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    }

    //  N12, \field Low Speed Evaporative Condenser Effectiveness
    d = modelObject.getLowSpeedEvaporativeCondenserEffectiveness();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserEffectiveness, *d);
    }

    //  N13, \field Low Speed Evaporative Condenser Air Flow Rate
    d = modelObject.getLowSpeedEvaporativeCondenserAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate, "Autosize");
    }

    //  N14, \field Low Speed Evaporative Condenser Pump Rated Power Consumption
    d = modelObject.getLowSpeedEvaporativeCondenserPumpRatedPowerConsumption();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    }

    //TODO
    //  A14, \field Supply Water Storage Tank Name
    //getSupplyWaterStorageTankName

    //TODO
    //  A15, \field Condensate Collection Water Storage Tank Name
    //getCondensateCollectionWaterStorageTankName

    //  N15, \field Basin Heater Capacity
    d = modelObject.getBasinHeaterCapacity();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterCapacity, *d);
    }

    //  N16, \field Basin Heater Setpoint Temperature
    d = modelObject.getBasinHeaterSetpointTemperature();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterSetpointTemperature, *d);
    }

    //  A16; \field Basin Heater Operating Schedule Name
    OptionalSchedule os = modelObject.getBasinHeaterOperatingSchedule();
    if (os) {
      translateAndMapModelObject(*os);
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterOperatingScheduleName, os->name().get());
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXTwoSpeed(CoilCoolingDXTwoSpeed& modelObject) {
    IdfObject coilSystemCoolingDXIdf(IddObjectType::CoilSystem_Cooling_DX);

    m_idfObjects.push_back(coilSystemCoolingDXIdf);

    boost::optional<IdfObject> oIdfObject = translateCoilCoolingDXTwoSpeedWithoutUnitary(modelObject);

    if (!oIdfObject) {
      return boost::none;
    }

    IdfObject idfObject = oIdfObject.get();

    OptionalString s;

    s = modelObject.name();
    if (s) {
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType, idfObject.iddObject().name());

      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilName, *s);

      coilSystemCoolingDXIdf.setName(*s + " CoilSystem");
    }

    Schedule sched = modelObject.getAvailabilitySchedule();
    translateAndMapModelObject(sched);

    coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::AvailabilityScheduleName, sched.name().get());

    OptionalModelObject omo = modelObject.inletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemInletNodeName, *s);
      }
    }

    omo = modelObject.outletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemOutletNodeName, *s);

        coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemSensorNodeName, *s);
      }
    }

    return coilSystemCoolingDXIdf;
  }

}  // namespace energyplus

}  // namespace openstudio
