/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKCOMPONENT_IMPL_HPP
#define MODEL_AIRFLOWNETWORKCOMPONENT_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API AirflowNetworkComponent_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkComponent_Impl(IddObjectType type, Model_Impl* model);

      AirflowNetworkComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkComponent_Impl(const AirflowNetworkComponent_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~AirflowNetworkComponent_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      //virtual ModelObject clone(Model model) const override;

      //virtual boost::optional<ParentObject> parent() const override;

      //virtual std::vector<ModelObject> children() const override;

      //virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent> & prev) override;

      //** Returns the optional ModelObject that this component represents. **/
      virtual boost::optional<ModelObject> componentModelObject() const;

      //@}
      /** @name Getters */
      //@{

      // unsigned inletPort() const;

      // unsigned outletPort() const;

      //virtual boost::optional<ModelObject> inletModelObject() const;

      //virtual boost::optional<ModelObject> outletModelObject() const;

      //virtual boost::optional<AirLoopHVAC> airLoopHVAC() const override;

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{
      //virtual bool addToNode(Node & node) override;

      //virtual std::vector<openstudio::IdfObject> remove() override;

      //bool removeFromLoop();

      //void disconnect() override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkComponent");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKCOMPONENT_IMPL_HPP
