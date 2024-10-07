/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNON_HPP
#define MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNON_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class AvailabilityManagerHighTemperatureTurnOn_Impl;

  }  // namespace detail

  /** AvailabilityManagerHighTemperatureTurnOn is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:HighTemperatureTurnOn'. */
  class MODEL_API AvailabilityManagerHighTemperatureTurnOn : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerHighTemperatureTurnOn(const Model& model);

    virtual ~AvailabilityManagerHighTemperatureTurnOn() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerHighTemperatureTurnOn(const AvailabilityManagerHighTemperatureTurnOn& other) = default;
    AvailabilityManagerHighTemperatureTurnOn(AvailabilityManagerHighTemperatureTurnOn&& other) = default;
    AvailabilityManagerHighTemperatureTurnOn& operator=(const AvailabilityManagerHighTemperatureTurnOn&) = default;
    AvailabilityManagerHighTemperatureTurnOn& operator=(AvailabilityManagerHighTemperatureTurnOn&&) = default;

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
    using ImplType = detail::AvailabilityManagerHighTemperatureTurnOn_Impl;

    explicit AvailabilityManagerHighTemperatureTurnOn(std::shared_ptr<detail::AvailabilityManagerHighTemperatureTurnOn_Impl> impl);

    friend class detail::AvailabilityManagerHighTemperatureTurnOn_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerHighTemperatureTurnOn");
  };

  /** \relates AvailabilityManagerHighTemperatureTurnOn*/
  using OptionalAvailabilityManagerHighTemperatureTurnOn = boost::optional<AvailabilityManagerHighTemperatureTurnOn>;

  /** \relates AvailabilityManagerHighTemperatureTurnOn*/
  using AvailabilityManagerHighTemperatureTurnOnVector = std::vector<AvailabilityManagerHighTemperatureTurnOn>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNON_HPP
