/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_HPP
#define MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class AvailabilityManagerDifferentialThermostat_Impl;

  }  // namespace detail

  /** AvailabilityManagerDifferentialThermostat is a AvailabilityManager that wraps the OpenStudio IDD object 'OS:AvailabilityManager:DifferentialThermostat'. */
  class MODEL_API AvailabilityManagerDifferentialThermostat : public AvailabilityManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AvailabilityManagerDifferentialThermostat(const Model& model);

    virtual ~AvailabilityManagerDifferentialThermostat() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AvailabilityManagerDifferentialThermostat(const AvailabilityManagerDifferentialThermostat& other) = default;
    AvailabilityManagerDifferentialThermostat(AvailabilityManagerDifferentialThermostat&& other) = default;
    AvailabilityManagerDifferentialThermostat& operator=(const AvailabilityManagerDifferentialThermostat&) = default;
    AvailabilityManagerDifferentialThermostat& operator=(AvailabilityManagerDifferentialThermostat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> hotNode() const;

    boost::optional<Node> coldNode() const;

    double temperatureDifferenceOnLimit() const;

    double temperatureDifferenceOffLimit() const;

    //@}
    /** @name Setters */
    //@{

    bool setHotNode(const Node& node);

    void resetHotNode();

    bool setColdNode(const Node& node);

    void resetColdNode();

    bool setTemperatureDifferenceOnLimit(double temperatureDifferenceOnLimit);

    bool setTemperatureDifferenceOffLimit(double temperatureDifferenceOffLimit);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AvailabilityManagerDifferentialThermostat_Impl;

    explicit AvailabilityManagerDifferentialThermostat(std::shared_ptr<detail::AvailabilityManagerDifferentialThermostat_Impl> impl);

    friend class detail::AvailabilityManagerDifferentialThermostat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerDifferentialThermostat");
  };

  /** \relates AvailabilityManagerDifferentialThermostat*/
  using OptionalAvailabilityManagerDifferentialThermostat = boost::optional<AvailabilityManagerDifferentialThermostat>;

  /** \relates AvailabilityManagerDifferentialThermostat*/
  using AvailabilityManagerDifferentialThermostatVector = std::vector<AvailabilityManagerDifferentialThermostat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERDIFFERENTIALTHERMOSTAT_HPP
