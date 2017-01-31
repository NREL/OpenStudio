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

#ifndef MODEL_COOLINGTOWERPERFORMANCECOOLTOOLS_HPP
#define MODEL_COOLINGTOWERPERFORMANCECOOLTOOLS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class CoolingTowerPerformanceCoolTools_Impl;

} // detail

/** CoolingTowerPerformanceCoolTools is a ModelObject that wraps the OpenStudio IDD object 'OS:CoolingTowerPerformance:CoolTools'. */
class MODEL_API CoolingTowerPerformanceCoolTools : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoolingTowerPerformanceCoolTools(const Model& model);

  virtual ~CoolingTowerPerformanceCoolTools() {}

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

  double coefficient28() const;

  double coefficient29() const;

  double coefficient30() const;

  double coefficient31() const;

  double coefficient32() const;

  double coefficient33() const;

  double coefficient34() const;

  double coefficient35() const;

  //@}
  /** @name Setters */
  //@{

  void setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature);

  void setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature);

  void setMinimumRangeTemperature(double minimumRangeTemperature);

  void setMaximumRangeTemperature(double maximumRangeTemperature);

  void setMinimumApproachTemperature(double minimumApproachTemperature);

  void setMaximumApproachTemperature(double maximumApproachTemperature);

  void setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio);

  void setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio);

  void setCoefficient1(double coefficient1);

  void setCoefficient2(double coefficient2);

  void setCoefficient3(double coefficient3);

  void setCoefficient4(double coefficient4);

  void setCoefficient5(double coefficient5);

  void setCoefficient6(double coefficient6);

  void setCoefficient7(double coefficient7);

  void setCoefficient8(double coefficient8);

  void setCoefficient9(double coefficient9);

  void setCoefficient10(double coefficient10);

  void setCoefficient11(double coefficient11);

  void setCoefficient12(double coefficient12);

  void setCoefficient13(double coefficient13);

  void setCoefficient14(double coefficient14);

  void setCoefficient15(double coefficient15);

  void setCoefficient16(double coefficient16);

  void setCoefficient17(double coefficient17);

  void setCoefficient18(double coefficient18);

  void setCoefficient19(double coefficient19);

  void setCoefficient20(double coefficient20);

  void setCoefficient21(double coefficient21);

  void setCoefficient22(double coefficient22);

  void setCoefficient23(double coefficient23);

  void setCoefficient24(double coefficient24);

  void setCoefficient25(double coefficient25);

  void setCoefficient26(double coefficient26);

  void setCoefficient27(double coefficient27);

  void setCoefficient28(double coefficient28);

  void setCoefficient29(double coefficient29);

  void setCoefficient30(double coefficient30);

  void setCoefficient31(double coefficient31);

  void setCoefficient32(double coefficient32);

  void setCoefficient33(double coefficient33);

  void setCoefficient34(double coefficient34);

  void setCoefficient35(double coefficient35);

 protected:

  /// @cond
  typedef detail::CoolingTowerPerformanceCoolTools_Impl ImplType;

  explicit CoolingTowerPerformanceCoolTools(std::shared_ptr<detail::CoolingTowerPerformanceCoolTools_Impl> impl);

  friend class detail::CoolingTowerPerformanceCoolTools_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.CoolingTowerPerformanceCoolTools");
};

/** \relates CoolingTowerPerformanceCoolTools*/
typedef boost::optional<CoolingTowerPerformanceCoolTools> OptionalCoolingTowerPerformanceCoolTools;

/** \relates CoolingTowerPerformanceCoolTools*/
typedef std::vector<CoolingTowerPerformanceCoolTools> CoolingTowerPerformanceCoolToolsVector;

} // model
} // openstudio

#endif // MODEL_COOLINGTOWERPERFORMANCECOOLTOOLS_HPP

