/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_HPP
#define MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkLinkage.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class AirflowNetworkNode;
  class AirflowNetworkDistributionNode;
  class AirflowNetworkFan;
  class AirflowNetworkComponent;
  class ThermalZone;

  namespace detail {

    class AirflowNetworkDistributionLinkage_Impl;

  }  // namespace detail

  /** AirflowNetworkDistributionLinkage is a AirflowNetworkLinkage that wraps the OpenStudio IDD object 'OS:AirflowNetworkDistributionLinkage'. */
  class MODEL_API AirflowNetworkDistributionLinkage : public AirflowNetworkLinkage
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirflowNetworkDistributionLinkage(const Model& model, const AirflowNetworkNode& node1, const AirflowNetworkNode& node2,
                                      const AirflowNetworkComponent& component);

    virtual ~AirflowNetworkDistributionLinkage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDistributionLinkage(const AirflowNetworkDistributionLinkage& other) = default;
    AirflowNetworkDistributionLinkage(AirflowNetworkDistributionLinkage&& other) = default;
    AirflowNetworkDistributionLinkage& operator=(const AirflowNetworkDistributionLinkage&) = default;
    AirflowNetworkDistributionLinkage& operator=(AirflowNetworkDistributionLinkage&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: AirflowNetworkNodeAndZone.
    AirflowNetworkNode node1() const;

    // TODO: Check return type. From object lists, some candidates are: AirflowNetworkNodeAndZone.
    AirflowNetworkNode node2() const;

    // TODO: Check return type. From object lists, some candidates are: AirflowNetworkComponent.
    AirflowNetworkComponent component() const;

    boost::optional<ThermalZone> thermalZone() const;

    //@}
    /** @name Setters */
    //@{

    bool setNode1(const AirflowNetworkNode& airflowNetworkNode);

    bool setNode2(const AirflowNetworkNode& airflowNetworkNode);

    bool setComponent(const AirflowNetworkComponent& airflowNetworkComponent);

    bool setThermalZone(const ThermalZone& zone);

    void resetThermalZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkDistributionLinkage_Impl;

    explicit AirflowNetworkDistributionLinkage(std::shared_ptr<detail::AirflowNetworkDistributionLinkage_Impl> impl);

    friend class detail::AirflowNetworkDistributionLinkage_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDistributionLinkage");
  };

  /** \relates AirflowNetworkDistributionLinkage*/
  using OptionalAirflowNetworkDistributionLinkage = boost::optional<AirflowNetworkDistributionLinkage>;

  /** \relates AirflowNetworkDistributionLinkage*/
  using AirflowNetworkDistributionLinkageVector = std::vector<AirflowNetworkDistributionLinkage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDISTRIBUTIONLINKAGE_HPP
