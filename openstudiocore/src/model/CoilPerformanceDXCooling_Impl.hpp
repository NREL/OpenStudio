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

#ifndef MODEL_COILPERFORMANCEDXCOOLING_IMPL_HPP
#define MODEL_COILPERFORMANCEDXCOOLING_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
class Node;

namespace detail {

  /** CoilPerformanceDXCooling_Impl is a ModelObject_Impl that is the implementation class for CoilPerformanceDXCooling.*/
  class MODEL_API CoilPerformanceDXCooling_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilPerformanceDXCooling_Impl(const IdfObject& idfObject,
                                  Model_Impl* model,
                                  bool keepHandle);

    CoilPerformanceDXCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    CoilPerformanceDXCooling_Impl(const CoilPerformanceDXCooling_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    virtual ~CoilPerformanceDXCooling_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> grossRatedTotalCoolingCapacity() const;

    bool isGrossRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> grossRatedSensibleHeatRatio() const;

    bool isGrossRatedSensibleHeatRatioAutosized() const;

    double grossRatedCoolingCOP() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    double fractionofAirFlowBypassedAroundCoil() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    boost::optional<Node> condenserAirInletNode() const;

    std::string condenserType() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<double> evaporativeCondenserAirFlowRate() const;

    bool isEvaporativeCondenserAirFlowRateAutosized() const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    boost::optional<Curve> sensibleHeatRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> sensibleHeatRatioFunctionofFlowFractionCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossRatedTotalCoolingCapacity(boost::optional<double> grossRatedTotalCoolingCapacity);

    void autosizeGrossRatedTotalCoolingCapacity();

    bool setGrossRatedSensibleHeatRatio(boost::optional<double> grossRatedSensibleHeatRatio);

    void autosizeGrossRatedSensibleHeatRatio();

    bool setGrossRatedCoolingCOP(double grossRatedCoolingCOP);

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    bool setFractionofAirFlowBypassedAroundCoil(double fractionofAirFlowBypassedAroundCoil);

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setCondenserAirInletNode(const boost::optional<Node>& node);

    void resetCondenserAirInletNode();

    bool setCondenserType(std::string condenserType);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate);

    void autosizeEvaporativeCondenserAirFlowRate();

    bool setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    bool setSensibleHeatRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve);

    void resetSensibleHeatRatioFunctionofTemperatureCurve();

    bool setSensibleHeatRatioFunctionofFlowFractionCurve(const boost::optional<Curve>& curve);

    void resetSensibleHeatRatioFunctionofFlowFractionCurve();

    //@}
    /** @name Other */
    //@{
    ModelObject clone(Model model) const override;
    std::vector<ModelObject> children() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilPerformanceDXCooling");

    boost::optional<Curve> optionalTotalCoolingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionofFlowFractionCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofFlowFractionCurve() const;
    boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILPERFORMANCEDXCOOLING_IMPL_HPP

