/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/DefaultSubSurfaceConstructions.hpp"

#include "../../model/ConstructionBase.hpp"

#include <utilities/idd/SubSurfaceConstructionAssignments_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDefaultSubSurfaceConstructions(model::DefaultSubSurfaceConstructions& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SubSurfaceConstructionAssignments, modelObject);

    // Fixed Window Construction Name: Optional Object
    if (boost::optional<ConstructionBase> fixedWindowConstruction_ = modelObject.fixedWindowConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*fixedWindowConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::FixedWindowConstructionName, wo_->nameString());
      }
    }

    // Operable Window Construction Name: Optional Object
    if (boost::optional<ConstructionBase> operableWindowConstruction_ = modelObject.operableWindowConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*operableWindowConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::OperableWindowConstructionName, wo_->nameString());
      }
    }

    // Door Construction Name: Optional Object
    if (boost::optional<ConstructionBase> doorConstruction_ = modelObject.doorConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*doorConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::DoorConstructionName, wo_->nameString());
      }
    }

    // Glass Door Construction Name: Optional Object
    if (boost::optional<ConstructionBase> glassDoorConstruction_ = modelObject.glassDoorConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*glassDoorConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::GlassDoorConstructionName, wo_->nameString());
      }
    }

    // Overhead Door Construction Name: Optional Object
    if (boost::optional<ConstructionBase> overheadDoorConstruction_ = modelObject.overheadDoorConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*overheadDoorConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::OverheadDoorConstructionName, wo_->nameString());
      }
    }

    // Skylight Construction Name: Optional Object
    if (boost::optional<ConstructionBase> skylightConstruction_ = modelObject.skylightConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*skylightConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::SkylightConstructionName, wo_->nameString());
      }
    }

    // Tubular Daylight Dome Construction Name: Optional Object
    if (boost::optional<ConstructionBase> tubularDaylightDomeConstruction_ = modelObject.tubularDaylightDomeConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*tubularDaylightDomeConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::TubularDaylightDomeConstructionName, wo_->nameString());
      }
    }

    // Tubular Daylight Diffuser Construction Name: Optional Object
    if (boost::optional<ConstructionBase> tubularDaylightDiffuserConstruction_ = modelObject.tubularDaylightDiffuserConstruction()) {
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(*tubularDaylightDiffuserConstruction_)) {
        idfObject.setString(SubSurfaceConstructionAssignmentsFields::TubularDaylightDiffuserConstructionName, wo_->nameString());
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
