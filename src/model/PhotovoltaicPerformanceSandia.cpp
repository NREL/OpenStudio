/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "PhotovoltaicPerformanceSandia.hpp"
#include "PhotovoltaicPerformanceSandia_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PhotovoltaicPerformance_Sandia_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PhotovoltaicPerformanceSandia_Impl::PhotovoltaicPerformanceSandia_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PhotovoltaicPerformance_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PhotovoltaicPerformanceSandia::iddObjectType());
  }

  PhotovoltaicPerformanceSandia_Impl::PhotovoltaicPerformanceSandia_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PhotovoltaicPerformance_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PhotovoltaicPerformanceSandia::iddObjectType());
  }

  PhotovoltaicPerformanceSandia_Impl::PhotovoltaicPerformanceSandia_Impl(const PhotovoltaicPerformanceSandia_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : PhotovoltaicPerformance_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PhotovoltaicPerformanceSandia_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PhotovoltaicPerformanceSandia_Impl::iddObjectType() const {
    return PhotovoltaicPerformanceSandia::iddObjectType();
  }

  double PhotovoltaicPerformanceSandia_Impl::activeArea() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::ActiveArea,true);
    OS_ASSERT(value);
    return value.get();
  }

  int PhotovoltaicPerformanceSandia_Impl::numberofCellsinSeries() const {
    boost::optional<int> value = getInt(OS_PhotovoltaicPerformance_SandiaFields::NumberofCellsinSeries,true);
    OS_ASSERT(value);
    return value.get();
  }

  int PhotovoltaicPerformanceSandia_Impl::numberofCellsinParallel() const {
    boost::optional<int> value = getInt(OS_PhotovoltaicPerformance_SandiaFields::NumberofCellsinParallel,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::shortCircuitCurrent() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::ShortCircuitCurrent,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::openCircuitVoltage() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::OpenCircuitVoltage,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::currentatMaximumPowerPoint() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::CurrentatMaximumPowerPoint,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::voltageatMaximumPowerPoint() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::VoltageatMaximumPowerPoint,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameteraIsc() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraIsc,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameteraImp() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraImp,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc0() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc0,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc1() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterBVoc0() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVoc0,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametermBVoc() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVoc,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterBVmp0() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVmp0,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametermBVmp() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVmp,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::diodeFactor() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::DiodeFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc2() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc3() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera0() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera0,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera1() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera2() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera3() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera4() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb0() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb0,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb1() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb1,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb2() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb2,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb3() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb3,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb4() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb5() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterDeltaTc() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterDelta_Tc_,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterfd() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterfd,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc4() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc4,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc5() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc5,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterIx0() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIx0,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterIxx0() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIxx0,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc6() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc6,true);
    OS_ASSERT(value);
    return value.get();
  }

  double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc7() const {
    boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc7,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PhotovoltaicPerformanceSandia_Impl::setActiveArea(double activeArea) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::ActiveArea, activeArea);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setNumberofCellsinSeries(int numberofCellsinSeries) {
    bool result = setInt(OS_PhotovoltaicPerformance_SandiaFields::NumberofCellsinSeries, numberofCellsinSeries);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setNumberofCellsinParallel(int numberofCellsinParallel) {
    bool result = setInt(OS_PhotovoltaicPerformance_SandiaFields::NumberofCellsinParallel, numberofCellsinParallel);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setShortCircuitCurrent(double shortCircuitCurrent) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::ShortCircuitCurrent, shortCircuitCurrent);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setOpenCircuitVoltage(double openCircuitVoltage) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::OpenCircuitVoltage, openCircuitVoltage);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setCurrentatMaximumPowerPoint(double currentatMaximumPowerPoint) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::CurrentatMaximumPowerPoint, currentatMaximumPowerPoint);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setVoltageatMaximumPowerPoint(double voltageatMaximumPowerPoint) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::VoltageatMaximumPowerPoint, voltageatMaximumPowerPoint);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameteraIsc(double sandiaDatabaseParameteraIsc) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraIsc, sandiaDatabaseParameteraIsc);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameteraImp(double sandiaDatabaseParameteraImp) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraImp, sandiaDatabaseParameteraImp);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc0(double sandiaDatabaseParameterc0) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc0, sandiaDatabaseParameterc0);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc1(double sandiaDatabaseParameterc1) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc1, sandiaDatabaseParameterc1);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterBVoc0(double sandiaDatabaseParameterBVoc0) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVoc0, sandiaDatabaseParameterBVoc0);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametermBVoc(double sandiaDatabaseParametermBVoc) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVoc, sandiaDatabaseParametermBVoc);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterBVmp0(double sandiaDatabaseParameterBVmp0) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVmp0, sandiaDatabaseParameterBVmp0);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametermBVmp(double sandiaDatabaseParametermBVmp) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVmp, sandiaDatabaseParametermBVmp);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setDiodeFactor(double diodeFactor) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::DiodeFactor, diodeFactor);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc2(double sandiaDatabaseParameterc2) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc2, sandiaDatabaseParameterc2);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc3(double sandiaDatabaseParameterc3) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc3, sandiaDatabaseParameterc3);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametera0(double sandiaDatabaseParametera0) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera0, sandiaDatabaseParametera0);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametera1(double sandiaDatabaseParametera1) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera1, sandiaDatabaseParametera1);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametera2(double sandiaDatabaseParametera2) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera2, sandiaDatabaseParametera2);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametera3(double sandiaDatabaseParametera3) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera3, sandiaDatabaseParametera3);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametera4(double sandiaDatabaseParametera4) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera4, sandiaDatabaseParametera4);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterb0(double sandiaDatabaseParameterb0) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb0, sandiaDatabaseParameterb0);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterb1(double sandiaDatabaseParameterb1) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb1, sandiaDatabaseParameterb1);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterb2(double sandiaDatabaseParameterb2) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb2, sandiaDatabaseParameterb2);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterb3(double sandiaDatabaseParameterb3) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb3, sandiaDatabaseParameterb3);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterb4(double sandiaDatabaseParameterb4) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb4, sandiaDatabaseParameterb4);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterb5(double sandiaDatabaseParameterb5) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb5, sandiaDatabaseParameterb5);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterDeltaTc(double sandiaDatabaseParameterDeltaTc) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterDelta_Tc_, sandiaDatabaseParameterDeltaTc);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterfd(double sandiaDatabaseParameterfd) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterfd, sandiaDatabaseParameterfd);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParametera(double sandiaDatabaseParametera) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera, sandiaDatabaseParametera);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterb(double sandiaDatabaseParameterb) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb, sandiaDatabaseParameterb);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc4(double sandiaDatabaseParameterc4) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc4, sandiaDatabaseParameterc4);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc5(double sandiaDatabaseParameterc5) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc5, sandiaDatabaseParameterc5);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterIx0(double sandiaDatabaseParameterIx0) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIx0, sandiaDatabaseParameterIx0);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterIxx0(double sandiaDatabaseParameterIxx0) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIxx0, sandiaDatabaseParameterIxx0);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc6(double sandiaDatabaseParameterc6) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc6, sandiaDatabaseParameterc6);
    OS_ASSERT(result);
    return result;
  }

  bool PhotovoltaicPerformanceSandia_Impl::setSandiaDatabaseParameterc7(double sandiaDatabaseParameterc7) {
    bool result = setDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc7, sandiaDatabaseParameterc7);
    OS_ASSERT(result);
    return result;
  }

} // detail

PhotovoltaicPerformanceSandia::PhotovoltaicPerformanceSandia(const Model& model)
  : PhotovoltaicPerformance(PhotovoltaicPerformanceSandia::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PhotovoltaicPerformanceSandia_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setActiveArea();
  OS_ASSERT(ok);
  // ok = setNumberofCellsinSeries();
  OS_ASSERT(ok);
  // ok = setNumberofCellsinParallel();
  OS_ASSERT(ok);
  // setShortCircuitCurrent();
  // setOpenCircuitVoltage();
  // setCurrentatMaximumPowerPoint();
  // setVoltageatMaximumPowerPoint();
  // setSandiaDatabaseParameteraIsc();
  // setSandiaDatabaseParameteraImp();
  // setSandiaDatabaseParameterc0();
  // setSandiaDatabaseParameterc1();
  // setSandiaDatabaseParameterBVoc0();
  // setSandiaDatabaseParametermBVoc();
  // setSandiaDatabaseParameterBVmp0();
  // setSandiaDatabaseParametermBVmp();
  // setDiodeFactor();
  // setSandiaDatabaseParameterc2();
  // setSandiaDatabaseParameterc3();
  // setSandiaDatabaseParametera0();
  // setSandiaDatabaseParametera1();
  // setSandiaDatabaseParametera2();
  // setSandiaDatabaseParametera3();
  // setSandiaDatabaseParametera4();
  // setSandiaDatabaseParameterb0();
  // setSandiaDatabaseParameterb1();
  // setSandiaDatabaseParameterb2();
  // setSandiaDatabaseParameterb3();
  // setSandiaDatabaseParameterb4();
  // setSandiaDatabaseParameterb5();
  // setSandiaDatabaseParameterDeltaTc();
  // setSandiaDatabaseParameterfd();
  // setSandiaDatabaseParametera();
  // setSandiaDatabaseParameterb();
  // setSandiaDatabaseParameterc4();
  // setSandiaDatabaseParameterc5();
  // setSandiaDatabaseParameterIx0();
  // setSandiaDatabaseParameterIxx0();
  // setSandiaDatabaseParameterc6();
  // setSandiaDatabaseParameterc7();
}

IddObjectType PhotovoltaicPerformanceSandia::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PhotovoltaicPerformance_Sandia);
}

double PhotovoltaicPerformanceSandia::activeArea() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->activeArea();
}

int PhotovoltaicPerformanceSandia::numberofCellsinSeries() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->numberofCellsinSeries();
}

int PhotovoltaicPerformanceSandia::numberofCellsinParallel() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->numberofCellsinParallel();
}

double PhotovoltaicPerformanceSandia::shortCircuitCurrent() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->shortCircuitCurrent();
}

double PhotovoltaicPerformanceSandia::openCircuitVoltage() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->openCircuitVoltage();
}

double PhotovoltaicPerformanceSandia::currentatMaximumPowerPoint() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->currentatMaximumPowerPoint();
}

double PhotovoltaicPerformanceSandia::voltageatMaximumPowerPoint() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->voltageatMaximumPowerPoint();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameteraIsc() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameteraIsc();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameteraImp() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameteraImp();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc0() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc0();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc1() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc1();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterBVoc0() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterBVoc0();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametermBVoc() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametermBVoc();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterBVmp0() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterBVmp0();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametermBVmp() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametermBVmp();
}

double PhotovoltaicPerformanceSandia::diodeFactor() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->diodeFactor();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc2() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc2();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc3() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc3();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametera0() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametera0();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametera1() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametera1();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametera2() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametera2();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametera3() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametera3();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametera4() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametera4();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterb0() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterb0();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterb1() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterb1();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterb2() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterb2();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterb3() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterb3();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterb4() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterb4();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterb5() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterb5();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterDeltaTc() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterDeltaTc();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterfd() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterfd();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParametera() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParametera();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterb() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterb();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc4() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc4();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc5() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc5();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterIx0() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterIx0();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterIxx0() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterIxx0();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc6() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc6();
}

double PhotovoltaicPerformanceSandia::sandiaDatabaseParameterc7() const {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->sandiaDatabaseParameterc7();
}

bool PhotovoltaicPerformanceSandia::setActiveArea(double activeArea) {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setActiveArea(activeArea);
}

bool PhotovoltaicPerformanceSandia::setNumberofCellsinSeries(int numberofCellsinSeries) {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setNumberofCellsinSeries(numberofCellsinSeries);
}

bool PhotovoltaicPerformanceSandia::setNumberofCellsinParallel(int numberofCellsinParallel) {
  return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setNumberofCellsinParallel(numberofCellsinParallel);
}

bool PhotovoltaicPerformanceSandia::setShortCircuitCurrent(double shortCircuitCurrent) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setShortCircuitCurrent(shortCircuitCurrent);
}

bool PhotovoltaicPerformanceSandia::setOpenCircuitVoltage(double openCircuitVoltage) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setOpenCircuitVoltage(openCircuitVoltage);
}

bool PhotovoltaicPerformanceSandia::setCurrentatMaximumPowerPoint(double currentatMaximumPowerPoint) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setCurrentatMaximumPowerPoint(currentatMaximumPowerPoint);
}

bool PhotovoltaicPerformanceSandia::setVoltageatMaximumPowerPoint(double voltageatMaximumPowerPoint) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setVoltageatMaximumPowerPoint(voltageatMaximumPowerPoint);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameteraIsc(double sandiaDatabaseParameteraIsc) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameteraIsc(sandiaDatabaseParameteraIsc);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameteraImp(double sandiaDatabaseParameteraImp) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameteraImp(sandiaDatabaseParameteraImp);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc0(double sandiaDatabaseParameterc0) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc0(sandiaDatabaseParameterc0);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc1(double sandiaDatabaseParameterc1) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc1(sandiaDatabaseParameterc1);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterBVoc0(double sandiaDatabaseParameterBVoc0) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterBVoc0(sandiaDatabaseParameterBVoc0);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametermBVoc(double sandiaDatabaseParametermBVoc) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametermBVoc(sandiaDatabaseParametermBVoc);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterBVmp0(double sandiaDatabaseParameterBVmp0) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterBVmp0(sandiaDatabaseParameterBVmp0);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametermBVmp(double sandiaDatabaseParametermBVmp) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametermBVmp(sandiaDatabaseParametermBVmp);
}

bool PhotovoltaicPerformanceSandia::setDiodeFactor(double diodeFactor) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setDiodeFactor(diodeFactor);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc2(double sandiaDatabaseParameterc2) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc2(sandiaDatabaseParameterc2);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc3(double sandiaDatabaseParameterc3) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc3(sandiaDatabaseParameterc3);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera0(double sandiaDatabaseParametera0) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera0(sandiaDatabaseParametera0);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera1(double sandiaDatabaseParametera1) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera1(sandiaDatabaseParametera1);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera2(double sandiaDatabaseParametera2) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera2(sandiaDatabaseParametera2);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera3(double sandiaDatabaseParametera3) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera3(sandiaDatabaseParametera3);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera4(double sandiaDatabaseParametera4) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera4(sandiaDatabaseParametera4);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb0(double sandiaDatabaseParameterb0) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb0(sandiaDatabaseParameterb0);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb1(double sandiaDatabaseParameterb1) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb1(sandiaDatabaseParameterb1);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb2(double sandiaDatabaseParameterb2) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb2(sandiaDatabaseParameterb2);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb3(double sandiaDatabaseParameterb3) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb3(sandiaDatabaseParameterb3);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb4(double sandiaDatabaseParameterb4) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb4(sandiaDatabaseParameterb4);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb5(double sandiaDatabaseParameterb5) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb5(sandiaDatabaseParameterb5);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterDeltaTc(double sandiaDatabaseParameterDeltaTc) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterDeltaTc(sandiaDatabaseParameterDeltaTc);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterfd(double sandiaDatabaseParameterfd) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterfd(sandiaDatabaseParameterfd);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera(double sandiaDatabaseParametera) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera(sandiaDatabaseParametera);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb(double sandiaDatabaseParameterb) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb(sandiaDatabaseParameterb);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc4(double sandiaDatabaseParameterc4) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc4(sandiaDatabaseParameterc4);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc5(double sandiaDatabaseParameterc5) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc5(sandiaDatabaseParameterc5);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterIx0(double sandiaDatabaseParameterIx0) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterIx0(sandiaDatabaseParameterIx0);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterIxx0(double sandiaDatabaseParameterIxx0) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterIxx0(sandiaDatabaseParameterIxx0);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc6(double sandiaDatabaseParameterc6) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc6(sandiaDatabaseParameterc6);
}

bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc7(double sandiaDatabaseParameterc7) {
  getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc7(sandiaDatabaseParameterc7);
}

/// @cond
PhotovoltaicPerformanceSandia::PhotovoltaicPerformanceSandia(std::shared_ptr<detail::PhotovoltaicPerformanceSandia_Impl> impl)
  : PhotovoltaicPerformance(impl)
{}
/// @endcond

} // model
} // openstudio

