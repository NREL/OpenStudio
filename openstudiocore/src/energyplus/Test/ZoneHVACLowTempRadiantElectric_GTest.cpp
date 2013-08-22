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

#include <gtest/gtest.h>
#include <energyplus/Test/EnergyPlusFixture.hpp>

#include <energyplus/ForwardTranslator.hpp>
#include <energyplus/ReverseTranslator.hpp>

#include <model/ZoneHVACLowTemperatureRadiantElectric.hpp>
#include <model/ZoneHVACLowTemperatureRadiantElectric_Impl.hpp>
#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>
#include <model/DefaultConstructionSet.hpp>
#include <model/DefaultConstructionSet_Impl.hpp>
#include <model/DefaultSurfaceConstructions.hpp>
#include <model/DefaultSurfaceConstructions_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ScheduleConstant.hpp>
#include <model/ScheduleConstant_Impl.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/StandardOpaqueMaterial_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <utilities/idd/ZoneHVAC_LowTemperatureRadiant_Electric_FieldEnums.hxx>
#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ZoneHVACLowTempRadiantElectric_Set_Flow_Fractions) 
{
	//make the example model
	Model model = model::exampleModel();

	//loop through all zones and add a radiant system to each one
	BOOST_FOREACH(ThermalZone thermalZone, model.getModelObjects<ThermalZone>()){

		//make an electric radiant unit
		ScheduleConstant availabilitySched(model);
		ScheduleConstant heatingControlTemperatureSchedule(model);
	
		availabilitySched.setValue(1.0);
		heatingControlTemperatureSchedule.setValue(10.0);

		ZoneHVACLowTemperatureRadiantElectric testRad(model,availabilitySched,heatingControlTemperatureSchedule);
		
		//add it to the thermal zone
		testRad.addToThermalZone(thermalZone);

		//attach to ceilings
		testRad.setRadiantSurfaceType("Ceilings");

		//test that "surfaces" method returns 0 since no 
		//ceilings have an internal source construction
		EXPECT_EQ(0,testRad.surfaces().size());

	}

	// Create some materials and make an internal source construction
	StandardOpaqueMaterial exterior(model);
	StandardOpaqueMaterial interior(model);
	OpaqueMaterialVector layers;
	layers.push_back(exterior);
	layers.push_back(interior);
	ConstructionWithInternalSource construction(layers);

	//set building's default ceiling construction to internal source construction
	DefaultConstructionSet defConSet = model.getModelObjects<DefaultConstructionSet>()[0];
	defConSet.defaultExteriorSurfaceConstructions()->setRoofCeilingConstruction(construction);

	//translate the model to EnergyPlus
	ForwardTranslator trans;
	Workspace workspace = trans.translateModel(model);

	//loop through all zones and check the flow fraction for each surface in the surface group.  it should be 0.25
	BOOST_FOREACH(ThermalZone thermalZone, model.getModelObjects<ThermalZone>()){

		//get the radiant zone equipment
		BOOST_FOREACH(ModelObject equipment, thermalZone.equipment()){
			if (equipment.optionalCast<ZoneHVACLowTemperatureRadiantElectric>()){
				ZoneHVACLowTemperatureRadiantElectric testRad = equipment.optionalCast<ZoneHVACLowTemperatureRadiantElectric>().get();
				BOOST_FOREACH(IdfExtensibleGroup extGrp, testRad.extensibleGroups()){
					EXPECT_EQ(0.25,extGrp.getDouble(1,false));
				}
			}
		}
	}

} 


