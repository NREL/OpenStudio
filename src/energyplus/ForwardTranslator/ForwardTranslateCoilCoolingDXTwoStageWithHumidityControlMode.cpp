/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"
#include "../../model/CoilPerformanceDXCooling.hpp"
#include "../../model/CoilPerformanceDXCooling_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoStageWithHumidityControlMode_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXTwoStageWithHumidityControlModeWithoutUnitary( model::CoilCoolingDXTwoStageWithHumidityControlMode & modelObject )
{
  //create the IdfObject that will be the coil
  IdfObject idfObject(IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode);

  //Name
  m_idfObjects.push_back(idfObject);
  if( auto value = modelObject.name() ) {
    idfObject.setName(value.get());
  }

  // AirInletNodeName
  if( auto mo = modelObject.inletModelObject() ) {
    if( auto node = mo->optionalCast<model::Node>() ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirInletNodeName,node->name().get());
    }
  }

  // AirOutletNodeName
  if( auto mo = modelObject.outletModelObject() ) {
    if( auto node = mo->optionalCast<model::Node>() ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirOutletNodeName,node->name().get());
    }
  }

  // AvailabilityScheduleName
  if( auto schedule = modelObject.availabilitySchedule() ) {
    if( auto idf = translateAndMapModelObject(schedule.get()) ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AvailabilityScheduleName,idf->name().get());
    }
  }

  // CrankcaseHeaterCapacity
  {
    auto value = modelObject.crankcaseHeaterCapacity();
    idfObject.setDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::CrankcaseHeaterCapacity,value);
  }

  // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
  {
    auto value = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();
    idfObject.setDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,value);
  }

  // NumberofCapacityStages
  {
    auto value = modelObject.numberofCapacityStages();
    idfObject.setInt(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofCapacityStages,value);
  }

  // NumberofEnhancedDehumidificationModes
  {
    auto value = modelObject.numberofEnhancedDehumidificationModes();
    idfObject.setInt(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NumberofEnhancedDehumidificationModes,value);
  }

  // NormalModeStage1CoilPerformanceName
  if( auto curve = modelObject.normalModeStage1CoilPerformance() ) {
    if( auto idf = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1CoilPerformanceObjectType,idf->iddObject().name());
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1CoilPerformanceName,idf->name().get());
    }
  }

  // NormalModeStage1_PLUS_2CoilPerformanceName
  if( auto curve = modelObject.normalModeStage1Plus2CoilPerformance() ) {
    if( auto idf = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1_PLUS_2CoilPerformanceObjectType,idf->iddObject().name());
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::NormalModeStage1_PLUS_2CoilPerformanceName,idf->name().get());
    }
  }

  // DehumidificationMode1Stage1CoilPerformanceName
  if( auto curve = modelObject.dehumidificationMode1Stage1CoilPerformance() ) {
    if( auto idf = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1CoilPerformanceObjectType,idf->iddObject().name());
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1CoilPerformanceName,idf->name().get());
    }
  }

  // DehumidificationMode1Stage1_PLUS_2CoilPerformanceName
  if( auto curve = modelObject.dehumidificationMode1Stage1Plus2CoilPerformance() ) {
    if( auto idf = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1_PLUS_2CoilPerformanceObjectType,idf->iddObject().name());
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::DehumidificationMode1Stage1_PLUS_2CoilPerformanceName,idf->name().get());
    }
  }

  // BasinHeaterCapacity
  {
    auto value = modelObject.basinHeaterCapacity();
    idfObject.setDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterCapacity,value);
  }

  // BasinHeaterSetpointTemperature
  {
    auto value = modelObject.basinHeaterSetpointTemperature();
    idfObject.setDouble(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterSetpointTemperature,value);
  }

  // BasinHeaterOperatingScheduleName
  if( auto schedule = modelObject.basinHeaterOperatingSchedule() ) {
    if( auto idf = translateAndMapModelObject(schedule.get()) ) {
      idfObject.setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::BasinHeaterOperatingScheduleName,idf->name().get());
    }
  }

  return idfObject;
}

boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXTwoStageWithHumidityControlMode( model::CoilCoolingDXTwoStageWithHumidityControlMode & modelObject )
{
  IdfObject coilSystemCoolingDXIdf(IddObjectType::CoilSystem_Cooling_DX);

  m_idfObjects.push_back(coilSystemCoolingDXIdf);

  boost::optional<IdfObject> oIdfObject = translateCoilCoolingDXTwoStageWithHumidityControlModeWithoutUnitary(modelObject);

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

  if( auto sched = modelObject.availabilitySchedule() ) {
    if( auto _sched = translateAndMapModelObject(sched.get()) ) {
      coilSystemCoolingDXIdf.setString(CoilSystem_Cooling_DXFields::AvailabilityScheduleName,_sched->name().get());
    }
  }

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

