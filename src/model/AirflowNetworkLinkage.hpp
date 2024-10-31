/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKLINKAGE_HPP
#define MODEL_AIRFLOWNETWORKLINKAGE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class AirflowNetworkLinkage_Impl;
  }

  /** AirflowNetworkLinkage is the base class for AirflowNetwork linkage objects that link nodes in the pressure network.
 *
 */
  class MODEL_API AirflowNetworkLinkage : public ModelObject
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    AirflowNetworkLinkage(IddObjectType type, const Model& model);

    virtual ~AirflowNetworkLinkage() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkLinkage(const AirflowNetworkLinkage& other) = default;
    AirflowNetworkLinkage(AirflowNetworkLinkage&& other) = default;
    AirflowNetworkLinkage& operator=(const AirflowNetworkLinkage&) = default;
    AirflowNetworkLinkage& operator=(AirflowNetworkLinkage&&) = default;

    //@}
    /** @name Virtual Methods */
    //@{

    //@}
    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::AirflowNetworkLinkage_Impl;

    explicit AirflowNetworkLinkage(std::shared_ptr<detail::AirflowNetworkLinkage_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkLinkage");

    /// @endcond
  };

  using OptionalAirflowNetworkLinkage = boost::optional<AirflowNetworkLinkage>;

  using AirflowNetworkLinkageVector = std::vector<AirflowNetworkLinkage>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKLINKAGE_HPP
