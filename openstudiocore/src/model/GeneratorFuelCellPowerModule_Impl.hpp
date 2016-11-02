/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLPOWERMODULE_IMPL_HPP
#define MODEL_GENERATORFUELCELLPOWERMODULE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class QuadraticCurves;
class ThermalZone;
class QuadraticCurves;
class Connection;
class Connection;

namespace detail {

  /** GeneratorFuelCellPowerModule_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellPowerModule.*/
  class MODEL_API GeneratorFuelCellPowerModule_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    GeneratorFuelCellPowerModule_Impl(const IdfObject& idfObject,
                                      Model_Impl* model,
                                      bool keepHandle);

    GeneratorFuelCellPowerModule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    GeneratorFuelCellPowerModule_Impl(const GeneratorFuelCellPowerModule_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    virtual ~GeneratorFuelCellPowerModule_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<std::string> efficiencyCurveMode() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    CurveQuadratic efficiencyCurve() const;

    boost::optional<double> nominalEfficiency() const;

    boost::optional<double> nominalElectricalPower() const;

    boost::optional<double> numberofStopsatStartofSimulation() const;

    boost::optional<double> cyclingPerformanceDegradationCoefficient() const;

    boost::optional<double> numberofRunHoursatBeginningofSimulation() const;

    boost::optional<double> accumulatedRunTimeDegradationCoefficient() const;

    boost::optional<double> runTimeDegradationInitiationTimeThreshold() const;

    boost::optional<double> powerUpTransientLimit() const;

    boost::optional<double> powerDownTransientLimit() const;

    boost::optional<double> startUpTime() const;

    boost::optional<double> startUpFuel() const;

    boost::optional<double> startUpElectricityConsumption() const;

    boost::optional<double> startUpElectricityProduced() const;

    boost::optional<double> shutDownTime() const;

    boost::optional<double> shutDownFuel() const;

    boost::optional<double> shutDownElectricityConsumption() const;

    boost::optional<double> ancillaryElectricityConstantTerm() const;

    boost::optional<double> ancillaryElectricityLinearTerm() const;

    boost::optional<std::string> skinLossCalculationMode() const;

    // TODO: Check return type. From object lists, some candidates are: Zone.
    boost::optional<ThermalZone> zone() const;

    boost::optional<double> skinLossRadiativeFraction() const;

    boost::optional<double> constantSkinLossRate() const;

    boost::optional<double> skinLossUFactorTimesAreaTerm() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    boost::optional<CurveQuadratic> skinLossQuadraticCurve() const;

    boost::optional<double> dilutionAirFlowRate() const;

    boost::optional<double> stackHeatlosstoDilutionAir() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> dilutionInletAirNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> dilutionOutletAirNode() const;

    boost::optional<double> minimumOperatingPoint() const;

    boost::optional<double> maximumOperatingPoint() const;

    //@}
    /** @name Setters */
    //@{

    bool setEfficiencyCurveMode(const std::string& efficiencyCurveMode);

    void resetEfficiencyCurveMode();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setEfficiencyCurve(const CurveQuadratic& quadraticCurve);

    void setNominalEfficiency(double nominalEfficiency);

    void resetNominalEfficiency();

    void setNominalElectricalPower(double nominalElectricalPower);

    void resetNominalElectricalPower();

    void setNumberofStopsatStartofSimulation(double numberofStopsatStartofSimulation);

    void resetNumberofStopsatStartofSimulation();

    void setCyclingPerformanceDegradationCoefficient(double cyclingPerformanceDegradationCoefficient);

    void resetCyclingPerformanceDegradationCoefficient();

    void setNumberofRunHoursatBeginningofSimulation(double numberofRunHoursatBeginningofSimulation);

    void resetNumberofRunHoursatBeginningofSimulation();

    void setAccumulatedRunTimeDegradationCoefficient(double accumulatedRunTimeDegradationCoefficient);

    void resetAccumulatedRunTimeDegradationCoefficient();

    void setRunTimeDegradationInitiationTimeThreshold(double runTimeDegradationInitiationTimeThreshold);

    void resetRunTimeDegradationInitiationTimeThreshold();

    void setPowerUpTransientLimit(double powerUpTransientLimit);

    void resetPowerUpTransientLimit();

    void setPowerDownTransientLimit(double powerDownTransientLimit);

    void resetPowerDownTransientLimit();

    void setStartUpTime(double startUpTime);

    void resetStartUpTime();

    void setStartUpFuel(double startUpFuel);

    void resetStartUpFuel();

    void setStartUpElectricityConsumption(double startUpElectricityConsumption);

    void resetStartUpElectricityConsumption();

    void setStartUpElectricityProduced(double startUpElectricityProduced);

    void resetStartUpElectricityProduced();

    void setShutDownTime(double shutDownTime);

    void resetShutDownTime();

    void setShutDownFuel(double shutDownFuel);

    void resetShutDownFuel();

    void setShutDownElectricityConsumption(double shutDownElectricityConsumption);

    void resetShutDownElectricityConsumption();

    void setAncillaryElectricityConstantTerm(double ancillaryElectricityConstantTerm);

    void resetAncillaryElectricityConstantTerm();

    void setAncillaryElectricityLinearTerm(double ancillaryElectricityLinearTerm);

    void resetAncillaryElectricityLinearTerm();

    bool setSkinLossCalculationMode(const std::string& skinLossCalculationMode);

    void resetSkinLossCalculationMode();

    // TODO: Check argument type. From object lists, some candidates are: Zone.
    bool setZone(const ThermalZone& zone);

    void resetZone();

    bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

    void resetSkinLossRadiativeFraction();

    void setConstantSkinLossRate(double constantSkinLossRate);

    void resetConstantSkinLossRate();

    void setSkinLossUFactorTimesAreaTerm(double skinLossUFactorTimesAreaTerm);

    void resetSkinLossUFactorTimesAreaTerm();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setSkinLossQuadraticCurve(const CurveQuadratic& quadraticCurves);

    void resetSkinLossQuadraticCurve();

    void setDilutionAirFlowRate(double dilutionAirFlowRate);

    void resetDilutionAirFlowRate();

    void setStackHeatlosstoDilutionAir(double stackHeatlosstoDilutionAir);

    void resetStackHeatlosstoDilutionAir();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setDilutionInletAirNode(const Connection& connection);

    void resetDilutionInletAirNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setDilutionOutletAirNode(const Connection& connection);

    void resetDilutionOutletAirNode();

    void setMinimumOperatingPoint(double minimumOperatingPoint);

    void resetMinimumOperatingPoint();

    void setMaximumOperatingPoint(double maximumOperatingPoint);

    void resetMaximumOperatingPoint();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellPowerModule");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CurveQuadratic> optionalEfficiencyCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GENERATORFUELCELLPOWERMODULE_IMPL_HPP

