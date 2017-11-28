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

#include "CoolingTowerPerformanceYorkCalc.hpp"
#include "CoolingTowerPerformanceYorkCalc_Impl.hpp"
#include <utilities/idd/OS_CoolingTowerPerformance_YorkCalc_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoolingTowerPerformanceYorkCalc_Impl::CoolingTowerPerformanceYorkCalc_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoolingTowerPerformanceYorkCalc::iddObjectType());
  }

  CoolingTowerPerformanceYorkCalc_Impl::CoolingTowerPerformanceYorkCalc_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoolingTowerPerformanceYorkCalc::iddObjectType());
  }

  CoolingTowerPerformanceYorkCalc_Impl::CoolingTowerPerformanceYorkCalc_Impl(const CoolingTowerPerformanceYorkCalc_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoolingTowerPerformanceYorkCalc_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoolingTowerPerformanceYorkCalc_Impl::iddObjectType() const {
    return CoolingTowerPerformanceYorkCalc::iddObjectType();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::minimumInletAirWetBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::maximumInletAirWetBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::minimumRangeTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumRangeTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::maximumRangeTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumRangeTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::minimumApproachTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumApproachTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::maximumApproachTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumApproachTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::minimumWaterFlowRateRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumWaterFlowRateRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::maximumWaterFlowRateRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumWaterFlowRateRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::maximumLiquidtoGasRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumLiquidtoGasRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient1() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient2() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient3() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient4() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient5() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient6() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient6,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient7() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient7,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient8() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient8,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient9() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient9,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient10() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient10,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient11() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient11,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient12() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient12,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient13() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient13,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient14() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient14,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient15() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient15,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient16() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient16,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient17() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient17,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient18() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient18,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient19() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient19,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient20() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient20,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient21() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient21,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient22() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient22,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient23() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient23,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient24() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient24,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient25() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient25,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient26() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient26,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceYorkCalc_Impl::coefficient27() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient27,true);
    OS_ASSERT(value);
    return value.get();
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumInletAirWetBulbTemperature, minimumInletAirWetBulbTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumInletAirWetBulbTemperature, maximumInletAirWetBulbTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMinimumRangeTemperature(double minimumRangeTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumRangeTemperature, minimumRangeTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMaximumRangeTemperature(double maximumRangeTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumRangeTemperature, maximumRangeTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMinimumApproachTemperature(double minimumApproachTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumApproachTemperature, minimumApproachTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMaximumApproachTemperature(double maximumApproachTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumApproachTemperature, maximumApproachTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MinimumWaterFlowRateRatio, minimumWaterFlowRateRatio);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumWaterFlowRateRatio, maximumWaterFlowRateRatio);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setMaximumLiquidtoGasRatio(double maximumLiquidtoGasRatio) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::MaximumLiquidtoGasRatio, maximumLiquidtoGasRatio);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient1(double coefficient1) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient1, coefficient1);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient2(double coefficient2) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient2, coefficient2);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient3(double coefficient3) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient3, coefficient3);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient4(double coefficient4) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient4, coefficient4);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient5(double coefficient5) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient5, coefficient5);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient6(double coefficient6) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient6, coefficient6);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient7(double coefficient7) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient7, coefficient7);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient8(double coefficient8) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient8, coefficient8);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient9(double coefficient9) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient9, coefficient9);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient10(double coefficient10) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient10, coefficient10);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient11(double coefficient11) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient11, coefficient11);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient12(double coefficient12) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient12, coefficient12);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient13(double coefficient13) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient13, coefficient13);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient14(double coefficient14) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient14, coefficient14);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient15(double coefficient15) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient15, coefficient15);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient16(double coefficient16) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient16, coefficient16);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient17(double coefficient17) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient17, coefficient17);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient18(double coefficient18) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient18, coefficient18);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient19(double coefficient19) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient19, coefficient19);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient20(double coefficient20) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient20, coefficient20);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient21(double coefficient21) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient21, coefficient21);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient22(double coefficient22) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient22, coefficient22);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient23(double coefficient23) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient23, coefficient23);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient24(double coefficient24) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient24, coefficient24);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient25(double coefficient25) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient25, coefficient25);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient26(double coefficient26) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient26, coefficient26);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceYorkCalc_Impl::setCoefficient27(double coefficient27) {
    bool result = setDouble(OS_CoolingTowerPerformance_YorkCalcFields::Coefficient27, coefficient27);
    OS_ASSERT(result);
  }

} // detail

CoolingTowerPerformanceYorkCalc::CoolingTowerPerformanceYorkCalc(const Model& model)
  : ModelObject(CoolingTowerPerformanceYorkCalc::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>());

  setMinimumInletAirWetBulbTemperature(-34.4);
  setMaximumInletAirWetBulbTemperature(26.6667);
  setMinimumRangeTemperature(1.1111);
  setMaximumRangeTemperature(22.2222);
  setMinimumApproachTemperature(1.1111);
  setMaximumApproachTemperature(40.0);
  setMinimumWaterFlowRateRatio(0.5);
  setMaximumWaterFlowRateRatio(4.0);
  setMaximumLiquidtoGasRatio(8.0);
  setCoefficient1(-0.359741205);
  setCoefficient2(-0.055053608);
  setCoefficient3(0.0023850432);
  setCoefficient4(0.173926877);
  setCoefficient5(-0.0248473764);
  setCoefficient6(0.00048430224);
  setCoefficient7(-0.005589849456);
  setCoefficient8(0.0005770079712);
  setCoefficient9(-0.00001342427256);
  setCoefficient10(2.84765801111111);
  setCoefficient11(-0.121765149);
  setCoefficient12(0.0014599242);
  setCoefficient13(1.680428651);
  setCoefficient14(-0.0166920786);
  setCoefficient15(-0.0007190532);
  setCoefficient16(-0.025485194448);
  setCoefficient17(0.0000487491696);
  setCoefficient18(0.00002719234152);
  setCoefficient19(-0.0653766255555556);
  setCoefficient20(-0.002278167);
  setCoefficient21(0.0002500254);
  setCoefficient22(-0.0910565458);
  setCoefficient23(0.00318176316);
  setCoefficient24(0.000038621772);
  setCoefficient25(-0.0034285382352);
  setCoefficient26(0.00000856589904);
  setCoefficient27(-0.000001516821552);
}

IddObjectType CoolingTowerPerformanceYorkCalc::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoolingTowerPerformance_YorkCalc);
}

double CoolingTowerPerformanceYorkCalc::minimumInletAirWetBulbTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->minimumInletAirWetBulbTemperature();
}

double CoolingTowerPerformanceYorkCalc::maximumInletAirWetBulbTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->maximumInletAirWetBulbTemperature();
}

double CoolingTowerPerformanceYorkCalc::minimumRangeTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->minimumRangeTemperature();
}

double CoolingTowerPerformanceYorkCalc::maximumRangeTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->maximumRangeTemperature();
}

double CoolingTowerPerformanceYorkCalc::minimumApproachTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->minimumApproachTemperature();
}

double CoolingTowerPerformanceYorkCalc::maximumApproachTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->maximumApproachTemperature();
}

double CoolingTowerPerformanceYorkCalc::minimumWaterFlowRateRatio() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->minimumWaterFlowRateRatio();
}

double CoolingTowerPerformanceYorkCalc::maximumWaterFlowRateRatio() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->maximumWaterFlowRateRatio();
}

double CoolingTowerPerformanceYorkCalc::maximumLiquidtoGasRatio() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->maximumLiquidtoGasRatio();
}

double CoolingTowerPerformanceYorkCalc::coefficient1() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient1();
}

double CoolingTowerPerformanceYorkCalc::coefficient2() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient2();
}

double CoolingTowerPerformanceYorkCalc::coefficient3() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient3();
}

double CoolingTowerPerformanceYorkCalc::coefficient4() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient4();
}

double CoolingTowerPerformanceYorkCalc::coefficient5() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient5();
}

double CoolingTowerPerformanceYorkCalc::coefficient6() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient6();
}

double CoolingTowerPerformanceYorkCalc::coefficient7() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient7();
}

double CoolingTowerPerformanceYorkCalc::coefficient8() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient8();
}

double CoolingTowerPerformanceYorkCalc::coefficient9() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient9();
}

double CoolingTowerPerformanceYorkCalc::coefficient10() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient10();
}

double CoolingTowerPerformanceYorkCalc::coefficient11() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient11();
}

double CoolingTowerPerformanceYorkCalc::coefficient12() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient12();
}

double CoolingTowerPerformanceYorkCalc::coefficient13() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient13();
}

double CoolingTowerPerformanceYorkCalc::coefficient14() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient14();
}

double CoolingTowerPerformanceYorkCalc::coefficient15() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient15();
}

double CoolingTowerPerformanceYorkCalc::coefficient16() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient16();
}

double CoolingTowerPerformanceYorkCalc::coefficient17() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient17();
}

double CoolingTowerPerformanceYorkCalc::coefficient18() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient18();
}

double CoolingTowerPerformanceYorkCalc::coefficient19() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient19();
}

double CoolingTowerPerformanceYorkCalc::coefficient20() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient20();
}

double CoolingTowerPerformanceYorkCalc::coefficient21() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient21();
}

double CoolingTowerPerformanceYorkCalc::coefficient22() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient22();
}

double CoolingTowerPerformanceYorkCalc::coefficient23() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient23();
}

double CoolingTowerPerformanceYorkCalc::coefficient24() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient24();
}

double CoolingTowerPerformanceYorkCalc::coefficient25() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient25();
}

double CoolingTowerPerformanceYorkCalc::coefficient26() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient26();
}

double CoolingTowerPerformanceYorkCalc::coefficient27() const {
  return getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->coefficient27();
}

void CoolingTowerPerformanceYorkCalc::setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMinimumInletAirWetBulbTemperature(minimumInletAirWetBulbTemperature);
}

void CoolingTowerPerformanceYorkCalc::setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMaximumInletAirWetBulbTemperature(maximumInletAirWetBulbTemperature);
}

void CoolingTowerPerformanceYorkCalc::setMinimumRangeTemperature(double minimumRangeTemperature) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMinimumRangeTemperature(minimumRangeTemperature);
}

void CoolingTowerPerformanceYorkCalc::setMaximumRangeTemperature(double maximumRangeTemperature) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMaximumRangeTemperature(maximumRangeTemperature);
}

void CoolingTowerPerformanceYorkCalc::setMinimumApproachTemperature(double minimumApproachTemperature) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMinimumApproachTemperature(minimumApproachTemperature);
}

void CoolingTowerPerformanceYorkCalc::setMaximumApproachTemperature(double maximumApproachTemperature) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMaximumApproachTemperature(maximumApproachTemperature);
}

void CoolingTowerPerformanceYorkCalc::setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMinimumWaterFlowRateRatio(minimumWaterFlowRateRatio);
}

void CoolingTowerPerformanceYorkCalc::setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMaximumWaterFlowRateRatio(maximumWaterFlowRateRatio);
}

void CoolingTowerPerformanceYorkCalc::setMaximumLiquidtoGasRatio(double maximumLiquidtoGasRatio) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setMaximumLiquidtoGasRatio(maximumLiquidtoGasRatio);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient1(double coefficient1) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient1(coefficient1);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient2(double coefficient2) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient2(coefficient2);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient3(double coefficient3) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient3(coefficient3);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient4(double coefficient4) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient4(coefficient4);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient5(double coefficient5) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient5(coefficient5);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient6(double coefficient6) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient6(coefficient6);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient7(double coefficient7) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient7(coefficient7);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient8(double coefficient8) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient8(coefficient8);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient9(double coefficient9) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient9(coefficient9);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient10(double coefficient10) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient10(coefficient10);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient11(double coefficient11) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient11(coefficient11);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient12(double coefficient12) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient12(coefficient12);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient13(double coefficient13) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient13(coefficient13);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient14(double coefficient14) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient14(coefficient14);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient15(double coefficient15) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient15(coefficient15);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient16(double coefficient16) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient16(coefficient16);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient17(double coefficient17) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient17(coefficient17);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient18(double coefficient18) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient18(coefficient18);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient19(double coefficient19) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient19(coefficient19);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient20(double coefficient20) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient20(coefficient20);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient21(double coefficient21) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient21(coefficient21);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient22(double coefficient22) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient22(coefficient22);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient23(double coefficient23) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient23(coefficient23);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient24(double coefficient24) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient24(coefficient24);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient25(double coefficient25) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient25(coefficient25);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient26(double coefficient26) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient26(coefficient26);
}

void CoolingTowerPerformanceYorkCalc::setCoefficient27(double coefficient27) {
  getImpl<detail::CoolingTowerPerformanceYorkCalc_Impl>()->setCoefficient27(coefficient27);
}

/// @cond
CoolingTowerPerformanceYorkCalc::CoolingTowerPerformanceYorkCalc(std::shared_ptr<detail::CoolingTowerPerformanceYorkCalc_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

