/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/TableMultiVariableLookup.hpp"
#include "../../model/TableMultiVariableLookup_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/Table_Lookup_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariableList_FieldEnums.hxx>
#include <utilities/idd/Table_IndependentVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/DeprecatedHelpers.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4996)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

  boost::optional<IdfObject> ForwardTranslator::translateTableMultiVariableLookup(TableMultiVariableLookup& modelObject) {
    DEPRECATED_AT_MSG(3, 5, 0, "The entire TableMultiVariableLookup will be removed.");
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;
    OptionalInt n;

    // create these objects here but do not add to file yet in case of failure
    std::vector<IdfObject> tableIndependentVariables;
    IdfObject tableIndependentVariableList(IddObjectType::Table_IndependentVariableList);
    s = modelObject.name();
    if (s) {
      tableIndependentVariableList.setName(*s + "_IndependentVariableList");
    }

    std::string interpolationMethod = modelObject.interpolationMethod();

    int numberofIndependentVariables = modelObject.numberofIndependentVariables();
    unsigned expectedNumberOfValues = 1;
    for (int i = 0; i < numberofIndependentVariables; ++i) {
      auto xValues = modelObject.xValues(i);
      auto sizeX = xValues.size();

      // expect a full grid of values
      expectedNumberOfValues *= sizeX;

      // create an independent variable for this x
      IdfObject tableIndependentVariable(IddObjectType::Table_IndependentVariable);
      if (s) {
        tableIndependentVariable.setName(*s + "_IndependentVariable_" + std::to_string(i));
      }
      tableIndependentVariableList.pushExtensibleGroup(std::vector<std::string>(1, tableIndependentVariable.nameString()));

      // there is not a straightforward mapping of the old interpolation methods to new ones, e.g. there is no "None" option for extrapolation
      if (istringEqual(interpolationMethod, "LagrangeInterpolationLinearExtrapolation")) {
        tableIndependentVariable.setString(Table_IndependentVariableFields::InterpolationMethod, "Cubic");
        tableIndependentVariable.setString(Table_IndependentVariableFields::ExtrapolationMethod, "Linear");
      } else if (istringEqual(interpolationMethod, "LinearInterpolationOfTable")) {
        tableIndependentVariable.setString(Table_IndependentVariableFields::InterpolationMethod, "Linear");
        tableIndependentVariable.setString(Table_IndependentVariableFields::ExtrapolationMethod, "Constant");
      } else if (istringEqual(interpolationMethod, "EvaluateCurveToLimits")) {
        tableIndependentVariable.setString(Table_IndependentVariableFields::InterpolationMethod, "Cubic");
        tableIndependentVariable.setString(Table_IndependentVariableFields::ExtrapolationMethod, "Constant");
      }

      // these values should be unique and sorted in ascending order
      tableIndependentVariable.setDouble(Table_IndependentVariableFields::MinimumValue, xValues[0]);
      tableIndependentVariable.setDouble(Table_IndependentVariableFields::MaximumValue, xValues[sizeX - 1]);

      switch (i) {
        case 0:
          tableIndependentVariable.setString(Table_IndependentVariableFields::UnitType, modelObject.inputUnitTypeforX1());
          break;
        case 1:
          tableIndependentVariable.setString(Table_IndependentVariableFields::UnitType, modelObject.inputUnitTypeforX2());
          break;
        case 2:
          tableIndependentVariable.setString(Table_IndependentVariableFields::UnitType, modelObject.inputUnitTypeforX3());
          break;
        case 3:
          tableIndependentVariable.setString(Table_IndependentVariableFields::UnitType, modelObject.inputUnitTypeforX4());
          break;
        case 4:
          tableIndependentVariable.setString(Table_IndependentVariableFields::UnitType, modelObject.inputUnitTypeforX5());
          break;
        default:
          break;
      }

      // add the values
      for (const auto& xValue : xValues) {
        auto eg = tableIndependentVariable.pushExtensibleGroup();
        eg.setDouble(Table_IndependentVariableExtensibleFields::Value, xValue);
      }

      tableIndependentVariables.push_back(tableIndependentVariable);
    }

    // check that we have the correct number of points
    auto points = modelObject.points();
    if (expectedNumberOfValues != points.size()) {
      LOG(Error, "Expected " << expectedNumberOfValues << " points but found " << points.size() << " in object " << modelObject.briefDescription()
                             << ". Object not translated");
      return boost::none;
    }

    // important to sort points in order expected by EnergyPlus
    std::sort(points.begin(), points.end());

    // Create a new IddObjectType::Table_Lookup, this is the main object returned
    IdfObject tableLookup(IddObjectType::Table_Lookup);
    m_idfObjects.push_back(tableLookup);
    m_idfObjects.push_back(tableIndependentVariableList);
    for (const auto& tableIndependentVariable : tableIndependentVariables) {
      m_idfObjects.push_back(tableIndependentVariable);
    }

    // Name
    s = modelObject.name();
    if (s) {
      tableLookup.setName(*s);
    }

    // Independent Variable List Name
    tableLookup.setString(Table_LookupFields::IndependentVariableListName, tableIndependentVariableList.nameString());

    // NormalizationReference
    if ((d = modelObject.normalizationReference())) {
      tableLookup.setString(Table_LookupFields::NormalizationMethod, "DivisorOnly");
      tableLookup.setDouble(Table_LookupFields::NormalizationDivisor, d.get());
    } else {
      tableLookup.setString(Table_LookupFields::NormalizationMethod, "None");
      tableLookup.setDouble(Table_LookupFields::NormalizationDivisor, 1.0);
    }

    // MinimumTableOutput
    if ((d = modelObject.minimumTableOutput())) {
      tableLookup.setDouble(Table_LookupFields::MinimumOutput, d.get());
    }

    // MaximumTableOutput
    if ((d = modelObject.maximumTableOutput())) {
      tableLookup.setDouble(Table_LookupFields::MaximumOutput, d.get());
    }

    // OutputUnitType
    if ((s = modelObject.outputUnitType())) {
      tableLookup.setString(Table_LookupFields::OutputUnitType, s.get());
    }

    // ExternalFileName
    // Not supported

    // add the sorted values
    for (const auto& point : points) {
      auto eg = tableLookup.pushExtensibleGroup();
      eg.setDouble(Table_LookupExtensibleFields::OutputValue, point.y());
    }

    return tableLookup;
  }

#if defined(_MSC_VER)
#  pragma warning(pop)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic pop
#endif

}  // namespace energyplus

}  // namespace openstudio
