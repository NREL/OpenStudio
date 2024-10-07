/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTOAIRCOMPONENT_IMPL_HPP
#define MODEL_AIRTOAIRCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVAC;

  namespace detail {

    class MODEL_API AirToAirComponent_Impl : public HVACComponent_Impl
    {
     public:
      AirToAirComponent_Impl(IddObjectType type, Model_Impl* model);

      AirToAirComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirToAirComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirToAirComponent_Impl(const AirToAirComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~AirToAirComponent_Impl() override = default;

      virtual unsigned primaryAirInletPort() const = 0;

      virtual unsigned primaryAirOutletPort() const = 0;

      virtual unsigned secondaryAirInletPort() const = 0;

      virtual unsigned secondaryAirOutletPort() const = 0;

      boost::optional<ModelObject> primaryAirInletModelObject() const;

      boost::optional<ModelObject> primaryAirOutletModelObject() const;

      boost::optional<ModelObject> secondaryAirInletModelObject() const;

      boost::optional<ModelObject> secondaryAirOutletModelObject() const;

      bool addToNode(Node& node) override;

      std::vector<openstudio::IdfObject> remove() override;

      ModelObject clone(Model model) const override;

     private:
      REGISTER_LOGGER("openstudio.model.AirToAirComponent");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTOAIRCOMPONENT_IMPL_HPP
