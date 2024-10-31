/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNON_HPP
#define MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNON_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class AvailabilityManagerLowTemperatureTurnOn_Impl;

  }  // namespace detail

  /** AvailabilityManagerLowTemperatureTurnOn is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:LowTemperatureTurnOn'. */
  class MODEL_API AvailabilityManagerLowTemperatureTurnOn : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerLowTemperatureTurnOn(const Model& model);

    virtual ~AvailabilityManagerLowTemperatureTurnOn() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerLowTemperatureTurnOn(const AvailabilityManagerLowTemperatureTurnOn& other) = default;
    AvailabilityManagerLowTemperatureTurnOn(AvailabilityManagerLowTemperatureTurnOn&& other) = default;
    AvailabilityManagerLowTemperatureTurnOn& operator=(const AvailabilityManagerLowTemperatureTurnOn&) = default;
    AvailabilityManagerLowTemperatureTurnOn& operator=(AvailabilityManagerLowTemperatureTurnOn&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> sensorNode() const;

    double temperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setSensorNode(const Node& node);

    void resetSensorNode();

    bool setTemperature(double temperature);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerLowTemperatureTurnOn_Impl;

    explicit AvailabilityManagerLowTemperatureTurnOn(std::shared_ptr<detail::AvailabilityManagerLowTemperatureTurnOn_Impl> impl);

    friend class detail::AvailabilityManagerLowTemperatureTurnOn_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerLowTemperatureTurnOn");
  };

  /** \relates AvailabilityManagerLowTemperatureTurnOn*/
  using OptionalAvailabilityManagerLowTemperatureTurnOn = boost::optional<AvailabilityManagerLowTemperatureTurnOn>;

  /** \relates AvailabilityManagerLowTemperatureTurnOn*/
  using AvailabilityManagerLowTemperatureTurnOnVector = std::vector<AvailabilityManagerLowTemperatureTurnOn>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNON_HPP
