/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../utilities/core/Assert.hpp"

#include <algorithm>
#include <iomanip>
#include <limits>

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
      // Technically this is dependent on the actual number of independent variables used, but this is a staic one, and it's not worth the hassle
      // (we could just mutate the static vector (and not make it const), still returning a const reference to it)
      static const std::vector<std::string> result{
        "Performance Curve Output Value",           "Performance Curve Input Variable 1 Value", "Performance Curve Input Variable 2 Value",
        "Performance Curve Input Variable 3 Value", "Performance Curve Input Variable 4 Value", "Performance Curve Input Variable 5 Value",
      };

      return result;
    }

    IddObjectType TableLookup_Impl::iddObjectType() const {
      return TableLookup::iddObjectType();
    }

    int TableLookup_Impl::numVariables() const {
      return independentVariables().size();
    }

    double TableLookup_Impl::evaluate(const std::vector<double>& /*independentVariables*/) const {
      LOG(Warn, "Curve evaluation isn't implemented for TableLookup");
      return -9999.0;
    }

    std::vector<ModelObject> TableLookup_Impl::children() const {
      // #4754 - in the case where we clone an object that uses this TableLookup as a resource into another model,
      // we need the ModelObjectList to be a child of this object, so that ResourceObject::getRecursiveResourceSubTrees finds it
      // (and the variables it references in a recursive manner)
      std::vector<ModelObject> result;
      if (auto const varList_ = independentVariableList()) {
        result.push_back(varList_.get());
      }
      return result;
    }

    ModelObject TableLookup_Impl::clone(Model model) const {

      // Call the ModelObject_Impl instead of ParentObject_Impl because we deal with the ModelObjectList ourselves
      auto modelObjectClone = ModelObject_Impl::clone(model).cast<TableLookup>();  // NOLINT

      // ModelObjectList is kinda dumb, cloning it also clones the underlying objects regardless of whether they are resourceObjects or not
      // We don't want that, so we make a new one, and populate it like the original
      auto newVarList = ModelObjectList(model);
      newVarList.setName(modelObjectClone.name().get() + "  Independent Variable List");
      bool ok = modelObjectClone.getImpl<detail::TableLookup_Impl>()->setIndependentVariableList(newVarList);
      OS_ASSERT(ok);

      // Now, the matter is to repopulate it.
      for (const TableIndependentVariable& var : this->independentVariables()) {
        if (model != this->model()) {
          auto varClone = var.clone(model).cast<TableIndependentVariable>();
          modelObjectClone.addIndependentVariable(varClone);
        } else {
          // Add that to the new object
          modelObjectClone.addIndependentVariable(var);
        }
      }
      return std::move(modelObjectClone);
    }

    std::vector<IdfObject> TableLookup_Impl::remove() {
      std::vector<IdfObject> result;

      // ModelObjectList is kinda dumb, it removes all the modelObjects it has, which we don't want
      // Instead we look at the independent variables listed, if they are used only once => delete them
      // Then clear the modelObjectList, and remove it
      if (boost::optional<ModelObjectList> varList = this->independentVariableList()) {
        auto vars = castVector<ResourceObject>(varList->modelObjects());

        for (auto& var : vars) {
          if (var.directUseCount() == 1) {
            std::vector<IdfObject> removedModelObject = var.remove();
            result.insert(result.end(), removedModelObject.begin(), removedModelObject.end());
          }
        }

        varList->clearExtensibleGroups();  // Clearer than removeAllModelObjects (which does the same thing, but could mean delete all objects)
        std::vector<IdfObject> removedVarList = varList->remove();
        result.insert(result.end(), removedVarList.begin(), removedVarList.end());
      }

      std::vector<IdfObject> removedObject = Curve_Impl::remove();
      result.insert(result.end(), removedObject.begin(), removedObject.end());

      return result;
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

    boost::optional<double> TableLookup_Impl::minimumOutput() const {
      return getDouble(OS_Table_LookupFields::MinimumOutput, true);
    }

    boost::optional<double> TableLookup_Impl::maximumOutput() const {
      return getDouble(OS_Table_LookupFields::MaximumOutput, true);
    }

    std::string TableLookup_Impl::outputUnitType() const {
      boost::optional<std::string> value = getString(OS_Table_LookupFields::OutputUnitType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool TableLookup_Impl::setNormalizationMethod(const std::string& normalizationMethod) {
      bool result = setString(OS_Table_LookupFields::NormalizationMethod, normalizationMethod);
      return result;
    }

    bool TableLookup_Impl::setNormalizationDivisor(double normalizationDivisor) {
      if (std::abs(normalizationDivisor) < std::numeric_limits<double>::min()) {
        LOG(Warn, "Unable to set " << briefDescription() << "'s Normalization divisor to zero.");
        return false;
      }

      bool result = setDouble(OS_Table_LookupFields::NormalizationDivisor, normalizationDivisor);
      return result;
    }

    bool TableLookup_Impl::setMinimumOutput(double minimumOutput) {
      bool result = setDouble(OS_Table_LookupFields::MinimumOutput, minimumOutput);
      return result;
    }

    void TableLookup_Impl::resetMinimumOutput() {
      bool result = setString(OS_Table_LookupFields::MinimumOutput, "");
      OS_ASSERT(result);
    }

    bool TableLookup_Impl::setMaximumOutput(double maximumOutput) {
      bool result = setDouble(OS_Table_LookupFields::MaximumOutput, maximumOutput);
      return result;
    }

    void TableLookup_Impl::resetMaximumOutput() {
      bool result = setString(OS_Table_LookupFields::MaximumOutput, "");
      OS_ASSERT(result);
    }

    bool TableLookup_Impl::setOutputUnitType(const std::string& outputUnitType) {
      bool result = setString(OS_Table_LookupFields::OutputUnitType, outputUnitType);
      return result;
    }

    bool TableLookup_Impl::addOutputValue(double outputValue) {
      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool result = eg.setDouble(OS_Table_LookupExtensibleFields::OutputValue, outputValue);
      if (!result) {
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      }

      return result;
    }

    bool TableLookup_Impl::removeOutputValue(unsigned groupIndex) {
      bool result;

      unsigned num = numberofOutputValues();
      if (groupIndex < num) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      } else {
        result = false;
      }
      return result;
    }

    void TableLookup_Impl::removeAllOutputValues() {
      this->clearExtensibleGroups();
    }

    std::vector<double> TableLookup_Impl::outputValues() const {
      std::vector<double> result;
      for (const auto& eg : extensibleGroups()) {
        auto _d = eg.getDouble(OS_Table_LookupExtensibleFields::OutputValue);
        OS_ASSERT(_d);
        result.push_back(_d.get());
      }
      return result;
    }

    unsigned TableLookup_Impl::numberofOutputValues() const {
      return numExtensibleGroups();
    }

    bool TableLookup_Impl::setOutputValues(const std::vector<double>& outputValues) {
      bool result = true;

      this->clearExtensibleGroups();
      for (const auto& val : outputValues) {
        result &= addOutputValue(val);
      }
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

    bool TableLookup_Impl::removeIndependentVariable(const TableIndependentVariable& tableIndependentVariable) {
      bool result = false;
      auto modelObjectList = independentVariableList();
      if (modelObjectList) {
        modelObjectList->removeModelObject(tableIndependentVariable);
        result = true;
      }
      return result;
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
    return {IddObjectType::OS_Table_Lookup};
  }

  std::vector<std::string> TableLookup::normalizationMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_LookupFields::NormalizationMethod);
  }

  std::vector<std::string> TableLookup::validNormalizationMethodValues() {
    return normalizationMethodValues();
  }

  std::vector<std::string> TableLookup::outputUnitTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Table_LookupFields::OutputUnitType);
  }

  std::vector<std::string> TableLookup::validOutputUnitTypeValues() {
    return outputUnitTypeValues();
  }

  std::string TableLookup::normalizationMethod() const {
    return getImpl<detail::TableLookup_Impl>()->normalizationMethod();
  }

  double TableLookup::normalizationDivisor() const {
    return getImpl<detail::TableLookup_Impl>()->normalizationDivisor();
  }

  boost::optional<double> TableLookup::minimumOutput() const {
    return getImpl<detail::TableLookup_Impl>()->minimumOutput();
  }

  boost::optional<double> TableLookup::maximumOutput() const {
    return getImpl<detail::TableLookup_Impl>()->maximumOutput();
  }

  std::string TableLookup::outputUnitType() const {
    return getImpl<detail::TableLookup_Impl>()->outputUnitType();
  }

  bool TableLookup::setNormalizationMethod(const std::string& normalizationMethod) {
    return getImpl<detail::TableLookup_Impl>()->setNormalizationMethod(normalizationMethod);
  }

  bool TableLookup::setNormalizationDivisor(double normalizationDivisor) {
    return getImpl<detail::TableLookup_Impl>()->setNormalizationDivisor(normalizationDivisor);
  }

  bool TableLookup::setMinimumOutput(double minimumOutput) {
    return getImpl<detail::TableLookup_Impl>()->setMinimumOutput(minimumOutput);
  }

  void TableLookup::resetMinimumOutput() {
    getImpl<detail::TableLookup_Impl>()->resetMinimumOutput();
  }

  bool TableLookup::setMaximumOutput(double maximumOutput) {
    return getImpl<detail::TableLookup_Impl>()->setMaximumOutput(maximumOutput);
  }

  void TableLookup::resetMaximumOutput() {
    getImpl<detail::TableLookup_Impl>()->resetMaximumOutput();
  }

  bool TableLookup::setOutputUnitType(const std::string& outputUnitType) {
    return getImpl<detail::TableLookup_Impl>()->setOutputUnitType(outputUnitType);
  }

  bool TableLookup::addOutputValue(double outputValue) {
    return getImpl<detail::TableLookup_Impl>()->addOutputValue(outputValue);
  }

  bool TableLookup::removeOutputValue(unsigned groupIndex) {
    return getImpl<detail::TableLookup_Impl>()->removeOutputValue(groupIndex);
  }

  void TableLookup::removeAllOutputValues() {
    getImpl<detail::TableLookup_Impl>()->removeAllOutputValues();
  }

  std::vector<double> TableLookup::outputValues() const {
    return getImpl<detail::TableLookup_Impl>()->outputValues();
  }

  unsigned TableLookup::numberofOutputValues() const {
    return getImpl<detail::TableLookup_Impl>()->numberofOutputValues();
  }

  bool TableLookup::setOutputValues(const std::vector<double>& outputValues) {
    return getImpl<detail::TableLookup_Impl>()->setOutputValues(outputValues);
  }

  bool TableLookup::addIndependentVariable(const TableIndependentVariable& tableIndependentVariable) {
    return getImpl<detail::TableLookup_Impl>()->addIndependentVariable(tableIndependentVariable);
  }

  bool TableLookup::removeIndependentVariable(const TableIndependentVariable& tableIndependentVariable) {
    return getImpl<detail::TableLookup_Impl>()->removeIndependentVariable(tableIndependentVariable);
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
