/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SurfacePropertyConvectionCoefficients.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/SurfaceProperty_ConvectionCoefficients_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSurfacePropertyConvectionCoefficients(model::SurfacePropertyConvectionCoefficients& modelObject) {
    boost::optional<ModelObject> convectionCoefficientSurface = modelObject.surfaceAsModelObject();
    boost::optional<std::string> convectionCoefficient1Location = modelObject.convectionCoefficient1Location();
    boost::optional<std::string> convectionCoefficient1Type = modelObject.convectionCoefficient1Type();
    boost::optional<double> convectionCoefficient1 = modelObject.convectionCoefficient1();
    boost::optional<Schedule> convectionCoefficient1Schedule = modelObject.convectionCoefficient1Schedule();
    boost::optional<std::string> convectionCoefficient2Location = modelObject.convectionCoefficient2Location();
    boost::optional<std::string> convectionCoefficient2Type = modelObject.convectionCoefficient2Type();
    boost::optional<double> convectionCoefficient2 = modelObject.convectionCoefficient2();
    boost::optional<Schedule> convectionCoefficient2Schedule = modelObject.convectionCoefficient2Schedule();

    if (!(convectionCoefficientSurface && convectionCoefficient1Location && convectionCoefficient1Type)) {
      if (convectionCoefficientSurface) {
        LOG(Error, "SurfacePropertyConvectionCoefficients for Surface '" << convectionCoefficientSurface->nameString()
                                                                         << "' missing required fields, will not be translated");
      } else {
        LOG(Error, "SurfacePropertyConvectionCoefficients does not reference a surface, it will not be translated.");
      }
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_ConvectionCoefficients, modelObject);

    if (convectionCoefficientSurface) {
      boost::optional<IdfObject> surface = translateAndMapModelObject(*convectionCoefficientSurface);
      if (surface && surface->name()) {
        idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::SurfaceName, surface->name().get());
      }
    }

    if (convectionCoefficient1Location) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Location, *convectionCoefficient1Location);
    }

    if (convectionCoefficient1Type) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1Type, *convectionCoefficient1Type);
    }

    if (convectionCoefficient1) {
      idfObject.setDouble(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1, *convectionCoefficient1);
    }

    if (convectionCoefficient1Schedule) {
      boost::optional<IdfObject> sch = translateAndMapModelObject(*convectionCoefficient1Schedule);
      if (sch && sch->name()) {
        idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient1ScheduleName, sch->name().get());
      }
    }

    if (convectionCoefficient2Location) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Location, *convectionCoefficient2Location);
    }

    if (convectionCoefficient2Type) {
      idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2Type, *convectionCoefficient2Type);
    }

    if (convectionCoefficient2) {
      idfObject.setDouble(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2, *convectionCoefficient2);
    }

    if (convectionCoefficient2Schedule) {
      boost::optional<IdfObject> sch = translateAndMapModelObject(*convectionCoefficient2Schedule);
      if (sch && sch->name()) {
        idfObject.setString(SurfaceProperty_ConvectionCoefficientsFields::ConvectionCoefficient2ScheduleName, sch->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
