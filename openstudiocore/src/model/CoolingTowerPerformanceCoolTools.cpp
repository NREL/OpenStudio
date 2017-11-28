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

#include "CoolingTowerPerformanceCoolTools.hpp"
#include "CoolingTowerPerformanceCoolTools_Impl.hpp"

#include <utilities/idd/OS_CoolingTowerPerformance_CoolTools_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoolingTowerPerformanceCoolTools_Impl::CoolingTowerPerformanceCoolTools_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoolingTowerPerformanceCoolTools::iddObjectType());
  }

  CoolingTowerPerformanceCoolTools_Impl::CoolingTowerPerformanceCoolTools_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoolingTowerPerformanceCoolTools::iddObjectType());
  }

  CoolingTowerPerformanceCoolTools_Impl::CoolingTowerPerformanceCoolTools_Impl(const CoolingTowerPerformanceCoolTools_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoolingTowerPerformanceCoolTools_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoolingTowerPerformanceCoolTools_Impl::iddObjectType() const {
    return CoolingTowerPerformanceCoolTools::iddObjectType();
  }

  double CoolingTowerPerformanceCoolTools_Impl::minimumInletAirWetBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::maximumInletAirWetBulbTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumInletAirWetBulbTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::minimumRangeTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumRangeTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::maximumRangeTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumRangeTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::minimumApproachTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumApproachTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::maximumApproachTemperature() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumApproachTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::minimumWaterFlowRateRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumWaterFlowRateRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::maximumWaterFlowRateRatio() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumWaterFlowRateRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient1() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient2() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient3() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient4() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient5() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient6() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient6,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient7() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient7,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient8() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient8,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient9() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient9,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient10() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient10,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient11() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient11,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient12() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient12,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient13() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient13,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient14() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient14,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient15() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient15,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient16() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient16,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient17() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient17,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient18() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient18,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient19() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient19,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient20() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient20,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient21() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient21,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient22() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient22,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient23() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient23,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient24() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient24,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient25() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient25,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient26() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient26,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient27() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient27,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient28() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient28,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient29() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient29,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient30() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient30,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient31() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient31,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient32() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient32,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient33() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient33,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient34() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient34,true);
    OS_ASSERT(value);
    return value.get();
  }

  double CoolingTowerPerformanceCoolTools_Impl::coefficient35() const {
    boost::optional<double> value = getDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient35,true);
    OS_ASSERT(value);
    return value.get();
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumInletAirWetBulbTemperature, minimumInletAirWetBulbTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumInletAirWetBulbTemperature, maximumInletAirWetBulbTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMinimumRangeTemperature(double minimumRangeTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumRangeTemperature, minimumRangeTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMaximumRangeTemperature(double maximumRangeTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumRangeTemperature, maximumRangeTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMinimumApproachTemperature(double minimumApproachTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumApproachTemperature, minimumApproachTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMaximumApproachTemperature(double maximumApproachTemperature) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumApproachTemperature, maximumApproachTemperature);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MinimumWaterFlowRateRatio, minimumWaterFlowRateRatio);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::MaximumWaterFlowRateRatio, maximumWaterFlowRateRatio);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient1(double coefficient1) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient1, coefficient1);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient2(double coefficient2) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient2, coefficient2);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient3(double coefficient3) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient3, coefficient3);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient4(double coefficient4) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient4, coefficient4);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient5(double coefficient5) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient5, coefficient5);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient6(double coefficient6) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient6, coefficient6);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient7(double coefficient7) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient7, coefficient7);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient8(double coefficient8) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient8, coefficient8);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient9(double coefficient9) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient9, coefficient9);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient10(double coefficient10) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient10, coefficient10);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient11(double coefficient11) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient11, coefficient11);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient12(double coefficient12) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient12, coefficient12);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient13(double coefficient13) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient13, coefficient13);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient14(double coefficient14) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient14, coefficient14);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient15(double coefficient15) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient15, coefficient15);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient16(double coefficient16) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient16, coefficient16);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient17(double coefficient17) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient17, coefficient17);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient18(double coefficient18) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient18, coefficient18);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient19(double coefficient19) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient19, coefficient19);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient20(double coefficient20) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient20, coefficient20);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient21(double coefficient21) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient21, coefficient21);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient22(double coefficient22) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient22, coefficient22);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient23(double coefficient23) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient23, coefficient23);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient24(double coefficient24) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient24, coefficient24);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient25(double coefficient25) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient25, coefficient25);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient26(double coefficient26) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient26, coefficient26);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient27(double coefficient27) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient27, coefficient27);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient28(double coefficient28) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient28, coefficient28);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient29(double coefficient29) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient29, coefficient29);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient30(double coefficient30) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient30, coefficient30);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient31(double coefficient31) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient31, coefficient31);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient32(double coefficient32) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient32, coefficient32);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient33(double coefficient33) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient33, coefficient33);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient34(double coefficient34) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient34, coefficient34);
    OS_ASSERT(result);
  }

  void CoolingTowerPerformanceCoolTools_Impl::setCoefficient35(double coefficient35) {
    bool result = setDouble(OS_CoolingTowerPerformance_CoolToolsFields::Coefficient35, coefficient35);
    OS_ASSERT(result);
  }

} // detail

CoolingTowerPerformanceCoolTools::CoolingTowerPerformanceCoolTools(const Model& model)
  : ModelObject(CoolingTowerPerformanceCoolTools::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>());

  setMinimumInletAirWetBulbTemperature(-1.0);
  setMaximumInletAirWetBulbTemperature(26.6667);
  setMinimumRangeTemperature(1.1111);
  setMaximumRangeTemperature(11.1111);
  setMinimumApproachTemperature(1.1111);
  setMaximumApproachTemperature(11.1111);
  setMinimumWaterFlowRateRatio(0.75);
  setMaximumWaterFlowRateRatio(1.25);
  setCoefficient1(0.52049709836241);
  setCoefficient2(-10.617046395344);
  setCoefficient3(10.7292974722538);
  setCoefficient4(-2.74988377158227);
  setCoefficient5(4.73629943913743);
  setCoefficient6(-8.25759700874711);
  setCoefficient7(1.57640938114136);
  setCoefficient8(6.51119643791324);
  setCoefficient9(1.50433525206692);
  setCoefficient10(-3.2888529287801);
  setCoefficient11(0.0257786145353773);
  setCoefficient12(0.182464289315254);
  setCoefficient13(-0.0818947291400898);
  setCoefficient14(-0.215010003996285);
  setCoefficient15(0.0186741309635284);
  setCoefficient16(0.0536824177590012);
  setCoefficient17(-0.00270968955115031);
  setCoefficient18(0.00112277498589279);
  setCoefficient19(-0.00127758497497718);
  setCoefficient20(0.0000760420796601607);
  setCoefficient21(1.43600088336017);
  setCoefficient22(-0.5198695909109);
  setCoefficient23(0.117339576910507);
  setCoefficient24(1.50492810819924);
  setCoefficient25(-0.135898905926974);
  setCoefficient26(-0.152577581866506);
  setCoefficient27(-0.0533843828114562);
  setCoefficient28(0.00493294869565511);
  setCoefficient29(-0.00796260394174197);
  setCoefficient30(0.000222619828621544);
  setCoefficient31(-0.0543952001568055);
  setCoefficient32(0.00474266879161693);
  setCoefficient33(-0.0185854671815598);
  setCoefficient34(0.00115667701293848);
  setCoefficient35(0.000807370664460284);
}

IddObjectType CoolingTowerPerformanceCoolTools::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoolingTowerPerformance_CoolTools);
}

double CoolingTowerPerformanceCoolTools::minimumInletAirWetBulbTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->minimumInletAirWetBulbTemperature();
}

double CoolingTowerPerformanceCoolTools::maximumInletAirWetBulbTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->maximumInletAirWetBulbTemperature();
}

double CoolingTowerPerformanceCoolTools::minimumRangeTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->minimumRangeTemperature();
}

double CoolingTowerPerformanceCoolTools::maximumRangeTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->maximumRangeTemperature();
}

double CoolingTowerPerformanceCoolTools::minimumApproachTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->minimumApproachTemperature();
}

double CoolingTowerPerformanceCoolTools::maximumApproachTemperature() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->maximumApproachTemperature();
}

double CoolingTowerPerformanceCoolTools::minimumWaterFlowRateRatio() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->minimumWaterFlowRateRatio();
}

double CoolingTowerPerformanceCoolTools::maximumWaterFlowRateRatio() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->maximumWaterFlowRateRatio();
}

double CoolingTowerPerformanceCoolTools::coefficient1() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient1();
}

double CoolingTowerPerformanceCoolTools::coefficient2() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient2();
}

double CoolingTowerPerformanceCoolTools::coefficient3() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient3();
}

double CoolingTowerPerformanceCoolTools::coefficient4() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient4();
}

double CoolingTowerPerformanceCoolTools::coefficient5() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient5();
}

double CoolingTowerPerformanceCoolTools::coefficient6() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient6();
}

double CoolingTowerPerformanceCoolTools::coefficient7() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient7();
}

double CoolingTowerPerformanceCoolTools::coefficient8() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient8();
}

double CoolingTowerPerformanceCoolTools::coefficient9() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient9();
}

double CoolingTowerPerformanceCoolTools::coefficient10() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient10();
}

double CoolingTowerPerformanceCoolTools::coefficient11() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient11();
}

double CoolingTowerPerformanceCoolTools::coefficient12() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient12();
}

double CoolingTowerPerformanceCoolTools::coefficient13() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient13();
}

double CoolingTowerPerformanceCoolTools::coefficient14() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient14();
}

double CoolingTowerPerformanceCoolTools::coefficient15() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient15();
}

double CoolingTowerPerformanceCoolTools::coefficient16() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient16();
}

double CoolingTowerPerformanceCoolTools::coefficient17() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient17();
}

double CoolingTowerPerformanceCoolTools::coefficient18() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient18();
}

double CoolingTowerPerformanceCoolTools::coefficient19() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient19();
}

double CoolingTowerPerformanceCoolTools::coefficient20() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient20();
}

double CoolingTowerPerformanceCoolTools::coefficient21() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient21();
}

double CoolingTowerPerformanceCoolTools::coefficient22() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient22();
}

double CoolingTowerPerformanceCoolTools::coefficient23() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient23();
}

double CoolingTowerPerformanceCoolTools::coefficient24() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient24();
}

double CoolingTowerPerformanceCoolTools::coefficient25() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient25();
}

double CoolingTowerPerformanceCoolTools::coefficient26() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient26();
}

double CoolingTowerPerformanceCoolTools::coefficient27() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient27();
}

double CoolingTowerPerformanceCoolTools::coefficient28() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient28();
}

double CoolingTowerPerformanceCoolTools::coefficient29() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient29();
}

double CoolingTowerPerformanceCoolTools::coefficient30() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient30();
}

double CoolingTowerPerformanceCoolTools::coefficient31() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient31();
}

double CoolingTowerPerformanceCoolTools::coefficient32() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient32();
}

double CoolingTowerPerformanceCoolTools::coefficient33() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient33();
}

double CoolingTowerPerformanceCoolTools::coefficient34() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient34();
}

double CoolingTowerPerformanceCoolTools::coefficient35() const {
  return getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->coefficient35();
}

void CoolingTowerPerformanceCoolTools::setMinimumInletAirWetBulbTemperature(double minimumInletAirWetBulbTemperature) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMinimumInletAirWetBulbTemperature(minimumInletAirWetBulbTemperature);
}

void CoolingTowerPerformanceCoolTools::setMaximumInletAirWetBulbTemperature(double maximumInletAirWetBulbTemperature) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMaximumInletAirWetBulbTemperature(maximumInletAirWetBulbTemperature);
}

void CoolingTowerPerformanceCoolTools::setMinimumRangeTemperature(double minimumRangeTemperature) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMinimumRangeTemperature(minimumRangeTemperature);
}

void CoolingTowerPerformanceCoolTools::setMaximumRangeTemperature(double maximumRangeTemperature) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMaximumRangeTemperature(maximumRangeTemperature);
}

void CoolingTowerPerformanceCoolTools::setMinimumApproachTemperature(double minimumApproachTemperature) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMinimumApproachTemperature(minimumApproachTemperature);
}

void CoolingTowerPerformanceCoolTools::setMaximumApproachTemperature(double maximumApproachTemperature) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMaximumApproachTemperature(maximumApproachTemperature);
}

void CoolingTowerPerformanceCoolTools::setMinimumWaterFlowRateRatio(double minimumWaterFlowRateRatio) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMinimumWaterFlowRateRatio(minimumWaterFlowRateRatio);
}

void CoolingTowerPerformanceCoolTools::setMaximumWaterFlowRateRatio(double maximumWaterFlowRateRatio) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setMaximumWaterFlowRateRatio(maximumWaterFlowRateRatio);
}

void CoolingTowerPerformanceCoolTools::setCoefficient1(double coefficient1) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient1(coefficient1);
}

void CoolingTowerPerformanceCoolTools::setCoefficient2(double coefficient2) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient2(coefficient2);
}

void CoolingTowerPerformanceCoolTools::setCoefficient3(double coefficient3) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient3(coefficient3);
}

void CoolingTowerPerformanceCoolTools::setCoefficient4(double coefficient4) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient4(coefficient4);
}

void CoolingTowerPerformanceCoolTools::setCoefficient5(double coefficient5) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient5(coefficient5);
}

void CoolingTowerPerformanceCoolTools::setCoefficient6(double coefficient6) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient6(coefficient6);
}

void CoolingTowerPerformanceCoolTools::setCoefficient7(double coefficient7) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient7(coefficient7);
}

void CoolingTowerPerformanceCoolTools::setCoefficient8(double coefficient8) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient8(coefficient8);
}

void CoolingTowerPerformanceCoolTools::setCoefficient9(double coefficient9) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient9(coefficient9);
}

void CoolingTowerPerformanceCoolTools::setCoefficient10(double coefficient10) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient10(coefficient10);
}

void CoolingTowerPerformanceCoolTools::setCoefficient11(double coefficient11) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient11(coefficient11);
}

void CoolingTowerPerformanceCoolTools::setCoefficient12(double coefficient12) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient12(coefficient12);
}

void CoolingTowerPerformanceCoolTools::setCoefficient13(double coefficient13) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient13(coefficient13);
}

void CoolingTowerPerformanceCoolTools::setCoefficient14(double coefficient14) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient14(coefficient14);
}

void CoolingTowerPerformanceCoolTools::setCoefficient15(double coefficient15) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient15(coefficient15);
}

void CoolingTowerPerformanceCoolTools::setCoefficient16(double coefficient16) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient16(coefficient16);
}

void CoolingTowerPerformanceCoolTools::setCoefficient17(double coefficient17) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient17(coefficient17);
}

void CoolingTowerPerformanceCoolTools::setCoefficient18(double coefficient18) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient18(coefficient18);
}

void CoolingTowerPerformanceCoolTools::setCoefficient19(double coefficient19) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient19(coefficient19);
}

void CoolingTowerPerformanceCoolTools::setCoefficient20(double coefficient20) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient20(coefficient20);
}

void CoolingTowerPerformanceCoolTools::setCoefficient21(double coefficient21) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient21(coefficient21);
}

void CoolingTowerPerformanceCoolTools::setCoefficient22(double coefficient22) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient22(coefficient22);
}

void CoolingTowerPerformanceCoolTools::setCoefficient23(double coefficient23) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient23(coefficient23);
}

void CoolingTowerPerformanceCoolTools::setCoefficient24(double coefficient24) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient24(coefficient24);
}

void CoolingTowerPerformanceCoolTools::setCoefficient25(double coefficient25) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient25(coefficient25);
}

void CoolingTowerPerformanceCoolTools::setCoefficient26(double coefficient26) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient26(coefficient26);
}

void CoolingTowerPerformanceCoolTools::setCoefficient27(double coefficient27) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient27(coefficient27);
}

void CoolingTowerPerformanceCoolTools::setCoefficient28(double coefficient28) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient28(coefficient28);
}

void CoolingTowerPerformanceCoolTools::setCoefficient29(double coefficient29) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient29(coefficient29);
}

void CoolingTowerPerformanceCoolTools::setCoefficient30(double coefficient30) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient30(coefficient30);
}

void CoolingTowerPerformanceCoolTools::setCoefficient31(double coefficient31) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient31(coefficient31);
}

void CoolingTowerPerformanceCoolTools::setCoefficient32(double coefficient32) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient32(coefficient32);
}

void CoolingTowerPerformanceCoolTools::setCoefficient33(double coefficient33) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient33(coefficient33);
}

void CoolingTowerPerformanceCoolTools::setCoefficient34(double coefficient34) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient34(coefficient34);
}

void CoolingTowerPerformanceCoolTools::setCoefficient35(double coefficient35) {
  getImpl<detail::CoolingTowerPerformanceCoolTools_Impl>()->setCoefficient35(coefficient35);
}

/// @cond
CoolingTowerPerformanceCoolTools::CoolingTowerPerformanceCoolTools(std::shared_ptr<detail::CoolingTowerPerformanceCoolTools_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

