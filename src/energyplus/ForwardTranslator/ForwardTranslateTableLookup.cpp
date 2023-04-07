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
