/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_GENERATORMICROTURBINE_IMPL_HPP
#define MODEL_GENERATORMICROTURBINE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class BiquadraticCurves;
class QuadraticCubicCurves;
class QuadraticCubicCurves;
class QuadraticCurves;
class Connection;
class Connection;
class BiquadraticCurves;
class BicubicBiquadraticCurves;
class QuadraticCubicCurves;
class QuadraticCurves;
class QuadraticCurves;
class Connection;
class Connection;
class QuadraticCubicCurves;
class QuadraticCubicCurves;
class QuadraticCubicCurves;
class QuadraticCubicCurves;

namespace detail {

  /** GeneratorMicroTurbine_Impl is a Generator_Impl that is the implementation class for GeneratorMicroTurbine.*/
  class MODEL_API GeneratorMicroTurbine_Impl : public Generator_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    GeneratorMicroTurbine_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

    GeneratorMicroTurbine_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    GeneratorMicroTurbine_Impl(const GeneratorMicroTurbine_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~GeneratorMicroTurbine_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    double referenceElectricalPowerOutput() const;

    double minimumFullLoadElectricalPowerOutput() const;

    bool isMinimumFullLoadElectricalPowerOutputDefaulted() const;

    boost::optional<double> maximumFullLoadElectricalPowerOutput() const;

    double referenceElectricalEfficiencyUsingLowerHeatingValue() const;

    double referenceCombustionAirInletTemperature() const;

    bool isReferenceCombustionAirInletTemperatureDefaulted() const;

    double referenceCombustionAirInletHumidityRatio() const;

    bool isReferenceCombustionAirInletHumidityRatioDefaulted() const;

    double referenceElevation() const;

    bool isReferenceElevationDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    BiquadraticCurves electricalPowerFunctionofTemperatureandElevationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves.
    QuadraticCubicCurves electricalEfficiencyFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves.
    QuadraticCubicCurves electricalEfficiencyFunctionofPartLoadRatioCurve() const;

    std::string fuelType() const;

    bool isFuelTypeDefaulted() const;

    double fuelHigherHeatingValue() const;

    bool isFuelHigherHeatingValueDefaulted() const;

    double fuelLowerHeatingValue() const;

    bool isFuelLowerHeatingValueDefaulted() const;

    double standbyPower() const;

    bool isStandbyPowerDefaulted() const;

    double ancillaryPower() const;

    bool isAncillaryPowerDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    boost::optional<QuadraticCurves> ancillaryPowerFunctionofFuelInputCurve() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> heatRecoveryWaterInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> heatRecoveryWaterOutletNode() const;

    double referenceThermalEfficiencyUsingLowerHeatValue() const;

    bool isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const;

    boost::optional<double> referenceInletWaterTemperature() const;

    std::string heatRecoveryWaterFlowOperatingMode() const;

    bool isHeatRecoveryWaterFlowOperatingModeDefaulted() const;

    boost::optional<double> referenceHeatRecoveryWaterFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    boost::optional<BiquadraticCurves> heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BicubicBiquadraticCurves, BiVariateTables.
    boost::optional<BicubicBiquadraticCurves> thermalEfficiencyFunctionofTemperatureandElevationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<QuadraticCubicCurves> heatRecoveryRateFunctionofPartLoadRatioCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    boost::optional<QuadraticCurves> heatRecoveryRateFunctionofInletWaterTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    boost::optional<QuadraticCurves> heatRecoveryRateFunctionofWaterFlowRateCurve() const;

    double minimumHeatRecoveryWaterFlowRate() const;

    bool isMinimumHeatRecoveryWaterFlowRateDefaulted() const;

    double maximumHeatRecoveryWaterFlowRate() const;

    bool isMaximumHeatRecoveryWaterFlowRateDefaulted() const;

    boost::optional<double> maximumHeatRecoveryWaterTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> combustionAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> combustionAirOutletNode() const;

    boost::optional<double> referenceExhaustAirMassFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<QuadraticCubicCurves> exhaustAirFlowRateFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<QuadraticCubicCurves> exhaustAirFlowRateFunctionofPartLoadRatioCurve() const;

    boost::optional<double> nominalExhaustAirOutletTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<QuadraticCubicCurves> exhaustAirTemperatureFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<QuadraticCubicCurves> exhaustAirTemperatureFunctionofPartLoadRatioCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceElectricalPowerOutput(double referenceElectricalPowerOutput);

    bool setMinimumFullLoadElectricalPowerOutput(double minimumFullLoadElectricalPowerOutput);

    void resetMinimumFullLoadElectricalPowerOutput();

    bool setMaximumFullLoadElectricalPowerOutput(boost::optional<double> maximumFullLoadElectricalPowerOutput);

    void resetMaximumFullLoadElectricalPowerOutput();

    bool setReferenceElectricalEfficiencyUsingLowerHeatingValue(double referenceElectricalEfficiencyUsingLowerHeatingValue);

    void setReferenceCombustionAirInletTemperature(double referenceCombustionAirInletTemperature);

    void resetReferenceCombustionAirInletTemperature();

    bool setReferenceCombustionAirInletHumidityRatio(double referenceCombustionAirInletHumidityRatio);

    void resetReferenceCombustionAirInletHumidityRatio();

    bool setReferenceElevation(double referenceElevation);

    void resetReferenceElevation();

    // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    bool setElectricalPowerFunctionofTemperatureandElevationCurve(const BiquadraticCurves& biquadraticCurves);

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
    bool setElectricalEfficiencyFunctionofTemperatureCurve(const QuadraticCubicCurves& quadraticCubicCurves);

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
    bool setElectricalEfficiencyFunctionofPartLoadRatioCurve(const QuadraticCubicCurves& quadraticCubicCurves);

    bool setFuelType(std::string fuelType);

    void resetFuelType();

    bool setFuelHigherHeatingValue(double fuelHigherHeatingValue);

    void resetFuelHigherHeatingValue();

    bool setFuelLowerHeatingValue(double fuelLowerHeatingValue);

    void resetFuelLowerHeatingValue();

    bool setStandbyPower(double standbyPower);

    void resetStandbyPower();

    bool setAncillaryPower(double ancillaryPower);

    void resetAncillaryPower();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setAncillaryPowerFunctionofFuelInputCurve(const boost::optional<QuadraticCurves>& quadraticCurves);

    void resetAncillaryPowerFunctionofFuelInputCurve();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setHeatRecoveryWaterInletNode(const boost::optional<Connection>& connection);

    void resetHeatRecoveryWaterInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setHeatRecoveryWaterOutletNode(const boost::optional<Connection>& connection);

    void resetHeatRecoveryWaterOutletNode();

    bool setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue);

    void resetReferenceThermalEfficiencyUsingLowerHeatValue();

    void setReferenceInletWaterTemperature(boost::optional<double> referenceInletWaterTemperature);

    void resetReferenceInletWaterTemperature();

    bool setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode);

    void resetHeatRecoveryWaterFlowOperatingMode();

    bool setReferenceHeatRecoveryWaterFlowRate(boost::optional<double> referenceHeatRecoveryWaterFlowRate);

    void resetReferenceHeatRecoveryWaterFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    bool setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const boost::optional<BiquadraticCurves>& biquadraticCurves);

    void resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();

    // TODO: Check argument type. From object lists, some candidates are: BicubicBiquadraticCurves, BiVariateTables.
    bool setThermalEfficiencyFunctionofTemperatureandElevationCurve(const boost::optional<BicubicBiquadraticCurves>& bicubicBiquadraticCurves);

    void resetThermalEfficiencyFunctionofTemperatureandElevationCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setHeatRecoveryRateFunctionofPartLoadRatioCurve(const boost::optional<QuadraticCubicCurves>& quadraticCubicCurves);

    void resetHeatRecoveryRateFunctionofPartLoadRatioCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const boost::optional<QuadraticCurves>& quadraticCurves);

    void resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setHeatRecoveryRateFunctionofWaterFlowRateCurve(const boost::optional<QuadraticCurves>& quadraticCurves);

    void resetHeatRecoveryRateFunctionofWaterFlowRateCurve();

    bool setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate);

    void resetMinimumHeatRecoveryWaterFlowRate();

    bool setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate);

    void resetMaximumHeatRecoveryWaterFlowRate();

    void setMaximumHeatRecoveryWaterTemperature(boost::optional<double> maximumHeatRecoveryWaterTemperature);

    void resetMaximumHeatRecoveryWaterTemperature();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCombustionAirInletNode(const boost::optional<Connection>& connection);

    void resetCombustionAirInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setCombustionAirOutletNode(const boost::optional<Connection>& connection);

    void resetCombustionAirOutletNode();

    bool setReferenceExhaustAirMassFlowRate(boost::optional<double> referenceExhaustAirMassFlowRate);

    void resetReferenceExhaustAirMassFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirFlowRateFunctionofTemperatureCurve(const boost::optional<QuadraticCubicCurves>& quadraticCubicCurves);

    void resetExhaustAirFlowRateFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirFlowRateFunctionofPartLoadRatioCurve(const boost::optional<QuadraticCubicCurves>& quadraticCubicCurves);

    void resetExhaustAirFlowRateFunctionofPartLoadRatioCurve();

    void setNominalExhaustAirOutletTemperature(boost::optional<double> nominalExhaustAirOutletTemperature);

    void resetNominalExhaustAirOutletTemperature();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirTemperatureFunctionofTemperatureCurve(const boost::optional<QuadraticCubicCurves>& quadraticCubicCurves);

    void resetExhaustAirTemperatureFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirTemperatureFunctionofPartLoadRatioCurve(const boost::optional<QuadraticCubicCurves>& quadraticCubicCurves);

    void resetExhaustAirTemperatureFunctionofPartLoadRatioCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbine");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<BiquadraticCurves> optionalElectricalPowerFunctionofTemperatureandElevationCurve() const;
    boost::optional<QuadraticCubicCurves> optionalElectricalEfficiencyFunctionofTemperatureCurve() const;
    boost::optional<QuadraticCubicCurves> optionalElectricalEfficiencyFunctionofPartLoadRatioCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GENERATORMICROTURBINE_IMPL_HPP

