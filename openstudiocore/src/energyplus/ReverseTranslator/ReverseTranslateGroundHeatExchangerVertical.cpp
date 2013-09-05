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

#include <energyplus/ReverseTranslator.hpp>

#include <model/GroundHeatExchangerVertical.hpp>
#include <model/GroundHeatExchangerVertical_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <utilities/idd/GroundHeatExchanger_Vertical_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateGroundHeatExchangerVertical( const WorkspaceObject & workspaceObject )
{
	if( workspaceObject.iddObject().type() != IddObjectType::GroundHeatExchanger_Vertical ){
		LOG(Error, "WorkspaceObject is not IddObjectType: GroundHeatExchanger:Vertical");
		return boost::none;
	}

	GroundHeatExchangerVertical ghex = GroundHeatExchangerVertical( m_model );

	boost::optional<double> d;
	boost::optional<std::string> s = workspaceObject.getString(GroundHeatExchanger_VerticalFields::Name);

	if (s) {
	  ghex.setName(s.get());
	}

	//TODO: Need to address Inlet Node Name and Outlet Node Name somehow, probably with PlantLoop.

	//Maximum Flow Rate
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::MaximumFlowRate);
	if (d) {
		ghex.setMaximumFlowRate(*d);
	}

	//Number of Bore Holes
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::NumberofBoreHoles);
	if (d) {
		ghex.setNumberofBoreHoles(*d);
	}

	//Bore Hole Length
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::BoreHoleLength);
	if (d) {
		ghex.setBoreHoleLength(*d);
	}

	//Bore Hole Radius
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::BoreHoleRadius);
	if (d) {
		ghex.setBoreHoleRadius(*d);
	}

	//Ground Thermal Conductivity
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::GroundThermalConductivity);
	if (d) {
		ghex.setGroundThermalConductivity(*d);
	}

	//Ground Thermal Heat Capacity
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity);
	if (d) {
		ghex.setGroundThermalHeatCapacity(*d);
	}

	//Ground Temperature
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::GroundTemperature);
	if (d) {
		ghex.setGroundTemperature(*d);
	}

	//Design Flow Rate
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::DesignFlowRate);
	if (d) {
		ghex.setDesignFlowRate(*d);
	}

	//Grout Thermal Conductivity
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::GroutThermalConductivity);
	if (d) {
		ghex.setGroutThermalConductivity(*d);
	}

	//Pipe Thermal Conductivity
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::PipeThermalConductivity);
	if (d) {
		ghex.setPipeThermalConductivity(*d);
	}

	//Pipe Out Diameter
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::PipeOutDiameter);
	if (d) {
		ghex.setPipeOutDiameter(*d);
	}

	//U-Tube Distance
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::UTubeDistance);
	if (d) {
		ghex.setUTubeDistance(*d);
	}

	//Pipe Thickness
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::PipeThickness);
	if (d) {
		ghex.setPipeThickness(*d);
	}

	//Maximum Length of Simulation
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation);
	if (d) {
		ghex.setMaximumLengthofSimulation(*d);
	}

	//G-Function Reference Ratio
	d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio);
	if (d) {
		ghex.setGFunctionReferenceRatio(*d);
	}

	std::vector<IdfExtensibleGroup> groups = workspaceObject.extensibleGroups();
	ghex.removeAllGFunctions();

    for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
         it != groups.end();
         it++ )
    {
    	ghex.pushExtensibleGroup(it->fields());
    }

    return ghex;
}

} // energyplus

} // openstudio

