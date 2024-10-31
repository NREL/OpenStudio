/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MIXER_IMPL_HPP
#define MODEL_MIXER_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class MODEL_API Mixer_Impl : public HVACComponent_Impl
    {
     public:
      Mixer_Impl(IddObjectType type, Model_Impl* model);

      Mixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Mixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Mixer_Impl(const Mixer_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~Mixer_Impl() override = default;

      virtual unsigned outletPort() const = 0;

      virtual unsigned inletPort(unsigned branchIndex) const = 0;

      virtual unsigned nextInletPort() const = 0;

      virtual boost::optional<ModelObject> outletModelObject() const;

      virtual boost::optional<ModelObject> inletModelObject(unsigned branchIndex) const;

      virtual boost::optional<ModelObject> lastInletModelObject() const;

      virtual std::vector<ModelObject> inletModelObjects() const;

      virtual unsigned newInletPortAfterBranch(unsigned branchIndex);

      virtual unsigned branchIndexForInletModelObject(const ModelObject& modelObject) const;

      virtual unsigned nextBranchIndex() const;

      virtual void removePortForBranch(unsigned branchIndex);

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      bool isRemovable() const override;

     private:
      REGISTER_LOGGER("openstudio.model.Mixer");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
