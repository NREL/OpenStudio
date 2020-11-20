/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent.hpp"

namespace openstudio {

namespace model {

  //class ControllerWaterCoil;

  namespace detail {
    class CoilCoolingWaterToAirHeatPumpEquationFit_Impl;
  }  // namespace detail

  /** CoilCoolingWaterToAirHeatPumpEquationFit is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:WaterToAirHeatPump:EquationFit'. */
  class MODEL_API CoilCoolingWaterToAirHeatPumpEquationFit : public WaterToAirComponent
  {
   public:
    /** @name Constructors and Destructors */

    /** Constructs a new CoilCoolingWaterToAirHeatPumpEquationFit object and places it inside the model.  The coil is fully initialized with all companion objects. */

    CoilCoolingWaterToAirHeatPumpEquationFit(const Model& model);

    virtual ~CoilCoolingWaterToAirHeatPumpEquationFit() {}

    static IddObjectType iddObjectType();

    /** @name Getters */

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateDefaulted() const;

    bool isRatedAirFlowRateAutosized() const;

    boost::optional<double> ratedWaterFlowRate() const;

    bool isRatedWaterFlowRateDefaulted() const;

    bool isRatedWaterFlowRateAutosized() const;

    boost::optional<double> ratedTotalCoolingCapacity() const;

    bool isRatedTotalCoolingCapacityDefaulted() const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> ratedSensibleCoolingCapacity() const;

    bool isRatedSensibleCoolingCapacityDefaulted() const;

    bool isRatedSensibleCoolingCapacityAutosized() const;

    double ratedCoolingCoefficientofPerformance() const;

    bool isRatedCoolingCoefficientofPerformanceDefaulted() const;

    double totalCoolingCapacityCoefficient1() const;

    double totalCoolingCapacityCoefficient2() const;

    double totalCoolingCapacityCoefficient3() const;

    double totalCoolingCapacityCoefficient4() const;

    double totalCoolingCapacityCoefficient5() const;

    double sensibleCoolingCapacityCoefficient1() const;

    double sensibleCoolingCapacityCoefficient2() const;

    double sensibleCoolingCapacityCoefficient3() const;

    double sensibleCoolingCapacityCoefficient4() const;

    double sensibleCoolingCapacityCoefficient5() const;

    double sensibleCoolingCapacityCoefficient6() const;

    double coolingPowerConsumptionCoefficient1() const;

    double coolingPowerConsumptionCoefficient2() const;

    double coolingPowerConsumptionCoefficient3() const;

    double coolingPowerConsumptionCoefficient4() const;

    double coolingPowerConsumptionCoefficient5() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    bool isNominalTimeforCondensateRemovaltoBeginDefaulted() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    bool isRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacityDefaulted() const;

    /** @name Setters */

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autosizeRatedAirFlowRate();

    bool setRatedWaterFlowRate(double ratedWaterFlowRate);

    void resetRatedWaterFlowRate();

    void autosizeRatedWaterFlowRate();

    bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

    void resetRatedTotalCoolingCapacity();

    void autosizeRatedTotalCoolingCapacity();

    bool setRatedSensibleCoolingCapacity(double ratedSensibleCoolingCapacity);

    void resetRatedSensibleCoolingCapacity();

    void autosizeRatedSensibleCoolingCapacity();

    bool setRatedCoolingCoefficientofPerformance(double ratedCoolingCoefficientofPerformance);

    bool setTotalCoolingCapacityCoefficient1(double totalCoolingCapacityCoefficient1);

    bool setTotalCoolingCapacityCoefficient2(double totalCoolingCapacityCoefficient2);

    bool setTotalCoolingCapacityCoefficient3(double totalCoolingCapacityCoefficient3);

    bool setTotalCoolingCapacityCoefficient4(double totalCoolingCapacityCoefficient4);

    bool setTotalCoolingCapacityCoefficient5(double totalCoolingCapacityCoefficient5);

    bool setSensibleCoolingCapacityCoefficient1(double sensibleCoolingCapacityCoefficient1);

    bool setSensibleCoolingCapacityCoefficient2(double sensibleCoolingCapacityCoefficient2);

    bool setSensibleCoolingCapacityCoefficient3(double sensibleCoolingCapacityCoefficient3);

    bool setSensibleCoolingCapacityCoefficient4(double sensibleCoolingCapacityCoefficient4);

    bool setSensibleCoolingCapacityCoefficient5(double sensibleCoolingCapacityCoefficient5);

    bool setSensibleCoolingCapacityCoefficient6(double sensibleCoolingCapacityCoefficient6);

    bool setCoolingPowerConsumptionCoefficient1(double coolingPowerConsumptionCoefficient1);

    bool setCoolingPowerConsumptionCoefficient2(double coolingPowerConsumptionCoefficient2);

    bool setCoolingPowerConsumptionCoefficient3(double coolingPowerConsumptionCoefficient3);

    bool setCoolingPowerConsumptionCoefficient4(double coolingPowerConsumptionCoefficient4);

    bool setCoolingPowerConsumptionCoefficient5(double coolingPowerConsumptionCoefficient5);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    void resetNominalTimeforCondensateRemovaltoBegin();

    bool
      setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    void resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedRatedWaterFlowRate() const;

    boost::optional<double> autosizedRatedTotalCoolingCapacity() const;

    boost::optional<double> autosizedRatedSensibleCoolingCapacity() const;

    //@}
   protected:
    /// @cond
    typedef detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl ImplType;

    explicit CoilCoolingWaterToAirHeatPumpEquationFit(std::shared_ptr<detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl> impl);

    friend class detail::CoilCoolingWaterToAirHeatPumpEquationFit_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpEquationFit");
  };

  /** \relates CoilCoolingWaterToAirHeatPumpEquationFit*/
  typedef boost::optional<CoilCoolingWaterToAirHeatPumpEquationFit> OptionalCoilCoolingWaterToAirHeatPumpEquationFit;

  /** \relates CoilCoolingWaterToAirHeatPumpEquationFit*/
  typedef std::vector<CoilCoolingWaterToAirHeatPumpEquationFit> CoilCoolingWaterToAirHeatPumpEquationFitVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_HPP
