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

#ifndef MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_IMPL_HPP
#define MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** ChillerHeaterPerformanceElectricEIR_Impl is a ParentObject_Impl that is the implementation class for ChillerHeaterPerformanceElectricEIR.*/
  class MODEL_API ChillerHeaterPerformanceElectricEIR_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ChillerHeaterPerformanceElectricEIR_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    ChillerHeaterPerformanceElectricEIR_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    ChillerHeaterPerformanceElectricEIR_Impl(const ChillerHeaterPerformanceElectricEIR_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~ChillerHeaterPerformanceElectricEIR_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> referenceCoolingModeEvaporatorCapacity() const;

    bool isReferenceCoolingModeEvaporatorCapacityAutosized() const;

    double referenceCoolingModeCOP() const;

    double referenceCoolingModeLeavingChilledWaterTemperature() const;

    double referenceCoolingModeEnteringCondenserFluidTemperature() const;

    double referenceCoolingModeLeavingCondenserWaterTemperature() const;

    double referenceHeatingModeCoolingCapacityRatio() const;

    double referenceHeatingModeCoolingPowerInputRatio() const;

    double referenceHeatingModeLeavingChilledWaterTemperature() const;

    double referenceHeatingModeLeavingCondenserWaterTemperature() const;

    double referenceHeatingModeEnteringCondenserFluidTemperature() const;

    double heatingModeEnteringChilledWaterTemperatureLowLimit() const;

    std::string chilledWaterFlowModeType() const;

    boost::optional<double> designChilledWaterFlowRate() const;

    bool isDesignChilledWaterFlowRateAutosized() const;

    boost::optional<double> designCondenserWaterFlowRate() const;

    bool isDesignCondenserWaterFlowRateAutosized() const;

    double designHotWaterFlowRate() const;

    double compressorMotorEfficiency() const;

    std::string condenserType() const;

    std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable() const;

    Curve coolingModeCoolingCapacityFunctionofTemperatureCurve() const;

    Curve coolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const;

    Curve coolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const;

    double coolingModeCoolingCapacityOptimumPartLoadRatio() const;

    std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable() const;

    Curve heatingModeCoolingCapacityFunctionofTemperatureCurve() const;

    Curve heatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const;

    Curve heatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const;

    double heatingModeCoolingCapacityOptimumPartLoadRatio() const;

    double sizingFactor() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceCoolingModeEvaporatorCapacity(boost::optional<double> referenceCoolingModeEvaporatorCapacity);

    void autosizeReferenceCoolingModeEvaporatorCapacity();

    bool setReferenceCoolingModeCOP(double referenceCoolingModeCOP);

    void setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature);

    void setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature);

    void setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature);

    void setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio);

    bool setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio);

    void setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature);

    void setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature);

    void setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature);

    void setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit);

    bool setChilledWaterFlowModeType(std::string chilledWaterFlowModeType);

    bool setDesignChilledWaterFlowRate(boost::optional<double> designChilledWaterFlowRate);

    void resetDesignChilledWaterFlowRate();

    void autosizeDesignChilledWaterFlowRate();

    bool setDesignCondenserWaterFlowRate(boost::optional<double> designCondenserWaterFlowRate);

    void resetDesignCondenserWaterFlowRate();

    void autosizeDesignCondenserWaterFlowRate();

    bool setDesignHotWaterFlowRate(double designHotWaterFlowRate);

    bool setCompressorMotorEfficiency(double compressorMotorEfficiency);

    bool setCondenserType(std::string condenserType);

    bool setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable);

    bool setCoolingModeCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const Curve& curve);

    bool setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio);

    bool setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable);

    bool setHeatingModeCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(const Curve& curve);

    bool setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio);

    bool setSizingFactor(double sizingFactor);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ChillerHeaterPerformanceElectricEIR");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Curve> optionalCoolingModeCoolingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const;
    boost::optional<Curve> optionalHeatingModeCoolingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_IMPL_HPP

