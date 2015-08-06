/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
#define MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class CurveBiquadratic;
class CurveQuadratic;

namespace detail {

  /** CoilHeatingDXMultiSpeedStageData_Impl is a ParentObject_Impl that is the implementation class for CoilHeatingDXMultiSpeedStageData.*/
  class MODEL_API CoilHeatingDXMultiSpeedStageData_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingDXMultiSpeedStageData_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilHeatingDXMultiSpeedStageData_Impl(const CoilHeatingDXMultiSpeedStageData_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~CoilHeatingDXMultiSpeedStageData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> grossRatedTotalHeatingCapacity() const;

    bool isGrossRatedTotalHeatingCapacityAutosized() const;

    boost::optional<double> grossRatedSensibleHeatRatio() const;

    bool isGrossRatedSensibleHeatRatioAutosized() const;

    double grossRatedHeatingCOP() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

    CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve() const;

    CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve() const;

    CurveBiquadratic energyInputRatioFunctionofTemperatureCurve() const;

    CurveQuadratic energyInputRatioFunctionofFlowFractionCurve() const;

    CurveQuadratic partLoadFractionCorrelationCurve() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    double ratedWasteHeatFractionofPowerInput() const;

    CurveBiquadratic wasteHeatFunctionofTemperatureCurve() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<double> evaporativeCondenserAirFlowRate() const;

    bool isEvaporativeCondenserAirFlowRateAutosized() const;

    boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption() const;

    bool isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossRatedTotalHeatingCapacity(boost::optional<double> grossRatedTotalHeatingCapacity);

    void autosizeGrossRatedTotalHeatingCapacity();

    bool setGrossRatedSensibleHeatRatio(boost::optional<double> grossRatedSensibleHeatRatio);

    void autosizeGrossRatedSensibleHeatRatio();

    bool setGrossRatedHeatingCOP(double grossRatedHeatingCOP);

    bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate);

    bool setTotalHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setTotalHeatingCapacityFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setEnergyInputRatioFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic);

    bool setPartLoadFractionCorrelationCurve(const CurveQuadratic& curveQuadratic);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

    bool setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate);

    void autosizeEvaporativeCondenserAirFlowRate();

    bool setRatedEvaporativeCondenserPumpPowerConsumption(boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption);

    void autosizeRatedEvaporativeCondenserPumpPowerConsumption();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeedStageData");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CurveBiquadratic> optionalTotalHeatingCapacityFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalTotalHeatingCapacityFunctionofFlowFractionCurve() const;
    boost::optional<CurveBiquadratic> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
    boost::optional<CurveQuadratic> optionalEnergyInputRatioFunctionofFlowFractionCurve() const;
    boost::optional<CurveQuadratic> optionalPartLoadFractionCorrelationCurve() const;
    boost::optional<CurveBiquadratic> optionalWasteHeatFunctionofTemperatureCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

