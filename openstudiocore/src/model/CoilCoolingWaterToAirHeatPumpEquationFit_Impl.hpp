/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
#define MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP

#include "ModelAPI.hpp"
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API CoilCoolingWaterToAirHeatPumpEquationFit_Impl : public WaterToAirComponent_Impl
  {

   public:
    /** @name Constructors and Destructors */

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const IdfObject& idfObject,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    CoilCoolingWaterToAirHeatPumpEquationFit_Impl(const CoilCoolingWaterToAirHeatPumpEquationFit_Impl& other,
                                                  Model_Impl* model,
                                                  bool keepHandle);

    virtual ~CoilCoolingWaterToAirHeatPumpEquationFit_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<IdfObject> remove() override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //virtual bool addToNode(Node & node);

    virtual unsigned airInletPort() const override;

    virtual unsigned airOutletPort() const override;

    virtual unsigned waterInletPort() const override;

    virtual unsigned waterOutletPort() const override;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

    //@}
    /** @name Getters */
    //@{

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

  boost::optional<double> autosizedRatedAirFlowRate() const ;

  boost::optional<double> autosizedRatedWaterFlowRate() const ;

  boost::optional<double> autosizedRatedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedRatedSensibleCoolingCapacity() const ;

  virtual void autosize() override;

  virtual void applySizingValues() override;

    //@}
    /** @name Setters */
    //@{

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autosizeRatedAirFlowRate();

    bool setRatedWaterFlowRate(boost::optional<double> ratedWaterFlowRate);

    void resetRatedWaterFlowRate();

    void autosizeRatedWaterFlowRate();

    bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

    void resetRatedTotalCoolingCapacity();

    void autosizeRatedTotalCoolingCapacity();

    bool setRatedSensibleCoolingCapacity(boost::optional<double> ratedSensibleCoolingCapacity);

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

    bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    void resetRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterToAirHeatPumpEquationFit");

    virtual boost::optional<HVACComponent> containingHVACComponent() const override;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGWATERTOAIRHEATPUMPEQUATIONFIT_IMPL_HPP
