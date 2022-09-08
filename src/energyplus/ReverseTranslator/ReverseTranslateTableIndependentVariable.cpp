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
