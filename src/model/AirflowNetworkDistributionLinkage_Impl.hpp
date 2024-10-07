/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkLinkage_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class AirflowNetworkNode;
  class AirflowNetworkComponent;
  class ThermalZone;

  namespace detail {

    /** AirflowNetworkDistributionLinkage_Impl is a AirflowNetworkLinkage_Impl that is the implementation class for AirflowNetworkDistributionLinkage.*/
    class MODEL_API AirflowNetworkDistributionLinkage_Impl : public AirflowNetworkLinkage_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkDistributionLinkage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkDistributionLinkage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkDistributionLinkage_Impl(const AirflowNetworkDistributionLinkage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkDistributionLinkage_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      // TODO: Check return type. From object lists, some candidates are: AirflowNetworkNodeAndZone.
      AirflowNetworkNode node1() const;

      // TODO: Check return type. From object lists, some candidates are: AirflowNetworkNodeAndZone.
      AirflowNetworkNode node2() const;

      // TODO: Check return type. From object lists, some candidates are: AirflowNetworkComponent.
      AirflowNetworkComponent component() const;

      // TODO: Check return type. From object lists, some candidates are: Zone.
      boost::optional<ThermalZone> thermalZone() const;

      //@}
      /** @name Setters */
      //@{

      /*
    template <typename T> bool setNode1(const T& airflowNetworkNode)
    {
      bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::Node1Name, airflowNetworkNode.handle());
      return result;
    }

    template <typename T> bool setNode2(const T& airflowNetworkNode)
    {
      bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::Node2Name, airflowNetworkNode.handle());
      return result;
    }

    template <typename T> bool setComponent(const T& airflowNetworkComponent)
    {
      bool result = setPointer(OS_AirflowNetworkDistributionLinkageFields::ComponentName, airflowNetworkComponent.handle());
      return result;
    }
    */

      bool setNode1(const AirflowNetworkNode& airflowNetworkNode);

      bool setNode2(const AirflowNetworkNode& airflowNetworkNode);

      bool setComponent(const AirflowNetworkComponent& airflowNetworkComponent);

      // TODO: Check argument type. From object lists, some candidates are: Zone.
      bool setThermalZone(const ThermalZone& zone);

      void resetThermalZone();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkDistributionLinkage");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<AirflowNetworkNode> optionalNode1() const;
      boost::optional<AirflowNetworkNode> optionalNode2() const;
      boost::optional<AirflowNetworkComponent> optionalComponent() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_IMPL_HPP
