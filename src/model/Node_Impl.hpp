/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_NODE_IMPL_HPP
#define MODEL_NODE_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Node;
  class SetpointManagerSingleZoneReheat;
  class SetpointManagerMixedAir;
  class SetpointManagerScheduled;
  class SetpointManagerFollowOutdoorAirTemperature;
  class SetpointManagerOutdoorAirReset;
  class SetpointManagerWarmest;
  class SetpointManager;
  class AirflowNetworkDistributionNode;

  namespace detail {

    class MODEL_API Node_Impl : public StraightComponent_Impl
    {
     public:
      // constructor
      Node_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      Node_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      Node_Impl(const Node_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~Node_Impl() = default;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      std::vector<SetpointManager> setpointManagers() const;

      void removeSetpointManagers();

      std::vector<ModelObject> children() const override;

      bool isRemovable() const override;

      std::vector<IdfObject> remove() override;

      bool addToNode(Node& node) override;

      ModelObject clone(Model model) const override;

      bool isConnected(const ModelObject& modelObject);

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

      boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.Node");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_NODE_IMPL_HPP
