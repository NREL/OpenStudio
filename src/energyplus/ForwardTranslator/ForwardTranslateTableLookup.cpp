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

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateTableLookup(TableLookup& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    std::vector<TableIndependentVariable> independentVariables = modelObject.independentVariables();

    // If the TableLookup doesn't have at least one TableIndependentVariable, then it shouldn't be translated
    if (independentVariables.empty()) {
      LOG(Warn, "TableLookup " << modelObject.name().get() << " has no TableIndependentVariables, it will not be translated");
      return boost::none;
    }

    // Table:Lookup
    IdfObject tableLookup = createRegisterAndNameIdfObject(openstudio::IddObjectType::Table_Lookup, modelObject);

    // NormalizationMethod
    if ((s = modelObject.normalizationMethod())) {
      tableLookup.setString(Table_LookupFields::NormalizationMethod, s.get());
    }

    // NormalizationDivisor
    if ((d = modelObject.normalizationDivisor())) {
      tableLookup.setDouble(Table_LookupFields::NormalizationDivisor, d.get());
    }

    // MinimumOutput
    if ((d = modelObject.minimumOutput())) {
      tableLookup.setDouble(Table_LookupFields::MinimumOutput, d.get());
    }

    // MaximumOutput
    if ((d = modelObject.maximumOutput())) {
      tableLookup.setDouble(Table_LookupFields::MaximumOutput, d.get());
    }

    // OutputUnitType
    if ((s = modelObject.outputUnitType())) {
      tableLookup.setString(Table_LookupFields::OutputUnitType, s.get());
    }

    // OutputValue
    for (const double& outputValue : modelObject.outputValues()) {
      IdfExtensibleGroup eg = tableLookup.pushExtensibleGroup();
      eg.setDouble(Table_LookupExtensibleFields::OutputValue, outputValue);
    }

    // Table:IndependentVariableList
    IdfObject tableIndependentVariableList(IddObjectType::Table_IndependentVariableList);
    tableIndependentVariableList.setName(modelObject.nameString() + " Independent Variable List");
    m_idfObjects.push_back(tableIndependentVariableList);

    // IndependentVariableListName
    tableLookup.setString(Table_LookupFields::IndependentVariableListName, tableIndependentVariableList.nameString());

    // Table:IndependentVariable
    for (auto& independentVariable : independentVariables) {
      IdfObject tableIndependentVariable(IddObjectType::Table_IndependentVariable);
      tableIndependentVariable.setName(independentVariable.nameString());
      m_idfObjects.push_back(tableIndependentVariable);

      IdfExtensibleGroup independentVariableGroup = tableIndependentVariableList.pushExtensibleGroup();
      independentVariableGroup.setString(Table_IndependentVariableListExtensibleFields::IndependentVariableName, independentVariable.nameString());

      // InterpolationMethod
      if ((s = independentVariable.interpolationMethod())) {
        tableIndependentVariable.setString(Table_IndependentVariableFields::InterpolationMethod, s.get());
      }

      // ExtrapolationMethod
      if ((s = independentVariable.extrapolationMethod())) {
        tableIndependentVariable.setString(Table_IndependentVariableFields::ExtrapolationMethod, s.get());
      }

      // MinimumValue
      if ((d = independentVariable.minimumValue())) {
        tableIndependentVariable.setDouble(Table_IndependentVariableFields::MinimumValue, d.get());
      }

      // MaximumValue
      if ((d = independentVariable.maximumValue())) {
        tableIndependentVariable.setDouble(Table_IndependentVariableFields::MaximumValue, d.get());
      }

      // NormalizationReferenceValue
      if ((d = independentVariable.normalizationReferenceValue())) {
        tableIndependentVariable.setDouble(Table_IndependentVariableFields::NormalizationReferenceValue, d.get());
      }

      // UnitType
      if ((s = independentVariable.unitType())) {
        tableIndependentVariable.setString(Table_IndependentVariableFields::UnitType, s.get());
      }

      // Value
      for (const double& value : independentVariable.values()) {
        IdfExtensibleGroup eg = tableIndependentVariable.pushExtensibleGroup();
        eg.setDouble(Table_IndependentVariableExtensibleFields::Value, value);
      }
    }

    return tableLookup;
  }

}  // namespace energyplus

}  // namespace openstudio
