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

#include "../ReverseTranslator.hpp"

#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/GroundHeatExchangerVertical_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/GroundHeatExchanger_Vertical_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
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
  d = workspaceObject.getDouble(GroundHeatExchanger_VerticalFields::DesignFlowRate);
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

  for( const auto & group : groups )
  {
    ghex.pushExtensibleGroup(group.fields());
  }

  return ghex;
}

} // energyplus

} // openstudio

