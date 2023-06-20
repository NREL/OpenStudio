/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
