/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/TableIndependentVariable.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Table_IndependentVariable_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateTableIndependentVariable(model::TableIndependentVariable& modelObject) {
    boost::optional<IdfObject> result;

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Table_IndependentVariable, modelObject);

    // Interpolation Method: String
    idfObject.setString(Table_IndependentVariableFields::InterpolationMethod, modelObject.interpolationMethod());

    // Extrapolation Method: String
    idfObject.setString(Table_IndependentVariableFields::ExtrapolationMethod, modelObject.extrapolationMethod());

    // Minimum Value: boost::optional<double>
    if (boost::optional<double> minimumValue_ = modelObject.minimumValue()) {
      idfObject.setDouble(Table_IndependentVariableFields::MinimumValue, minimumValue_.get());
    }

    // Maximum Value: boost::optional<double>
    if (boost::optional<double> maximumValue_ = modelObject.maximumValue()) {
      idfObject.setDouble(Table_IndependentVariableFields::MaximumValue, maximumValue_.get());
    }

    // Normalization Reference Value: boost::optional<double>
    if (boost::optional<double> normalizationReferenceValue_ = modelObject.normalizationReferenceValue()) {
      idfObject.setDouble(Table_IndependentVariableFields::NormalizationReferenceValue, normalizationReferenceValue_.get());
    }

    // Unit Type: String
    idfObject.setString(Table_IndependentVariableFields::UnitType, modelObject.unitType());

    // Value
    for (const double& value : modelObject.values()) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setDouble(Table_IndependentVariableExtensibleFields::Value, value);
    }

    // External File Name: boost::optional<std::string>

    // External File Column Number: boost::optional<int>

    // External File Starting Row Number: boost::optional<int>

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
