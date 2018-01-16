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

#ifndef MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP
#define MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../model/CoilHeatingDXMultiSpeed.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilHeatingDXMultiSpeedStageData_Impl;

} // detail

/** CoilHeatingDXMultiSpeedStageData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:MultiSpeed:StageData'. */
class MODEL_API CoilHeatingDXMultiSpeedStageData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDXMultiSpeedStageData(const Model& model);

  explicit CoilHeatingDXMultiSpeedStageData(const Model& model,
    const Curve& heatingCapacityFunctionofTemperature,
    const Curve& heatingCapacityFunctionofFlowFraction,
    const Curve& energyInputRatioFunctionofTemperature,
    const Curve& energyInputRatioFunctionofFlowFraction,
    const Curve& partLoadFractionCorrelation,
    const Curve& wasteHeatFunctionofTemperature);

  virtual ~CoilHeatingDXMultiSpeedStageData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<double> grossRatedHeatingCapacity() const;

  bool isGrossRatedHeatingCapacityAutosized() const;

  double grossRatedHeatingCOP() const;

  boost::optional<double> ratedAirFlowRate() const;

  bool isRatedAirFlowRateAutosized() const;

  double ratedSupplyAirFanPowerPerVolumeFlowRate() const;

  Curve heatingCapacityFunctionofTemperatureCurve() const;

  Curve heatingCapacityFunctionofFlowFractionCurve() const;

  Curve energyInputRatioFunctionofTemperatureCurve() const;

  Curve energyInputRatioFunctionofFlowFractionCurve() const;

  Curve partLoadFractionCorrelationCurve() const;

  double ratedWasteHeatFractionofPowerInput() const;

  Curve wasteHeatFunctionofTemperatureCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setGrossRatedHeatingCapacity(double grossRatedHeatingCapacity);

  void autosizeGrossRatedHeatingCapacity();

  bool setGrossRatedHeatingCOP(double grossRatedHeatingCOP);

  bool setRatedAirFlowRate(double ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  bool setRatedSupplyAirFanPowerPerVolumeFlowRate(double ratedSupplyAirFanPowerPerVolumeFlowRate);

  bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

  bool setPartLoadFractionCorrelationCurve(const Curve& curve);

  bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

  bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedGrossRatedHeatingCapacity() const ;

  boost::optional<double> autosizedRatedAirFlowRate() const ;

  void autosize();

  void applySizingValues();

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingDXMultiSpeedStageData_Impl ImplType;

  explicit CoilHeatingDXMultiSpeedStageData(std::shared_ptr<detail::CoilHeatingDXMultiSpeedStageData_Impl> impl);

  friend class detail::CoilHeatingDXMultiSpeedStageData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeedStageData");

  // Used to determine the index of this performance data in the
  // list of stages in the parent object.
  boost::optional<std::tuple<int, CoilHeatingDXMultiSpeed>> stageIndexAndParentCoil() const;

};

/** \relates CoilHeatingDXMultiSpeedStageData*/
typedef boost::optional<CoilHeatingDXMultiSpeedStageData> OptionalCoilHeatingDXMultiSpeedStageData;

/** \relates CoilHeatingDXMultiSpeedStageData*/
typedef std::vector<CoilHeatingDXMultiSpeedStageData> CoilHeatingDXMultiSpeedStageDataVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP

