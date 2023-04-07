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

#include "TableIndependentVariable.hpp"
#include "TableIndependentVariable_Impl.hpp"

#include "TableLookup.hpp"
#include "TableLookup_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Table_IndependentVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == TableIndependentVariable::iddObjectType());
    }

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == TableIndependentVariable::iddObjectType());
    }

    TableIndependentVariable_Impl::TableIndependentVariable_Impl(const TableIndependentVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& TableIndependentVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType TableIndependentVariable_Impl::iddObjectType() const {
      return TableIndependentVariable::iddObjectType();
    }

    std::string TableIndependentVariable_Impl::interpolationMethod() const {
      boost::optional<std::string> value = getString(OS_Table_IndependentVariableFields::InterpolationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string TableIndependentVariable_Impl::extrapolationMethod() const {
      boost::optional<std::string> value = getString(OS_Table_IndependentVariableFields::ExtrapolationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> TableIndependentVariable_Impl::minimumValue() const {
      return getDouble(OS_Table_IndependentVariableFields::MinimumValue, true);
    }

    boost::optional<double> TableIndependentVariable_Impl::maximumValue() const {
      return getDouble(OS_Table_IndependentVariableFields::MaximumValue, true);
    }

    boost::optional<double> TableIndependentVariable_Impl::normalizationReferenceValue() const {
      return getDouble(OS_Table_IndependentVariableFields::NormalizationReferenceValue, true);
    }

    std::string TableIndependentVariable_Impl::unitType() const {
      boost::optional<std::string> value = getString(OS_Table_IndependentVariableFields::UnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::vector<TableLookup> TableIndependentVariable_Impl::tableLookups() const {
      std::vector<TableLookup> result;
      for (const TableLookup& tableLookup : this->model().getConcreteModelObjects<TableLookup>()) {
        for (const TableIndependentVariable& independentVariable : tableLookup.independentVariables()) {
          if (independentVariable.handle() == this->handle()) {
            result.push_back(tableLookup);
          }
        }
      }
      return result;
    }

    bool TableIndependentVariable_Impl::setInterpolationMethod(const std::string& interpolationMethod) {
      bool result = setString(OS_Table_IndependentVariableFields::InterpolationMethod, interpolationMethod);
      return result;
    }

    bool TableIndependentVariable_Impl::setExtrapolationMethod(const std::string& extrapolationMethod) {
      bool result = setString(OS_Table_IndependentVariableFields::ExtrapolationMethod, extrapolationMethod);
      return result;
    }

    bool TableIndependentVariable_Impl::setMinimumValue(double minimumValue) {
      bool result = setDouble(OS_Table_IndependentVariableFields::MinimumValue, minimumValue);
      return result;
    }

    void TableIndependentVariable_Impl::resetMinimumValue() {
      bool result = setString(OS_Table_IndependentVariableFields::MinimumValue, "");
      OS_ASSERT(result);
    }

    bool TableIndependentVariable_Impl::setMaximumValue(double maximumValue) {
      bool result = setDouble(OS_Table_IndependentVariableFields::MaximumValue, maximumValue);
      return result;
    }

    void TableIndependentVariable_Impl::resetMaximumValue() {
      bool result = setString(OS_Table_IndependentVariableFields::MaximumValue, "");
      OS_ASSERT(result);
    }

    bool TableIndependentVariable_Impl::setNormalizationReferenceValue(double normalizationReferenceValue) {
      bool result = setDouble(OS_Table_IndependentVariableFields::NormalizationReferenceValue, normalizationReferenceValue);
      return result;
    }

    void TableIndependentVariable_Impl::resetNormalizationReferenceValue() {
      bool result = setString(OS_Table_IndependentVariableFields::NormalizationReferenceValue, "");
      OS_ASSERT(result);
    }

    bool TableIndependentVariable_Impl::setUnitType(const std::string& unitType) {
      bool result = setString(OS_Table_IndependentVariableFields::UnitType, unitType);
      return result;
    }

    bool TableIndependentVariable_Impl::addValue(double value) {
      IdfExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup();
      bool result = eg.setDouble(OS_Table_IndependentVariableExtensibleFields::Value, value);
      if (!result) {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }

      return result;
    }

    bool TableIndependentVariable_Impl::removeValue(unsigned groupIndex) {
      bool result = false;

      unsigned num = numberofValues();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      }
      return result;
    }

    void TableIndependentVariable_Impl::removeAllValues() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    std::vector<double> TableIndependentVariable_Impl::values() const {
      std::vector<double> result;
      for (const auto& eg : extensibleGroups()) {
        auto _d = eg.getDouble(OS_Table_IndependentVariableExtensibleFields::Value);
        OS_ASSERT(_d);
        result.push_back(_d.get());
      }
      return result;
    }

    unsigned TableIndependentVariable_Impl::numberofValues() const {
      return numExtensibleGroups();
    }

    bool TableIndependentVariable_Impl::setValues(const std::vector<double>& values) {
      bool result = true;

      this->clearExtensibleGroups();
      for (const auto& val : values) {
        result &= addValue(val);
      }
      return result;
    }

  }  // namespace detail

  TableIndependentVariable::TableIndependentVariable(const Model& model) : ResourceObject(TableIndependentVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::TableIndependentVariable_Impl>());

    bool ok = true;
    ok = setInterpolationMethod("Linear");
    OS_ASSERT(ok);
    ok = setExtrapolationMethod("Constant");
    OS_ASSERT(ok);
    ok = setUnitType("Dimensionless");
    OS_ASSERT(ok);
  }

  IddObjectType TableIndependentVariable::iddObjectType() {
    return {IddObjectType::OS_Table_IndependentVariable};
  }

  std::vector<std::string> TableIndependentVariable::interpolationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_IndependentVariableFields::InterpolationMethod);
  }

  std::vector<std::string> TableIndependentVariable::validInterpolationMethodValues() {
    return interpolationMethodValues();
  }

  std::vector<std::string> TableIndependentVariable::extrapolationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_IndependentVariableFields::ExtrapolationMethod);
  }

  std::vector<std::string> TableIndependentVariable::validExtrapolationMethodValues() {
    return extrapolationMethodValues();
  }

  std::vector<std::string> TableIndependentVariable::unitTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_IndependentVariableFields::UnitType);
  }

  std::vector<std::string> TableIndependentVariable::validUnitTypeValues() {
    return unitTypeValues();
  }

  std::string TableIndependentVariable::interpolationMethod() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->interpolationMethod();
  }

  std::string TableIndependentVariable::extrapolationMethod() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->extrapolationMethod();
  }

  boost::optional<double> TableIndependentVariable::minimumValue() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->minimumValue();
  }

  boost::optional<double> TableIndependentVariable::maximumValue() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->maximumValue();
  }

  boost::optional<double> TableIndependentVariable::normalizationReferenceValue() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->normalizationReferenceValue();
  }

  std::string TableIndependentVariable::unitType() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->unitType();
  }

  std::vector<TableLookup> TableIndependentVariable::tableLookups() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->tableLookups();
  }

  bool TableIndependentVariable::setInterpolationMethod(const std::string& interpolationMethod) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setInterpolationMethod(interpolationMethod);
  }

  bool TableIndependentVariable::setExtrapolationMethod(const std::string& extrapolationMethod) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setExtrapolationMethod(extrapolationMethod);
  }

  bool TableIndependentVariable::setMinimumValue(double minimumValue) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setMinimumValue(minimumValue);
  }

  void TableIndependentVariable::resetMinimumValue() {
    getImpl<detail::TableIndependentVariable_Impl>()->resetMinimumValue();
  }

  bool TableIndependentVariable::setMaximumValue(double maximumValue) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setMaximumValue(maximumValue);
  }

  void TableIndependentVariable::resetMaximumValue() {
    getImpl<detail::TableIndependentVariable_Impl>()->resetMaximumValue();
  }

  bool TableIndependentVariable::setNormalizationReferenceValue(double normalizationReferenceValue) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setNormalizationReferenceValue(normalizationReferenceValue);
  }

  void TableIndependentVariable::resetNormalizationReferenceValue() {
    getImpl<detail::TableIndependentVariable_Impl>()->resetNormalizationReferenceValue();
  }

  bool TableIndependentVariable::setUnitType(const std::string& unitType) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setUnitType(unitType);
  }

  bool TableIndependentVariable::addValue(double value) {
    return getImpl<detail::TableIndependentVariable_Impl>()->addValue(value);
  }

  bool TableIndependentVariable::removeValue(unsigned groupIndex) {
    return getImpl<detail::TableIndependentVariable_Impl>()->removeValue(groupIndex);
  }

  void TableIndependentVariable::removeAllValues() {
    getImpl<detail::TableIndependentVariable_Impl>()->removeAllValues();
  }

  std::vector<double> TableIndependentVariable::values() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->values();
  }

  unsigned TableIndependentVariable::numberofValues() const {
    return getImpl<detail::TableIndependentVariable_Impl>()->numberofValues();
  }

  bool TableIndependentVariable::setValues(const std::vector<double>& values) {
    return getImpl<detail::TableIndependentVariable_Impl>()->setValues(values);
  }

  /// @cond
  TableIndependentVariable::TableIndependentVariable(std::shared_ptr<detail::TableIndependentVariable_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
