/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      virtual ~AirLoopHVACOutdoorAirSystem_Impl() override = default;

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

      // Take a shortcut to avoid also checking each AirLoopHVAC's OutdoorAirSystem's component like HVACComponent logically does
      virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      std::vector<ModelObject> oaComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      std::vector<ModelObject> reliefComponents(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      std::vector<ModelObject> components(openstudio::IddObjectType type = openstudio::IddObjectType("Catchall")) const;

      boost::optional<Node> outboardOANode() const;
      boost::optional<Node> outboardReliefNode() const;

      boost::optional<ModelObject> component(openstudio::Handle handle) const;
      boost::optional<ModelObject> oaComponent(openstudio::Handle handle) const;
      boost::optional<ModelObject> reliefComponent(openstudio::Handle handle) const;

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
      boost::optional<ModelObject> outboardOANodeAsModelObject() const;
      boost::optional<ModelObject> outboardReliefNodeAsModelObject() const;
      boost::optional<ModelObject> airLoopAsModelObject() const;

      bool setControllerOutdoorAirAsModelObject(const boost::optional<ModelObject>& modelObject);
    };
  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif
