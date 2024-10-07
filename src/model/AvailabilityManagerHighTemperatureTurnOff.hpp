/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNOFF_HPP
#define MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNOFF_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class AvailabilityManagerHighTemperatureTurnOff_Impl;

  }  // namespace detail

  /** AvailabilityManagerHighTemperatureTurnOff is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:HighTemperatureTurnOff'. */
  class MODEL_API AvailabilityManagerHighTemperatureTurnOff : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerHighTemperatureTurnOff(const Model& model);

    virtual ~AvailabilityManagerHighTemperatureTurnOff() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerHighTemperatureTurnOff(const AvailabilityManagerHighTemperatureTurnOff& other) = default;
    AvailabilityManagerHighTemperatureTurnOff(AvailabilityManagerHighTemperatureTurnOff&& other) = default;
    AvailabilityManagerHighTemperatureTurnOff& operator=(const AvailabilityManagerHighTemperatureTurnOff&) = default;
    AvailabilityManagerHighTemperatureTurnOff& operator=(AvailabilityManagerHighTemperatureTurnOff&&) = default;

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
    using ImplType = detail::AvailabilityManagerHighTemperatureTurnOff_Impl;

    explicit AvailabilityManagerHighTemperatureTurnOff(std::shared_ptr<detail::AvailabilityManagerHighTemperatureTurnOff_Impl> impl);

    friend class detail::AvailabilityManagerHighTemperatureTurnOff_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerHighTemperatureTurnOff");
  };

  /** \relates AvailabilityManagerHighTemperatureTurnOff*/
  using OptionalAvailabilityManagerHighTemperatureTurnOff = boost::optional<AvailabilityManagerHighTemperatureTurnOff>;

  /** \relates AvailabilityManagerHighTemperatureTurnOff*/
  using AvailabilityManagerHighTemperatureTurnOffVector = std::vector<AvailabilityManagerHighTemperatureTurnOff>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERHIGHTEMPERATURETURNOFF_HPP
