/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <json/value.h>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_PhotovoltaicPerformance_Sandia_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

#include <model/embedded_files.hxx>
#include "../utilities/filetypes/StandardsJSON.hpp"
#include "../utilities/core/Json.hpp"
#include <json/json.h>

namespace openstudio {
namespace model {

  namespace detail {

    Json::Value parseSandiaJSON() {
      // Embedded file path
      std::string embedded_path = ":/Resources/sandia/Sandia_Modules.json";
      std::string fileContent = ::openstudiomodel::embedded_files::getFileAsString(embedded_path);

      std::istringstream ss(fileContent);
      Json::Value sandiaRoot;
      ss >> sandiaRoot;

      return sandiaRoot["data"];
    }

    const Json::Value& PhotovoltaicPerformanceSandia_Impl::getSandiaJSON() {
      // Relies on utilties/StandardsJSON since it's used in several places
      // Here we store a Json::Value rather than the StandardsJSON because we only care about the "space_types" primaryKey
      // Not the whole StandardsJSON
      static const Json::Value sandiaRoot{parseSandiaJSON()};
      return sandiaRoot;
    }

    std::vector<std::string> PhotovoltaicPerformanceSandia_Impl::sandiaModulePerformanceNames() {
      return getSandiaJSON().getMemberNames();
    }

    PhotovoltaicPerformanceSandia_Impl::PhotovoltaicPerformanceSandia_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : PhotovoltaicPerformance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PhotovoltaicPerformanceSandia::iddObjectType());
    }

    PhotovoltaicPerformanceSandia_Impl::PhotovoltaicPerformanceSandia_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : PhotovoltaicPerformance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PhotovoltaicPerformanceSandia::iddObjectType());
    }

    PhotovoltaicPerformanceSandia_Impl::PhotovoltaicPerformanceSandia_Impl(const PhotovoltaicPerformanceSandia_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : PhotovoltaicPerformance_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PhotovoltaicPerformanceSandia_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType PhotovoltaicPerformanceSandia_Impl::iddObjectType() const {
      return PhotovoltaicPerformanceSandia::iddObjectType();
    }

    double PhotovoltaicPerformanceSandia_Impl::activeArea() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::ActiveArea, true);
      OS_ASSERT(value);
      return value.get();
    }

    int PhotovoltaicPerformanceSandia_Impl::numberofCellsinSeries() const {
      boost::optional<int> value = getInt(OS_PhotovoltaicPerformance_SandiaFields::NumberofCellsinSeries, true);
      OS_ASSERT(value);
      return value.get();
    }

    int PhotovoltaicPerformanceSandia_Impl::numberofCellsinParallel() const {
      boost::optional<int> value = getInt(OS_PhotovoltaicPerformance_SandiaFields::NumberofCellsinParallel, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::shortCircuitCurrent() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::ShortCircuitCurrent, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::openCircuitVoltage() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::OpenCircuitVoltage, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::currentatMaximumPowerPoint() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::CurrentatMaximumPowerPoint, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::voltageatMaximumPowerPoint() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::VoltageatMaximumPowerPoint, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameteraIsc() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraIsc, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameteraImp() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameteraImp, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc0() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc0, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc1() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterBVoc0() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVoc0, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametermBVoc() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVoc, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterBVmp0() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterBVmp0, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametermBVmp() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametermBVmp, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::diodeFactor() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::DiodeFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc2() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc3() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera0() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera0, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera1() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera2() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera3() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera4() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb0() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb0, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb1() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb1, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb2() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb2, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb3() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb3, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb4() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb5() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb5, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterDeltaTc() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterDelta_Tc_, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterfd() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterfd, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParametera() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParametera, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterb() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterb, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc4() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc4, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc5() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc5, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterIx0() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIx0, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterIxx0() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterIxx0, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc6() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc6, true);
      OS_ASSERT(value);
      return value.get();
    }

    double PhotovoltaicPerformanceSandia_Impl::sandiaDatabaseParameterc7() const {
      boost::optional<double> value = getDouble(OS_PhotovoltaicPerformance_SandiaFields::SandiaDatabaseParameterc7, true);
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

  }  // namespace detail

  PhotovoltaicPerformanceSandia::PhotovoltaicPerformanceSandia(const Model& model)
    : PhotovoltaicPerformance(PhotovoltaicPerformanceSandia::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PhotovoltaicPerformanceSandia_Impl>());

    // Note: these values are from the Sandia Database
    // I randomly sampled one out of 523 rows and used the values without looking at which commercial product it ended up choosing, so that it'd be fair

    bool ok = true;
    setActiveArea(0.606);
    OS_ASSERT(ok);
    setNumberofCellsinSeries(36);
    OS_ASSERT(ok);
    setNumberofCellsinParallel(1);
    OS_ASSERT(ok);

    setShortCircuitCurrent(3.87);
    setOpenCircuitVoltage(21);
    setCurrentatMaximumPowerPoint(3.56);
    setVoltageatMaximumPowerPoint(16.8);
    setSandiaDatabaseParameteraIsc(0.00065);
    setSandiaDatabaseParameteraImp(-0.0002);
    setSandiaDatabaseParameterc0(0.989);
    setSandiaDatabaseParameterc1(0.011);
    setSandiaDatabaseParameterBVoc0(-0.08);
    setSandiaDatabaseParametermBVoc(0);
    setSandiaDatabaseParameterBVmp0(-0.082);
    setSandiaDatabaseParametermBVmp(0);
    setDiodeFactor(1.32);
    setSandiaDatabaseParameterc2(-0.117);
    setSandiaDatabaseParameterc3(-11.082);
    setSandiaDatabaseParametera0(0.9415);
    setSandiaDatabaseParametera1(0.052728);
    setSandiaDatabaseParametera2(-0.0095876);
    setSandiaDatabaseParametera3(0.00067629);
    setSandiaDatabaseParametera4(-0.000018111);
    setSandiaDatabaseParameterb0(1);
    setSandiaDatabaseParameterb1(-0.002438);
    setSandiaDatabaseParameterb2(0.0003103);
    setSandiaDatabaseParameterb3(-0.00001246);
    setSandiaDatabaseParameterb4(2.11E-07);
    setSandiaDatabaseParameterb5(-1.36E-09);
    setSandiaDatabaseParameterDeltaTc(3);
    setSandiaDatabaseParameterfd(1);
    setSandiaDatabaseParametera(-3.537);
    setSandiaDatabaseParameterb(-0.0721);
    setSandiaDatabaseParameterc4(0.9866);
    setSandiaDatabaseParameterc5(0.0134);
    setSandiaDatabaseParameterIx0(3.82);
    setSandiaDatabaseParameterIxx0(2.49);
    setSandiaDatabaseParameterc6(1.1183);
    setSandiaDatabaseParameterc7(-0.1183);
  }

  PhotovoltaicPerformanceSandia::PhotovoltaicPerformanceSandia(
    const Model& model, double activeArea, int numberofCellsinSeries, int numberofCellsinParallel, double shortCircuitCurrent,
    double openCircuitVoltage, double currentatMaximumPowerPoint, double voltageatMaximumPowerPoint, double sandiaDatabaseParameteraIsc,
    double sandiaDatabaseParameteraImp, double sandiaDatabaseParameterc0, double sandiaDatabaseParameterc1, double sandiaDatabaseParameterBVoc0,
    double sandiaDatabaseParametermBVoc, double sandiaDatabaseParameterBVmp0, double sandiaDatabaseParametermBVmp, double diodeFactor,
    double sandiaDatabaseParameterc2, double sandiaDatabaseParameterc3, double sandiaDatabaseParametera0, double sandiaDatabaseParametera1,
    double sandiaDatabaseParametera2, double sandiaDatabaseParametera3, double sandiaDatabaseParametera4, double sandiaDatabaseParameterb0,
    double sandiaDatabaseParameterb1, double sandiaDatabaseParameterb2, double sandiaDatabaseParameterb3, double sandiaDatabaseParameterb4,
    double sandiaDatabaseParameterb5, double sandiaDatabaseParameterDeltaTc, double sandiaDatabaseParameterfd, double sandiaDatabaseParametera,
    double sandiaDatabaseParameterb, double sandiaDatabaseParameterc4, double sandiaDatabaseParameterc5, double sandiaDatabaseParameterIx0,
    double sandiaDatabaseParameterIxx0, double sandiaDatabaseParameterc6, double sandiaDatabaseParameterc7)
    : PhotovoltaicPerformance(PhotovoltaicPerformanceSandia::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PhotovoltaicPerformanceSandia_Impl>());

    setActiveArea(activeArea);
    setNumberofCellsinSeries(numberofCellsinSeries);
    setNumberofCellsinParallel(numberofCellsinParallel);
    setShortCircuitCurrent(shortCircuitCurrent);
    setOpenCircuitVoltage(openCircuitVoltage);
    setCurrentatMaximumPowerPoint(currentatMaximumPowerPoint);
    setVoltageatMaximumPowerPoint(voltageatMaximumPowerPoint);
    setSandiaDatabaseParameteraIsc(sandiaDatabaseParameteraIsc);
    setSandiaDatabaseParameteraImp(sandiaDatabaseParameteraImp);
    setSandiaDatabaseParameterc0(sandiaDatabaseParameterc0);
    setSandiaDatabaseParameterc1(sandiaDatabaseParameterc1);
    setSandiaDatabaseParameterBVoc0(sandiaDatabaseParameterBVoc0);
    setSandiaDatabaseParametermBVoc(sandiaDatabaseParametermBVoc);
    setSandiaDatabaseParameterBVmp0(sandiaDatabaseParameterBVmp0);
    setSandiaDatabaseParametermBVmp(sandiaDatabaseParametermBVmp);
    setDiodeFactor(diodeFactor);
    setSandiaDatabaseParameterc2(sandiaDatabaseParameterc2);
    setSandiaDatabaseParameterc3(sandiaDatabaseParameterc3);
    setSandiaDatabaseParametera0(sandiaDatabaseParametera0);
    setSandiaDatabaseParametera1(sandiaDatabaseParametera1);
    setSandiaDatabaseParametera2(sandiaDatabaseParametera2);
    setSandiaDatabaseParametera3(sandiaDatabaseParametera3);
    setSandiaDatabaseParametera4(sandiaDatabaseParametera4);
    setSandiaDatabaseParameterb0(sandiaDatabaseParameterb0);
    setSandiaDatabaseParameterb1(sandiaDatabaseParameterb1);
    setSandiaDatabaseParameterb2(sandiaDatabaseParameterb2);
    setSandiaDatabaseParameterb3(sandiaDatabaseParameterb3);
    setSandiaDatabaseParameterb4(sandiaDatabaseParameterb4);
    setSandiaDatabaseParameterb5(sandiaDatabaseParameterb5);
    setSandiaDatabaseParameterDeltaTc(sandiaDatabaseParameterDeltaTc);
    setSandiaDatabaseParameterfd(sandiaDatabaseParameterfd);
    setSandiaDatabaseParametera(sandiaDatabaseParametera);
    setSandiaDatabaseParameterb(sandiaDatabaseParameterb);
    setSandiaDatabaseParameterc4(sandiaDatabaseParameterc4);
    setSandiaDatabaseParameterc5(sandiaDatabaseParameterc5);
    setSandiaDatabaseParameterIx0(sandiaDatabaseParameterIx0);
    setSandiaDatabaseParameterIxx0(sandiaDatabaseParameterIxx0);
    setSandiaDatabaseParameterc6(sandiaDatabaseParameterc6);
    setSandiaDatabaseParameterc7(sandiaDatabaseParameterc7);
  }

  // Factory method
  PhotovoltaicPerformanceSandia PhotovoltaicPerformanceSandia::fromSandiaDatabase(const Model& model,
                                                                                  const std::string& sandiaModulePerformanceName) {
    PhotovoltaicPerformanceSandia performance(model);

    auto data = detail::PhotovoltaicPerformanceSandia_Impl::getSandiaJSON();
    // Throw if wrong sandiaModulePerformanceName
    openstudio::assertKeyAndType(data, sandiaModulePerformanceName, Json::objectValue);

    auto this_data = data[sandiaModulePerformanceName];

    // I find it clearer and less prone to error to call each setter instead of relying on the 35-args Ctor...
    performance.setName(sandiaModulePerformanceName);
    performance.setActiveArea(this_data["snl_area"].asDouble());

    performance.setNumberofCellsinSeries(this_data["snl_series_cells"].asInt());
    performance.setNumberofCellsinParallel(this_data["snl_parallel_cells"].asInt());

    performance.setShortCircuitCurrent(this_data["snl_isco"].asDouble());
    performance.setOpenCircuitVoltage(this_data["snl_voco"].asDouble());
    performance.setCurrentatMaximumPowerPoint(this_data["snl_impo"].asDouble());
    performance.setVoltageatMaximumPowerPoint(this_data["snl_vmpo"].asDouble());
    performance.setSandiaDatabaseParameteraIsc(this_data["snl_aisc"].asDouble());
    performance.setSandiaDatabaseParameteraImp(this_data["snl_aimp"].asDouble());
    performance.setSandiaDatabaseParameterc0(this_data["snl_c0"].asDouble());
    performance.setSandiaDatabaseParameterc1(this_data["snl_c1"].asDouble());
    performance.setSandiaDatabaseParameterBVoc0(this_data["snl_bvoco"].asDouble());
    performance.setSandiaDatabaseParametermBVoc(this_data["snl_mbvoc"].asDouble());
    performance.setSandiaDatabaseParameterBVmp0(this_data["snl_bvmpo"].asDouble());
    performance.setSandiaDatabaseParametermBVmp(this_data["snl_mbvmp"].asDouble());
    performance.setDiodeFactor(this_data["snl_n"].asDouble());
    performance.setSandiaDatabaseParameterc2(this_data["snl_c2"].asDouble());
    performance.setSandiaDatabaseParameterc3(this_data["snl_c3"].asDouble());
    performance.setSandiaDatabaseParametera0(this_data["snl_a0"].asDouble());
    performance.setSandiaDatabaseParametera1(this_data["snl_a1"].asDouble());
    performance.setSandiaDatabaseParametera2(this_data["snl_a2"].asDouble());
    performance.setSandiaDatabaseParametera3(this_data["snl_a3"].asDouble());
    performance.setSandiaDatabaseParametera4(this_data["snl_a4"].asDouble());
    performance.setSandiaDatabaseParameterb0(this_data["snl_b0"].asDouble());
    performance.setSandiaDatabaseParameterb1(this_data["snl_b1"].asDouble());
    performance.setSandiaDatabaseParameterb2(this_data["snl_b2"].asDouble());
    performance.setSandiaDatabaseParameterb3(this_data["snl_b3"].asDouble());
    performance.setSandiaDatabaseParameterb4(this_data["snl_b4"].asDouble());
    performance.setSandiaDatabaseParameterb5(this_data["snl_b5"].asDouble());
    performance.setSandiaDatabaseParameterDeltaTc(this_data["snl_dtc"].asDouble());
    performance.setSandiaDatabaseParameterfd(this_data["snl_fd"].asDouble());
    performance.setSandiaDatabaseParametera(this_data["snl_a"].asDouble());
    performance.setSandiaDatabaseParameterb(this_data["snl_b"].asDouble());
    performance.setSandiaDatabaseParameterc4(this_data["snl_c4"].asDouble());
    performance.setSandiaDatabaseParameterc5(this_data["snl_c5"].asDouble());
    performance.setSandiaDatabaseParameterIx0(this_data["snl_ixo"].asDouble());
    performance.setSandiaDatabaseParameterIxx0(this_data["snl_ixxo"].asDouble());
    performance.setSandiaDatabaseParameterc6(this_data["snl_c6"].asDouble());
    performance.setSandiaDatabaseParameterc7(this_data["snl_c7"].asDouble());

    performance.setComment(this_data["snl_sandia_notes"].asString());

    return performance;
  }

  std::vector<std::string> PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames() {
    return detail::PhotovoltaicPerformanceSandia_Impl::sandiaModulePerformanceNames();
  }

  IddObjectType PhotovoltaicPerformanceSandia::iddObjectType() {
    return {IddObjectType::OS_PhotovoltaicPerformance_Sandia};
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
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setShortCircuitCurrent(shortCircuitCurrent);
  }

  bool PhotovoltaicPerformanceSandia::setOpenCircuitVoltage(double openCircuitVoltage) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setOpenCircuitVoltage(openCircuitVoltage);
  }

  bool PhotovoltaicPerformanceSandia::setCurrentatMaximumPowerPoint(double currentatMaximumPowerPoint) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setCurrentatMaximumPowerPoint(currentatMaximumPowerPoint);
  }

  bool PhotovoltaicPerformanceSandia::setVoltageatMaximumPowerPoint(double voltageatMaximumPowerPoint) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setVoltageatMaximumPowerPoint(voltageatMaximumPowerPoint);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameteraIsc(double sandiaDatabaseParameteraIsc) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameteraIsc(sandiaDatabaseParameteraIsc);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameteraImp(double sandiaDatabaseParameteraImp) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameteraImp(sandiaDatabaseParameteraImp);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc0(double sandiaDatabaseParameterc0) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc0(sandiaDatabaseParameterc0);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc1(double sandiaDatabaseParameterc1) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc1(sandiaDatabaseParameterc1);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterBVoc0(double sandiaDatabaseParameterBVoc0) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterBVoc0(sandiaDatabaseParameterBVoc0);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametermBVoc(double sandiaDatabaseParametermBVoc) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametermBVoc(sandiaDatabaseParametermBVoc);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterBVmp0(double sandiaDatabaseParameterBVmp0) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterBVmp0(sandiaDatabaseParameterBVmp0);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametermBVmp(double sandiaDatabaseParametermBVmp) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametermBVmp(sandiaDatabaseParametermBVmp);
  }

  bool PhotovoltaicPerformanceSandia::setDiodeFactor(double diodeFactor) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setDiodeFactor(diodeFactor);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc2(double sandiaDatabaseParameterc2) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc2(sandiaDatabaseParameterc2);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc3(double sandiaDatabaseParameterc3) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc3(sandiaDatabaseParameterc3);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera0(double sandiaDatabaseParametera0) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera0(sandiaDatabaseParametera0);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera1(double sandiaDatabaseParametera1) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera1(sandiaDatabaseParametera1);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera2(double sandiaDatabaseParametera2) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera2(sandiaDatabaseParametera2);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera3(double sandiaDatabaseParametera3) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera3(sandiaDatabaseParametera3);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera4(double sandiaDatabaseParametera4) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera4(sandiaDatabaseParametera4);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb0(double sandiaDatabaseParameterb0) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb0(sandiaDatabaseParameterb0);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb1(double sandiaDatabaseParameterb1) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb1(sandiaDatabaseParameterb1);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb2(double sandiaDatabaseParameterb2) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb2(sandiaDatabaseParameterb2);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb3(double sandiaDatabaseParameterb3) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb3(sandiaDatabaseParameterb3);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb4(double sandiaDatabaseParameterb4) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb4(sandiaDatabaseParameterb4);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb5(double sandiaDatabaseParameterb5) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb5(sandiaDatabaseParameterb5);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterDeltaTc(double sandiaDatabaseParameterDeltaTc) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterDeltaTc(sandiaDatabaseParameterDeltaTc);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterfd(double sandiaDatabaseParameterfd) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterfd(sandiaDatabaseParameterfd);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParametera(double sandiaDatabaseParametera) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParametera(sandiaDatabaseParametera);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterb(double sandiaDatabaseParameterb) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterb(sandiaDatabaseParameterb);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc4(double sandiaDatabaseParameterc4) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc4(sandiaDatabaseParameterc4);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc5(double sandiaDatabaseParameterc5) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc5(sandiaDatabaseParameterc5);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterIx0(double sandiaDatabaseParameterIx0) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterIx0(sandiaDatabaseParameterIx0);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterIxx0(double sandiaDatabaseParameterIxx0) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterIxx0(sandiaDatabaseParameterIxx0);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc6(double sandiaDatabaseParameterc6) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc6(sandiaDatabaseParameterc6);
  }

  bool PhotovoltaicPerformanceSandia::setSandiaDatabaseParameterc7(double sandiaDatabaseParameterc7) {
    return getImpl<detail::PhotovoltaicPerformanceSandia_Impl>()->setSandiaDatabaseParameterc7(sandiaDatabaseParameterc7);
  }

  /// @cond
  PhotovoltaicPerformanceSandia::PhotovoltaicPerformanceSandia(std::shared_ptr<detail::PhotovoltaicPerformanceSandia_Impl> impl)
    : PhotovoltaicPerformance(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
