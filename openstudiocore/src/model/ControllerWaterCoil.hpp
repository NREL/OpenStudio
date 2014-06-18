/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_CONTROLLERWATERCOIL_HPP
#define MODEL_CONTROLLERWATERCOIL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

class Node;

namespace detail {

  class ControllerWaterCoil_Impl;

} // detail

/** ControllerWaterCoil is a HVACComponent that wraps the OpenStudio IDD object 
 *  'OS:Controller:WaterCoil'. */
class MODEL_API ControllerWaterCoil : public HVACComponent 
{
  public:

  /** @name Constructors and Destructors */
  //@{

  explicit ControllerWaterCoil(const Model& model);

  virtual ~ControllerWaterCoil() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validControlVariableValues();

  static std::vector<std::string> validActionValues();

  static std::vector<std::string> validActuatorVariableValues();

  //@}
  /** @name Getters */
  //@{

  boost::optional<std::string> controlVariable() const;

  boost::optional<std::string> action() const;

  boost::optional<std::string> actuatorVariable() const;

  boost::optional<Node> sensorNode() const;

  boost::optional<Node> actuatorNode() const;

  boost::optional<double> controllerConvergenceTolerance() const;

  bool isControllerConvergenceToleranceDefaulted() const;

  bool isControllerConvergenceToleranceAutosized() const;

  boost::optional<double> maximumActuatedFlow() const;

  bool isMaximumActuatedFlowAutosized() const;

  double minimumActuatedFlow() const;

  bool isMinimumActuatedFlowDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(std::string controlVariable);

  void resetControlVariable();

  bool setAction(std::string action);

  void resetAction();

  bool setActuatorVariable(std::string actuatorVariable);

  void resetActuatorVariable();

  void setSensorNode( Node & node );

  void setActuatorNode( Node & node );

  void setControllerConvergenceTolerance(double controllerConvergenceTolerance);

  void resetControllerConvergenceTolerance();

  void autosizeControllerConvergenceTolerance();

  void setMaximumActuatedFlow(double maximumActuatedFlow);

  void resetMaximumActuatedFlow();

  void autosizeMaximumActuatedFlow();

  void setMinimumActuatedFlow(double minimumActuatedFlow);

  void resetMinimumActuatedFlow();

  //@}
  protected:

  /// @cond
  typedef detail::ControllerWaterCoil_Impl ImplType;

  friend class detail::ControllerWaterCoil_Impl;

  friend class Model;

  friend class openstudio::IdfObject;

  explicit ControllerWaterCoil(std::shared_ptr<detail::ControllerWaterCoil_Impl> impl);

  /// @endcond
  private:

  REGISTER_LOGGER("openstudio.model.ControllerWaterCoil");
};

/** \relates ControllerWaterCoil*/
typedef boost::optional<ControllerWaterCoil> OptionalControllerWaterCoil;

/** \relates ControllerWaterCoil*/
typedef std::vector<ControllerWaterCoil> ControllerWaterCoilVector;

} // model

} // openstudio

#endif // MODEL_CONTROLLERWATERCOIL_HPP

