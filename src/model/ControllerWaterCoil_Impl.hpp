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

#ifndef MODEL_CONTROLLERWATERCOIL_IMPL_HPP
#define MODEL_CONTROLLERWATERCOIL_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** ControllerWaterCoil_Impl is a HVACComponent_Impl that is the implementation class for ControllerWaterCoil.*/
    class MODEL_API ControllerWaterCoil_Impl : public HVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ControllerWaterCoil_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ControllerWaterCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ControllerWaterCoil_Impl(const ControllerWaterCoil_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ControllerWaterCoil_Impl() {}

      //@}
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      boost::optional<double> autosizedControllerConvergenceTolerance() const;

      boost::optional<double> autosizedMaximumActuatedFlow() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setControlVariable(boost::optional<std::string> controlVariable);

      void resetControlVariable();

      bool setAction(boost::optional<std::string> action);

      void resetAction();

      bool setActuatorVariable(boost::optional<std::string> actuatorVariable);

      void resetActuatorVariable();

      bool setControllerConvergenceTolerance(boost::optional<double> controllerConvergenceTolerance);

      void resetControllerConvergenceTolerance();

      void autosizeControllerConvergenceTolerance();

      bool setMaximumActuatedFlow(boost::optional<double> maximumActuatedFlow);

      void resetMaximumActuatedFlow();

      void autosizeMaximumActuatedFlow();

      bool setMinimumActuatedFlow(double minimumActuatedFlow);

      void resetMinimumActuatedFlow();

      //@}

      boost::optional<Node> sensorNode() const;

      boost::optional<Node> actuatorNode() const;

      bool setSensorNode(const Node& node);

      bool setActuatorNode(const Node& node);

      bool setWaterCoil(const HVACComponent& comp);

      boost::optional<HVACComponent> waterCoil() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ControllerWaterCoil");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONTROLLERWATERCOIL_IMPL_HPP
