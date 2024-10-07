/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGGAS_HPP
#define MODEL_COILHEATINGGAS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class AirflowNetworkEquivlentDuct;
  class Schedule;

  namespace detail {
    class CoilHeatingGas_Impl;
  }  // namespace detail

  /** CoilHeatingGas is an interface to the IDD object named "OS:Coil:Heating:Gas"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  OS:Coil:Heating:Gas objects in energyplus.
 *
 *  As of EnergyPlus Version 8.6 this object maps to Coil:Heating:Fuel.
 */
  class MODEL_API CoilHeatingGas : public StraightComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new CoilHeatingGas object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   *
   * Sets GasBurnerEffeciency to .8
   * Sets GasFields to AutoSize
   * Sets ParasiticElectricLoad to 0
   * Sets ParasiticGasLoad to 0
   */
    explicit CoilHeatingGas(const Model& model, Schedule& schedule);

    explicit CoilHeatingGas(const Model& model);

    virtual ~CoilHeatingGas() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingGas(const CoilHeatingGas& other) = default;
    CoilHeatingGas(CoilHeatingGas&& other) = default;
    CoilHeatingGas& operator=(const CoilHeatingGas&) = default;
    CoilHeatingGas& operator=(CoilHeatingGas&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters and Setters */
    //@{

    /**  Returns the Schedule referred to by the Availability Schedule field. **/
    Schedule availabilitySchedule() const;

    /** \deprecated */
    Schedule availableSchedule() const;

    /** Sets the Schedule referred to by the Availability Schedule field. **/
    bool setAvailabilitySchedule(Schedule& schedule);

    /** \deprecated */
    bool setAvailableSchedule(Schedule& schedule);

    /** Returns a list of valid fuel types. **/
    static std::vector<std::string> validFuelTypeValues();

    /** Gets the coil fuel type **/
    std::string fuelType() const;

    /** Sets the fuel type. **/
    bool setFuelType(const std::string& fuelType);

    /** Resets the fuel type to its default. **/
    void resetFuelType();

    /** Returns the value of the GasBurnerEfficiency field. **/
    double gasBurnerEfficiency() const;

    /** Sets the value of the GasBurnerEfficiency field. **/
    bool setGasBurnerEfficiency(double value);

    /** Returns the value of the ParasiticElectricLoad field. **/
    OS_DEPRECATED(3, 7, 0) double parasiticElectricLoad() const;

    /** Returns the value of the OnCycleParasiticElectricLoad field. **/
    double onCycleParasiticElectricLoad() const;

    /** Sets the value of the ParasiticElectricLoad field. **/
    OS_DEPRECATED(3, 7, 0) bool setParasiticElectricLoad(double value);

    /** Sets the value of the OnCycleParasiticElectricLoad field. **/
    bool setOnCycleParasiticElectricLoad(double value);

    /** Returns the value of the ParasiticGasLoad field. **/
    OS_DEPRECATED(3, 7, 0) double parasiticGasLoad() const;

    /** Returns the value of the OffCycleParasiticGasLoad field. **/
    double offCycleParasiticGasLoad() const;

    /** Sets the value of the ParasiticGasLoad field. **/
    OS_DEPRECATED(3, 7, 0) bool setParasiticGasLoad(double value);

    /** Sets the value of the OffCycleParasiticGasLoad field. **/
    bool setOffCycleParasiticGasLoad(double value);

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    bool setNominalCapacity(double nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    boost::optional<Curve> partLoadFractionCorrelationCurve() const;

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    void resetPartLoadFractionCorrelationCurve();

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    boost::optional<double> autosizedNominalCapacity() const;
    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::CoilHeatingGas_Impl;

    explicit CoilHeatingGas(std::shared_ptr<detail::CoilHeatingGas_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingGas");

    /// @endcond
  };

  /** \relates CoilHeatingGas */
  using OptionalCoilHeatingGas = boost::optional<CoilHeatingGas>;

  /** \relates CoilHeatingGas */
  using CoilHeatingGasVector = std::vector<CoilHeatingGas>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGGAS_HPP
