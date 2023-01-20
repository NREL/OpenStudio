/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../model/UnivariateFunctions.hpp"
#include "../../model/UnivariateFunctions_Impl.hpp"

#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl( model::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl& modelObject )
{
  boost::optional<IdfObject> result;
  boost::optional<WorkspaceObject> _wo;
  boost::optional<ModelObject> _mo;

  // Instantiate an IdfObject of the class to store the values
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl, modelObject);
  // If it doesn't have a name, or if you aren't sure you are going to want to return it
  // IdfObject idfObject( openstudio::IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl );
  // m_idfObjects.push_back(idfObject);

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
  // But in some cases, you'll want to handle failure without pushing to the map
  // Name
  if (boost::optional<std::string> moName = modelObject.name()) {
    idfObject.setName(*moName);
  }

  // Availability Schedule: Optional Object
  if (boost::optional<Schedule> _availabilitySchedule = modelObject.availabilitySchedule()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_availabilitySchedule.get()) )  {
      idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule, _owo->nameString());
    }
  }

  // Coil Air Inlet Node: Required Node
  Node coilAirInletNode = modelObject.coilAirInletNode();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(coilAirInletNode) )  {
    idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode, _owo->nameString());
  }

  // Coil Air Outlet Node: Required Node
  Node coilAirOutletNode = modelObject.coilAirOutletNode();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(coilAirOutletNode) )  {
    idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode, _owo->nameString());
  }

  if (modelObject.isRatedTotalHeatingCapacityAutosized()) {
    idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, "Autosize");
  } else {
    // Rated Total Heating Capacity: boost::optional<double>
    if (boost::optional<double> _ratedTotalHeatingCapacity = modelObject.ratedTotalHeatingCapacity()) {
      idfObject.setDouble(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedTotalHeatingCapacity, _ratedTotalHeatingCapacity.get());
    }
  }

  // Indoor Unit Reference Subcooling: Optional Double
  double indoorUnitReferenceSubcooling = modelObject.indoorUnitReferenceSubcooling();
  idfObject.setDouble(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitReferenceSubcooling, indoorUnitReferenceSubcooling);


  // Indoor Unit Condensing Temperature Function of Subcooling Curve Name: Required Object
  UnivariateFunctions indoorUnitCondensingTemperatureFunctionofSubcoolingCurve = modelObject.indoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(indoorUnitCondensingTemperatureFunctionofSubcoolingCurve) )  {
    idfObject.setString(Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::IndoorUnitCondensingTemperatureFunctionofSubcoolingCurveName, _owo->nameString());
  }

  return idfObject;
} // End of translate function

} // end namespace energyplus
} // end namespace openstudio