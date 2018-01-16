/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
#define MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  /** CoilCoolingDXMultiSpeedStageData_Impl is a ModelObject_Impl that is the implementation class for CoilCoolingDXMultiSpeedStageData.*/
  class MODEL_API CoilCoolingDXMultiSpeedStageData_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingDXMultiSpeedStageData_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilCoolingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    CoilCoolingDXMultiSpeedStageData_Impl(const CoilCoolingDXMultiSpeedStageData_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~CoilCoolingDXMultiSpeedStageData_Impl() {}

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

    double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    double ratedWasteHeatFractionofPowerInput() const;

    Curve wasteHeatFunctionofTemperatureCurve() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<double> evaporativeCondenserAirFlowRate() const;

    bool isEvaporativeCondenserAirFlowRateAutosized() const;

    boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption() const;

    bool isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const;

  boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedGrossRatedSensibleHeatRatio() const ;

  boost::optional<double> autosizedRatedAirFlowRate() const ;

  boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const ;

  boost::optional<double> autosizedRatedEvaporativeCondenserPumpPowerConsumption() const ;

  void autosize();

  void applySizingValues();

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

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate);

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

    bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate);

    void autosizeEvaporativeCondenserAirFlowRate();

    bool setRatedEvaporativeCondenserPumpPowerConsumption(boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption);

    void autosizeRatedEvaporativeCondenserPumpPowerConsumption();

    //@}
    /** @name Other */
    //@{

    virtual std::vector<ModelObject> children() const override;
    virtual ModelObject clone(Model model) const override;

    // Used to determine the index of this performance data in the
    // list of stages in the parent object.
    boost::optional<std::tuple<int, CoilCoolingDXMultiSpeed>> stageIndexAndParentCoil() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeedStageData");

    boost::optional<Curve> optionalTotalCoolingCapacityFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalTotalCoolingCapacityFunctionofFlowFractionCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
    boost::optional<Curve> optionalEnergyInputRatioFunctionofFlowFractionCurve() const;
    boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;

  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

