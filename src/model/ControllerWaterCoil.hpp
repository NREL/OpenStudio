/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_CONTROLLERWATERCOIL_HPP
#define MODEL_CONTROLLERWATERCOIL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class ControllerWaterCoil_Impl;

  }  // namespace detail

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

    bool setSensorNode(const Node& node);

    bool setActuatorNode(const Node& node);

    bool setControllerConvergenceTolerance(double controllerConvergenceTolerance);

    void resetControllerConvergenceTolerance();

    void autosizeControllerConvergenceTolerance();

    bool setMaximumActuatedFlow(double maximumActuatedFlow);

    void resetMaximumActuatedFlow();

    void autosizeMaximumActuatedFlow();

    bool setMinimumActuatedFlow(double minimumActuatedFlow);

    void resetMinimumActuatedFlow();

    boost::optional<double> autosizedControllerConvergenceTolerance() const;

    boost::optional<double> autosizedMaximumActuatedFlow() const;

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

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONTROLLERWATERCOIL_HPP
