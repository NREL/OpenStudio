/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STRAIGHTCOMPONENT_IMPL_HPP
#define MODEL_STRAIGHTCOMPONENT_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVAC;
  class AirflowNetworkEquivalentDuct;

  namespace detail {

    class MODEL_API StraightComponent_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      StraightComponent_Impl(IddObjectType type, Model_Impl* model);

      StraightComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      StraightComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      StraightComponent_Impl(const StraightComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~StraightComponent_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual boost::optional<ParentObject> parent() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      //@}
      /** @name Getters */
      //@{

      virtual unsigned inletPort() const = 0;

      virtual unsigned outletPort() const = 0;

      virtual boost::optional<ModelObject> inletModelObject() const;

      virtual boost::optional<ModelObject> outletModelObject() const;

      virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{
      virtual bool addToNode(Node& node) override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual bool removeFromLoop();

      virtual void disconnect() override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.StraightComponent");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STRAIGHTCOMPONENT_IMPL_HPP
