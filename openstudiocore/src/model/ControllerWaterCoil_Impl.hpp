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

#ifndef MODEL_CONTROLLERWATERCOIL_IMPL_HPP
#define MODEL_CONTROLLERWATERCOIL_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Node;

namespace detail {

/** ControllerWaterCoil_Impl is a HVACComponent_Impl that is the implementation class for ControllerWaterCoil.*/
class MODEL_API ControllerWaterCoil_Impl : public HVACComponent_Impl {
  Q_OBJECT;

  Q_PROPERTY(boost::optional<std::string> controlVariable READ controlVariable WRITE setControlVariable RESET resetControlVariable);
  Q_PROPERTY(boost::optional<std::string> action READ action WRITE setAction RESET resetAction);
  Q_PROPERTY(boost::optional<std::string> actuatorVariable READ actuatorVariable WRITE setActuatorVariable RESET resetActuatorVariable);
  Q_PROPERTY(boost::optional<double> controllerConvergenceTolerance READ controllerConvergenceTolerance WRITE setControllerConvergenceTolerance RESET resetControllerConvergenceTolerance);
  Q_PROPERTY(bool isControllerConvergenceToleranceDefaulted READ isControllerConvergenceToleranceDefaulted);
  Q_PROPERTY(bool isControllerConvergenceToleranceAutosized READ isControllerConvergenceToleranceAutosized);
  Q_PROPERTY(boost::optional<double> maximumActuatedFlow READ maximumActuatedFlow WRITE setMaximumActuatedFlow RESET resetMaximumActuatedFlow);
  Q_PROPERTY(bool isMaximumActuatedFlowAutosized READ isMaximumActuatedFlowAutosized);
  Q_PROPERTY(double minimumActuatedFlow READ minimumActuatedFlow WRITE setMinimumActuatedFlow RESET resetMinimumActuatedFlow);
  Q_PROPERTY(bool isMinimumActuatedFlowDefaulted READ isMinimumActuatedFlowDefaulted);
 public:

  /** @name Constructors and Destructors */
  //@{

  ControllerWaterCoil_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  ControllerWaterCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

  ControllerWaterCoil_Impl(const ControllerWaterCoil_Impl& other,
                           Model_Impl* model,
                           bool keepHandle);

  virtual ~ControllerWaterCoil_Impl() {}

  //@}
  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  /** @name Getters */
  //@{

  boost::optional<std::string> controlVariable() const;

  boost::optional<std::string> action() const;

  boost::optional<std::string> actuatorVariable() const;

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

  bool setControlVariable(boost::optional<std::string> controlVariable);

  void resetControlVariable();

  bool setAction(boost::optional<std::string> action);

  void resetAction();

  bool setActuatorVariable(boost::optional<std::string> actuatorVariable);

  void resetActuatorVariable();

  void setControllerConvergenceTolerance(boost::optional<double> controllerConvergenceTolerance);

  void resetControllerConvergenceTolerance();

  void autosizeControllerConvergenceTolerance();

  void setMaximumActuatedFlow(boost::optional<double> maximumActuatedFlow);

  void resetMaximumActuatedFlow();

  void autosizeMaximumActuatedFlow();

  void setMinimumActuatedFlow(double minimumActuatedFlow);

  void resetMinimumActuatedFlow();

  //@}

  boost::optional<Node> sensorNode() const;

  boost::optional<Node> actuatorNode() const;

  void setSensorNode( Node & node );

  void setActuatorNode( Node & node );

  protected:

  private:

  REGISTER_LOGGER("openstudio.model.ControllerWaterCoil");
};

} // detail

} // model

} // openstudio

#endif // MODEL_CONTROLLERWATERCOIL_IMPL_HPP

