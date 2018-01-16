/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXSingleSpeedWithoutUnitary( model::CoilCoolingDXSingleSpeed & modelObject )
{
  OptionalString s;
  IdfObject idfObject(IddObjectType::Coil_Cooling_DX_SingleSpeed);

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // hook up required objects
  try {
    Schedule sched = modelObject.getAvailabilitySchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName,
                        sched.name().get() );

    Curve cb =  modelObject.getTotalCoolingCapacityFunctionOfTemperatureCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName,
                       cb.name().get());

    Curve cq = modelObject.getTotalCoolingCapacityFunctionOfFlowFractionCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName,
                        cq.name().get());

    cb =modelObject.getEnergyInputRatioFunctionOfTemperatureCurve();
    translateAndMapModelObject(cb);
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName,
                        cb.name().get());

    cq=modelObject.getEnergyInputRatioFunctionOfFlowFractionCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName,
                        cq.name().get());

    cq=modelObject.getPartLoadFractionCorrelationCurve();
    translateAndMapModelObject(cq);
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName,
                        cq.name().get());
  }
  catch (std::exception& e) {
    LOG(Error,"Could not translate " << modelObject.briefDescription() << ", because "
        << e.what() << ".");
    return boost::none;
  }

  OptionalDouble d = modelObject.ratedTotalCoolingCapacity();
  if( d )
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedTotalCoolingCapacity,*d);
  }
  else
  {
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::GrossRatedTotalCoolingCapacity,"Autosize");
  }

  d = modelObject.ratedSensibleHeatRatio();
  if( d )
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedSensibleHeatRatio,*d);
  }
  else
  {
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::GrossRatedSensibleHeatRatio,"Autosize");
  }

  d = modelObject.getRatedCOP();
  if( d )
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedCoolingCOP,*d);
  }

  d = modelObject.ratedAirFlowRate();
  if( d )
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate,*d);
  }
  else
  {
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate,"Autosize");
  }

  d = modelObject.getRatedEvaporatorFanPowerPerVolumeFlowRate();
  if( d )
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate,*d);
  }

  OptionalModelObject omo = modelObject.inletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,*s );
    }
  }

  omo= modelObject.outletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,*s);
    }
  }

  d=modelObject.getNominalTimeForCondensateRemovalToBegin();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin,*d);
  }

  d=modelObject.getRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,*d);
  }

  d=modelObject.getMaximumCyclingRate();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate,*d);
  }

  d=modelObject.getLatentCapacityTimeConstant();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant,*d);
  }

  s=modelObject.getCondenserAirInletNodeName();
  if(s)
  {
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName,*s);
  }

  idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::CondenserType,modelObject.getCondenserType());

  d=modelObject.getEvaporativeCondenserEffectiveness();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness,*d);
  }

  d=modelObject.getEvaporativeCondenserAirFlowRate();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate,*d);
  }
  else {
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate, "Autosize");
  }

  d=modelObject.getEvaporativeCondenserPumpRatedPowerConsumption();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption,*d);
  }
  else {
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
  }

  d=modelObject.getCrankcaseHeaterCapacity();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity,*d);
  }

  d=modelObject.getMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,*d);
  }

  //TODO
  //getSupplyWaterStorageTankName
  //getCondensateCollectionWaterStorageTankName

  d=modelObject.getBasinHeaterCapacity();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterCapacity,*d);
  }

  d=modelObject.getBasinHeaterSetpointTemperature();
  if(d)
  {
    idfObject.setDouble(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterSetpointTemperature,*d);
  }

  OptionalSchedule os = modelObject.getBasinHeaterOperatingSchedule();
  if( os )
  {
    translateAndMapModelObject(*os);
    idfObject.setString(Coil_Cooling_DX_SingleSpeedFields::BasinHeaterOperatingScheduleName,
                        os->name().get() );
  }

  m_idfObjects.push_back(idfObject);
  return idfObject;
}

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXSingleSpeed( CoilCoolingDXSingleSpeed& modelObject )
{
  IdfObject coilSystemCoolingDXIdf(IddObjectType::CoilSystem_Cooling_DX);

  m_idfObjects.push_back(coilSystemCoolingDXIdf);

  boost::optional<IdfObject> oIdfObject = translateCoilCoolingDXSingleSpeedWithoutUnitary(modelObject);

  if( ! oIdfObject ) { return boost::none; }

  IdfObject idfObject = oIdfObject.get();

  OptionalString s;

  s = modelObject.name();
  if( s )
  {
    coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType,idfObject.iddObject().name());

    coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::CoolingCoilName,*s);

    coilSystemCoolingDXIdf.setName(*s + " CoilSystem");
  }

  Schedule sched = modelObject.getAvailabilitySchedule();
  translateAndMapModelObject(sched);

  coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::AvailabilityScheduleName,sched.name().get());

  OptionalModelObject omo = modelObject.inletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemInletNodeName,*s);
    }
  }

  omo= modelObject.outletModelObject();
  if( omo )
  {
    translateAndMapModelObject(*omo);
    s = omo->name();
    if(s)
    {
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemOutletNodeName,*s);

      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::DXCoolingCoilSystemSensorNodeName,*s);
    }
  }

  return coilSystemCoolingDXIdf;
}

} // energyplus

} // openstudio

