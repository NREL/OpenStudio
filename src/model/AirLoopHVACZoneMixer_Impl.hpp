/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACZONEMIXER_IMPL_HPP
#define MODEL_AIRLOOPHVACZONEMIXER_IMPL_HPP

#include "Mixer_Impl.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkDistributionNode;

  namespace detail {

    class MODEL_API AirLoopHVACZoneMixer_Impl : public Mixer_Impl
    {
     public:
      // constructor
      AirLoopHVACZoneMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      AirLoopHVACZoneMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      AirLoopHVACZoneMixer_Impl(const AirLoopHVACZoneMixer_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~AirLoopHVACZoneMixer_Impl() override = default;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual IddObjectType iddObjectType() const override;

      std::vector<openstudio::IdfObject> remove() override;

      unsigned outletPort() const override;

      unsigned inletPort(unsigned branchIndex) const override;

      unsigned nextInletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      void disconnect() override;

      AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

      boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneMixer");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACZONEMIXER_IMPL_HPP
