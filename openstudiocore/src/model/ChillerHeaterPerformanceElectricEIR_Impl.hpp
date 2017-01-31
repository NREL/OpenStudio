/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

