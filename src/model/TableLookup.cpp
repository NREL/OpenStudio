/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "TableLookup.hpp"
#include "TableLookup_Impl.hpp"

#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "TableIndependentVariable.hpp"
#include "TableIndependentVariable_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Table_Lookup_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

#include <algorithm>
#include <iomanip>

namespace openstudio {
namespace model {

  namespace detail {

    TableLookup_Impl::TableLookup_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : Curve_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == TableLookup::iddObjectType());
    }

    TableLookup_Impl::TableLookup_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Curve_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == TableLookup::iddObjectType());
    }

    TableLookup_Impl::TableLookup_Impl(const TableLookup_Impl& other, Model_Impl* model, bool keepHandle) : Curve_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& TableLookup_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType TableLookup_Impl::iddObjectType() const {
      return TableLookup::iddObjectType();
    }

    int TableLookup_Impl::numVariables() const {
      return 1;
    }

    double TableLookup_Impl::evaluate(const std::vector<double>& independentVariables) const {
      LOG(Warn, "Curve evaluation isn't implemented for TableLookup");
      return -9999.0;
    }

    boost::optional<ModelObjectList> TableLookup_Impl::independentVariableList() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_Table_LookupFields::IndependentVariableListName);
    }

    std::string TableLookup_Impl::normalizationMethod() const {
      boost::optional<std::string> value = getString(OS_Table_LookupFields::NormalizationMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double TableLookup_Impl::normalizationDivisor() const {
      boost::optional<double> value = getDouble(OS_Table_LookupFields::NormalizationDivisor, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string TableLookup_Impl::outputUnitType() const {
      boost::optional<std::string> value = getString(OS_Table_LookupFields::OutputUnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableLookup_Impl::setNormalizationMethod(std::string normalizationMethod) {
      bool result = setString(OS_Table_LookupFields::NormalizationMethod, normalizationMethod);
      return result;
    }

    bool TableLookup_Impl::setNormalizationDivisor(double normalizationDivisior) {
      bool result = setDouble(OS_Table_LookupFields::NormalizationDivisor, normalizationDivisior);
      return result;
    }

    bool TableLookup_Impl::setOutputUnitType(std::string outputUnitType) {
      bool result = setString(OS_Table_LookupFields::OutputUnitType, outputUnitType);
      return result;
    }

    bool TableLookup_Impl::addIndependentVariable(const TableIndependentVariable& tableIndependentVariable) {
      bool result = false;
      auto modelObjectList = independentVariableList();
      if (modelObjectList) {
        result = modelObjectList->addModelObject(tableIndependentVariable);
      }
      return result;
    }

    void TableLookup_Impl::removeIndependentVariable(const TableIndependentVariable& tableIndependentVariable) {
      auto modelObjectList = independentVariableList();
      if (modelObjectList) {
        modelObjectList->removeModelObject(tableIndependentVariable);
      }
    }

    void TableLookup_Impl::removeAllIndependentVariables() {
      auto modelObjectList = independentVariableList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<TableIndependentVariable>();
          if (modelObject) {
            modelObjectList->removeModelObject(elem);
          }
        }
      }
    }

    std::vector<TableIndependentVariable> TableLookup_Impl::independentVariables() const {
      std::vector<TableIndependentVariable> result;
      auto const modelObjectList = independentVariableList();
      if (modelObjectList) {
        auto const modelObjects = modelObjectList->modelObjects();

        for (const auto& elem : modelObjects) {
          auto const modelObject = elem.optionalCast<TableIndependentVariable>();
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }
      return result;
    }

    bool TableLookup_Impl::setIndependentVariableList(const boost::optional<ModelObjectList>& modelObjectList) {
      bool result(false);
      if (modelObjectList) {
        result = setPointer(OS_Table_LookupFields::IndependentVariableListName, modelObjectList.get().handle());
      } else {
        resetIndependentVariableList();
        result = true;
      }
      return result;
    }

    void TableLookup_Impl::resetIndependentVariableList() {
      bool result = setString(OS_Table_LookupFields::IndependentVariableListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  TableLookup::TableLookup(const Model& model) : Curve(TableLookup::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::TableLookup_Impl>());

    bool ok = true;
    ok = setNormalizationMethod("None");
    OS_ASSERT(ok);
    ok = setNormalizationDivisor(1.0);
    OS_ASSERT(ok);
    ok = setOutputUnitType("Dimensionless");
    OS_ASSERT(ok);

    auto tableIndependentVariableList = ModelObjectList(model);
    tableIndependentVariableList.setName(this->name().get() + " Independent Variable List");
    ok = getImpl<detail::TableLookup_Impl>()->setIndependentVariableList(tableIndependentVariableList);
    OS_ASSERT(ok);
  }

  IddObjectType TableLookup::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Table_Lookup);
  }

  std::string TableLookup::normalizationMethod() const {
    return getImpl<detail::TableLookup_Impl>()->normalizationMethod();
  }

  double TableLookup::normalizationDivisor() const {
    return getImpl<detail::TableLookup_Impl>()->normalizationDivisor();
  }

  std::string TableLookup::outputUnitType() const {
    return getImpl<detail::TableLookup_Impl>()->outputUnitType();
  }

  bool TableLookup::setNormalizationMethod(std::string normalizationMethod) {
    return getImpl<detail::TableLookup_Impl>()->setNormalizationMethod(normalizationMethod);
  }

  bool TableLookup::setNormalizationDivisor(double normalizationDivisior) {
    return getImpl<detail::TableLookup_Impl>()->setNormalizationDivisor(normalizationDivisior);
  }

  bool TableLookup::setOutputUnitType(std::string outputUnitType) {
    return getImpl<detail::TableLookup_Impl>()->setOutputUnitType(outputUnitType);
  }

  bool TableLookup::addIndependentVariable(const TableIndependentVariable& tableIndependentVariable) {
    return getImpl<detail::TableLookup_Impl>()->addIndependentVariable(tableIndependentVariable);
  }

  void TableLookup::removeIndependentVariable(const TableIndependentVariable& tableIndependentVariable) {
    getImpl<detail::TableLookup_Impl>()->removeIndependentVariable(tableIndependentVariable);
  }

  void TableLookup::removeAllIndependentVariables() {
    getImpl<detail::TableLookup_Impl>()->removeAllIndependentVariables();
  }

  std::vector<TableIndependentVariable> TableLookup::independentVariables() const {
    return getImpl<detail::TableLookup_Impl>()->independentVariables();
  }

  /// @cond
  TableLookup::TableLookup(std::shared_ptr<detail::TableLookup_Impl> impl) : Curve(std::dynamic_pointer_cast<detail::Curve_Impl>(std::move(impl))) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
