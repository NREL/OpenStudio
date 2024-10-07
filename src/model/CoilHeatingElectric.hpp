/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGELECTRIC_HPP
#define MODEL_COILHEATINGELECTRIC_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Node;
  class AirflowNewtorkEquivalentDuct;

  namespace detail {

    class CoilHeatingElectric_Impl;

  }  // namespace detail

  /** CoilHeatingElectric is a StraightComponent that wraps the OpenStudio IDD
 *  object 'OS_Coil_Heating_Electric'. */
  class MODEL_API CoilHeatingElectric : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingElectric(const Model& model, Schedule& schedule);

    CoilHeatingElectric(const Model& model);

    virtual ~CoilHeatingElectric() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingElectric(const CoilHeatingElectric& other) = default;
    CoilHeatingElectric(CoilHeatingElectric&& other) = default;
    CoilHeatingElectric& operator=(const CoilHeatingElectric&) = default;
    CoilHeatingElectric& operator=(CoilHeatingElectric&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double efficiency() const;

    bool isEfficiencyDefaulted() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityDefaulted() const;

    bool isNominalCapacityAutosized() const;

    boost::optional<Node> temperatureSetpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setEfficiency(double efficiency);

    void resetEfficiency();

    bool setNominalCapacity(double nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    bool setTemperatureSetpointNode(Node& temperatureSetpointNode);

    void resetTemperatureSetpointNode();

    //@}

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond

    using ImplType = detail::CoilHeatingElectric_Impl;

    friend class detail::CoilHeatingElectric_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    explicit CoilHeatingElectric(std::shared_ptr<detail::CoilHeatingElectric_Impl> impl);

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingElectric");
  };

  /** \relates CoilHeatingElectric*/
  using OptionalCoilHeatingElectric = boost::optional<CoilHeatingElectric>;

  /** \relates CoilHeatingElectric*/
  using CoilHeatingElectricVector = std::vector<CoilHeatingElectric>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRIC_HPP
