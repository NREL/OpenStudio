/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONNECTORMIXER_IMPL_HPP
#define MODEL_CONNECTORMIXER_IMPL_HPP

#include "Mixer_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class MODEL_API ConnectorMixer_Impl : public Mixer_Impl
    {
     public:
      ConnectorMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ConnectorMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ConnectorMixer_Impl(const ConnectorMixer_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ConnectorMixer_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      std::vector<openstudio::IdfObject> remove() override;

      unsigned outletPort() const override;

      unsigned inletPort(unsigned branchIndex) const override;

      unsigned nextInletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.ConnectorMixer");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONNECTORMIXER_IMPL_HPP
