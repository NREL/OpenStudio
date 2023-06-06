/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CurveQuintLinear.hpp"

#include <utilities/idd/Curve_QuintLinear_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveQuintLinear(model::CurveQuintLinear& modelObject) {
    boost::optional<WorkspaceObject> result;

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Curve_QuintLinear, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject( openstudio::IddObjectType::Curve_QuintLinear );
    // m_idfObjects.push_back(idfObject);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
    // But in some cases, you'll want to handle failure without pushing to the map
    // Name
    if (boost::optional<std::string> moName = modelObject.name()) {
      idfObject.setName(*moName);
    }

    // Coefficient1 Constant: Required Double
    double coefficient1Constant = modelObject.coefficient1Constant();
    idfObject.setDouble(Curve_QuintLinearFields::Coefficient1Constant, coefficient1Constant);

    // Coefficient2 v: Required Double
    double coefficient2v = modelObject.coefficient2v();
    idfObject.setDouble(Curve_QuintLinearFields::Coefficient2v, coefficient2v);

    // Coefficient3 w: Required Double
    double coefficient3w = modelObject.coefficient3w();
    idfObject.setDouble(Curve_QuintLinearFields::Coefficient3w, coefficient3w);

    // Coefficient4 x: Required Double
    double coefficient4x = modelObject.coefficient4x();
    idfObject.setDouble(Curve_QuintLinearFields::Coefficient4x, coefficient4x);

    // Coefficient5 y: Required Double
    double coefficient5y = modelObject.coefficient5y();
    idfObject.setDouble(Curve_QuintLinearFields::Coefficient5y, coefficient5y);

    // Coefficient6 z: Required Double
    double coefficient6z = modelObject.coefficient6z();
    idfObject.setDouble(Curve_QuintLinearFields::Coefficient6z, coefficient6z);

    // Minimum Value of v: Required Double
    double minimumValueofv = modelObject.minimumValueofv();
    idfObject.setDouble(Curve_QuintLinearFields::MinimumValueofv, minimumValueofv);

    // Maximum Value of v: Required Double
    double maximumValueofv = modelObject.maximumValueofv();
    idfObject.setDouble(Curve_QuintLinearFields::MaximumValueofv, maximumValueofv);

    // Minimum Value of w: Required Double
    double minimumValueofw = modelObject.minimumValueofw();
    idfObject.setDouble(Curve_QuintLinearFields::MinimumValueofw, minimumValueofw);

    // Maximum Value of w: Required Double
    double maximumValueofw = modelObject.maximumValueofw();
    idfObject.setDouble(Curve_QuintLinearFields::MaximumValueofw, maximumValueofw);

    // Minimum Value of x: Required Double
    double minimumValueofx = modelObject.minimumValueofx();
    idfObject.setDouble(Curve_QuintLinearFields::MinimumValueofx, minimumValueofx);

    // Maximum Value of x: Required Double
    double maximumValueofx = modelObject.maximumValueofx();
    idfObject.setDouble(Curve_QuintLinearFields::MaximumValueofx, maximumValueofx);

    // Minimum Value of y: Required Double
    double minimumValueofy = modelObject.minimumValueofy();
    idfObject.setDouble(Curve_QuintLinearFields::MinimumValueofy, minimumValueofy);

    // Maximum Value of y: Required Double
    double maximumValueofy = modelObject.maximumValueofy();
    idfObject.setDouble(Curve_QuintLinearFields::MaximumValueofy, maximumValueofy);

    // Minimum Value of z: Required Double
    double minimumValueofz = modelObject.minimumValueofz();
    idfObject.setDouble(Curve_QuintLinearFields::MinimumValueofz, minimumValueofz);

    // Maximum Value of z: Required Double
    double maximumValueofz = modelObject.maximumValueofz();
    idfObject.setDouble(Curve_QuintLinearFields::MaximumValueofz, maximumValueofz);

    // Minimum Curve Output: boost::optional<double>
    if (boost::optional<double> _minimumCurveOutput = modelObject.minimumCurveOutput()) {
      idfObject.setDouble(Curve_QuintLinearFields::MinimumCurveOutput, _minimumCurveOutput.get());
    }

    // Maximum Curve Output: boost::optional<double>
    if (boost::optional<double> _maximumCurveOutput = modelObject.maximumCurveOutput()) {
      idfObject.setDouble(Curve_QuintLinearFields::MaximumCurveOutput, _maximumCurveOutput.get());
    }

    // Input Unit Type for v: Optional String
    std::string inputUnitTypeforv = modelObject.inputUnitTypeforv();
    idfObject.setString(Curve_QuintLinearFields::InputUnitTypeforv, inputUnitTypeforv);

    // Input Unit Type for w: Optional String
    std::string inputUnitTypeforw = modelObject.inputUnitTypeforw();
    idfObject.setString(Curve_QuintLinearFields::InputUnitTypeforw, inputUnitTypeforw);

    // Input Unit Type for x: Optional String
    std::string inputUnitTypeforx = modelObject.inputUnitTypeforx();
    idfObject.setString(Curve_QuintLinearFields::InputUnitTypeforx, inputUnitTypeforx);

    // Input Unit Type for y: Optional String
    std::string inputUnitTypefory = modelObject.inputUnitTypefory();
    idfObject.setString(Curve_QuintLinearFields::InputUnitTypefory, inputUnitTypefory);

    // Input Unit Type for z: Optional String
    std::string inputUnitTypeforz = modelObject.inputUnitTypeforz();
    idfObject.setString(Curve_QuintLinearFields::InputUnitTypeforz, inputUnitTypeforz);

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
