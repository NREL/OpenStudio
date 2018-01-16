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

#ifndef MODEL_COOLINGTOWERPERFORMANCEYORKCALC_HPP
#define MODEL_COOLINGTOWERPERFORMANCEYORKCALC_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class CoolingTowerPerformanceYorkCalc_Impl;

} // detail

/** CoolingTowerPerformanceYorkCalc is a ModelObject that wraps the OpenStudio IDD object 'OS:CoolingTowerPerformance:YorkCalc'. */
class MODEL_API CoolingTowerPerformanceYorkCalc : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoolingTowerPerformanceYorkCalc(const Model& model);

  virtual ~CoolingTowerPerformanceYorkCalc() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double minimumInletAirWetBulbTemperature() const;

  double maximumInletAirWetBulbTemperature() const;

  double minimumRangeTemperature() const;

  double maximumRangeTemperature() const;

  double minimumApproachTemperature() const;

  double maximumApproachTemperature() const;

  double minimumWaterFlowRateRatio() const;

  double maximumWaterFlowRateRatio() const;

  double maximumLiquidtoGasRatio() const;

  double coefficient1() const;

  double coefficient2() const;

  double coefficient3() const;

  double coefficient4() const;

  double coefficient5() const;

  double coefficient6() const;

  double coefficient7() const;

  double coefficient8() const;

  double coefficient9() const;

  double coefficient10() const;

  double coefficient11() const;

  double coefficient12() const;

  double coefficient13() const;

  double coefficient14() const;

  double coefficient15() const;

  double coefficient16() const;

  double coefficient17() const;

  double coefficient18() const;

  double coefficient19() const;

  double coefficient20() const;

  double coefficient21() const;

  double coefficient22() const;

  double coefficient23() const;

  double coefficient24() const;

  double coefficient25() const;

  double coefficient26() const;

  double coefficient27() const;

  //@}
  /** @name Setters */
  //@{

  bool setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature);

  bool setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature);

  bool setMinimumRangeTemperature(double minimumRangeTemperature);

  bool setMaximumRangeTemperature(double maximumRangeTemperature);

  bool setMinimumApproachTemperature(double minimumApproachTemperature);

  bool setMaximumApproachTemperature(double maximumApproachTemperature);

  bool setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio);

  bool setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio);

  bool setMaximumLiquidtoGasRatio(double maximumLiquidtoGasRatio);

  bool setCoefficient1(double coefficient1);

  bool setCoefficient2(double coefficient2);

  bool setCoefficient3(double coefficient3);

  bool setCoefficient4(double coefficient4);

  bool setCoefficient5(double coefficient5);

  bool setCoefficient6(double coefficient6);

  bool setCoefficient7(double coefficient7);

  bool setCoefficient8(double coefficient8);

  bool setCoefficient9(double coefficient9);

  bool setCoefficient10(double coefficient10);

  bool setCoefficient11(double coefficient11);

  bool setCoefficient12(double coefficient12);

  bool setCoefficient13(double coefficient13);

  bool setCoefficient14(double coefficient14);

  bool setCoefficient15(double coefficient15);

  bool setCoefficient16(double coefficient16);

  bool setCoefficient17(double coefficient17);

  bool setCoefficient18(double coefficient18);

  bool setCoefficient19(double coefficient19);

  bool setCoefficient20(double coefficient20);

  bool setCoefficient21(double coefficient21);

  bool setCoefficient22(double coefficient22);

  bool setCoefficient23(double coefficient23);

  bool setCoefficient24(double coefficient24);

  bool setCoefficient25(double coefficient25);

  bool setCoefficient26(double coefficient26);

  bool setCoefficient27(double coefficient27);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CoolingTowerPerformanceYorkCalc_Impl ImplType;

  explicit CoolingTowerPerformanceYorkCalc(std::shared_ptr<detail::CoolingTowerPerformanceYorkCalc_Impl> impl);

  friend class detail::CoolingTowerPerformanceYorkCalc_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoolingTowerPerformanceYorkCalc");
};

/** \relates CoolingTowerPerformanceYorkCalc*/
typedef boost::optional<CoolingTowerPerformanceYorkCalc> OptionalCoolingTowerPerformanceYorkCalc;

/** \relates CoolingTowerPerformanceYorkCalc*/
typedef std::vector<CoolingTowerPerformanceYorkCalc> CoolingTowerPerformanceYorkCalcVector;

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERPERFORMANCEYORKCALC_HPP
