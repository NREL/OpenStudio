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

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Curve;

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
    
    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Connection.
    //boost::optional<Connection> heatRecoveryWaterInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    //boost::optional<Connection> heatRecoveryWaterOutletNode() const;

    double referenceThermalEfficiencyUsingLowerHeatValue() const;
    bool isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const;

    double referenceInletWaterTemperature() const;

    std::string heatRecoveryWaterFlowOperatingMode() const;
    bool isHeatRecoveryWaterFlowOperatingModeDefaulted() const;

    double referenceHeatRecoveryWaterFlowRate() const;

    // TODO: Check return type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    boost::optional<Curve> heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const;

    // TODO: Check return type. From object lists, some candidates are: BicubicBiquadraticCurves, BiVariateTables.
    boost::optional<Curve> thermalEfficiencyFunctionofTemperatureandElevationCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    boost::optional<Curve> heatRecoveryRateFunctionofPartLoadRatioCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    boost::optional<Curve> heatRecoveryRateFunctionofInletWaterTemperatureCurve() const;

    // TODO: Check return type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    boost::optional<Curve> heatRecoveryRateFunctionofWaterFlowRateCurve() const;

    double minimumHeatRecoveryWaterFlowRate() const;
    bool isMinimumHeatRecoveryWaterFlowRateDefaulted() const;

    double maximumHeatRecoveryWaterFlowRate() const;
    bool isMaximumHeatRecoveryWaterFlowRateDefaulted() const;

    boost::optional<double> maximumHeatRecoveryWaterTemperature() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    //bool setHeatRecoveryWaterInletNode(const Connection& connection);
    //void resetHeatRecoveryWaterInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    //bool setHeatRecoveryWaterOutletNode(const Connection& connection);
    //void resetHeatRecoveryWaterOutletNode();

    bool setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue);
    void resetReferenceThermalEfficiencyUsingLowerHeatValue();

    void setReferenceInletWaterTemperature(double referenceInletWaterTemperature);

    bool setHeatRecoveryWaterFlowOperatingMode(std::string heatRecoveryWaterFlowOperatingMode);
    void resetHeatRecoveryWaterFlowOperatingMode();

    bool setReferenceHeatRecoveryWaterFlowRate(double referenceHeatRecoveryWaterFlowRate);

    // TODO: Check argument type. From object lists, some candidates are: BiquadraticCurves, BiVariateTables.
    bool setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve);
    void resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();

    // TODO: Check argument type. From object lists, some candidates are: BicubicBiquadraticCurves, BiVariateTables.
    bool setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& thermalEfficiencyFunctionofTemperatureandElevationCurve);
    void resetThermalEfficiencyFunctionofTemperatureandElevationCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCubicCurves, UniVariateTables.
    bool setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& heatRecoveryRateFunctionofPartLoadRatioCurve);
    void resetHeatRecoveryRateFunctionofPartLoadRatioCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& heatRecoveryRateFunctionofInletWaterTemperatureCurve);
    void resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();

    // TODO: Check argument type. From object lists, some candidates are: QuadraticCurves, UniVariateTables.
    bool setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& heatRecoveryRateFunctionofWaterFlowRateCurve);
    void resetHeatRecoveryRateFunctionofWaterFlowRateCurve();

    bool setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate);
    void resetMinimumHeatRecoveryWaterFlowRate();

    bool setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate);
    void resetMaximumHeatRecoveryWaterFlowRate();

    void setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature);
    void resetMaximumHeatRecoveryWaterTemperature();

    //@}
    /** @name Other */
    //@{
    bool addToNode(Node & node) override;

    //ModelObject clone(Model model) const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbine");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    //boost::optional<Curve> optionalElectricalPowerFunctionofTemperatureandElevationCurve() const;
    //boost::optional<Curve> optionalElectricalEfficiencyFunctionofTemperatureCurve() const;
    //boost::optional<Curve> optionalElectricalEfficiencyFunctionofPartLoadRatioCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GENERATORMICROTURBINE_IMPL_HPP

