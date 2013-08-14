/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp>
#include <model/CoilCoolingCooledBeam.hpp>
#include <model/CoilCoolingCooledBeam_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_CooledBeam_FieldEnums.hxx>
//#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctConstantVolumeCooledBeam(
    AirTerminalSingleDuctConstantVolumeCooledBeam & modelObject ){
    
  // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in 
  // the final file. Also set's the idfObject's name.
  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::AirTerminal_SingleDuct_ConstantVolume_CooledBeam,modelObject);

  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<int> number;
  boost::optional<ModelObject> temp;
                        
  // Forward Translate Cooling Coil
  //get the cooling coil as a straight component, cast it to optional cool coil, if cast is successful, 
  //get the object of class CoilCoolingCooledBeam
  StraightComponent coilCool = modelObject.coilCoolingCooledBeam();
  boost::optional<CoilCoolingCooledBeam>  optionalCoilCoolingCooledBeam = coilCool.optionalCast<CoilCoolingCooledBeam>();
                      
  if (optionalCoilCoolingCooledBeam) {
				
				CoilCoolingCooledBeam coilCoolingCooledBeam = *optionalCoilCoolingCooledBeam;
				
				// Field A6 Chilled Water Inlet Node Name
				temp = coilCoolingCooledBeam.inletModelObject();
				if(temp)
				{
						s = temp -> name();
						if(s)
						{
								idfObject.setString(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ChilledWaterInletNodeName,*s);
						}		
				}
				
				// Field A7 Chilled Water Inlet Node Name
				temp = coilCoolingCooledBeam.outletModelObject();
				if(temp)
				{
						s = temp -> name();
						if(s)
						{
								idfObject.setString(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ChilledWaterOutletNodeName,*s);
						}		
				}
				
				// Field N7 Coil Surface Area per Coil Length
				if(coilCoolingCooledBeam.isCoilSurfaceAreaperCoilLengthDefaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoilSurfaceAreaperCoilLength,5.422);
				}
				
				else if(value = coilCoolingCooledBeam.coilSurfaceAreaperCoilLength())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoilSurfaceAreaperCoilLength,*value);
				}
				
				// Field N8 Model Parameter a
				if(coilCoolingCooledBeam.isModelParameteraDefaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera,15.3);
				}
				
				else if(value = coilCoolingCooledBeam.modelParametera())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera,*value);
				}
				
				// Field N9 Model Parameter n1
				
				if(coilCoolingCooledBeam.isModelParametern1Defaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern1,0);
				}
				
				else if(value = coilCoolingCooledBeam.modelParametern1())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern1,*value);
				}
				
				// Field N10 Model Parameter n2
				
				if(coilCoolingCooledBeam.isModelParametern2Defaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern2,0.84);
				}
				
				else if(value = coilCoolingCooledBeam.modelParametern2())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern2,*value);
				}
				
				// Field N11 Model Parameter n3
				
				if(coilCoolingCooledBeam.isModelParametern3Defaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern3,0.12);
				}
				
				else if(value = coilCoolingCooledBeam.modelParametern3())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern3,*value);
				}
				
				// Field N12 Model Parameter a0
				
				if(coilCoolingCooledBeam.isModelParametera0Defaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera0,0.171);
				}
				
				else if(value = coilCoolingCooledBeam.modelParametera0())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametera0,*value);
				}
				
				// Field N13 Model Parameter K1
				
				if(coilCoolingCooledBeam.isModelParameterK1Defaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParameterK1,0.0057);
				}
				
				else if(value = coilCoolingCooledBeam.modelParameterK1())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParameterK1,*value);
				}
				
				// Field N14 Model Parameter n
				
				if(coilCoolingCooledBeam.isModelParameternDefaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern,0.4);
				}
				
				else if(value = coilCoolingCooledBeam.modelParametern())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::ModelParametern,*value);
				}
				
				// Field N16 Leaving Pipe Inside Diameter
				
				if(coilCoolingCooledBeam.isLeavingPipeInsideDiameterDefaulted())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::LeavingPipeInsideDiameter,0.0145);
				}
				
				else if(value = coilCoolingCooledBeam.leavingPipeInsideDiameter())
				{
						idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::LeavingPipeInsideDiameter,*value);
				}
  }
  
		else
		{
				LOG(Error,modelObject.briefDescription() << ": Could not translate cooling coil");
				return boost::none;
		}
		
		// Field A2 Availability Schedule Name
  Schedule availabilitySchedule = modelObject.availabilitySchedule();
  translateAndMapModelObject(availabilitySchedule);
  idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::AvailabilityScheduleName,availabilitySchedule.name().get() );
		
		// Field A3 Cooled Beam Type

		if(s = modelObject.cooledBeamType())
		{
				idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CooledBeamType,*s);
		}
		
		// Field A4 Supply Air Inlet Node Name
		
		temp = modelObject.inletModelObject();
		if(temp)
		{
				s = temp -> name();
				if(s)
				{
						idfObject.setString(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirInletNodeName,*s);
				}		
		}
		
		// Field A5 Supply Air Outlet Node Name
		
		temp = modelObject.outletModelObject();
		if(temp)
		{
				s = temp -> name();
				if(s)
				{
						idfObject.setString(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirOutletNodeName,*s);
				}		
		}
		
		// Field N1 Supply Air Volumetric Flow Rate
		
		if(modelObject.isSupplyAirVolumetricFlowRateDefaulted())
		{
				idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate,"autosize");
		}
		
		if(value = modelObject.supplyAirVolumetricFlowRate())
		{
				idfObject.setDouble(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::SupplyAirVolumetricFlowRate,*value);
		}
		
		// Field N2 Maximum Total Chilled Water Volumetric Flow Rate
		
		if(modelObject.isMaximumTotalChilledWaterVolumetricFlowRateDefaulted())
		{
				idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate,"autosize");
		}
		
		if(value = modelObject.maximumTotalChilledWaterVolumetricFlowRate())
		{
				idfObject.setDouble(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::MaximumTotalChilledWaterVolumetricFlowRate,*value);
		}
		
		// Field N3 Number of Beams
		
		if(modelObject.isNumberofBeamsDefaulted())
		{
				idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams,"autosize");
		}
		
		if(number = modelObject.numberofBeams())
		{
				idfObject.setInt(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::NumberofBeams,*number);
		}
		
		// Field N4 Beam Length
		
		if(modelObject.isBeamLengthDefaulted())
		{
				idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength,"autosize");
		}
		
		if(value = modelObject.beamLength())
		{
				idfObject.setDouble(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::BeamLength,*value);
		}
		
		// Field N5 Design Inlet Water Temperature
		
		if(modelObject.isDesignInletWaterTemperatureDefaulted())
		{
				idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature,15.0);
		}
		
		if(value = modelObject.designInletWaterTemperature())
		{
				idfObject.setDouble(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignInletWaterTemperature,*value);
		}
		
		// Field N6 Design Outlet Water Temperature
		
		if(modelObject.isDesignOutletWaterTemperatureDefaulted())
		{
				idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature,17.0);
		}
		
		if(value = modelObject.designOutletWaterTemperature())
		{
				idfObject.setDouble(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::DesignOutletWaterTemperature,*value);
		}
		
		// Filed N15 Coefficient of Induction Kin
		
		if(modelObject.isCoefficientofInductionKinDefaulted())
		{
				idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin,"Autocalculate");
		}
		
		if(value = modelObject.coefficientofInductionKin())
		{
				idfObject.setDouble(openstudio::AirTerminal_SingleDuct_ConstantVolume_CooledBeamFields::CoefficientofInductionKin,*value);
		}
  
		return idfObject;
}

} // energyplus

} // openstudio