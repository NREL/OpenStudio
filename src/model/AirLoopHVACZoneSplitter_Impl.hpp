/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRLOOPHVACZONESPLITTER_IMPL_HPP
#define MODEL_AIRLOOPHVACZONESPLITTER_IMPL_HPP

#include "Splitter_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class AirflowNetworkDistributionNode;

  namespace detail {

    class MODEL_API AirLoopHVACZoneSplitter_Impl : public Splitter_Impl
    {
     public:
      // constructor
      AirLoopHVACZoneSplitter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      AirLoopHVACZoneSplitter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      AirLoopHVACZoneSplitter_Impl(const AirLoopHVACZoneSplitter_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~AirLoopHVACZoneSplitter_Impl() override = default;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      std::vector<openstudio::IdfObject> remove() override;

      virtual unsigned inletPort() const override;

      unsigned outletPort(unsigned branchIndex) const override;

      unsigned nextOutletPort() const override;

      std::vector<ThermalZone> thermalZones();

      boost::optional<ModelObject> zoneEquipmentForBranch(int branchIndex);

      void disconnect() override;

      AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

      boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACZoneSplitter");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACZONESPLITTER_IMPL_HPP
