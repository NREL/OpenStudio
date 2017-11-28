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

#include "TableMultiVariableLookup.hpp"
#include "TableMultiVariableLookup_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Table_MultiVariableLookup_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Assert.hpp"

#include <algorithm>

namespace openstudio {
namespace model {

namespace detail {

  bool TableMultiVariableLookup_Impl::xValuesEqual(const std::vector<double> & a, const std::vector<double> & b)
  {
    bool result = true;

    if( a.size() != b.size() )
    {
      result = false;
    }
    else
    {
      for(auto it1 = a.begin(),it2 = b.begin();
          it1 != a.end() && it2 != b.end();
          ++it1,++it2)
      {
        if( ! equal(*it1,*it2) )
        {
          result = false;
        }
      }
    }

    return result;
  }

  TableMultiVariableLookup_Impl::TableMultiVariableLookup_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : Curve_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == TableMultiVariableLookup::iddObjectType());
  }

  TableMultiVariableLookup_Impl::TableMultiVariableLookup_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == TableMultiVariableLookup::iddObjectType());
  }

  TableMultiVariableLookup_Impl::TableMultiVariableLookup_Impl(const TableMultiVariableLookup_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : Curve_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& TableMultiVariableLookup_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType TableMultiVariableLookup_Impl::iddObjectType() const {
    return TableMultiVariableLookup::iddObjectType();
  }

  std::string TableMultiVariableLookup_Impl::interpolationMethod() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InterpolationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isInterpolationMethodDefaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::InterpolationMethod);
  }

  int TableMultiVariableLookup_Impl::numberofInterpolationPoints() const {
    boost::optional<int> value = getInt(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isNumberofInterpolationPointsDefaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints);
  }

  boost::optional<std::string> TableMultiVariableLookup_Impl::curveType() const {
    return getString(OS_Table_MultiVariableLookupFields::CurveType,true);
  }

  std::string TableMultiVariableLookup_Impl::tableDataFormat() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::TableDataFormat,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isTableDataFormatDefaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::TableDataFormat);
  }

  boost::optional<std::string> TableMultiVariableLookup_Impl::externalFileName() const {
    return getString(OS_Table_MultiVariableLookupFields::ExternalFileName,true);
  }

  std::string TableMultiVariableLookup_Impl::x1SortOrder() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::X1SortOrder,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isX1SortOrderDefaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::X1SortOrder);
  }

  std::string TableMultiVariableLookup_Impl::x2SortOrder() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::X2SortOrder,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isX2SortOrderDefaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::X2SortOrder);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::normalizationReference() const {
    return getDouble(OS_Table_MultiVariableLookupFields::NormalizationReference,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX1() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX1,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX1() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX1,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX2() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX2,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX2() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX2,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX3() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX3,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX3() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX3,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX4() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX4,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX4() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX4,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::minimumValueofX5() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX5,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::maximumValueofX5() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX5,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::minimumTableOutput() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MinimumTableOutput,true);
  }

  boost::optional<double> TableMultiVariableLookup_Impl::maximumTableOutput() const {
    return getDouble(OS_Table_MultiVariableLookupFields::MaximumTableOutput,true);
  }

  std::string TableMultiVariableLookup_Impl::inputUnitTypeforX1() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isInputUnitTypeforX1Defaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1);
  }

  std::string TableMultiVariableLookup_Impl::inputUnitTypeforX2() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isInputUnitTypeforX2Defaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2);
  }

  std::string TableMultiVariableLookup_Impl::inputUnitTypeforX3() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isInputUnitTypeforX3Defaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3);
  }

  std::string TableMultiVariableLookup_Impl::inputUnitTypeforX4() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isInputUnitTypeforX4Defaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4);
  }

  std::string TableMultiVariableLookup_Impl::inputUnitTypeforX5() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isInputUnitTypeforX5Defaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5);
  }

  std::string TableMultiVariableLookup_Impl::outputUnitType() const {
    boost::optional<std::string> value = getString(OS_Table_MultiVariableLookupFields::OutputUnitType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::isOutputUnitTypeDefaulted() const {
    return isEmpty(OS_Table_MultiVariableLookupFields::OutputUnitType);
  }

  int TableMultiVariableLookup_Impl::numberofIndependentVariables() const {
    boost::optional<int> value = getInt(OS_Table_MultiVariableLookupFields::NumberofIndependentVariables,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool TableMultiVariableLookup_Impl::setInterpolationMethod(std::string interpolationMethod) {
    bool result = setString(OS_Table_MultiVariableLookupFields::InterpolationMethod, interpolationMethod);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetInterpolationMethod() {
    bool result = setString(OS_Table_MultiVariableLookupFields::InterpolationMethod, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setNumberofInterpolationPoints(int numberofInterpolationPoints) {
    bool result = setInt(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints, numberofInterpolationPoints);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetNumberofInterpolationPoints() {
    bool result = setString(OS_Table_MultiVariableLookupFields::NumberofInterpolationPoints, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setCurveType(boost::optional<std::string> curveType) {
    bool result(false);
    if (curveType) {
      result = setString(OS_Table_MultiVariableLookupFields::CurveType, curveType.get());
    }
    else {
      resetCurveType();
      result = true;
    }
    return result;
  }

  void TableMultiVariableLookup_Impl::resetCurveType() {
    bool result = setString(OS_Table_MultiVariableLookupFields::CurveType, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setTableDataFormat(std::string tableDataFormat) {
    bool result = setString(OS_Table_MultiVariableLookupFields::TableDataFormat, tableDataFormat);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetTableDataFormat() {
    bool result = setString(OS_Table_MultiVariableLookupFields::TableDataFormat, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setExternalFileName(boost::optional<std::string> externalFileName) {
    bool result(false);
    if (externalFileName) {
      result = setString(OS_Table_MultiVariableLookupFields::ExternalFileName, externalFileName.get());
    }
    else {
      resetExternalFileName();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetExternalFileName() {
    bool result = setString(OS_Table_MultiVariableLookupFields::ExternalFileName, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setX1SortOrder(std::string x1SortOrder) {
    bool result = setString(OS_Table_MultiVariableLookupFields::X1SortOrder, x1SortOrder);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetX1SortOrder() {
    bool result = setString(OS_Table_MultiVariableLookupFields::X1SortOrder, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setX2SortOrder(std::string x2SortOrder) {
    bool result = setString(OS_Table_MultiVariableLookupFields::X2SortOrder, x2SortOrder);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetX2SortOrder() {
    bool result = setString(OS_Table_MultiVariableLookupFields::X2SortOrder, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setNormalizationReference(boost::optional<double> normalizationReference) {
    bool result(false);
    if (normalizationReference) {
      result = setDouble(OS_Table_MultiVariableLookupFields::NormalizationReference, normalizationReference.get());
    }
    else {
      resetNormalizationReference();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetNormalizationReference() {
    bool result = setString(OS_Table_MultiVariableLookupFields::NormalizationReference, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMinimumValueofX1(boost::optional<double> minimumValueofX1) {
    bool result(false);
    if (minimumValueofX1) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX1, minimumValueofX1.get());
    }
    else {
      resetMinimumValueofX1();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMinimumValueofX1() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX1, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMaximumValueofX1(boost::optional<double> maximumValueofX1) {
    bool result(false);
    if (maximumValueofX1) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX1, maximumValueofX1.get());
    }
    else {
      resetMaximumValueofX1();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMaximumValueofX1() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX1, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMinimumValueofX2(boost::optional<double> minimumValueofX2) {
    bool result(false);
    if (minimumValueofX2) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX2, minimumValueofX2.get());
    }
    else {
      resetMinimumValueofX2();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMinimumValueofX2() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX2, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMaximumValueofX2(boost::optional<double> maximumValueofX2) {
    bool result(false);
    if (maximumValueofX2) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX2, maximumValueofX2.get());
    }
    else {
      resetMaximumValueofX2();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMaximumValueofX2() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX2, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMinimumValueofX3(boost::optional<double> minimumValueofX3) {
    bool result(false);
    if (minimumValueofX3) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX3, minimumValueofX3.get());
    }
    else {
      resetMinimumValueofX3();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMinimumValueofX3() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX3, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMaximumValueofX3(boost::optional<double> maximumValueofX3) {
    bool result(false);
    if (maximumValueofX3) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX3, maximumValueofX3.get());
    }
    else {
      resetMaximumValueofX3();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMaximumValueofX3() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX3, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMinimumValueofX4(boost::optional<double> minimumValueofX4) {
    bool result(false);
    if (minimumValueofX4) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX4, minimumValueofX4.get());
    }
    else {
      resetMinimumValueofX4();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMinimumValueofX4() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX4, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMaximumValueofX4(boost::optional<double> maximumValueofX4) {
    bool result(false);
    if (maximumValueofX4) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX4, maximumValueofX4.get());
    }
    else {
      resetMaximumValueofX4();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMaximumValueofX4() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX4, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMinimumValueofX5(boost::optional<double> minimumValueofX5) {
    bool result(false);
    if (minimumValueofX5) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MinimumValueofX5, minimumValueofX5.get());
    }
    else {
      resetMinimumValueofX5();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMinimumValueofX5() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MinimumValueofX5, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMaximumValueofX5(boost::optional<double> maximumValueofX5) {
    bool result(false);
    if (maximumValueofX5) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MaximumValueofX5, maximumValueofX5.get());
    }
    else {
      resetMaximumValueofX5();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMaximumValueofX5() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MaximumValueofX5, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMinimumTableOutput(boost::optional<double> minimumTableOutput) {
    bool result(false);
    if (minimumTableOutput) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MinimumTableOutput, minimumTableOutput.get());
    }
    else {
      resetMinimumTableOutput();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMinimumTableOutput() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MinimumTableOutput, "");
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::setMaximumTableOutput(boost::optional<double> maximumTableOutput) {
    bool result(false);
    if (maximumTableOutput) {
      result = setDouble(OS_Table_MultiVariableLookupFields::MaximumTableOutput, maximumTableOutput.get());
    }
    else {
      resetMaximumTableOutput();
      result = true;
    }
    OS_ASSERT(result);
  }

  void TableMultiVariableLookup_Impl::resetMaximumTableOutput() {
    bool result = setString(OS_Table_MultiVariableLookupFields::MaximumTableOutput, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setInputUnitTypeforX1(std::string inputUnitTypeforX1) {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1, inputUnitTypeforX1);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetInputUnitTypeforX1() {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX1, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setInputUnitTypeforX2(std::string inputUnitTypeforX2) {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2, inputUnitTypeforX2);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetInputUnitTypeforX2() {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX2, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setInputUnitTypeforX3(std::string inputUnitTypeforX3) {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3, inputUnitTypeforX3);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetInputUnitTypeforX3() {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX3, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setInputUnitTypeforX4(std::string inputUnitTypeforX4) {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4, inputUnitTypeforX4);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetInputUnitTypeforX4() {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX4, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setInputUnitTypeforX5(std::string inputUnitTypeforX5) {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5, inputUnitTypeforX5);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetInputUnitTypeforX5() {
    bool result = setString(OS_Table_MultiVariableLookupFields::InputUnitTypeforX5, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setOutputUnitType(std::string outputUnitType) {
    bool result = setString(OS_Table_MultiVariableLookupFields::OutputUnitType, outputUnitType);
    return result;
  }

  void TableMultiVariableLookup_Impl::resetOutputUnitType() {
    bool result = setString(OS_Table_MultiVariableLookupFields::OutputUnitType, "");
    OS_ASSERT(result);
  }

  bool TableMultiVariableLookup_Impl::setNumberofIndependentVariables(int numberofIndependentVariables) {
    bool result = setInt(OS_Table_MultiVariableLookupFields::NumberofIndependentVariables, numberofIndependentVariables);
    return result;
  }

  int TableMultiVariableLookup_Impl::numVariables() const
  {
    return numberofIndependentVariables();
  }

  double TableMultiVariableLookup_Impl::evaluate(const std::vector<double>& x) const
  {
    return 1.0;
  }

  bool TableMultiVariableLookup_Impl::addPoint(const std::vector<double> & t_xValues, double t_yValue)
  {
    if( t_xValues.size() != numberofIndependentVariables() )
    {
      return false;
    }
    else if( yValue(t_xValues) )
    {
      return false;
    }
    else
    {
      for(auto it = t_xValues.begin();
          it != t_xValues.end();
          ++it)
      {
        IdfExtensibleGroup eg = getObject<model::TableMultiVariableLookup>().pushExtensibleGroup();
        eg.setDouble(0,*it);
      }

      IdfExtensibleGroup eg = getObject<model::TableMultiVariableLookup>().pushExtensibleGroup();
      eg.setDouble(0,t_yValue);
    }

    return true;
  }

  std::vector<std::pair<std::vector<double>,double> > TableMultiVariableLookup_Impl::points() const
  {
    std::vector<std::pair<std::vector<double>,double> > result;

    int t_numberofIndependentVariables = numberofIndependentVariables();

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();
    int numberOfPoints = groups.size() / (t_numberofIndependentVariables + 1);

    for(int iPoint = 0; iPoint != numberOfPoints; ++iPoint)
    {
      std::vector<double> xValues(t_numberofIndependentVariables);
      for(int i = 0; i != t_numberofIndependentVariables; ++i)
      {
        IdfExtensibleGroup g = groups[iPoint * (t_numberofIndependentVariables + 1) + i];
        boost::optional<double> d = g.getDouble(OS_Table_MultiVariableLookupExtensibleFields::Data);
        OS_ASSERT(d);
        xValues[i] = d.get();
      }

      IdfExtensibleGroup g = groups[iPoint * (t_numberofIndependentVariables + 1) + t_numberofIndependentVariables];
      boost::optional<double> d = g.getDouble(OS_Table_MultiVariableLookupExtensibleFields::Data);
      OS_ASSERT(d);
      double yValue = d.get();

      std::pair<std::vector<double>,double> p = std::pair<std::vector<double>,double>(xValues,yValue);
      result.push_back(p);
    }

    return result;
  }

  std::vector<double> TableMultiVariableLookup_Impl::xValues(int xIndex) const
  {
    std::vector<double> result;

    if( xIndex >= numberofIndependentVariables() )
    {
      return result;
    }

    std::vector<std::pair<std::vector<double>,double> > t_points = points();
    for(std::vector<std::pair<std::vector<double>,double> >::const_iterator it = t_points.begin();
        it != t_points.end();
        ++it)
    {
      std::vector<double> xValues = it->first;
      result.push_back(xValues[xIndex]);
    }

    std::sort(result.begin(),result.end());
    result.erase(std::unique(result.begin(),result.end()),result.end());

    return result;
  }

  boost::optional<double> TableMultiVariableLookup_Impl::yValue(const std::vector<double> & xValues) const
  {
    boost::optional<double> result;

    std::vector<std::pair<std::vector<double>,double> > t_points = points();
    for(std::vector<std::pair<std::vector<double>,double> >::const_iterator it = t_points.begin();
        it != t_points.end();
        ++it)
    {
      std::vector<double> t_c = it->first;
      if( xValuesEqual( t_c, xValues ) )
      {
        result = it->second;
        break;
      }
    }

    return result;
  }

  bool TableMultiVariableLookup_Impl::addPoint(double x1, double yValue)
  {
    std::vector<double> xValues(1);
    xValues[0] = x1;
    return addPoint(xValues,yValue);
  }

  bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double yValue)
  {
    std::vector<double> xValues(2);
    xValues[0] = x1;
    xValues[1] = x2;
    return addPoint(xValues,yValue);
  }

  bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double x3, double yValue)
  {
    std::vector<double> xValues(3);
    xValues[0] = x1;
    xValues[1] = x2;
    xValues[2] = x3;
    return addPoint(xValues,yValue);
  }

  bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double x3, double x4, double yValue)
  {
    std::vector<double> xValues(4);
    xValues[0] = x1;
    xValues[1] = x2;
    xValues[2] = x3;
    xValues[3] = x4;
    return addPoint(xValues,yValue);
  }

  bool TableMultiVariableLookup_Impl::addPoint(double x1, double x2, double x3, double x4, double x5, double yValue)
  {
    std::vector<double> xValues(5);
    xValues[0] = x1;
    xValues[1] = x2;
    xValues[2] = x3;
    xValues[3] = x4;
    xValues[4] = x5;
    return addPoint(xValues,yValue);
  }

} // detail

TableMultiVariableLookup::TableMultiVariableLookup(const Model& model, const int& numberofIndependentVariables)
  : Curve(TableMultiVariableLookup::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::TableMultiVariableLookup_Impl>());

  getImpl<detail::TableMultiVariableLookup_Impl>()->setNumberofIndependentVariables(numberofIndependentVariables);

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setNumberofIndependentVariables();
  OS_ASSERT(ok);
}

IddObjectType TableMultiVariableLookup::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Table_MultiVariableLookup);
}

std::vector<std::string> TableMultiVariableLookup::interpolationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::InterpolationMethod);
}

std::vector<std::string> TableMultiVariableLookup::curveTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::CurveType);
}

std::vector<std::string> TableMultiVariableLookup::tableDataFormatValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::TableDataFormat);
}

std::vector<std::string> TableMultiVariableLookup::x1SortOrderValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::X1SortOrder);
}

std::vector<std::string> TableMultiVariableLookup::x2SortOrderValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::X2SortOrder);
}

std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX1Values() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::InputUnitTypeforX1);
}

std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX2Values() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::InputUnitTypeforX2);
}

std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX3Values() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::InputUnitTypeforX3);
}

std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX4Values() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::InputUnitTypeforX4);
}

std::vector<std::string> TableMultiVariableLookup::inputUnitTypeforX5Values() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::InputUnitTypeforX5);
}

std::vector<std::string> TableMultiVariableLookup::outputUnitTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Table_MultiVariableLookupFields::OutputUnitType);
}

std::string TableMultiVariableLookup::interpolationMethod() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->interpolationMethod();
}

bool TableMultiVariableLookup::isInterpolationMethodDefaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isInterpolationMethodDefaulted();
}

int TableMultiVariableLookup::numberofInterpolationPoints() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->numberofInterpolationPoints();
}

bool TableMultiVariableLookup::isNumberofInterpolationPointsDefaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isNumberofInterpolationPointsDefaulted();
}

boost::optional<std::string> TableMultiVariableLookup::curveType() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->curveType();
}

std::string TableMultiVariableLookup::tableDataFormat() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->tableDataFormat();
}

bool TableMultiVariableLookup::isTableDataFormatDefaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isTableDataFormatDefaulted();
}

boost::optional<double> TableMultiVariableLookup::normalizationReference() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->normalizationReference();
}

boost::optional<double> TableMultiVariableLookup::minimumValueofX1() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX1();
}

boost::optional<double> TableMultiVariableLookup::maximumValueofX1() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX1();
}

boost::optional<double> TableMultiVariableLookup::minimumValueofX2() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX2();
}

boost::optional<double> TableMultiVariableLookup::maximumValueofX2() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX2();
}

boost::optional<double> TableMultiVariableLookup::minimumValueofX3() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX3();
}

boost::optional<double> TableMultiVariableLookup::maximumValueofX3() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX3();
}

boost::optional<double> TableMultiVariableLookup::minimumValueofX4() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX4();
}

boost::optional<double> TableMultiVariableLookup::maximumValueofX4() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX4();
}

boost::optional<double> TableMultiVariableLookup::minimumValueofX5() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumValueofX5();
}

boost::optional<double> TableMultiVariableLookup::maximumValueofX5() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumValueofX5();
}

boost::optional<double> TableMultiVariableLookup::minimumTableOutput() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->minimumTableOutput();
}

boost::optional<double> TableMultiVariableLookup::maximumTableOutput() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->maximumTableOutput();
}

std::string TableMultiVariableLookup::inputUnitTypeforX1() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX1();
}

bool TableMultiVariableLookup::isInputUnitTypeforX1Defaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX1Defaulted();
}

std::string TableMultiVariableLookup::inputUnitTypeforX2() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX2();
}

bool TableMultiVariableLookup::isInputUnitTypeforX2Defaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX2Defaulted();
}

std::string TableMultiVariableLookup::inputUnitTypeforX3() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX3();
}

bool TableMultiVariableLookup::isInputUnitTypeforX3Defaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX3Defaulted();
}

std::string TableMultiVariableLookup::inputUnitTypeforX4() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX4();
}

bool TableMultiVariableLookup::isInputUnitTypeforX4Defaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX4Defaulted();
}

std::string TableMultiVariableLookup::inputUnitTypeforX5() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->inputUnitTypeforX5();
}

bool TableMultiVariableLookup::isInputUnitTypeforX5Defaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isInputUnitTypeforX5Defaulted();
}

std::string TableMultiVariableLookup::outputUnitType() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->outputUnitType();
}

bool TableMultiVariableLookup::isOutputUnitTypeDefaulted() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->isOutputUnitTypeDefaulted();
}

int TableMultiVariableLookup::numberofIndependentVariables() const {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->numberofIndependentVariables();
}

bool TableMultiVariableLookup::setInterpolationMethod(std::string interpolationMethod) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setInterpolationMethod(interpolationMethod);
}

void TableMultiVariableLookup::resetInterpolationMethod() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetInterpolationMethod();
}

bool TableMultiVariableLookup::setNumberofInterpolationPoints(int numberofInterpolationPoints) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setNumberofInterpolationPoints(numberofInterpolationPoints);
}

void TableMultiVariableLookup::resetNumberofInterpolationPoints() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetNumberofInterpolationPoints();
}

bool TableMultiVariableLookup::setCurveType(std::string curveType) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setCurveType(curveType);
}

void TableMultiVariableLookup::resetCurveType() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetCurveType();
}

bool TableMultiVariableLookup::setTableDataFormat(std::string tableDataFormat) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setTableDataFormat(tableDataFormat);
}

void TableMultiVariableLookup::resetTableDataFormat() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetTableDataFormat();
}

void TableMultiVariableLookup::setNormalizationReference(double normalizationReference) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setNormalizationReference(normalizationReference);
}

void TableMultiVariableLookup::resetNormalizationReference() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetNormalizationReference();
}

void TableMultiVariableLookup::setMinimumValueofX1(double minimumValueofX1) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX1(minimumValueofX1);
}

void TableMultiVariableLookup::resetMinimumValueofX1() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX1();
}

void TableMultiVariableLookup::setMaximumValueofX1(double maximumValueofX1) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX1(maximumValueofX1);
}

void TableMultiVariableLookup::resetMaximumValueofX1() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX1();
}

void TableMultiVariableLookup::setMinimumValueofX2(double minimumValueofX2) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX2(minimumValueofX2);
}

void TableMultiVariableLookup::resetMinimumValueofX2() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX2();
}

void TableMultiVariableLookup::setMaximumValueofX2(double maximumValueofX2) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX2(maximumValueofX2);
}

void TableMultiVariableLookup::resetMaximumValueofX2() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX2();
}

void TableMultiVariableLookup::setMinimumValueofX3(double minimumValueofX3) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX3(minimumValueofX3);
}

void TableMultiVariableLookup::resetMinimumValueofX3() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX3();
}

void TableMultiVariableLookup::setMaximumValueofX3(double maximumValueofX3) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX3(maximumValueofX3);
}

void TableMultiVariableLookup::resetMaximumValueofX3() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX3();
}

void TableMultiVariableLookup::setMinimumValueofX4(double minimumValueofX4) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX4(minimumValueofX4);
}

void TableMultiVariableLookup::resetMinimumValueofX4() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX4();
}

void TableMultiVariableLookup::setMaximumValueofX4(double maximumValueofX4) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX4(maximumValueofX4);
}

void TableMultiVariableLookup::resetMaximumValueofX4() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX4();
}

void TableMultiVariableLookup::setMinimumValueofX5(double minimumValueofX5) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumValueofX5(minimumValueofX5);
}

void TableMultiVariableLookup::resetMinimumValueofX5() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumValueofX5();
}

void TableMultiVariableLookup::setMaximumValueofX5(double maximumValueofX5) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumValueofX5(maximumValueofX5);
}

void TableMultiVariableLookup::resetMaximumValueofX5() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumValueofX5();
}

void TableMultiVariableLookup::setMinimumTableOutput(double minimumTableOutput) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMinimumTableOutput(minimumTableOutput);
}

void TableMultiVariableLookup::resetMinimumTableOutput() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMinimumTableOutput();
}

void TableMultiVariableLookup::setMaximumTableOutput(double maximumTableOutput) {
  getImpl<detail::TableMultiVariableLookup_Impl>()->setMaximumTableOutput(maximumTableOutput);
}

void TableMultiVariableLookup::resetMaximumTableOutput() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetMaximumTableOutput();
}

bool TableMultiVariableLookup::setInputUnitTypeforX1(std::string inputUnitTypeforX1) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX1(inputUnitTypeforX1);
}

void TableMultiVariableLookup::resetInputUnitTypeforX1() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX1();
}

bool TableMultiVariableLookup::setInputUnitTypeforX2(std::string inputUnitTypeforX2) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX2(inputUnitTypeforX2);
}

void TableMultiVariableLookup::resetInputUnitTypeforX2() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX2();
}

bool TableMultiVariableLookup::setInputUnitTypeforX3(std::string inputUnitTypeforX3) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX3(inputUnitTypeforX3);
}

void TableMultiVariableLookup::resetInputUnitTypeforX3() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX3();
}

bool TableMultiVariableLookup::setInputUnitTypeforX4(std::string inputUnitTypeforX4) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX4(inputUnitTypeforX4);
}

void TableMultiVariableLookup::resetInputUnitTypeforX4() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX4();
}

bool TableMultiVariableLookup::setInputUnitTypeforX5(std::string inputUnitTypeforX5) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setInputUnitTypeforX5(inputUnitTypeforX5);
}

void TableMultiVariableLookup::resetInputUnitTypeforX5() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetInputUnitTypeforX5();
}

bool TableMultiVariableLookup::setOutputUnitType(std::string outputUnitType) {
  return getImpl<detail::TableMultiVariableLookup_Impl>()->setOutputUnitType(outputUnitType);
}

void TableMultiVariableLookup::resetOutputUnitType() {
  getImpl<detail::TableMultiVariableLookup_Impl>()->resetOutputUnitType();
}

int TableMultiVariableLookup::numVariables() const
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->numVariables();
}

double TableMultiVariableLookup::evaluate(const std::vector<double>& x) const
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->evaluate(x);
}

bool TableMultiVariableLookup::addPoint(const std::vector<double> & xValues, double yValue)
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(xValues,yValue);
}

bool TableMultiVariableLookup::addPoint(double x1, double yValue)
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1,yValue);
}

bool TableMultiVariableLookup::addPoint(double x1, double x2, double yValue)
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1,x2,yValue);
}

bool TableMultiVariableLookup::addPoint(double x1, double x2, double x3, double yValue)
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1,x2,x3,yValue);
}

bool TableMultiVariableLookup::addPoint(double x1, double x2, double x3, double x4, double yValue)
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1,x2,x3,x4,yValue);
}

bool TableMultiVariableLookup::addPoint(double x1, double x2, double x3, double x4, double x5, double yValue)
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->addPoint(x1,x2,x3,x4,x5,yValue);
}

std::vector<std::pair<std::vector<double>,double> > TableMultiVariableLookup::points() const
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->points();
}

boost::optional<double> TableMultiVariableLookup::yValue(const std::vector<double> & xValues) const
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->yValue(xValues);
}

std::vector<double> TableMultiVariableLookup::xValues(int i) const
{
  return getImpl<detail::TableMultiVariableLookup_Impl>()->xValues(i);
}


/// @cond
TableMultiVariableLookup::TableMultiVariableLookup(std::shared_ptr<detail::TableMultiVariableLookup_Impl> impl)
  : Curve(std::dynamic_pointer_cast<detail::Curve_Impl>(std::move(impl)))
{}
/// @endcond

} // model
} // openstudio

