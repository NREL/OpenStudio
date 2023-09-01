/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXSingleSpeedWithoutUnitary(model::CoilCoolingDXSingleSpeed& modelObject) {
    OptionalString s;
    IdfObject idfObject(IddObjectType::Coil_Cooling_DX_SingleSpeed);

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // hook up required objects
    try {
      Schedule sched = modelObject.availabilitySchedule();
      translateAndMapModelObject(sched);
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName, sched.name().get());

      Curve cb = modelObject.totalCoolingCapacityFunctionOfTemperatureCurve();
      translateAndMapModelObject(cb);
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName, cb.name().get());

      Curve cq = modelObject.totalCoolingCapacityFunctionOfFlowFractionCurve();
      translateAndMapModelObject(cq);
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName, cq.name().get());

      cb = modelObject.energyInputRatioFunctionOfTemperatureCurve();
      translateAndMapModelObject(cb);
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName, cb.name().get());

      cq = modelObject.energyInputRatioFunctionOfFlowFractionCurve();
      translateAndMapModelObject(cq);
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName, cq.name().get());

      cq = modelObject.partLoadFractionCorrelationCurve();
      translateAndMapModelObject(cq);
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName, cq.name().get());
    } catch (std::exception& e) {
      LOG(Error, "Could not translate " << modelObject.briefDescription() << ", because " << e.what() << ".");
      return boost::none;
    }

    OptionalDouble d = modelObject.ratedTotalCoolingCapacity();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedTotalCoolingCapacity, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::GrossRatedTotalCoolingCapacity, "Autosize");
    }

    d = modelObject.ratedSensibleHeatRatio();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedSensibleHeatRatio, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::GrossRatedSensibleHeatRatio, "Autosize");
    }

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedCoolingCOP, modelObject.ratedCOP());

    d = modelObject.ratedAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate, "Autosize");
    }

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2017,
                        modelObject.ratedEvaporatorFanPowerPerVolumeFlowRate2017());

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2023,
                        modelObject.ratedEvaporatorFanPowerPerVolumeFlowRate2023());

    OptionalModelObject omo = modelObject.inletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName, *s);
      }
    }

    omo = modelObject.outletModelObject();
    if (omo) {
      translateAndMapModelObject(*omo);
      s = omo->name();
      if (s) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName, *s);
      }
    }

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin,
                        modelObject.nominalTimeForCondensateRemovalToBegin());

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                        modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation());

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,
                        modelObject.ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity());

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate, modelObject.maximumCyclingRate());

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant, modelObject.latentCapacityTimeConstant());

    s = modelObject.condenserAirInletNodeName();
    if (s) {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName, *s);
    }

    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::CondenserType, modelObject.condenserType());

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness, modelObject.evaporativeCondenserEffectiveness());

    d = modelObject.evaporativeCondenserAirFlowRate();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, "Autosize");
    }

    d = modelObject.evaporativeCondenserPumpRatedPowerConsumption();
    if (d) {
      idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, *d);
    } else {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    }

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity, modelObject.crankcaseHeaterCapacity());

    if (auto crankCurve_ = modelObject.crankcaseHeaterCapacityFunctionofTemperatureCurve()) {
      if (auto curve_ = translateAndMapModelObject(*crankCurve_)) {
        idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName, curve_->nameString());
      }
    }

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation());

    //TODO
    //getSupplyWaterStorageTankName
    //getCondensateCollectionWaterStorageTankName

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity, modelObject.basinHeaterCapacity());

    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature, modelObject.basinHeaterSetpointTemperature());

    OptionalSchedule os = modelObject.basinHeaterOperatingSchedule();
    if (os) {
      translateAndMapModelObject(*os);
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName, os->name().get());
    }

    m_idfObjects.push_back(idfObject);
    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXSingleSpeed(CoilCoolingDXSingleSpeed& modelObject) {
    IdfObject coilSystemCoolingDXIdf(IddObjectType::CoilSystem_Cooling_DX);

    m_idfObjects.push_back(coilSystemCoolingDXIdf);

    boost::optional<IdfObject> oIdfObject = translateCoilCoolingDXSingleSpeedWithoutUnitary(modelObject);

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
