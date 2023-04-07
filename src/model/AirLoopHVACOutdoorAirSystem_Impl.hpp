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

#ifndef MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_IMPL_HPP
#define MODEL_AIRLOOPHVACOUTDOORAIRSYSTEM_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class ModelObject;
  class AirLoopHVAC;
  class ControllerOutdoorAir;
  class Node;
  class AirflowNetworkDistributionNode;
  class AirLoopHVACDedicatedOutdoorAirSystem;

  namespace detail {

    // derive AirLoopHVACOutdoorAirSystem_Impl from ModelObject_Impl to override virtual methods
    class MODEL_API AirLoopHVACOutdoorAirSystem_Impl : public HVACComponent_Impl
    {
     public:
      // constructor
      AirLoopHVACOutdoorAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      AirLoopHVACOutdoorAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      AirLoopHVACOutdoorAirSystem_Impl(const AirLoopHVACOutdoorAirSystem_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~AirLoopHVACOutdoorAirSystem_Impl() = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      unsigned returnAirPort() const;
      unsigned outdoorAirPort() const;
      unsigned reliefAirPort() const;
      unsigned mixedAirPort() const;

      boost::optional<ModelObject> returnAirModelObject() const;
      boost::optional<ModelObject> outdoorAirModelObject() const;
      boost::optional<ModelObject> reliefAirModelObject() const;
      boost::optional<ModelObject> mixedAirModelObject() const;

      ControllerOutdoorAir getControllerOutdoorAir() const;

      bool setControllerOutdoorAir(const ControllerOutdoorAir& controllerOutdoorAir);

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      std::vector<ModelObject> oaComponents() const;

      std::vector<ModelObject> reliefComponents() const;

      boost::optional<Node> outboardOANode() const;
      boost::optional<Node> outboardReliefNode() const;

      boost::optional<AirLoopHVAC> airLoop() const;
      std::vector<ModelObject> components() const;
      boost::optional<ModelObject> component(openstudio::Handle handle);
      boost::optional<ModelObject> oaComponent(openstudio::Handle handle);
      boost::optional<ModelObject> reliefComponent(openstudio::Handle handle);

      AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

      boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

      boost::optional<AirLoopHVACDedicatedOutdoorAirSystem> airLoopHVACDedicatedOutdoorAirSystem() const;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACOutdoorAirSystem");

      boost::optional<ModelObject> controllerOutdoorAirAsModelObject() const;
      std::vector<ModelObject> oaComponentsAsModelObjects() const;
      std::vector<ModelObject> reliefComponentsAsModelObjects() const;
      boost::optional<ModelObject> outboardOANodeAsModelObject() const;
      boost::optional<ModelObject> outboardReliefNodeAsModelObject() const;
      boost::optional<ModelObject> airLoopAsModelObject() const;
      std::vector<ModelObject> componentsAsModelObjects() const;

      bool setControllerOutdoorAirAsModelObject(const boost::optional<ModelObject>& modelObject);
    };
  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif
