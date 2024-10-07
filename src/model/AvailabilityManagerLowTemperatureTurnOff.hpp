/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNOFF_HPP
#define MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNOFF_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Schedule;

  namespace detail {

    class AvailabilityManagerLowTemperatureTurnOff_Impl;

  }  // namespace detail

  /** AvailabilityManagerLowTemperatureTurnOff is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:LowTemperatureTurnOff'. */
  class MODEL_API AvailabilityManagerLowTemperatureTurnOff : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerLowTemperatureTurnOff(const Model& model);

    virtual ~AvailabilityManagerLowTemperatureTurnOff() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerLowTemperatureTurnOff(const AvailabilityManagerLowTemperatureTurnOff& other) = default;
    AvailabilityManagerLowTemperatureTurnOff(AvailabilityManagerLowTemperatureTurnOff&& other) = default;
    AvailabilityManagerLowTemperatureTurnOff& operator=(const AvailabilityManagerLowTemperatureTurnOff&) = default;
    AvailabilityManagerLowTemperatureTurnOff& operator=(AvailabilityManagerLowTemperatureTurnOff&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> sensorNode() const;

    double temperature() const;

    Schedule applicabilitySchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSensorNode(const Node& node);

    void resetSensorNode();

    bool setTemperature(double temperature);

    bool setApplicabilitySchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerLowTemperatureTurnOff_Impl;

    explicit AvailabilityManagerLowTemperatureTurnOff(std::shared_ptr<detail::AvailabilityManagerLowTemperatureTurnOff_Impl> impl);

    friend class detail::AvailabilityManagerLowTemperatureTurnOff_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerLowTemperatureTurnOff");
  };

  /** \relates AvailabilityManagerLowTemperatureTurnOff*/
  using OptionalAvailabilityManagerLowTemperatureTurnOff = boost::optional<AvailabilityManagerLowTemperatureTurnOff>;

  /** \relates AvailabilityManagerLowTemperatureTurnOff*/
  using AvailabilityManagerLowTemperatureTurnOffVector = std::vector<AvailabilityManagerLowTemperatureTurnOff>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERLOWTEMPERATURETURNOFF_HPP
