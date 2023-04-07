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

#ifndef MODEL_GENERATORFUELCELLSTACKCOOLER_HPP
#define MODEL_GENERATORFUELCELLSTACKCOOLER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Connection;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellStackCooler_Impl;

  }  // namespace detail

  /** GeneratorFuelCellStackCooler is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:StackCooler'. */
  class MODEL_API GeneratorFuelCellStackCooler : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellStackCooler(const Model& model);

    virtual ~GeneratorFuelCellStackCooler() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellStackCooler(const GeneratorFuelCellStackCooler& other) = default;
    GeneratorFuelCellStackCooler(GeneratorFuelCellStackCooler&& other) = default;
    GeneratorFuelCellStackCooler& operator=(const GeneratorFuelCellStackCooler&) = default;
    GeneratorFuelCellStackCooler& operator=(GeneratorFuelCellStackCooler&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double nominalStackTemperature() const;

    double actualStackTemperature() const;

    double coefficientr0() const;

    double coefficientr1() const;

    double coefficientr2() const;

    double coefficientr3() const;

    double stackCoolantFlowRate() const;

    double stackCoolerUFactorTimesAreaValue() const;

    double fscogenAdjustmentFactor() const;

    double stackCogenerationExchangerArea() const;

    double stackCogenerationExchangerNominalFlowRate() const;

    double stackCogenerationExchangerNominalHeatTransferCoefficient() const;

    double stackCogenerationExchangerNominalHeatTransferCoefficientExponent() const;

    double stackCoolerPumpPower() const;

    double stackCoolerPumpHeatLossFraction() const;

    double stackAirCoolerFanCoefficientf0() const;

    double stackAirCoolerFanCoefficientf1() const;

    double stackAirCoolerFanCoefficientf2() const;

    // Return optional parent generator
    GeneratorFuelCell fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalStackTemperature(double nominalStackTemperature);

    void resetNominalStackTemperature();

    bool setActualStackTemperature(double actualStackTemperature);

    void resetActualStackTemperature();

    bool setCoefficientr0(double coefficientr0);

    void resetCoefficientr0();

    bool setCoefficientr1(double coefficientr1);

    void resetCoefficientr1();

    bool setCoefficientr2(double coefficientr2);

    void resetCoefficientr2();

    bool setCoefficientr3(double coefficientr3);

    void resetCoefficientr3();

    bool setStackCoolantFlowRate(double stackCoolantFlowRate);

    void resetStackCoolantFlowRate();

    bool setStackCoolerUFactorTimesAreaValue(double stackCoolerUFactorTimesAreaValue);

    void resetStackCoolerUFactorTimesAreaValue();

    bool setFscogenAdjustmentFactor(double fscogenAdjustmentFactor);

    void resetFscogenAdjustmentFactor();

    bool setStackCogenerationExchangerArea(double stackCogenerationExchangerArea);

    void resetStackCogenerationExchangerArea();

    bool setStackCogenerationExchangerNominalFlowRate(double stackCogenerationExchangerNominalFlowRate);

    void resetStackCogenerationExchangerNominalFlowRate();

    bool setStackCogenerationExchangerNominalHeatTransferCoefficient(double stackCogenerationExchangerNominalHeatTransferCoefficient);

    void resetStackCogenerationExchangerNominalHeatTransferCoefficient();

    bool setStackCogenerationExchangerNominalHeatTransferCoefficientExponent(double stackCogenerationExchangerNominalHeatTransferCoefficientExponent);

    void resetStackCogenerationExchangerNominalHeatTransferCoefficientExponent();

    bool setStackCoolerPumpPower(double stackCoolerPumpPower);

    void resetStackCoolerPumpPower();

    bool setStackCoolerPumpHeatLossFraction(double stackCoolerPumpHeatLossFraction);

    void resetStackCoolerPumpHeatLossFraction();

    bool setStackAirCoolerFanCoefficientf0(double stackAirCoolerFanCoefficientf0);

    void resetStackAirCoolerFanCoefficientf0();

    bool setStackAirCoolerFanCoefficientf1(double stackAirCoolerFanCoefficientf1);

    void resetStackAirCoolerFanCoefficientf1();

    bool setStackAirCoolerFanCoefficientf2(double stackAirCoolerFanCoefficientf2);

    void resetStackAirCoolerFanCoefficientf2();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellStackCooler_Impl;

    explicit GeneratorFuelCellStackCooler(std::shared_ptr<detail::GeneratorFuelCellStackCooler_Impl> impl);

    friend class detail::GeneratorFuelCellStackCooler_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellStackCooler");
  };

  /** \relates GeneratorFuelCellStackCooler*/
  using OptionalGeneratorFuelCellStackCooler = boost::optional<GeneratorFuelCellStackCooler>;

  /** \relates GeneratorFuelCellStackCooler*/
  using GeneratorFuelCellStackCoolerVector = std::vector<GeneratorFuelCellStackCooler>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLSTACKCOOLER_HPP
