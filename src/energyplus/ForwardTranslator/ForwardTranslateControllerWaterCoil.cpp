/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ControllerWaterCoil.hpp"
#include "../../model/ControllerWaterCoil_Impl.hpp"
#include "../../model/WaterToAirComponent.hpp"
#include "../../model/WaterToAirComponent_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Controller_WaterCoil_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateControllerWaterCoil( ControllerWaterCoil & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::Controller_WaterCoil);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // ControlVariable

  if( (s = modelObject.controlVariable()) )
  {
    idfObject.setString(Controller_WaterCoilFields::ControlVariable,s.get());
  }

  // Action

  if( (s = modelObject.action()) )
  {
    idfObject.setString(Controller_WaterCoilFields::Action,s.get());
  }

  // ActuatorVariable

  if( (s = modelObject.actuatorVariable()) )
  {
    idfObject.setString(Controller_WaterCoilFields::ActuatorVariable,s.get());
  }

  boost::optional<model::WaterToAirComponent> waterCoil;
  if( auto hvacComp = modelObject.getImpl<model::detail::ControllerWaterCoil_Impl>()->waterCoil() ) {
    waterCoil = hvacComp->optionalCast<model::WaterToAirComponent>();
  }

  // SensorNodeName

  if( boost::optional<Node> node = modelObject.sensorNode() ) {
    idfObject.setString(Controller_WaterCoilFields::SensorNodeName,node->name().get());
  } else if( waterCoil ) {
    if( auto node = waterCoil->airOutletModelObject() ) {
      idfObject.setString(Controller_WaterCoilFields::SensorNodeName,node->name().get());
    }
  }

  // ActuatorNodeName

  if( boost::optional<Node> node = modelObject.actuatorNode() ) {
    idfObject.setString(Controller_WaterCoilFields::ActuatorNodeName,node->name().get());
  } else if( waterCoil ) {
    if( auto node = waterCoil->waterInletModelObject() ) {
      idfObject.setString(Controller_WaterCoilFields::ActuatorNodeName,node->name().get());
    }
  }

  // ControllerConvergenceTolerance

  if( modelObject.isControllerConvergenceToleranceAutosized() )
  {
    idfObject.setString(Controller_WaterCoilFields::ControllerConvergenceTolerance,"Autosize");
  }
  else if( (value = modelObject.controllerConvergenceTolerance()) )
  {
    idfObject.setDouble(Controller_WaterCoilFields::ControllerConvergenceTolerance,value.get());
  }

  // MaximumActuatedFlow

  if( modelObject.isMaximumActuatedFlowAutosized() )
  {
    idfObject.setString(Controller_WaterCoilFields::MaximumActuatedFlow,"Autosize");
  }
  else if( (value = modelObject.maximumActuatedFlow()) )
  {
    idfObject.setDouble(Controller_WaterCoilFields::MaximumActuatedFlow,value.get());
  }

  // MinimumActuatedFlow

  if( (value = modelObject.minimumActuatedFlow()) )
  {
    idfObject.setDouble(Controller_WaterCoilFields::MinimumActuatedFlow,value.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

