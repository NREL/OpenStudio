/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingGasMultiStage_Impl.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingGasMultiStage( CoilHeatingGasMultiStage & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_Gas_MultiStage, modelObject);

  // AvailabilityScheduleName
  if( auto schedule = modelObject.availabilitySchedule() ) {
    if( auto _schedule = translateAndMapModelObject(schedule.get()) ) {
      idfObject.setString(Coil_Heating_Gas_MultiStageFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName
  if( auto node = modelObject.inletModelObject() ) {
    idfObject.setString(Coil_Heating_Gas_MultiStageFields::AirInletNodeName,node->name().get());
  }  

  // AirOutletNodeName
  // TemperatureSetpointNodeName
  if( auto node = modelObject.outletModelObject() ) {
    idfObject.setString(Coil_Heating_Gas_MultiStageFields::AirOutletNodeName,node->name().get());
    idfObject.setString(Coil_Heating_Gas_MultiStageFields::TemperatureSetpointNodeName,node->name().get());
  }

  // PartLoadFractionCorrelationCurveName
  if( auto curve = modelObject.partLoadFractionCorrelationCurve() ) {
    if( auto _curve = translateAndMapModelObject(curve.get()) ) {
      idfObject.setString(Coil_Heating_Gas_MultiStageFields::PartLoadFractionCorrelationCurveName,_curve->name().get());
    }
  }

  // ParasiticGasLoad
  if( (value = modelObject.parasiticGasLoad()) ) {
    idfObject.setDouble(Coil_Heating_Gas_MultiStageFields::ParasiticGasLoad,value.get());
  }

  // NumberofStages
  {
    auto num = modelObject.stages().size();
    idfObject.setInt(Coil_Heating_Gas_MultiStageFields::NumberofStages,num);
  }

  for( auto stage: modelObject.stages() ) {
    auto eg = idfObject.pushExtensibleGroup();

    // Stage1GasBurnerEfficiency
    if( (value = stage.gasBurnerEfficiency()) ) {
      eg.setDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageGasBurnerEfficiency,value.get());
    }

    // Stage1NominalCapacity
    if( stage.isNominalCapacityAutosized() ) {
      eg.setString(Coil_Heating_Gas_MultiStageExtensibleFields::StageNominalCapacity,"AutoSize");
    } else if( (value = stage.nominalCapacity()) ) {
      eg.setDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageNominalCapacity,value.get());
    }

    // Stage1ParasiticElectricLoad
    if( (value = stage.parasiticElectricLoad()) ) {
      eg.setDouble(Coil_Heating_Gas_MultiStageExtensibleFields::StageParasiticElectricLoad,value.get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

