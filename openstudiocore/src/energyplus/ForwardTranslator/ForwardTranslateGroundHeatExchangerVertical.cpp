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
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/GroundHeatExchangerVertical.hpp>
#include <model/GroundHeatExchangerVertical_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/GroundHeatExchanger_Vertical_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGroundHeatExchangerVertical( GroundHeatExchangerVertical & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::GroundHeatExchanger_Vertical);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // Inlet Node Name

  //if( s = modelObject.inletNodeName() )
  //{
  //  idfObject.setString(GroundHeatExchanger_VerticalFields::InletNodeName,s.get());
  //}

  // Outlet Node Name

  //if( s = modelObject.outletNodeName() )
  //{
  //  idfObject.setString(GroundHeatExchanger_VerticalFields::OutletNodeName,s.get());
  //}

  // Maximum Flow Rate
  
  if( value = modelObject.maximumFlowRate() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::MaximumFlowRate,value.get());
  }

  // Number of Bore Holes
  
  if( value = modelObject.numberofBoreHoles() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::NumberofBoreHoles,value.get());
  }

  // Bore Hole Radius
  
  if( value = modelObject.boreHoleRadius() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::BoreHoleRadius,value.get());
  }

  // Ground Thermal Conductivity
  
  if( value = modelObject.groundThermalConductivity() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundThermalConductivity,value.get());
  }

  // Ground Thermal Heat Capacity
  
  if( value = modelObject.groundThermalHeatCapacity() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity,value.get());
  }
  // Ground Temperature
  
  if( value = modelObject.groundTemperature() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundTemperature,value.get());
  }

  // Design Flow Rate
  
  if( value = modelObject.designFlowRate() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::DesignFlowRate,value.get());
  }

  // Grout Thermal Conductivity
  
  if( value = modelObject.groutThermalConductivity() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroutThermalConductivity,value.get());
  }

  // Pipe Thermal Conductivity
  
  if( value = modelObject.pipeThermalConductivity() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeThermalConductivity,value.get());
  }

  // Pipe Out Diameter
  
  if( value = modelObject.pipeOutDiameter() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeOutDiameter,value.get());
  }

  // U-Tube Distance
  
  if( value = modelObject.uTubeDistance() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::UTubeDistance,value.get());
  }

  // Pipe Thickness
  
  if( value = modelObject.pipeThickness() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeThickness,value.get());
  }

  // Maximum Length of Simulation
  
  if( value = modelObject.maximumLengthofSimulation() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation,value.get());
  }

  // G-Function Reference Ratio
  
  if( value = modelObject.gFunctionReferenceRatio() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio,value.get());
  }

  // Number of Data Pairs of the G Function
  
  if( value = modelObject.numberofDataPairsoftheGFunction() )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction,value.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

