/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKFAN_HPP
#define MODEL_AIRFLOWNETWORKFAN_HPP

#include "ModelAPI.hpp"
#include "AirflowNetworkComponent.hpp"

namespace openstudio {
namespace model {

  class StraightComponent;
  class FanConstantVolume;
  class FanVariableVolume;
  class FanOnOff;
  class FanSystemModel;

  namespace detail {

    class AirflowNetworkFan_Impl;
    class FanConstantVolume_Impl;
    class FanVariableVolume_Impl;
    class FanOnOff_Impl;
    class FanSystemModel_Impl;

  }  // namespace detail

  /** AirflowNetworkFan is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:Fan'. */
  class MODEL_API AirflowNetworkFan : public AirflowNetworkComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~AirflowNetworkFan() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkFan(const AirflowNetworkFan& other) = default;
    AirflowNetworkFan(AirflowNetworkFan&& other) = default;
    AirflowNetworkFan& operator=(const AirflowNetworkFan&) = default;
    AirflowNetworkFan& operator=(AirflowNetworkFan&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<FanConstantVolume> fanConstantVolume() const;
    boost::optional<FanVariableVolume> fanVariableVolume() const;
    boost::optional<FanOnOff> fanOnOff() const;
    boost::optional<FanSystemModel> fanSystemModel() const;

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkFan_Impl;

    explicit AirflowNetworkFan(std::shared_ptr<detail::AirflowNetworkFan_Impl> impl);

    AirflowNetworkFan(const Model& model, const Handle& handle);

    friend class detail::FanConstantVolume_Impl;
    friend class detail::FanVariableVolume_Impl;
    friend class detail::FanOnOff_Impl;
    friend class detail::FanSystemModel_Impl;

    void resetFan();

    friend class detail::AirflowNetworkFan_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkFan");
  };

  /** \relates AirflowNetworkFan*/
  using OptionalAirflowNetworkFan = boost::optional<AirflowNetworkFan>;

  /** \relates AirflowNetworkFan*/
  using AirflowNetworkFanVector = std::vector<AirflowNetworkFan>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKFAN_HPP
