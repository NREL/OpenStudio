/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKOUTDOORAIRFLOW_HPP
#define MODEL_AIRFLOWNETWORKOUTDOORAIRFLOW_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class ControllerOutdoorAir;
  class AirflowNetworkCrack;

  namespace detail {

    class ControllerOutdoorAir_Impl;
    class AirflowNetworkOutdoorAirflow_Impl;

  }  // namespace detail

  /** AirflowNetworkOutdoorAirflow is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkOutdoorAirflow'. */
  class MODEL_API AirflowNetworkOutdoorAirflow : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~AirflowNetworkOutdoorAirflow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkOutdoorAirflow(const AirflowNetworkOutdoorAirflow& other) = default;
    AirflowNetworkOutdoorAirflow(AirflowNetworkOutdoorAirflow&& other) = default;
    AirflowNetworkOutdoorAirflow& operator=(const AirflowNetworkOutdoorAirflow&) = default;
    AirflowNetworkOutdoorAirflow& operator=(AirflowNetworkOutdoorAirflow&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the ControllerOutdoorAir object associated with this object, if any */
    boost::optional<ControllerOutdoorAir> controllerOutdoorAir() const;
    /** Returns the AirflowNetworkCrack object associated with this object, if any */
    boost::optional<AirflowNetworkCrack> crack() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the AirflowNetworkCrack object associated with this object. */
    bool setCrack(const AirflowNetworkCrack& crack);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkOutdoorAirflow_Impl;

    AirflowNetworkOutdoorAirflow(const Model& model, const AirflowNetworkCrack& crack, const Handle& handle);

    explicit AirflowNetworkOutdoorAirflow(std::shared_ptr<detail::AirflowNetworkOutdoorAirflow_Impl> impl);

    void resetControllerOutdoorAir();
    /** Resets the AirflowNetworkCrack object for this object. */
    void resetCrack();

    friend class detail::ControllerOutdoorAir_Impl;
    friend class detail::AirflowNetworkOutdoorAirflow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkOutdoorAirflow");
  };

  /** \relates AirflowNetworkOutdoorAirflow*/
  using OptionalAirflowNetworkOutdoorAirflow = boost::optional<AirflowNetworkOutdoorAirflow>;

  /** \relates AirflowNetworkOutdoorAirflow*/
  using AirflowNetworkOutdoorAirflowVector = std::vector<AirflowNetworkOutdoorAirflow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKOUTDOORAIRFLOW_HPP
