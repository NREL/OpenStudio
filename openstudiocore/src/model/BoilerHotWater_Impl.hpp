/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_BOILERHOTWATER_IMPL_HPP
#define MODEL_BOILERHOTWATER_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** BoilerHotWater_Impl is a StraightComponent_Impl that is the implementation class for BoilerHotWater.*/
  class MODEL_API BoilerHotWater_Impl : public StraightComponent_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string fuelType READ fuelType WRITE setFuelType);
    Q_PROPERTY(boost::optional<double> nominalCapacity READ nominalCapacity WRITE setNominalCapacity RESET resetNominalCapacity);
    Q_PROPERTY(bool isNominalCapacityAutosized READ isNominalCapacityAutosized);
    Q_PROPERTY(double nominalThermalEfficiency READ nominalThermalEfficiency WRITE setNominalThermalEfficiency);
    Q_PROPERTY(boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable READ efficiencyCurveTemperatureEvaluationVariable WRITE setEfficiencyCurveTemperatureEvaluationVariable RESET resetEfficiencyCurveTemperatureEvaluationVariable);
    Q_PROPERTY(boost::optional<double> designWaterOutletTemperature READ designWaterOutletTemperature WRITE setDesignWaterOutletTemperature RESET resetDesignWaterOutletTemperature);
    Q_PROPERTY(boost::optional<double> designWaterFlowRate READ designWaterFlowRate WRITE setDesignWaterFlowRate RESET resetDesignWaterFlowRate);
    Q_PROPERTY(bool isDesignWaterFlowRateAutosized READ isDesignWaterFlowRateAutosized);
    Q_PROPERTY(double minimumPartLoadRatio READ minimumPartLoadRatio WRITE setMinimumPartLoadRatio RESET resetMinimumPartLoadRatio);
    Q_PROPERTY(bool isMinimumPartLoadRatioDefaulted READ isMinimumPartLoadRatioDefaulted);
    Q_PROPERTY(double maximumPartLoadRatio READ maximumPartLoadRatio WRITE setMaximumPartLoadRatio RESET resetMaximumPartLoadRatio);
    Q_PROPERTY(bool isMaximumPartLoadRatioDefaulted READ isMaximumPartLoadRatioDefaulted);
    Q_PROPERTY(double optimumPartLoadRatio READ optimumPartLoadRatio WRITE setOptimumPartLoadRatio RESET resetOptimumPartLoadRatio);
    Q_PROPERTY(bool isOptimumPartLoadRatioDefaulted READ isOptimumPartLoadRatioDefaulted);
    Q_PROPERTY(double waterOutletUpperTemperatureLimit READ waterOutletUpperTemperatureLimit WRITE setWaterOutletUpperTemperatureLimit RESET resetWaterOutletUpperTemperatureLimit);
    Q_PROPERTY(bool isWaterOutletUpperTemperatureLimitDefaulted READ isWaterOutletUpperTemperatureLimitDefaulted);
    Q_PROPERTY(std::string boilerFlowMode READ boilerFlowMode WRITE setBoilerFlowMode RESET resetBoilerFlowMode);
    Q_PROPERTY(bool isBoilerFlowModeDefaulted READ isBoilerFlowModeDefaulted);
    Q_PROPERTY(boost::optional<double> parasiticElectricLoad READ parasiticElectricLoad WRITE setParasiticElectricLoad RESET resetParasiticElectricLoad);
    Q_PROPERTY(double sizingFactor READ sizingFactor WRITE setSizingFactor RESET resetSizingFactor);
    Q_PROPERTY(bool isSizingFactorDefaulted READ isSizingFactorDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> normalizedBoilerEfficiencyCurve READ normalizedBoilerEfficiencyCurveAsModelObject WRITE setNormalizedBoilerEfficiencyCurveAsModelObject RESET resetNormalizedBoilerEfficiencyCurve);
   public:

    /** @name Constructors and Destructors */
    //@{

    BoilerHotWater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    BoilerHotWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    BoilerHotWater_Impl(const BoilerHotWater_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~BoilerHotWater_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    //@}
    /** @name Getters */
    //@{

    std::string fuelType() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    double nominalThermalEfficiency() const;

    boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable() const;

    boost::optional<Curve> normalizedBoilerEfficiencyCurve() const;

    boost::optional<double> designWaterOutletTemperature() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    double minimumPartLoadRatio() const;

    bool isMinimumPartLoadRatioDefaulted() const;

    double maximumPartLoadRatio() const;

    bool isMaximumPartLoadRatioDefaulted() const;

    double optimumPartLoadRatio() const;

    bool isOptimumPartLoadRatioDefaulted() const;

    // TODO: Handle Non-Extensible IddField Boiler Water Inlet Node Name.

    // TODO: Handle Non-Extensible IddField Boiler Water Outlet Node Name.

    double waterOutletUpperTemperatureLimit() const;

    bool isWaterOutletUpperTemperatureLimitDefaulted() const;

    std::string boilerFlowMode() const;

    bool isBoilerFlowModeDefaulted() const;

    boost::optional<double> parasiticElectricLoad() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setFuelType(std::string fuelType);

    bool setNominalCapacity(boost::optional<double> nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    bool setNominalThermalEfficiency(double nominalThermalEfficiency);

    bool setEfficiencyCurveTemperatureEvaluationVariable(
        boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable);

    void resetEfficiencyCurveTemperatureEvaluationVariable();

    bool setNormalizedBoilerEfficiencyCurve(
        const boost::optional<Curve>& normalizedBoilerEfficiencyCurve);

    void resetNormalizedBoilerEfficiencyCurve();

    void setDesignWaterOutletTemperature(boost::optional<double> designWaterOutletTemperature);

    void resetDesignWaterOutletTemperature();

    bool setDesignWaterFlowRate(boost::optional<double> designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    void resetMinimumPartLoadRatio();

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    void resetMaximumPartLoadRatio();

    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

    void resetOptimumPartLoadRatio();

    // TODO: Handle Non-Extensible IddField Boiler Water Inlet Node Name.

    // TODO: Handle Non-Extensible IddField Boiler Water Outlet Node Name.

    void setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit);

    void resetWaterOutletUpperTemperatureLimit();

    bool setBoilerFlowMode(std::string boilerFlowMode);

    void resetBoilerFlowMode();

    bool setParasiticElectricLoad(boost::optional<double> parasiticElectricLoad);

    void resetParasiticElectricLoad();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    bool addToNode(Node & node);

    ModelObject clone(Model model) const;

    //@}
   private:

    REGISTER_LOGGER("openstudio.model.BoilerHotWater");

    boost::optional<ModelObject> normalizedBoilerEfficiencyCurveAsModelObject() const;

    bool setNormalizedBoilerEfficiencyCurveAsModelObject(const boost::optional<ModelObject>& modelObject);

  };

} // detail

} // model
} // openstudio

#endif // MODEL_BOILERHOTWATER_IMPL_HPP
