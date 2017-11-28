/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/GroundHeatExchangerVertical_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/GroundHeatExchanger_Vertical_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
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
    idfObject.setDouble(GroundHeatExchanger_VerticalFields::DesignFlowRate,value.get());
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

  std::vector< GFunction > gFunctions = modelObject.gFunctions();

  // Number of Data Pairs of the G Function
  idfObject.setInt(GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction,gFunctions.size());

  if( !gFunctions.empty() )
  {
    for( const auto & gFunction : gFunctions )
    {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

      eg.setDouble(GroundHeatExchanger_VerticalExtensibleFields::GFunctionLn_T_Ts_Value,gFunction.lnValue());
      eg.setDouble(GroundHeatExchanger_VerticalExtensibleFields::GFunctionGValue,gFunction.gValue());
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

