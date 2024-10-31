/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONECOOLING_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONECOOLING_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Node;

  namespace detail {

    class SetpointManagerSingleZoneCooling_Impl;

  }  // namespace detail

  /** SetpointManagerSingleZoneCooling is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:Cooling'. */
  class MODEL_API SetpointManagerSingleZoneCooling : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSingleZoneCooling(const Model& model);

    virtual ~SetpointManagerSingleZoneCooling() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneCooling(const SetpointManagerSingleZoneCooling& other) = default;
    SetpointManagerSingleZoneCooling(SetpointManagerSingleZoneCooling&& other) = default;
    SetpointManagerSingleZoneCooling& operator=(const SetpointManagerSingleZoneCooling&) = default;
    SetpointManagerSingleZoneCooling& operator=(SetpointManagerSingleZoneCooling&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double minimumSupplyAirTemperature() const;

    double maximumSupplyAirTemperature() const;

    boost::optional<ThermalZone> controlZone() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature);

    bool setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSingleZoneCooling_Impl;

    explicit SetpointManagerSingleZoneCooling(std::shared_ptr<detail::SetpointManagerSingleZoneCooling_Impl> impl);

    friend class detail::SetpointManagerSingleZoneCooling_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneCooling");
  };

  /** \relates SetpointManagerSingleZoneCooling*/
  using OptionalSetpointManagerSingleZoneCooling = boost::optional<SetpointManagerSingleZoneCooling>;

  /** \relates SetpointManagerSingleZoneCooling*/
  using SetpointManagerSingleZoneCoolingVector = std::vector<SetpointManagerSingleZoneCooling>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONECOOLING_HPP
