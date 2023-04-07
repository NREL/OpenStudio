/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGGAS_HPP
#define MODEL_COILHEATINGGAS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

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

    virtual ~CoilHeatingGas() = default;
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
    double parasiticElectricLoad() const;

    /** Sets the value of the ParasiticElectricLoad field. **/
    bool setParasiticElectricLoad(double value);

    /** Returns the value of the ParasiticGasLoad field. **/
    double parasiticGasLoad() const;

    /** Sets the value of the ParasiticGasLoad field. **/
    bool setParasiticGasLoad(double value);

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
