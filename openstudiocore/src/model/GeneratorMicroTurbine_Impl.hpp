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
#include "GeneratorMicroTurbineHeatRecovery_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
// For the optional Generator:MicroTurbine:HeatRecovery
// It was broken out because that part needs to connect to a plant loop
class StraightComponent;

// TODO: add the tables class if they get added to OS later?
//class DataTables // UniVariateTables and BiVariateTables

namespace detail {

  /** GeneratorMicroTurbine_Impl is a Generator_Impl that is the implementation class for GeneratorMicroTurbine.*/
  class MODEL_API GeneratorMicroTurbine_Impl : public Generator_Impl {

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
    
    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::string generatorObjectType() const;

    virtual boost::optional<double> ratedElectricPowerOutput() const;

    virtual boost::optional<Schedule> availabilitySchedule() const override;

    virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

    //@}
    /** @name Getters */
    //@{

    double referenceElectricalPowerOutput() const;

    double minimumFullLoadElectricalPowerOutput() const;
    bool isMinimumFullLoadElectricalPowerOutputDefaulted() const;

    // This will default to referenceElectricalPowerOutput if not defined, like E+ does
    double maximumFullLoadElectricalPowerOutput() const;
    bool isMaximumFullLoadElectricalPowerOutputDefaulted() const;

    double referenceElectricalEfficiencyUsingLowerHeatingValue() const;
    
    double referenceCombustionAirInletTemperature() const;
    bool isReferenceCombustionAirInletTemperatureDefaulted() const;
    
    double referenceCombustionAirInletHumidityRatio() const;
    bool isReferenceCombustionAirInletHumidityRatioDefaulted() const;

    double referenceElevation() const;
    bool isReferenceElevationDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    Curve electricalPowerFunctionofTemperatureandElevationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves.
    Curve electricalEfficiencyFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves.
    Curve electricalEfficiencyFunctionofPartLoadRatioCurve() const;

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
    boost::optional<Curve> ancillaryPowerFunctionofFuelInputCurve() const;

    // Optional Generator:MicroTurbine:HeatRecovery
    boost::optional<GeneratorMicroTurbineHeatRecovery> generatorMicroTurbineHeatRecovery() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    //boost::optional<Connection> combustionAirInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    //boost::optional<Connection> combustionAirOutletNode() const;

    boost::optional<double> referenceExhaustAirMassFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirFlowRateFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirFlowRateFunctionofPartLoadRatioCurve() const;

    boost::optional<double> nominalExhaustAirOutletTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirTemperatureFunctionofTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> exhaustAirTemperatureFunctionofPartLoadRatioCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);
    void resetAvailabilitySchedule();
    
    bool setReferenceElectricalPowerOutput(double referenceElectricalPowerOutput);

    bool setMinimumFullLoadElectricalPowerOutput(double minimumFullLoadElectricalPowerOutput);
    void resetMinimumFullLoadElectricalPowerOutput();

    bool setMaximumFullLoadElectricalPowerOutput(double maximumFullLoadElectricalPowerOutput);
    void resetMaximumFullLoadElectricalPowerOutput();

    bool setReferenceElectricalEfficiencyUsingLowerHeatingValue(double referenceElectricalEfficiencyUsingLowerHeatingValue);

    void setReferenceCombustionAirInletTemperature(double referenceCombustionAirInletTemperature);
    void resetReferenceCombustionAirInletTemperature();

    bool setReferenceCombustionAirInletHumidityRatio(double referenceCombustionAirInletHumidityRatio);
    void resetReferenceCombustionAirInletHumidityRatio();

    bool setReferenceElevation(double referenceElevation);
    void resetReferenceElevation();

    // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    bool setElectricalPowerFunctionofTemperatureandElevationCurve(const Curve& electricalPowerFunctionofTemperatureandElevationCurve);

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
    bool setElectricalEfficiencyFunctionofTemperatureCurve(const Curve& electricalEfficiencyFunctionofTemperatureCurve);

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves.
    bool setElectricalEfficiencyFunctionofPartLoadRatioCurve(const Curve& electricalEfficiencyFunctionofPartLoadRatioCurve);

    bool setFuelType(const std::string& fuelType);
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
    bool setAncillaryPowerFunctionofFuelInputCurve(const Curve& ancillaryPowerFunctionofFuelInputCurve);
    void resetAncillaryPowerFunctionofFuelInputCurve();

    // Private setter
    bool setGeneratorMicroTurbineHeatRecovery(const GeneratorMicroTurbineHeatRecovery& generatorMicroTurbineHeatRecovery);
    //void resetGeneratorMicroTurbineHeatRecovery();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    //bool setCombustionAirInletNode(const Connection& connection);
    //void resetCombustionAirInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    //bool setCombustionAirOutletNode(const Connection& connection);
    //void resetCombustionAirOutletNode();

    bool setReferenceExhaustAirMassFlowRate(double referenceExhaustAirMassFlowRate);
    void resetReferenceExhaustAirMassFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirFlowRateFunctionofTemperatureCurve(const Curve& exhaustAirFlowRateFunctionofTemperatureCurve);
    void resetExhaustAirFlowRateFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirFlowRateFunctionofPartLoadRatioCurve(const Curve& exhaustAirFlowRateFunctionofPartLoadRatioCurve);
    void resetExhaustAirFlowRateFunctionofPartLoadRatioCurve();

    void setNominalExhaustAirOutletTemperature(double nominalExhaustAirOutletTemperature);
    void resetNominalExhaustAirOutletTemperature();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirTemperatureFunctionofTemperatureCurve(const Curve& exhaustAirTemperatureFunctionofTemperatureCurve);
    void resetExhaustAirTemperatureFunctionofTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setExhaustAirTemperatureFunctionofPartLoadRatioCurve(const Curve& exhaustAirTemperatureFunctionofPartLoadRatioCurve);
    void resetExhaustAirTemperatureFunctionofPartLoadRatioCurve();

    //@}
    /** @name Other */
    //@{
      
    ModelObject clone(Model model) const override;

    std::vector<IddObjectType> allowableChildTypes() const override;

    std::vector<ModelObject> children() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbine");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_GENERATORMICROTURBINE_IMPL_HPP

