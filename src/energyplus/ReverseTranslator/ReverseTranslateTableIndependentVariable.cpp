/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/TableIndependentVariable.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Table_IndependentVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateTableIndependentVariable(const WorkspaceObject& workspaceObject) {

    // External File Name: Optional String
    if (boost::optional<std::string> _externalFileName = workspaceObject.getString(Table_IndependentVariableFields::ExternalFileName, false, true)) {
      LOG(Error, workspaceObject.briefDescription() << " is referencing an External File, which is not supported");
      return boost::none;
    }

    // Instantiate an object of the class to store the values,
    openstudio::model::TableIndependentVariable modelObject(m_model);

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Interpolation Method: Optional String
    if (boost::optional<std::string> _interpolationMethod =
          workspaceObject.getString(Table_IndependentVariableFields::InterpolationMethod, false, true)) {
      modelObject.setInterpolationMethod(_interpolationMethod.get());
    }

    // Extrapolation Method: Optional String
    if (boost::optional<std::string> _extrapolationMethod =
          workspaceObject.getString(Table_IndependentVariableFields::ExtrapolationMethod, false, true)) {
      modelObject.setExtrapolationMethod(_extrapolationMethod.get());
    }

    // Minimum Value: Optional Double
    if (boost::optional<double> _minimumValue = workspaceObject.getDouble(Table_IndependentVariableFields::MinimumValue)) {
      modelObject.setMinimumValue(_minimumValue.get());
    }

    // Maximum Value: Optional Double
    if (boost::optional<double> _maximumValue = workspaceObject.getDouble(Table_IndependentVariableFields::MaximumValue)) {
      modelObject.setMaximumValue(_maximumValue.get());
    }

    // Normalization Reference Value: Optional Double
    if (boost::optional<double> _normalizationReferenceValue =
          workspaceObject.getDouble(Table_IndependentVariableFields::NormalizationReferenceValue)) {
      modelObject.setNormalizationReferenceValue(_normalizationReferenceValue.get());
    }

    // Unit Type: Optional String
    if (boost::optional<std::string> _unitType = workspaceObject.getString(Table_IndependentVariableFields::UnitType, false, true)) {
      modelObject.setUnitType(_unitType.get());
    }

    // // External File Column Number: Optional Integer
    // if (boost::optional<int> _externalFileColumnNumber = workspaceObject.getInt(Table_IndependentVariableFields::ExternalFileColumnNumber)) {
    //   modelObject.setExternalFileColumnNumber(_externalFileColumnNumber.get());
    // }

    // // External File Starting Row Number: Optional Integer
    // if (boost::optional<int> _externalFileStartingRowNumber = workspaceObject.getInt(Table_IndependentVariableFields::ExternalFileStartingRowNumber)) {
    //   modelObject.setExternalFileStartingRowNumber(_externalFileStartingRowNumber.get());
    // }

    // Extensible Values
    for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
      if (boost::optional<double> value = eg.getDouble(Table_IndependentVariableExtensibleFields::Value)) {
        modelObject.addValue(value.get());
      }
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
