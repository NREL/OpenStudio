/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
      virtual ~Node_Impl() override = default;

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
