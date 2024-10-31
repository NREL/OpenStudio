/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_HPP
#define MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class FanZoneExhaust;
  class AirflowNetworkCrack;

  namespace detail {

    class FanZoneExhaust_Impl;
    class AirflowNetworkZoneExhaustFan_Impl;

  }  // namespace detail

  /** AirflowNetworkZoneExhaustFan is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkZoneExhaustFan'. */
  class MODEL_API AirflowNetworkZoneExhaustFan : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~AirflowNetworkZoneExhaustFan() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkZoneExhaustFan(const AirflowNetworkZoneExhaustFan& other) = default;
    AirflowNetworkZoneExhaustFan(AirflowNetworkZoneExhaustFan&& other) = default;
    AirflowNetworkZoneExhaustFan& operator=(const AirflowNetworkZoneExhaustFan&) = default;
    AirflowNetworkZoneExhaustFan& operator=(AirflowNetworkZoneExhaustFan&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the FanZoneExhaust object associated with this object, if any */
    boost::optional<FanZoneExhaust> fanZoneExhaust() const;
    /** Returns the AirflowNetworkCrack object associated with this object, if any */
    boost::optional<AirflowNetworkCrack> crack() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the AirflowNetworkCrack object associated with this object. */
    bool setCrack(const AirflowNetworkCrack& crack);
    /** Resets the AirflowNetworkCrack object for this object. */
    void resetCrack();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkZoneExhaustFan_Impl;

    AirflowNetworkZoneExhaustFan(const Model& model, const AirflowNetworkCrack& crack, const Handle& handle);

    explicit AirflowNetworkZoneExhaustFan(std::shared_ptr<detail::AirflowNetworkZoneExhaustFan_Impl> impl);

    void resetFanZoneExhaust();

    friend class detail::FanZoneExhaust_Impl;
    friend class detail::AirflowNetworkZoneExhaustFan_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkZoneExhaustFan");
  };

  /** \relates AirflowNetworkZoneExhaustFan*/
  using OptionalAirflowNetworkZoneExhaustFan = boost::optional<AirflowNetworkZoneExhaustFan>;

  /** \relates AirflowNetworkZoneExhaustFan*/
  using AirflowNetworkZoneExhaustFanVector = std::vector<AirflowNetworkZoneExhaustFan>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKZONEEXHAUSTFAN_HPP
