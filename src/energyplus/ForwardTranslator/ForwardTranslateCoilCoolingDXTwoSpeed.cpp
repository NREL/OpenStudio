/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
    Schedule sched = modelObject.availabilitySchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::AvailabilityScheduleName, sched.name().get());

    //  N1 , \field Rated High Speed Total Cooling Capacity
    d = modelObject.ratedHighSpeedTotalCoolingCapacity();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedTotalCoolingCapacity, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedTotalCoolingCapacity, "Autosize");
    }

    //  N2 , \field Rated High Speed Sensible Heat Ratio
    d = modelObject.ratedHighSpeedSensibleHeatRatio();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedSensibleHeatRatio, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedRatedSensibleHeatRatio, "Autosize");
    }

    //  N3 , \field Rated High Speed COP
    d = modelObject.ratedHighSpeedCOP();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedGrossRatedCoolingCOP, *d);
    }

    //  N4 , \field Rated High Speed Air Flow Rate
    d = modelObject.ratedHighSpeedAirFlowRate();
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
    Curve cb = modelObject.totalCoolingCapacityFunctionOfTemperatureCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName, cb.name().get());

    //  A6 , \field Total Cooling Capacity Function of Flow Fraction Curve Name
    cb = modelObject.totalCoolingCapacityFunctionOfFlowFractionCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName, cb.name().get());

    //  A7 , \field Energy Input Ratio Function of Temperature Curve Name
    cb = modelObject.energyInputRatioFunctionOfTemperatureCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName, cb.name().get());

    //  A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
    Curve cq = modelObject.energyInputRatioFunctionOfFlowFractionCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName, cq.name().get());

    //  A9 , \field Part Load Fraction Correlation Curve Name
    cq = modelObject.partLoadFractionCorrelationCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::PartLoadFractionCorrelationCurveName, cq.name().get());

    //  N5 , \field Rated Low Speed Total Cooling Capacity
    d = modelObject.ratedLowSpeedTotalCoolingCapacity();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedTotalCoolingCapacity, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedTotalCoolingCapacity, "Autosize");
    }

    //  N6 , \field Rated Low Speed Sensible Heat Ratio
    d = modelObject.ratedLowSpeedSensibleHeatRatio();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedSensibleHeatRatio, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedSensibleHeatRatio, "Autosize");
    }

    //  N7 , \field Rated Low Speed COP
    d = modelObject.ratedLowSpeedCOP();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedGrossRatedCoolingCOP, *d);
    }

    //  N8 , \field Rated Low Speed Air Flow Rate
    d = modelObject.ratedLowSpeedAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedRatedAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedRatedAirFlowRate, "Autosize");
    }

    //  A10, \field Low Speed Total Cooling Capacity Function of Temperature Curve Name
    cq = modelObject.lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedTotalCoolingCapacityFunctionofTemperatureCurveName, cq.name().get());

    //  A11, \field Low Speed Energy Input Ratio Function of Temperature Curve Name
    cq = modelObject.lowSpeedEnergyInputRatioFunctionOfTemperatureCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEnergyInputRatioFunctionofTemperatureCurveName, cq.name().get());

    //  A12, \field Condenser Air Inlet Node Name
    s = modelObject.condenserAirInletNodeName();
    if (s) {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::CondenserAirInletNodeName, *s);
    }

    //  A13, \field Condenser Type
    idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::CondenserType, modelObject.condenserType());

    // Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
    {
      auto value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation();
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, value);
    }

    //   N9, \field High Speed Evaporative Condenser Effectiveness
    d = modelObject.highSpeedEvaporativeCondenserEffectiveness();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserEffectiveness, *d);
    }

    //  N10, \field High Speed Evaporative Condenser Air Flow Rate
    d = modelObject.highSpeedEvaporativeCondenserAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserAirFlowRate, "Autosize");
    }

    //  N11, \field High Speed Evaporative Condenser Pump Rated Power Consumption
    d = modelObject.highSpeedEvaporativeCondenserPumpRatedPowerConsumption();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::HighSpeedEvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    }

    //  N12, \field Low Speed Evaporative Condenser Effectiveness
    d = modelObject.lowSpeedEvaporativeCondenserEffectiveness();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserEffectiveness, *d);
    }

    //  N13, \field Low Speed Evaporative Condenser Air Flow Rate
    d = modelObject.lowSpeedEvaporativeCondenserAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_TwoSpeedFields::LowSpeedEvaporativeCondenserAirFlowRate, "Autosize");
    }

    //  N14, \field Low Speed Evaporative Condenser Pump Rated Power Consumption
    d = modelObject.lowSpeedEvaporativeCondenserPumpRatedPowerConsumption();
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
    d = modelObject.basinHeaterCapacity();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterCapacity, *d);
    }

    //  N16, \field Basin Heater Setpoint Temperature
    d = modelObject.basinHeaterSetpointTemperature();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_TwoSpeedFields::BasinHeaterSetpointTemperature, *d);
    }

    //  A16; \field Basin Heater Operating Schedule Name
    OptionalSchedule os = modelObject.basinHeaterOperatingSchedule();
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

    Schedule sched = modelObject.availabilitySchedule();
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
