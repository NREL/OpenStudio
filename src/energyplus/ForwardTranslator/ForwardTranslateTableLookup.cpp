/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/TableLookup.hpp"
#include "../../model/TableLookup_Impl.hpp"
#include "../../model/TableIndependentVariable.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"

#include <utilities/idd/Table_Lookup_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariableList_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>  // For sort, unique
#include <iterator>   // For distance

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateTableLookup(TableLookup& modelObject) {

    std::vector<TableIndependentVariable> independentVariables = modelObject.independentVariables();

    // If the TableLookup doesn't have at least one TableIndependentVariable, then it shouldn't be translated
    if (independentVariables.empty()) {
      LOG(Warn, "TableLookup " << modelObject.name().get() << " has no TableIndependentVariables, it will not be translated");
      return boost::none;
    }

    size_t independentVariableGridSize = 1;
    std::vector<double> outputValues = modelObject.outputValues();
    size_t outValueSize = outputValues.size();
    for (const auto& independentVariable : independentVariables) {
      auto v = independentVariable.values();
      std::sort(v.begin(), v.end());
      size_t uniqueCount = std::distance(v.begin(), std::unique(v.begin(), v.end()));
      independentVariableGridSize *= uniqueCount;
    }

    if (independentVariableGridSize != outValueSize) {
      LOG(Warn, "Not translating " << modelObject.briefDescription() << ", it has a mismatch between number of outputValues (=" << outValueSize
                                   << ") and the grid size of the TableIndependentVariable(s) (=" << independentVariableGridSize << ").");
      return boost::none;
    }

    // Table:Lookup
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Table_Lookup, modelObject);

    // Normalization Method: String
    idfObject.setString(Table_LookupFields::NormalizationMethod, modelObject.normalizationMethod());

    // Normalization Divisor: Double
    idfObject.setDouble(Table_LookupFields::NormalizationDivisor, modelObject.normalizationDivisor());

    // Minimum Output: boost::optional<double>
    if (boost::optional<double> minimumOutput_ = modelObject.minimumOutput()) {
      idfObject.setDouble(Table_LookupFields::MinimumOutput, minimumOutput_.get());
    }

    // Maximum Output: boost::optional<double>
    if (boost::optional<double> maximumOutput_ = modelObject.maximumOutput()) {
      idfObject.setDouble(Table_LookupFields::MaximumOutput, maximumOutput_.get());
    }

    // Output Unit Type: String
    idfObject.setString(Table_LookupFields::OutputUnitType, modelObject.outputUnitType());

    // OutputValue
    for (const double& outputValue : outputValues) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setDouble(Table_LookupExtensibleFields::OutputValue, outputValue);
    }

    // Table:IndependentVariableList
    IdfObject tableIndependentVariableList(IddObjectType::Table_IndependentVariableList);
    tableIndependentVariableList.setName(modelObject.nameString() + " Independent Variable List");
    m_idfObjects.push_back(tableIndependentVariableList);

    // IndependentVariableListName
    idfObject.setString(Table_LookupFields::IndependentVariableListName, tableIndependentVariableList.nameString());

    // Table:IndependentVariable
    for (auto& independentVariable : independentVariables) {
      if (auto tableIndependentVariable_ = translateAndMapModelObject(independentVariable)) {
        IdfExtensibleGroup independentVariableGroup = tableIndependentVariableList.pushExtensibleGroup({tableIndependentVariable_->nameString()});
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
