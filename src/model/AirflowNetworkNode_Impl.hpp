/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKNODE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKNODE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class AirLoopHVAC;

  namespace detail {

    class MODEL_API AirflowNetworkNode_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkNode_Impl(IddObjectType type, Model_Impl* model);

      AirflowNetworkNode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkNode_Impl(const AirflowNetworkNode_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~AirflowNetworkNode_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      //virtual ModelObject clone(Model model) const override;

      //virtual boost::optional<ParentObject> parent() const override;

      //virtual std::vector<ModelObject> children() const override;

      //virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent> & prev) override;

      //@}
      /** @name Getters */
      //@{

      //virtual unsigned inletPort() = 0;

      //virtual unsigned outletPort() = 0;

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
      REGISTER_LOGGER("openstudio.model.AirflowNetworkNode");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKNODE_IMPL_HPP
