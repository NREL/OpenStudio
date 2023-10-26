/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter.hpp"
#include "../../model/SurfacePropertyExposedFoundationPerimeter_Impl.hpp"
#include "../../model/Surface.hpp"

#include <utilities/idd/SurfaceProperty_ExposedFoundationPerimeter_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSurfacePropertyExposedFoundationPerimeter(SurfacePropertyExposedFoundationPerimeter& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::SurfaceProperty_ExposedFoundationPerimeter);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(SurfaceProperty_ExposedFoundationPerimeterFields::SurfaceName, modelObject.surface().nameString());

    idfObject.setString(SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterCalculationMethod,
                        modelObject.exposedPerimeterCalculationMethod());

    boost::optional<double> value;

    if ((value = modelObject.totalExposedPerimeter())) {
      idfObject.setDouble(SurfaceProperty_ExposedFoundationPerimeterFields::TotalExposedPerimeter, value.get());
      idfObject.setString(SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterFraction, "");
    } else {
      idfObject.setString(SurfaceProperty_ExposedFoundationPerimeterFields::TotalExposedPerimeter, "");
      idfObject.setDouble(SurfaceProperty_ExposedFoundationPerimeterFields::ExposedPerimeterFraction, modelObject.exposedPerimeterFraction());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
