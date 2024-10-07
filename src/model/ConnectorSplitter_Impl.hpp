/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONNECTORSPLITTER_IMPL_HPP
#define MODEL_CONNECTORSPLITTER_IMPL_HPP

#include "Splitter_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API ConnectorSplitter_Impl : public Splitter_Impl
    {
     public:
      ConnectorSplitter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ConnectorSplitter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ConnectorSplitter_Impl(const ConnectorSplitter_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ConnectorSplitter_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      std::vector<openstudio::IdfObject> remove() override;

      virtual unsigned inletPort() const override;

      unsigned outletPort(unsigned branchIndex) const override;

      unsigned nextOutletPort() const override;

      //boost::optional<ModelObject> inletModelObject() const;
      //
      //virtual boost::optional<ModelObject> outletModelObject(unsigned branchIndex) const override;
      //
      //virtual boost::optional<ModelObject> lastOutletModelObject() const override;
      //
      //virtual std::vector<ModelObject> outletModelObjects() const override;
      //
      //virtual unsigned newOutletPortAfterBranch(unsigned branchIndex) override;
      //
      //virtual unsigned branchIndexForOutletModelObject( ModelObject modelObject ) const override;
      //
      //virtual unsigned nextBranchIndex() const override;
      //
      //virtual void removePortForBranch(unsigned branchIndex) override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      boost::optional<ModelObject> zoneEquipmentForBranch(int branchIndex);

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.Splitter");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONNECTORSPLITTER_IMPL_HPP
