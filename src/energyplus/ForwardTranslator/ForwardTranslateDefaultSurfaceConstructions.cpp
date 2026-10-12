/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/DefaultSurfaceConstructions.hpp"

#include "../../model/ConstructionBase.hpp"

#include <utilities/idd/SurfaceConstructionAssignments_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDefaultSurfaceConstructions(model::DefaultSurfaceConstructions& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceConstructionAssignments, modelObject);

    // Floor Construction Name: Optional Object
    if (boost::optional<ConstructionBase> floorConstruction_ = modelObject.floorConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*floorConstruction_)) {
        idfObject.setString(SurfaceConstructionAssignmentsFields::FloorConstructionName, wo_->nameString());
      }
    }

    // Wall Construction Name: Optional Object
    if (boost::optional<ConstructionBase> wallConstruction_ = modelObject.wallConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*wallConstruction_)) {
        idfObject.setString(SurfaceConstructionAssignmentsFields::WallConstructionName, wo_->nameString());
      }
    }

    // Roof Ceiling Construction Name: Optional Object
    if (boost::optional<ConstructionBase> roofCeilingConstruction_ = modelObject.roofCeilingConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*roofCeilingConstruction_)) {
        idfObject.setString(SurfaceConstructionAssignmentsFields::RoofCeilingConstructionName, wo_->nameString());
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
