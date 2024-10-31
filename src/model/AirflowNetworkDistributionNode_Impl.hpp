/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode_Impl.hpp"

namespace openstudio {
namespace model {

  class HVACComponent;

  namespace detail {

    /** AirflowNetworkDistributionNode_Impl is a AirflowNetworkNode_Impl that is the implementation class for AirflowNetworkDistributionNode.*/
    class MODEL_API AirflowNetworkDistributionNode_Impl : public AirflowNetworkNode_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkDistributionNode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkDistributionNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkDistributionNode_Impl(const AirflowNetworkDistributionNode_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkDistributionNode_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{
      /*
    template <typename T> boost::optional<typename std::enable_if<std::is_same<AirLoopHVACZoneMixer, T>::value ||
      std::is_same<AirLoopHVACZoneSplitter, T>::value ||
      std::is_same<AirLoopHVACOutdoorAirSystem, T>::value || //::type> component() const
      //OAMixerOutdoorAirStreamNode
      //OutdoorAir : NodeList
      //OutdoorAir : Node
      std::is_same<Node, T>::value, T>::type> component() const
    {
      return getObject<ModelObject>().getModelObjectTarget<T>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
    }
    */

      boost::optional<HVACComponent> component() const;

      boost::optional<Node> node() const;

      boost::optional<AirLoopHVACZoneMixer> airLoopHVACZoneMixer() const;

      boost::optional<AirLoopHVACZoneSplitter> airLoopHVACZoneSplitter() const;

      boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;

      double nodeHeight() const;

      bool isNodeHeightDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      void setComponentNameorNodeName(const std::string& componentNameorNodeName);

      void resetComponent();

      void setNodeHeight(double nodeHeight);

      void resetNodeHeight();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkDistributionNode");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_IMPL_HPP
