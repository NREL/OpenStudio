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
#include "../../model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "../../model/CoilCoolingCooledBeam.hpp"
#include "../../model/CoilCoolingCooledBeam_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/HVACComponent_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include "../../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_CooledBeam_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctConstantVolumeCooledBeam(
  AirTerminalSingleDuctConstantVolumeCooledBeam & modelObject ){

  //Name
  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_CooledBeam);
  m_idfObjects.push_back(idfObject);

  std:: string baseName = modelObject.name().get();
  idfObject.setName(baseName);

  //register the air distribution unit in the map rather than the chilled beam air terminal itself.
  //this is necessary because the air distribution unit encapsulating the chilled beam air terminal
  //goes on both the brachlist and the zone HVAC equipment list
  //since the branches are translated first, when the chilled beam's coil is encountered in the branchlist, the air distribution unit
  //that encapsulates the chilled beam is returned and put onto the branch list.
  //When the chilled beam air terminal is encountered again on the zone HVAC equipment list,
  //the map must return the air distribution unit to ensure that the air distribution unit is on the translated
  //equipment list.

  IdfObject _airDistributionUnit(IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName("ADU " + baseName ); //ADU: Air Distribution Unit
  m_idfObjects.push_back(_airDistributionUnit);

  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<int> number;
  boost::optional<ModelObject> temp;

  // Forward Translate Cooling Coil
  //get the cooling coil as a HVAC component, cast it to optional cool coil, if cast is successful,
  //get the object of class CoilCoolingCooledBeam
  HVACComponent coilCool = modelObject.coilCoolingCooledBeam();
  boost::optional<CoilCoolingCooledBeam>  optionalCoilCoolingCooledBeam = coilCool.optionalCast<CoilCoolingCooledBeam>();

  // Field A2 Availability Schedule Name
  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // Field A3 Cooled Beam Type
  idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CooledBeamType, modelObject.cooledBeamType());

  // Field A4 Supply Air Inlet Node Name
  temp = modelObject.inletModelObject();
  if(temp)
  {
    if((s = temp->name()))
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirInletNodeName,s.get());
    }
  }

  // Field A5 Supply Air Outlet Node Name
  temp = modelObject.outletModelObject();
  if(temp)
  {
    if((s = temp->name()))
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirOutletNodeName,s.get());
    }
  }

  if (optionalCoilCoolingCooledBeam) {

    CoilCoolingCooledBeam coilCoolingCooledBeam = *optionalCoilCoolingCooledBeam;

    // Field A6 Chilled Water Inlet Node Name
    temp = coilCoolingCooledBeam.inletModelObject();
    if(temp)
    {
      if((s = temp->name()))
      {
        idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ChilledWaterInletNodeName,s.get());
      }
    }

    // Field A7 Chilled Water Outlet Node Name
    temp = coilCoolingCooledBeam.outletModelObject();
    if(temp)
    {
      if((s = temp->name()))
      {
        idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ChilledWaterOutletNodeName,s.get());
      }
    }

    // Field N1 Supply Air Volumetric Flow Rate
    if(modelObject.isSupplyAirVolumetricFlowRateDefaulted())
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate,"autosize");
    }
    else if((value = modelObject.supplyAirVolumetricFlowRate()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate,value.get());
    }

    // Field N2 Maximum Total Chilled Water Volumetric Flow Rate
    if(modelObject.isMaximumTotalChilledWaterVolumetricFlowRateDefaulted())
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate,"autosize");
    }
    else if((value = modelObject.maximumTotalChilledWaterVolumetricFlowRate()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate,value.get());
    }

    // Field N3 Number of Beams
    if(modelObject.isNumberofBeamsDefaulted())
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams,"autosize");
    }
    else if((number = modelObject.numberofBeams()))
    {
      idfObject.setInt(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams,number.get());
    }

    // Field N4 Beam Length
    if(modelObject.isBeamLengthDefaulted())
    {
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength,"autosize");
    }
    else if((value = modelObject.beamLength()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength,value.get());
    }

    // Field N5 Design Inlet Water Temperature
    if(modelObject.isDesignInletWaterTemperatureDefaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature,15.0);
    }
    else if((value = modelObject.designInletWaterTemperature()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature,value.get());
    }

    // Field N6 Design Outlet Water Temperature
    if(modelObject.isDesignOutletWaterTemperatureDefaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature,17.0);
    }
    else if((value = modelObject.designOutletWaterTemperature()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature,value.get());
    }

    // Field N7 Coil Surface Area per Coil Length
    if(coilCoolingCooledBeam.isCoilSurfaceAreaperCoilLengthDefaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoilSurfaceAreaperCoilLength,5.422);
    }
    else if((value = coilCoolingCooledBeam.coilSurfaceAreaperCoilLength()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoilSurfaceAreaperCoilLength,value.get());
    }

    // Field N8 Model Parameter a
    if(coilCoolingCooledBeam.isModelParameteraDefaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera,15.3);
    }
    else if((value = coilCoolingCooledBeam.modelParametera()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera,value.get());
    }

    // Field N9 Model Parameter n1
    if(coilCoolingCooledBeam.isModelParametern1Defaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern1,0);
    }
    else if((value = coilCoolingCooledBeam.modelParametern1()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern1,value.get());
    }

    // Field N10 Model Parameter n2
    if(coilCoolingCooledBeam.isModelParametern2Defaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern2,0.84);
    }
    else if((value = coilCoolingCooledBeam.modelParametern2()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern2,value.get());
    }

    // Field N11 Model Parameter n3
    if(coilCoolingCooledBeam.isModelParametern3Defaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern3,0.12);
    }
    else if((value = coilCoolingCooledBeam.modelParametern3()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern3,value.get());
    }

    // Field N12 Model Parameter a0
    if(coilCoolingCooledBeam.isModelParametera0Defaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera0,0.171);
    }
    else if((value = coilCoolingCooledBeam.modelParametera0()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera0,value.get());
    }

    // Field N13 Model Parameter K1
    if(coilCoolingCooledBeam.isModelParameterK1Defaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParameterK1,0.0057);
    }
    else if((value = coilCoolingCooledBeam.modelParameterK1()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParameterK1,value.get());
    }

    // Field N14 Model Parameter n
    if(coilCoolingCooledBeam.isModelParameternDefaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern,0.4);
    }
    else if((value = coilCoolingCooledBeam.modelParametern()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern,value.get());
    }

    // Field N16 Leaving Pipe Inside Diameter
    if(coilCoolingCooledBeam.isLeavingPipeInsideDiameterDefaulted())
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::LeavingPipeInsideDiameter,0.0145);
    }
    else if((value = coilCoolingCooledBeam.leavingPipeInsideDiameter()))
    {
      idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::LeavingPipeInsideDiameter,value.get());
    }
  }
  else
  {
    LOG(Error,modelObject.briefDescription() << ": Could not translate cooling coil");
    return boost::none;
  }

  // Field N15 Coefficient of Induction Kin
  if(modelObject.isCoefficientofInductionKinDefaulted())
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin,"autocalculate");
  }

  if((value = modelObject.coefficientofInductionKin()))
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin,value.get());
  }

  // Populate fields for AirDistributionUnit
  if( boost::optional<ModelObject> outletNode = modelObject.outletModelObject() )
  {
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName,outletNode->name().get());
  }
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType,idfObject.iddObject().name());
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName,idfObject.name().get());

  return _airDistributionUnit;

}

} // energyplus

} // openstudio
