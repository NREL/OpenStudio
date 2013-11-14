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
#include <utilities/idf/IdfExtensibleGroup.hpp>
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
  boost::optional<ModelObject> temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::GroundHeatExchanger_Vertical, modelObject);

  // Inlet Node Name
  if( (temp = modelObject.inletModelObject()) )
  {
    idfObject.setString(GroundHeatExchanger_VerticalFields::InletNodeName,temp->name().get());
  }

  // Outlet Node Name
  if( (temp = modelObject.outletModelObject()) )
  {
    idfObject.setString(GroundHeatExchanger_VerticalFields::OutletNodeName,temp->name().get());
  }

  // Maximum Flow Rate
  if( (value = modelObject.maximumFlowRate()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::MaximumFlowRate,value.get());
  }

  // Number of Bore Holes
  if( (value = modelObject.numberofBoreHoles()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::NumberofBoreHoles,value.get());
  }

  // Bore Hole Length
  if( (value = modelObject.boreHoleLength()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::BoreHoleLength,value.get());
  }

  // Bore Hole Radius
  if( (value = modelObject.boreHoleRadius()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::BoreHoleRadius,value.get());
  }

  // Ground Thermal Conductivity
  if( (value = modelObject.groundThermalConductivity()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundThermalConductivity,value.get());
  }

  // Ground Thermal Heat Capacity
  if( (value = modelObject.groundThermalHeatCapacity()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity,value.get());
  }

  // Ground Temperature
  if( (value = modelObject.groundTemperature()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroundTemperature,value.get());
  }

  // Design Flow Rate
  if( (value = modelObject.designFlowRate()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::DesignFlowRate,value.get());
  }

  // Grout Thermal Conductivity
  if( (value = modelObject.groutThermalConductivity()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GroutThermalConductivity,value.get());
  }

  // Pipe Thermal Conductivity
  if( (value = modelObject.pipeThermalConductivity()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeThermalConductivity,value.get());
  }

  // Pipe Out Diameter
  if( (value = modelObject.pipeOutDiameter()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeOutDiameter,value.get());
  }

  // U-Tube Distance
  if( (value = modelObject.uTubeDistance()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::UTubeDistance,value.get());
  }

  // Pipe Thickness
  if( (value = modelObject.pipeThickness()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::PipeThickness,value.get());
  }

  // Maximum Length of Simulation
  if( (value = modelObject.maximumLengthofSimulation()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation,value.get());
  }

  // G-Function Reference Ratio
  if( (value = modelObject.gFunctionReferenceRatio()) )
  {
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio,value.get());
  }

  std::vector< std::pair<double,double> > gFunctions = modelObject.gFunctions();

  // Number of Data Pairs of the G Function
  idfObject.setInt(GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction,gFunctions.size());

  if( !gFunctions.empty() )
  {
    for( std::vector< std::pair<double,double> >::iterator it = gFunctions.begin();
       it != gFunctions.end();
       it++ )
    {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

      eg.setDouble(GroundHeatExchanger_VerticalExtensibleFields::GFunctionLn_T_Ts_Value,it->first); 
      eg.setDouble(GroundHeatExchanger_VerticalExtensibleFields::GFunctionGValue,it->second); 
    }
  }
  else {
    LOG(Error,modelObject.briefDescription() << ": At least one pair of GFunction data required");
    return boost::none;
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

