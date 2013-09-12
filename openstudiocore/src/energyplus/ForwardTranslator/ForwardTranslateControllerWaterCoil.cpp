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
#include <model/ControllerWaterCoil.hpp>
#include <model/ControllerWaterCoil_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/Controller_WaterCoil_FieldEnums.hxx>
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

  // SensorNodeName

  if( boost::optional<Node> node = modelObject.sensorNode() )
  {
    idfObject.setString(Controller_WaterCoilFields::SensorNodeName,node->name().get());
  }

  // ActuatorNodeName

  if( boost::optional<Node> node = modelObject.actuatorNode() )
  {
    idfObject.setString(Controller_WaterCoilFields::ActuatorNodeName,node->name().get());
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

