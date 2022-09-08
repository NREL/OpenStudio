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

#include "../ReverseTranslator.hpp"

#include "../../model/TableLookup.hpp"

#include "../../model/TableIndependentVariable.hpp"
#include "../../model/TableIndependentVariable_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Table_Lookup_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariableList_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateTableLookup(const WorkspaceObject& workspaceObject) {

    // External File Name: Optional String
    if (boost::optional<std::string> _externalFileName = workspaceObject.getString(Table_LookupFields::ExternalFileName, false, true)) {
      LOG(Error, workspaceObject.briefDescription()
                   << " is referencing an External File '" << _externalFileName.get() << "', which is not supported.");
      return boost::none;
    }

    // Instantiate an object of the class to store the values,
    openstudio::model::TableLookup modelObject(m_model);

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Independent Variable List Name: Required Object
    if (boost::optional<WorkspaceObject> varList_ = workspaceObject.getTarget(Table_LookupFields::IndependentVariableListName)) {

      for (unsigned i = 0; const IdfExtensibleGroup& idfGroup : varList_->extensibleGroups()) {
        auto weg = idfGroup.cast<WorkspaceExtensibleGroup>();
        if (boost::optional<WorkspaceObject> var_ = weg.getTarget(Table_IndependentVariableListExtensibleFields::IndependentVariableName)) {

          if (auto mo_ = translateTableIndependentVariable(var_.get())) {
            if (auto v_ = mo_->optionalCast<openstudio::model::TableIndependentVariable>()) {
              modelObject.addIndependentVariable(v_.get());
            } else {
              LOG(Warn, "For " << workspaceObject.briefDescription() << ", in the Table:IndependentVariableList named '" << varList_->nameString()
                               << "', the extensible group " << i << " has a wrong type for 'Independent Variable', skipping.");
            }
          }

        } else {
          LOG(Error, "For " << workspaceObject.briefDescription() << ", in the Table:IndependentVariableList named '" << varList_->nameString()
                            << "', the extensible group " << i << " does not refer to a Table:IndependentVariable, skipping.");
        }

        ++i;
      }

    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Independent Variable List Name'");
      return boost::none;
    }

    // Normalization Method: Optional String
    if (boost::optional<std::string> _normalizationMethod = workspaceObject.getString(Table_LookupFields::NormalizationMethod, false, true)) {
      modelObject.setNormalizationMethod(_normalizationMethod.get());
    }

    // Normalization Divisor: Optional Double
    if (boost::optional<double> _normalizationDivisor = workspaceObject.getDouble(Table_LookupFields::NormalizationDivisor)) {
      modelObject.setNormalizationDivisor(_normalizationDivisor.get());
    }

    // Minimum Output: Optional Double
    if (boost::optional<double> _minimumOutput = workspaceObject.getDouble(Table_LookupFields::MinimumOutput)) {
      modelObject.setMinimumOutput(_minimumOutput.get());
    }

    // Maximum Output: Optional Double
    if (boost::optional<double> _maximumOutput = workspaceObject.getDouble(Table_LookupFields::MaximumOutput)) {
      modelObject.setMaximumOutput(_maximumOutput.get());
    }

    // Output Unit Type: Optional String
    if (boost::optional<std::string> _outputUnitType = workspaceObject.getString(Table_LookupFields::OutputUnitType, false, true)) {
      modelObject.setOutputUnitType(_outputUnitType.get());
    }

    // // External File Column Number: Optional Integer
    // if (boost::optional<int> _externalFileColumnNumber = workspaceObject.getInt(Table_LookupFields::ExternalFileColumnNumber)) {
    //   modelObject.setExternalFileColumnNumber(_externalFileColumnNumber.get());
    // }

    // // External File Starting Row Number: Optional Integer
    // if (boost::optional<int> _externalFileStartingRowNumber = workspaceObject.getInt(Table_LookupFields::ExternalFileStartingRowNumber)) {
    //   modelObject.setExternalFileStartingRowNumber(_externalFileStartingRowNumber.get());
    // }

    // Extensible Output Values
    for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
      if (boost::optional<double> value = eg.getDouble(Table_LookupExtensibleFields::OutputValue)) {
        modelObject.addOutputValue(value.get());
      }
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
