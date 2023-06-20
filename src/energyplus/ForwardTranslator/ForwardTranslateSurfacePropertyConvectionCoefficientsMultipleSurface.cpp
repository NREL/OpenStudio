/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SurfacePropertyConvectionCoefficientsMultipleSurface.hpp"
#include "../../model/SurfacePropertyConvectionCoefficientsMultipleSurface_Impl.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_ConvectionCoefficients_MultipleSurface_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSurfacePropertyConvectionCoefficientsMultipleSurface(
    model::SurfacePropertyConvectionCoefficientsMultipleSurface& modelObject) {
    boost::optional<std::string> surfaceType = modelObject.surfaceType();
    boost::optional<std::string> convectionCoefficient1Location = modelObject.convectionCoefficient1Location();
    boost::optional<std::string> convectionCoefficient1Type = modelObject.convectionCoefficient1Type();
    boost::optional<double> convectionCoefficient1 = modelObject.convectionCoefficient1();
    boost::optional<Schedule> convectionCoefficient1Schedule = modelObject.convectionCoefficient1Schedule();
    boost::optional<std::string> convectionCoefficient2Location = modelObject.convectionCoefficient2Location();
    boost::optional<std::string> convectionCoefficient2Type = modelObject.convectionCoefficient2Type();
    boost::optional<double> convectionCoefficient2 = modelObject.convectionCoefficient2();
    boost::optional<Schedule> convectionCoefficient2Schedule = modelObject.convectionCoefficient2Schedule();

    if (!(surfaceType && convectionCoefficient1Location && convectionCoefficient1Type)) {
      LOG(Error, "SurfacePropertyConvectionCoefficientsMultipleSurface '" << modelObject.name().get()
                                                                          << "' missing required fields, will not be translated");
      return boost::none;
    }

    IdfObject idfObject =
      createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_ConvectionCoefficients_MultipleSurface, modelObject);

    if (surfaceType) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::SurfaceType, *surfaceType);
    }

    if (convectionCoefficient1Location) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Location,
                          *convectionCoefficient1Location);
    }

    if (convectionCoefficient1Type) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1Type, *convectionCoefficient1Type);
    }

    if (convectionCoefficient1) {
      idfObject.setDouble(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1, *convectionCoefficient1);
    }

    if (convectionCoefficient1Schedule) {
      boost::optional<IdfObject> sch = translateAndMapModelObject(*convectionCoefficient1Schedule);
      if (sch && sch->name()) {
        idfObject.setString(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient1ScheduleName, sch->name().get());
      }
    }

    if (convectionCoefficient2Location) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Location,
                          *convectionCoefficient2Location);
    }

    if (convectionCoefficient2Type) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2Type, *convectionCoefficient2Type);
    }

    if (convectionCoefficient2) {
      idfObject.setDouble(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2, *convectionCoefficient2);
    }

    if (convectionCoefficient2Schedule) {
      boost::optional<IdfObject> sch = translateAndMapModelObject(*convectionCoefficient2Schedule);
      if (sch && sch->name()) {
        idfObject.setString(SurfaceProperty_ConvectionCoefficients_MultipleSurfaceFields::ConvectionCoefficient2ScheduleName, sch->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
