/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <energyplus/GeometryTranslator.hpp>
#include <energyplus/MapFields.hpp>

#include <model/Building.hpp>
#include <model/Space.hpp>
#include <model/Surface.hpp>
#include <model/SubSurface.hpp>
#include <model/ShadingSurface.hpp>

#include <utilities/geometry/Geometry.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Containers.hpp>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/Output_IlluminanceMap_FieldEnums.hxx>
#include <utilities/idd/Wall_Detailed_FieldEnums.hxx>
#include <utilities/idd/RoofCeiling_Detailed_FieldEnums.hxx>
#include <utilities/idd/Floor_Detailed_FieldEnums.hxx>
#include <utilities/idd/Wall_Exterior_FieldEnums.hxx>
#include <utilities/idd/Wall_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/Wall_Underground_FieldEnums.hxx>
#include <utilities/idd/Wall_Interzone_FieldEnums.hxx>
#include <utilities/idd/Roof_FieldEnums.hxx>
#include <utilities/idd/Ceiling_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/Ceiling_Interzone_FieldEnums.hxx>
#include <utilities/idd/Floor_GroundContact_FieldEnums.hxx>
#include <utilities/idd/Floor_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/Floor_Interzone_FieldEnums.hxx>
#include <utilities/idd/Window_FieldEnums.hxx>
#include <utilities/idd/Door_FieldEnums.hxx>
#include <utilities/idd/GlazedDoor_FieldEnums.hxx>
#include <utilities/idd/Window_Interzone_FieldEnums.hxx>
#include <utilities/idd/Door_Interzone_FieldEnums.hxx>
#include <utilities/idd/GlazedDoor_Interzone_FieldEnums.hxx>
#include <utilities/idd/Shading_Site_FieldEnums.hxx>
#include <utilities/idd/Shading_Building_FieldEnums.hxx>
#include <utilities/idd/Shading_Overhang_FieldEnums.hxx>
#include <utilities/idd/Shading_Overhang_Projection_FieldEnums.hxx>
#include <utilities/idd/Shading_Fin_FieldEnums.hxx>
#include <utilities/idd/Shading_Fin_Projection_FieldEnums.hxx>
#include <utilities/idd/Shading_Site_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Building_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Zone_Detailed_FieldEnums.hxx>

#include <utilities/idf/WorkspaceObject.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

using openstudio::IddObjectType;
using openstudio::GlobalGeometryRulesFields;
using openstudio::OptionalString;
using openstudio::OptionalDouble;
using openstudio::WorkspaceObject;
using openstudio::degToRad;
using openstudio::model::Building;
using openstudio::model::Space;
using openstudio::model::Surface;
using openstudio::model::SubSurface;

namespace openstudio {
namespace energyplus {

  /// test equality of coordinate systems
  bool equal(const CoordinateSystem& left, const CoordinateSystem& right)
  {
    bool result = false;
    switch(left.value()){
      case CoordinateSystem::Absolute:
        result = (right == CoordinateSystem::Absolute || right == CoordinateSystem::World);
        break;
      case CoordinateSystem::World:
        result = (right == CoordinateSystem::Absolute || right == CoordinateSystem::World);
        break;
      case CoordinateSystem::Relative:
        result = (right == CoordinateSystem::Relative);
        break;
    }
    return result;
  }

  /// constructor with an EnergyPlus Workspace
  GeometryTranslator::GeometryTranslator(const openstudio::Workspace& workspace)
    : m_workspace(workspace)
  {}

  /// convert workspace to given coordinate systems
  /// all geometry will be converted to upper-left-corner, counterclockwise
  /// all simple geometry will be converted to detailed geometry
  bool GeometryTranslator::convert(const CoordinateSystem& detailedSystem, const CoordinateSystem& daylightingSystem)
  {
    bool result = true;

    // current geometry rules
    GlobalGeometryRules currentRules = globalGeometryRules();

    // convert to counter clockwise vertex entry direction
    if (currentRules.ved == VertexEntryDirection::Clockwise){
      result = result && reverseAllDetailedVertices();
    }

    // change from current detailed system to new detailed system
    CoordinateChange detailedCoordChange = CoordinateChange::NoChange;
    if (equal(currentRules.detailedSystem, CoordinateSystem::Relative) && equal(detailedSystem, CoordinateSystem::Absolute)){
      detailedCoordChange = CoordinateChange::RelativeToAbsolute;
    }else if(equal(currentRules.detailedSystem, CoordinateSystem::Absolute) && equal(detailedSystem, CoordinateSystem::Relative)){
      detailedCoordChange = CoordinateChange::AbsoluteToRelative;
    }

    // change from current daylighting system to new daylighting system
    CoordinateChange daylightingCoordChange = CoordinateChange::NoChange;
    if (equal(currentRules.daylightingSystem, CoordinateSystem::Relative) && equal(daylightingSystem, CoordinateSystem::Absolute)){
      daylightingCoordChange = CoordinateChange::RelativeToAbsolute;
    }else if(equal(currentRules.daylightingSystem, CoordinateSystem::Absolute) && equal(daylightingSystem, CoordinateSystem::Relative)){
      daylightingCoordChange = CoordinateChange::AbsoluteToRelative;
    }

    // change from current simple system to current detailed system
    CoordinateChange rectangularCoordChange = CoordinateChange::NoChange;
    if (equal(currentRules.rectangularSystem, CoordinateSystem::Relative) && equal(detailedSystem, CoordinateSystem::Absolute)){
      rectangularCoordChange = CoordinateChange::RelativeToAbsolute;
    }else if(equal(currentRules.rectangularSystem, CoordinateSystem::Absolute) && equal(detailedSystem, CoordinateSystem::Relative)){
      rectangularCoordChange = CoordinateChange::AbsoluteToRelative;
    }

    // transform detailed geometry from the current system to the new system
    // do first to avoid transforming new detailed surfaces from simple geom twice
    result = result && convertDetailedGeometry(detailedCoordChange);

    // transform daylighting geometry from the current system to the new system
    result = result && convertDaylightingGeometry(daylightingCoordChange);

    // convert simple surfaces to detailed in the current system
    result = result && convertSimpleSurfaces(rectangularCoordChange);

    // convert simple subsurfaces to detailed in the current system
    result = result && convertSimpleSubSurfaces();

    // convert simple shading to detailed in the current system
    result = result && convertSimpleShading(rectangularCoordChange);

    // apply starting vertex position rule
    result = result && applyUpperLeftCornerRule();

    // set GlobalGeometryRules to the new rules
    result = result && setGlobalGeometryRules(StartingVertexPosition::UpperLeftCorner,
                                              VertexEntryDirection::Counterclockwise,
                                              detailedSystem,
                                              daylightingSystem,
                                              detailedSystem);

    return result;
  }

  // get the current GlobalGeometryRules
  GeometryTranslator::GlobalGeometryRules GeometryTranslator::globalGeometryRules() const
  {
    GlobalGeometryRules result;

    // get the GlobalGeometryRules
    WorkspaceObjectVector objects = m_workspace.getObjectsByType(IddObjectType::GlobalGeometryRules);
    if (objects.size() != 1){
      LOG(Warn, "Could not find GlobalGeometryRules object, assuming defaults");
      result.svp = StartingVertexPosition::UpperLeftCorner;
      result.ved = VertexEntryDirection::Counterclockwise;
      result.detailedSystem = CoordinateSystem::Relative;
      result.daylightingSystem = CoordinateSystem::Relative;
      result.rectangularSystem = CoordinateSystem::Relative;
      return result;
    }
    WorkspaceObject globalGeometryRules = objects[0];

    // get current geometry rules
    try{
      OptionalString strSvp = globalGeometryRules.getString(GlobalGeometryRulesFields::StartingVertexPosition, true);
      OptionalString strVed = globalGeometryRules.getString(GlobalGeometryRulesFields::VertexEntryDirection, true);
      OptionalString strDetailedSystem = globalGeometryRules.getString(GlobalGeometryRulesFields::CoordinateSystem, true);
      OptionalString strDaylightingSystem = globalGeometryRules.getString(GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, true);
      OptionalString strRectangularSystem = globalGeometryRules.getString(GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, true);

      result.svp = StartingVertexPosition(*strSvp);
      result.ved = VertexEntryDirection(*strVed);
      result.detailedSystem = CoordinateSystem(*strDetailedSystem);
      result.daylightingSystem = CoordinateSystem(*strDaylightingSystem);
      result.rectangularSystem = CoordinateSystem(*strRectangularSystem);
    }catch(...){
      LOG(Error, "Could not read GlobalGeometryRules object");
    }

    return result;
  }

  // set the GlobalGeometryRules, only changes the object does not transform geometry
  bool GeometryTranslator::setGlobalGeometryRules(const StartingVertexPosition& svp, const VertexEntryDirection& ved,
                                                  const CoordinateSystem& detailedSystem,
                                                  const CoordinateSystem& daylightingSystem,
                                                  const CoordinateSystem& rectangularSystem)
  {
    bool result = true;

    // get the GlobalGeometryRules
    WorkspaceObjectVector objects = m_workspace.getObjectsByType(IddObjectType::GlobalGeometryRules);
    if (objects.empty()){
      m_workspace.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
      objects = m_workspace.getObjectsByType(IddObjectType::GlobalGeometryRules);
    }
    WorkspaceObject globalGeometryRules = objects[0];

    // after conversion, set GlobalGeometryRules
    result = result && globalGeometryRules.setString(GlobalGeometryRulesFields::StartingVertexPosition, svp.valueName());
    result = result && globalGeometryRules.setString(GlobalGeometryRulesFields::VertexEntryDirection, ved.valueName());
    result = result && globalGeometryRules.setString(GlobalGeometryRulesFields::CoordinateSystem, detailedSystem.valueName());
    result = result && globalGeometryRules.setString(GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, daylightingSystem.valueName());
    result = result && globalGeometryRules.setString(GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, rectangularSystem.valueName());

    return result;
  }

  // get the transformation from building to world
  Transformation GeometryTranslator::buildingTransformation() const
  {
    // get the Building
    WorkspaceObjectVector objects = m_workspace.getObjectsByType(IddObjectType::Building);
    if (objects.size() != 1){
      LOG(Warn, "Could not find Building object, assuming 0 rotation");
      return Transformation();
    }
    WorkspaceObject building = objects[0];

    OptionalDouble northAxis = building.getDouble(BuildingFields::NorthAxis, true);
    if (!northAxis){
       northAxis = 0;
       LOG(Warn, "North Axis unknown, using 0");
     }
     // rotate negative amount around the z axis, EnergyPlus defines rotation opposite to OpenStudio
     return Transformation::rotation(Vector3d(0,0,1), -degToRad(*northAxis));
  }

  // get the transformation from zone to building
  Transformation GeometryTranslator::zoneTransformation(const WorkspaceObject& zone) const
  {
    OS_ASSERT(zone.iddObject().type() == IddObjectType::Zone);

    OptionalDouble x = zone.getDouble(ZoneFields::XOrigin, true);
    OptionalDouble y = zone.getDouble(ZoneFields::YOrigin, true);
    OptionalDouble z = zone.getDouble(ZoneFields::ZOrigin, true);
    if (!x || !y || !z){
       x = 0; y = 0; z = 0;
       LOG(Error, "Zone origin unknown, using 0, 0, 0");
     }

    // translation
    Transformation translation = Transformation::translation(Vector3d(*x, *y, *z));

    OptionalDouble directionOfRelativeNorth = zone.getDouble(ZoneFields::DirectionofRelativeNorth, true);
    if (!directionOfRelativeNorth){
       directionOfRelativeNorth = 0;
       LOG(Error, "Zone direction of relative North unknown, using 0");
     }

    // rotate negative amount around the z axis, EnergyPlus defines rotation opposite to OpenStudio
    Transformation rotation = Transformation::rotation(Vector3d(0,0,1), -degToRad(*directionOfRelativeNorth));

    return translation*rotation;
  }

  // convert simple shading to detailed in the current system
  bool GeometryTranslator::convertSimpleShading(const CoordinateChange& coordChange)
  {
    // building transformation
    Transformation buildingTransformation = this->buildingTransformation();

    // Shading::Site
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Shading_Site)){

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Shading_SiteFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Shading_SiteFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Shading_SiteFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Shading_SiteFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Shading_SiteFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Shading_SiteFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Shading_SiteFields::Height, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get vertices
      Point3dVector vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *height);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Shading_SiteFields::Name, Shading_Site_DetailedFields::Name));

      // new object
      IdfObject newObject(IddObjectType::Shading_Site_Detailed);

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setDouble(Shading_Site_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::Shading_Site_Detailed);
        // set vertices
        setVertices(Shading_Site_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject.handle());
    }

    // Shading::Building
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Shading_Building)){

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        t = buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        t = buildingTransformation;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Shading_BuildingFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Shading_BuildingFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Shading_BuildingFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Shading_BuildingFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Shading_BuildingFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Shading_BuildingFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Shading_BuildingFields::Height, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get vertices
      Point3dVector vertices = verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *height);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Shading_BuildingFields::Name, Shading_Building_DetailedFields::Name));

      // new object
      IdfObject newObject(IddObjectType::Shading_Building_Detailed);

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setDouble(Shading_Building_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::Shading_Building_Detailed);
        // set vertices
        setVertices(Shading_Building_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject.handle());
    }

    // Shading:Overhang
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Shading_Overhang)){

      // find subSurface
      OptionalWorkspaceObject subSurface = oldObject.getTarget(Shading_OverhangFields::WindoworDoorName);
      if (!subSurface){
        LOG(Error, "Could not find subSurface");
        continue;
      }

      // find surface
      OptionalWorkspaceObject surface = subSurface->getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Point3dVector subSurfaceVerts = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, *subSurface);
      Transformation t = Transformation::alignFace(subSurfaceVerts);

      // figure out bounds of the subsurface in face coordinates
      OptionalDouble xMin;
      OptionalDouble xMax;
      OptionalDouble yMin;
      OptionalDouble yMax;
      BOOST_FOREACH(const Point3d& faceVert, t.inverse()*subSurfaceVerts){

        OS_ASSERT(abs(faceVert.z()) < 0.001);

        if (!xMin || (faceVert.x() < *xMin)){
          xMin = faceVert.x();
        }
        if (!xMax || (faceVert.x() > *xMax)){
          xMax = faceVert.x();
        }
        if (!yMin || (faceVert.y() < *yMin)){
          yMin = faceVert.y();
        }
        if (!yMax || (faceVert.y() > *yMax)){
          yMax = faceVert.y();
        }
      }
      if (!xMin || !xMax || !yMin || !yMax){
        LOG(Error, "Cannot determine sub surface bounds");
        continue;
      }

      // simple params
      OptionalDouble height = oldObject.getDouble(Shading_OverhangFields::HeightaboveWindoworDoor, true);
      OptionalDouble tilt = oldObject.getDouble(Shading_OverhangFields::TiltAnglefromWindow_Door, true);
      OptionalDouble leftOffset = oldObject.getDouble(Shading_OverhangFields::LeftextensionfromWindow_DoorWidth, true);
      OptionalDouble rightOffset = oldObject.getDouble(Shading_OverhangFields::RightextensionfromWindow_DoorWidth, true);
      OptionalDouble depth = oldObject.getDouble(Shading_OverhangFields::Depth, true);
      if (!height || !tilt || !leftOffset || !rightOffset || !depth){
        LOG(Error, "Missing required fields");
        continue;
      }

      if (*depth == 0){
        LOG(Error, "Zero depth surface not allowed");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*xMax + *rightOffset, *yMax + *height + *depth*cos(degToRad(*tilt)), *depth*sin(degToRad(*tilt)));
      faceVertices[1] = Point3d(*xMax + *rightOffset, *yMax + *height, 0.0);
      faceVertices[2] = Point3d(*xMin - *leftOffset, *yMax + *height, 0.0);
      faceVertices[3] = Point3d(*xMin - *leftOffset, *yMax + *height + *depth*cos(degToRad(*tilt)), *depth*sin(degToRad(*tilt)));

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::Shading_Zone_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Shading_OverhangFields::Name, Shading_Zone_DetailedFields::Name));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(Shading_Zone_DetailedFields::BaseSurfaceName, surface->name().get());
      newObject.setDouble(Shading_Zone_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject, false);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::Shading_Zone_Detailed);
        // set vertices
        setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject.handle());
    }

    // Shading:Overhang:Projection
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Shading_Overhang_Projection)){

      // find subSurface
      OptionalWorkspaceObject subSurface = oldObject.getTarget(Shading_Overhang_ProjectionFields::WindoworDoorName);
      if (!subSurface){
        LOG(Error, "Could not find subSurface");
        continue;
      }

      // find surface
      OptionalWorkspaceObject surface = subSurface->getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Point3dVector subSurfaceVerts = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, *subSurface);
      Transformation t = Transformation::alignFace(subSurfaceVerts);

      // figure out bounds of the subsurface in face coordinates
      OptionalDouble xMin;
      OptionalDouble xMax;
      OptionalDouble yMin;
      OptionalDouble yMax;
      BOOST_FOREACH(const Point3d& faceVert, t.inverse()*subSurfaceVerts){

        OS_ASSERT(abs(faceVert.z()) < 0.001);

        if (!xMin || (faceVert.x() < *xMin)){
          xMin = faceVert.x();
        }
        if (!xMax || (faceVert.x() > *xMax)){
          xMax = faceVert.x();
        }
        if (!yMin || (faceVert.y() < *yMin)){
          yMin = faceVert.y();
        }
        if (!yMax || (faceVert.y() > *yMax)){
          yMax = faceVert.y();
        }
      }
      if (!xMin || !xMax || !yMin || !yMax){
        LOG(Error, "Cannot determine sub surface bounds");
        continue;
      }

      // simple params
      OptionalDouble height = oldObject.getDouble(Shading_Overhang_ProjectionFields::HeightaboveWindoworDoor, true);
      OptionalDouble tilt = oldObject.getDouble(Shading_Overhang_ProjectionFields::TiltAnglefromWindow_Door, true);
      OptionalDouble leftOffset = oldObject.getDouble(Shading_Overhang_ProjectionFields::LeftextensionfromWindow_DoorWidth, true);
      OptionalDouble rightOffset = oldObject.getDouble(Shading_Overhang_ProjectionFields::RightextensionfromWindow_DoorWidth, true);
      OptionalDouble projectionFactor = oldObject.getDouble(Shading_Overhang_ProjectionFields::DepthasFractionofWindow_DoorHeight, true);
      if (!height || !tilt || !leftOffset || !rightOffset || !projectionFactor){
        LOG(Error, "Missing required fields");
        continue;
      }

      double depth = *projectionFactor*(*yMax-*yMin);

      if (depth == 0){
        LOG(Error, "Zero depth surface not allowed");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*xMax + *rightOffset, *yMax + *height + depth*cos(degToRad(*tilt)), depth*sin(degToRad(*tilt)));
      faceVertices[1] = Point3d(*xMax + *rightOffset, *yMax + *height, 0.0);
      faceVertices[2] = Point3d(*xMin - *leftOffset, *yMax + *height, 0.0);
      faceVertices[3] = Point3d(*xMin - *leftOffset, *yMax + *height + depth*cos(degToRad(*tilt)), depth*sin(degToRad(*tilt)));

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::Shading_Zone_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Shading_Overhang_ProjectionFields::Name, Shading_Zone_DetailedFields::Name));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(Shading_Zone_DetailedFields::BaseSurfaceName, surface->name().get());
      newObject.setDouble(Shading_Zone_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject, false);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::Shading_Zone_Detailed);
        // set vertices
        setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }
    }

    // Shading:Fin
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Shading_Fin)){

      // find subSurface
      OptionalWorkspaceObject subSurface = oldObject.getTarget(Shading_FinFields::WindoworDoorName);
      if (!subSurface){
        LOG(Error, "Could not find subSurface");
        continue;
      }

      // find surface
      OptionalWorkspaceObject surface = subSurface->getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Point3dVector subSurfaceVerts = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, *subSurface);
      Transformation t = Transformation::alignFace(subSurfaceVerts);

      // figure out bounds of the subsurface in face coordinates
      OptionalDouble xMin;
      OptionalDouble xMax;
      OptionalDouble yMin;
      OptionalDouble yMax;
      BOOST_FOREACH(const Point3d& faceVert, t.inverse()*subSurfaceVerts){

        OS_ASSERT(abs(faceVert.z()) < 0.001);

        if (!xMin || (faceVert.x() < *xMin)){
          xMin = faceVert.x();
        }
        if (!xMax || (faceVert.x() > *xMax)){
          xMax = faceVert.x();
        }
        if (!yMin || (faceVert.y() < *yMin)){
          yMin = faceVert.y();
        }
        if (!yMax || (faceVert.y() > *yMax)){
          yMax = faceVert.y();
        }
      }
      if (!xMin || !xMax || !yMin || !yMax){
        LOG(Error, "Cannot determine sub surface bounds");
        continue;
      }

      // simple params
      OptionalDouble leftOffset = oldObject.getDouble(Shading_FinFields::LeftExtensionfromWindow_Door, true);
      OptionalDouble leftAbove = oldObject.getDouble(Shading_FinFields::LeftDistanceAboveTopofWindow, true);
      OptionalDouble leftBelow = oldObject.getDouble(Shading_FinFields::LeftDistanceBelowBottomofWindow, true);
      OptionalDouble leftTilt = oldObject.getDouble(Shading_FinFields::LeftTiltAnglefromWindow_Door, true);
      OptionalDouble leftDepth = oldObject.getDouble(Shading_FinFields::LeftDepth, true);
      OptionalDouble rightOffset = oldObject.getDouble(Shading_FinFields::RightExtensionfromWindow_Door, true);
      OptionalDouble rightAbove = oldObject.getDouble(Shading_FinFields::RightDistanceAboveTopofWindow, true);
      OptionalDouble rightBelow = oldObject.getDouble(Shading_FinFields::RightDistanceBelowBottomofWindow, true);
      OptionalDouble rightTilt = oldObject.getDouble(Shading_FinFields::RightTiltAnglefromWindow_Door, true);
      OptionalDouble rightDepth = oldObject.getDouble(Shading_FinFields::RightDepth, true);

      if (!leftOffset || !leftAbove || !leftBelow || !leftTilt || !leftDepth || !rightOffset || !rightAbove || !rightBelow || !rightTilt || !rightDepth){
        LOG(Error, "Missing required fields");
        continue;
      }

      if (*leftDepth == 0 || *rightDepth == 0){
        LOG(Error, "Zero depth surface not allowed");
        continue;
      }

      // left vertices
      openstudio::Point3dVector leftVertices(4);
      leftVertices[0] = Point3d(*xMin - *leftOffset + *leftDepth*cos(degToRad(*leftTilt)), *yMax + *leftAbove, *leftDepth*sin(degToRad(*leftTilt)));
      leftVertices[1] = Point3d(*xMin - *leftOffset + *leftDepth*cos(degToRad(*leftTilt)), *yMin - *leftBelow, *leftDepth*sin(degToRad(*leftTilt)));
      leftVertices[2] = Point3d(*xMin - *leftOffset, *yMin - *leftBelow, 0.0);
      leftVertices[3] = Point3d(*xMin - *leftOffset, *yMax + *leftAbove, 0.0);

      // right vertices
      openstudio::Point3dVector rightVertices(4);
      rightVertices[0] = Point3d(*xMax + *rightOffset + *rightDepth*cos(degToRad(*rightTilt)), *yMax + *rightAbove, *rightDepth*sin(degToRad(*rightTilt)));
      rightVertices[1] = Point3d(*xMax + *rightOffset + *rightDepth*cos(degToRad(*rightTilt)), *yMin - *rightBelow, *rightDepth*sin(degToRad(*rightTilt)));
      rightVertices[2] = Point3d(*xMax + *rightOffset, *yMin - *rightBelow, 0.0);
      rightVertices[3] = Point3d(*xMax + *rightOffset, *yMax + *rightAbove, 0.0);

      // transform vertices
      leftVertices = t*leftVertices;
      rightVertices = t*rightVertices;

      // add two new objects
      IdfObject leftObject(IddObjectType::Shading_Zone_Detailed);
      IdfObject rightObject(IddObjectType::Shading_Zone_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;

      // copy fields over directly
      mapFields(oldObject, leftObject, fieldMap);
      mapFields(oldObject, rightObject, fieldMap);

      // set other fields
      leftObject.setString(Shading_Zone_DetailedFields::Name, oldObject.name().get() + " Left");
      leftObject.setString(Shading_Zone_DetailedFields::BaseSurfaceName, surface->name().get());
      leftObject.setDouble(Shading_Zone_DetailedFields::NumberofVertices, 4);
      rightObject.setString(Shading_Zone_DetailedFields::Name, oldObject.name().get() + " Right");
      rightObject.setString(Shading_Zone_DetailedFields::BaseSurfaceName, surface->name().get());
      rightObject.setDouble(Shading_Zone_DetailedFields::NumberofVertices, 4);

      // swap old object with left one
      bool ok = m_workspace.swap(oldObject, leftObject, false);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::Shading_Zone_Detailed);
        // set vertices
        setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, oldObject, leftVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // add right object
      OptionalWorkspaceObject w = m_workspace.addObject(rightObject);
      if (w){
        setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, *w, rightVertices);
      }else{
        LOG(Error, "Workspace object add failed");
        continue;
      }
    }

    // Shading:Fin:Projection
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Shading_Fin_Projection)){

      // find subSurface
      OptionalWorkspaceObject subSurface = oldObject.getTarget(Shading_Fin_ProjectionFields::WindoworDoorName);
      if (!subSurface){
        LOG(Error, "Could not find subSurface");
        continue;
      }

      // find surface
      OptionalWorkspaceObject surface = subSurface->getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Point3dVector subSurfaceVerts = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, *subSurface);
      Transformation t = Transformation::alignFace(subSurfaceVerts);

      // figure out bounds of the subsurface in face coordinates
      OptionalDouble xMin;
      OptionalDouble xMax;
      OptionalDouble yMin;
      OptionalDouble yMax;
      BOOST_FOREACH(const Point3d& faceVert, t.inverse()*subSurfaceVerts){

        OS_ASSERT(abs(faceVert.z()) < 0.001);

        if (!xMin || (faceVert.x() < *xMin)){
          xMin = faceVert.x();
        }
        if (!xMax || (faceVert.x() > *xMax)){
          xMax = faceVert.x();
        }
        if (!yMin || (faceVert.y() < *yMin)){
          yMin = faceVert.y();
        }
        if (!yMax || (faceVert.y() > *yMax)){
          yMax = faceVert.y();
        }
      }
      if (!xMin || !xMax || !yMin || !yMax){
        LOG(Error, "Cannot determine sub surface bounds");
        continue;
      }

      // simple params
      OptionalDouble leftOffset = oldObject.getDouble(Shading_Fin_ProjectionFields::LeftExtensionfromWindow_Door, true);
      OptionalDouble leftAbove = oldObject.getDouble(Shading_Fin_ProjectionFields::LeftDistanceAboveTopofWindow, true);
      OptionalDouble leftBelow = oldObject.getDouble(Shading_Fin_ProjectionFields::LeftDistanceBelowBottomofWindow, true);
      OptionalDouble leftTilt = oldObject.getDouble(Shading_Fin_ProjectionFields::LeftTiltAnglefromWindow_Door, true);
      OptionalDouble leftProjection = oldObject.getDouble(Shading_Fin_ProjectionFields::LeftDepthasFractionofWindow_DoorWidth, true);
      OptionalDouble rightOffset = oldObject.getDouble(Shading_Fin_ProjectionFields::RightExtensionfromWindow_Door, true);
      OptionalDouble rightAbove = oldObject.getDouble(Shading_Fin_ProjectionFields::RightDistanceAboveTopofWindow, true);
      OptionalDouble rightBelow = oldObject.getDouble(Shading_Fin_ProjectionFields::RightDistanceBelowBottomofWindow, true);
      OptionalDouble rightTilt = oldObject.getDouble(Shading_Fin_ProjectionFields::RightTiltAnglefromWindow_Door, true);
      OptionalDouble rightProjection = oldObject.getDouble(Shading_Fin_ProjectionFields::RightDepthasFractionofWindow_DoorWidth, true);

      if (!leftOffset || !leftAbove || !leftBelow || !leftTilt || !leftProjection || !rightOffset || !rightAbove || !rightBelow || !rightTilt || !rightProjection){
        LOG(Error, "Missing required fields");
        continue;
      }

      double leftDepth = *leftProjection*(*yMax-*yMin);
      double rightDepth = *rightProjection*(*yMax-*yMin);

      if (leftDepth == 0 || rightDepth == 0){
        LOG(Error, "Zero depth surface not allowed");
        continue;
      }

      // left vertices
      openstudio::Point3dVector leftVertices(4);
      leftVertices[0] = Point3d(*xMin - *leftOffset + leftDepth*cos(degToRad(*leftTilt)), *yMax + *leftAbove, leftDepth*sin(degToRad(*leftTilt)));
      leftVertices[1] = Point3d(*xMin - *leftOffset + leftDepth*cos(degToRad(*leftTilt)), *yMin - *leftBelow, leftDepth*sin(degToRad(*leftTilt)));
      leftVertices[2] = Point3d(*xMin - *leftOffset, *yMin - *leftBelow, 0.0);
      leftVertices[3] = Point3d(*xMin - *leftOffset, *yMax + *leftAbove, 0.0);

      // right vertices
      openstudio::Point3dVector rightVertices(4);
      rightVertices[0] = Point3d(*xMax + *rightOffset + rightDepth*cos(degToRad(*rightTilt)), *yMax + *rightAbove, rightDepth*sin(degToRad(*rightTilt)));
      rightVertices[1] = Point3d(*xMax + *rightOffset + rightDepth*cos(degToRad(*rightTilt)), *yMin - *rightBelow, rightDepth*sin(degToRad(*rightTilt)));
      rightVertices[2] = Point3d(*xMax + *rightOffset, *yMin - *rightBelow, 0.0);
      rightVertices[3] = Point3d(*xMax + *rightOffset, *yMax + *rightAbove, 0.0);

      // transform vertices
      leftVertices = t*leftVertices;
      rightVertices = t*rightVertices;

      // add two new objects
      IdfObject leftObject(IddObjectType::Shading_Zone_Detailed);
      IdfObject rightObject(IddObjectType::Shading_Zone_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;

      // copy fields over directly
      mapFields(oldObject, leftObject, fieldMap);
      mapFields(oldObject, rightObject, fieldMap);

      // set other fields
      leftObject.setString(Shading_Zone_DetailedFields::Name, oldObject.name().get() + " Left");
      leftObject.setString(Shading_Zone_DetailedFields::BaseSurfaceName, surface->name().get());
      leftObject.setDouble(Shading_Zone_DetailedFields::NumberofVertices, 4);
      rightObject.setString(Shading_Zone_DetailedFields::Name, oldObject.name().get() + " Right");
      rightObject.setString(Shading_Zone_DetailedFields::BaseSurfaceName, surface->name().get());
      rightObject.setDouble(Shading_Zone_DetailedFields::NumberofVertices, 4);

      // swap old object with left one
      bool ok = m_workspace.swap(oldObject, leftObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::Shading_Zone_Detailed);
        // set vertices
        setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, oldObject, leftVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // add right object
      OptionalWorkspaceObject w = m_workspace.addObject(rightObject);
      OptionalWorkspaceObject workspaceObject;
      if (w){
        workspaceObject = m_workspace.getObject(w->handle ());
        if (workspaceObject){
          // set vertices
          setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, *w, rightVertices);
        }else{
          LOG(Error, "Workspace object add failed");
          continue;
        }
      }else{
        LOG(Error, "Workspace object add failed");
        continue;
      }
    }

    return true;
  }

  // convert simple subsurfaces to detailed in the current system
  bool GeometryTranslator::convertSimpleSubSurfaces()
  {
    // Window
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Window)){

      // find surface
      OptionalWorkspaceObject surface = oldObject.getTarget(WindowFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Transformation t = Transformation::alignFace(getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, *surface));

      // simple params
      OptionalDouble x0 = oldObject.getDouble(WindowFields::StartingXCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(WindowFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(WindowFields::Length, true);
      OptionalDouble height = oldObject.getDouble(WindowFields::Height, true);
      if (!x0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*x0, *z0 + *height, 0.0);
      faceVertices[1] = Point3d(*x0, *z0, 0.0);
      faceVertices[2] = Point3d(*x0 + *length, *z0, 0.0);
      faceVertices[3] = Point3d(*x0 + *length, *z0 + *height, 0.0);

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::FenestrationSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(WindowFields::Name, FenestrationSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(WindowFields::ConstructionName, FenestrationSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(WindowFields::BuildingSurfaceName, FenestrationSurface_DetailedFields::BuildingSurfaceName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(WindowFields::ShadingControlName, FenestrationSurface_DetailedFields::ShadingControlName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(WindowFields::FrameandDividerName, FenestrationSurface_DetailedFields::FrameandDividerName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(WindowFields::Multiplier, FenestrationSurface_DetailedFields::Multiplier));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(FenestrationSurface_DetailedFields::SurfaceType, "Window");
      newObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(FenestrationSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(FenestrationSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::FenestrationSurface_Detailed);
        // set vertices
        setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }
    }

    // Door
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Door)){

      // find surface
      OptionalWorkspaceObject surface = oldObject.getTarget(DoorFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Transformation t = Transformation::alignFace(getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, *surface));

      // simple params
      OptionalDouble x0 = oldObject.getDouble(DoorFields::StartingXCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(DoorFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(DoorFields::Length, true);
      OptionalDouble height = oldObject.getDouble(DoorFields::Height, true);
      if (!x0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*x0, *z0 + *height, 0.0);
      faceVertices[1] = Point3d(*x0, *z0, 0.0);
      faceVertices[2] = Point3d(*x0 + *length, *z0, 0.0);
      faceVertices[3] = Point3d(*x0 + *length, *z0 + *height, 0.0);

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::FenestrationSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(DoorFields::Name, FenestrationSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(DoorFields::ConstructionName, FenestrationSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(DoorFields::BuildingSurfaceName, FenestrationSurface_DetailedFields::BuildingSurfaceName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(DoorFields::Multiplier, FenestrationSurface_DetailedFields::Multiplier));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(FenestrationSurface_DetailedFields::SurfaceType, "Door");
      newObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(FenestrationSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(FenestrationSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::FenestrationSurface_Detailed);
        // set vertices
        setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }
    }

    // GlazedDoor
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::GlazedDoor)){

      // find surface
      OptionalWorkspaceObject surface = oldObject.getTarget(GlazedDoorFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Transformation t = Transformation::alignFace(getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, *surface));

      // simple params
      OptionalDouble x0 = oldObject.getDouble(GlazedDoorFields::StartingXCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(GlazedDoorFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(GlazedDoorFields::Length, true);
      OptionalDouble height = oldObject.getDouble(GlazedDoorFields::Height, true);
      if (!x0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*x0, *z0 + *height, 0.0);
      faceVertices[1] = Point3d(*x0, *z0, 0.0);
      faceVertices[2] = Point3d(*x0 + *length, *z0, 0.0);
      faceVertices[3] = Point3d(*x0 + *length, *z0 + *height, 0.0);

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::FenestrationSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoorFields::Name, FenestrationSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoorFields::ConstructionName, FenestrationSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoorFields::BuildingSurfaceName, FenestrationSurface_DetailedFields::BuildingSurfaceName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoorFields::ShadingControlName, FenestrationSurface_DetailedFields::ShadingControlName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoorFields::FrameandDividerName, FenestrationSurface_DetailedFields::FrameandDividerName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoorFields::Multiplier, FenestrationSurface_DetailedFields::Multiplier));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(FenestrationSurface_DetailedFields::SurfaceType, "GlassDoor");
      newObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(FenestrationSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(FenestrationSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::FenestrationSurface_Detailed);
        // set vertices
        setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }
    }

    // Window:Interzone
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Window_Interzone)){

      // find surface
      OptionalWorkspaceObject surface = oldObject.getTarget(Window_InterzoneFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Transformation t = Transformation::alignFace(getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, *surface));

      // simple params
      OptionalDouble x0 = oldObject.getDouble(Window_InterzoneFields::StartingXCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Window_InterzoneFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Window_InterzoneFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Window_InterzoneFields::Height, true);
      if (!x0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*x0, *z0 + *height, 0.0);
      faceVertices[1] = Point3d(*x0, *z0, 0.0);
      faceVertices[2] = Point3d(*x0 + *length, *z0, 0.0);
      faceVertices[3] = Point3d(*x0 + *length, *z0 + *height, 0.0);

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::FenestrationSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Window_InterzoneFields::Name, FenestrationSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Window_InterzoneFields::ConstructionName, FenestrationSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Window_InterzoneFields::BuildingSurfaceName, FenestrationSurface_DetailedFields::BuildingSurfaceName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Window_InterzoneFields::OutsideBoundaryConditionObject, FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Window_InterzoneFields::Multiplier, FenestrationSurface_DetailedFields::Multiplier));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(FenestrationSurface_DetailedFields::SurfaceType, "Window");
      newObject.setString(FenestrationSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(FenestrationSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::FenestrationSurface_Detailed);
        // set vertices
        setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }
    }

    // Door:Interzone
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Door_Interzone)){

      // find surface
      OptionalWorkspaceObject surface = oldObject.getTarget(Door_InterzoneFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Transformation t = Transformation::alignFace(getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, *surface));

      // simple params
      OptionalDouble x0 = oldObject.getDouble(Door_InterzoneFields::StartingXCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Door_InterzoneFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Door_InterzoneFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Door_InterzoneFields::Height, true);
      if (!x0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*x0, *z0 + *height, 0.0);
      faceVertices[1] = Point3d(*x0, *z0, 0.0);
      faceVertices[2] = Point3d(*x0 + *length, *z0, 0.0);
      faceVertices[3] = Point3d(*x0 + *length, *z0 + *height, 0.0);

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::FenestrationSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Door_InterzoneFields::Name, FenestrationSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Door_InterzoneFields::ConstructionName, FenestrationSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Door_InterzoneFields::BuildingSurfaceName, FenestrationSurface_DetailedFields::BuildingSurfaceName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Door_InterzoneFields::OutsideBoundaryConditionObject, FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Door_InterzoneFields::Multiplier, FenestrationSurface_DetailedFields::Multiplier));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(FenestrationSurface_DetailedFields::SurfaceType, "Door");
      newObject.setString(FenestrationSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(FenestrationSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::FenestrationSurface_Detailed);
        // set vertices
        setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }
    }

    // GlazedDoor:Interzone
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::GlazedDoor_Interzone)){

      // find surface
      OptionalWorkspaceObject surface = oldObject.getTarget(GlazedDoor_InterzoneFields::BuildingSurfaceName);
      if (!surface){
        LOG(Error, "Could not find surface");
        continue;
      }

      // get transformation from face coordinates to zone
      Transformation t = Transformation::alignFace(getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, *surface));

      // simple params
      OptionalDouble x0 = oldObject.getDouble(GlazedDoor_InterzoneFields::StartingXCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(GlazedDoor_InterzoneFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(GlazedDoor_InterzoneFields::Length, true);
      OptionalDouble height = oldObject.getDouble(GlazedDoor_InterzoneFields::Height, true);
      if (!x0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // vertices
      openstudio::Point3dVector faceVertices(4);
      faceVertices[0] = Point3d(*x0, *z0 + *height, 0.0);
      faceVertices[1] = Point3d(*x0, *z0, 0.0);
      faceVertices[2] = Point3d(*x0 + *length, *z0, 0.0);
      faceVertices[3] = Point3d(*x0 + *length, *z0 + *height, 0.0);

      // transform vertices
      faceVertices = t*faceVertices;

      // add new object
      IdfObject newObject(IddObjectType::FenestrationSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoor_InterzoneFields::Name, FenestrationSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoor_InterzoneFields::ConstructionName, FenestrationSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoor_InterzoneFields::BuildingSurfaceName, FenestrationSurface_DetailedFields::BuildingSurfaceName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoor_InterzoneFields::OutsideBoundaryConditionObject, FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject));
      fieldMap.push_back(std::pair<unsigned, unsigned>(GlazedDoor_InterzoneFields::Multiplier, FenestrationSurface_DetailedFields::Multiplier));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(FenestrationSurface_DetailedFields::SurfaceType, "GlassDoor");
      newObject.setString(FenestrationSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(FenestrationSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::FenestrationSurface_Detailed);
        // set vertices
        setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, oldObject, faceVertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }
    }
    return true;
  }

  // convert simple surfaces to detailed in the current system
  bool GeometryTranslator::convertSimpleSurfaces(const CoordinateChange& coordChange)
  {

    // building transformation
    Transformation buildingTransformation = this->buildingTransformation();

    // Wall::Detailed
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Wall_Detailed)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Wall_DetailedFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*getVertices(Wall_DetailedFields::NumberofVertices + 1, oldObject);

      // new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::OutsideBoundaryCondition, BuildingSurface_DetailedFields::OutsideBoundaryCondition));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::OutsideBoundaryConditionObject, BuildingSurface_DetailedFields::OutsideBoundaryConditionObject));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::SunExposure, BuildingSurface_DetailedFields::SunExposure));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::WindExposure, BuildingSurface_DetailedFields::WindExposure));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::ViewFactortoGround, BuildingSurface_DetailedFields::ViewFactortoGround));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_DetailedFields::NumberofVertices, BuildingSurface_DetailedFields::NumberofVertices));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // RoofCeiling::Detailed
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::RoofCeiling_Detailed)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Wall_DetailedFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*getVertices(RoofCeiling_DetailedFields::NumberofVertices + 1, oldObject);

      // new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::OutsideBoundaryCondition, BuildingSurface_DetailedFields::OutsideBoundaryCondition));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::OutsideBoundaryConditionObject, BuildingSurface_DetailedFields::OutsideBoundaryConditionObject));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::SunExposure, BuildingSurface_DetailedFields::SunExposure));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::WindExposure, BuildingSurface_DetailedFields::WindExposure));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::ViewFactortoGround, BuildingSurface_DetailedFields::ViewFactortoGround));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofCeiling_DetailedFields::NumberofVertices, BuildingSurface_DetailedFields::NumberofVertices));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Roof");

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Floor::Detailed
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Floor_Detailed)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Floor_DetailedFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*getVertices(Floor_DetailedFields::NumberofVertices + 1, oldObject);

      // new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::OutsideBoundaryCondition, BuildingSurface_DetailedFields::OutsideBoundaryCondition));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::OutsideBoundaryConditionObject, BuildingSurface_DetailedFields::OutsideBoundaryConditionObject));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::SunExposure, BuildingSurface_DetailedFields::SunExposure));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::WindExposure, BuildingSurface_DetailedFields::WindExposure));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::ViewFactortoGround, BuildingSurface_DetailedFields::ViewFactortoGround));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_DetailedFields::NumberofVertices, BuildingSurface_DetailedFields::NumberofVertices));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Floor");

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Wall:Exterior
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Wall_Exterior)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Wall_ExteriorFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Wall_ExteriorFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Wall_ExteriorFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Wall_ExteriorFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Wall_ExteriorFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Wall_ExteriorFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Wall_ExteriorFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Wall_ExteriorFields::Height, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *height);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_ExteriorFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_ExteriorFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_ExteriorFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Wall:Adiabatic
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Wall_Adiabatic)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Wall_AdiabaticFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Wall_AdiabaticFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Wall_AdiabaticFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Wall_AdiabaticFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Wall_AdiabaticFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Wall_AdiabaticFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Wall_AdiabaticFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Wall_AdiabaticFields::Height, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *height);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_AdiabaticFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_AdiabaticFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_AdiabaticFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Adiabatic");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Wall:Underground
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Wall_Underground)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Wall_UndergroundFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Wall_UndergroundFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Wall_UndergroundFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Wall_UndergroundFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Wall_UndergroundFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Wall_UndergroundFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Wall_UndergroundFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Wall_UndergroundFields::Height, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *height);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_UndergroundFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_UndergroundFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_UndergroundFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Ground");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Wall:Interzone
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Wall_Interzone)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Wall_InterzoneFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Wall_InterzoneFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Wall_InterzoneFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Wall_InterzoneFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Wall_InterzoneFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Wall_InterzoneFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Wall_InterzoneFields::Length, true);
      OptionalDouble height = oldObject.getDouble(Wall_InterzoneFields::Height, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !height){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *height);

      // new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_InterzoneFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_InterzoneFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_InterzoneFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Wall_InterzoneFields::OutsideBoundaryConditionObject, BuildingSurface_DetailedFields::OutsideBoundaryConditionObject));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Zone");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Roof
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Roof)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(RoofFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(RoofFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(RoofFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(RoofFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(RoofFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(RoofFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(RoofFields::Length, true);
      OptionalDouble width = oldObject.getDouble(RoofFields::Width, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !width){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *width);


      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(RoofFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Roof");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Ceiling:Adiabatic
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Ceiling_Adiabatic)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Ceiling_AdiabaticFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Ceiling_AdiabaticFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Ceiling_AdiabaticFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Ceiling_AdiabaticFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Ceiling_AdiabaticFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Ceiling_AdiabaticFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Ceiling_AdiabaticFields::Length, true);
      OptionalDouble width = oldObject.getDouble(Ceiling_AdiabaticFields::Width, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !width){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *width);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Ceiling_AdiabaticFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Ceiling_AdiabaticFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Ceiling_AdiabaticFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Ceiling");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Adiabatic");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Ceiling:Interzone
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Ceiling_Interzone)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Ceiling_InterzoneFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Ceiling_InterzoneFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Ceiling_InterzoneFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Ceiling_InterzoneFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Ceiling_InterzoneFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Ceiling_InterzoneFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Ceiling_InterzoneFields::Length, true);
      OptionalDouble width = oldObject.getDouble(Ceiling_InterzoneFields::Width, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !width){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *width);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Ceiling_InterzoneFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Ceiling_InterzoneFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Ceiling_InterzoneFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Ceiling_InterzoneFields::OutsideBoundaryConditionObject, BuildingSurface_DetailedFields::OutsideBoundaryConditionObject));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Ceiling");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Zone");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Floor:GroundContact
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Floor_GroundContact)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Floor_GroundContactFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Floor_GroundContactFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Floor_GroundContactFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Floor_GroundContactFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Floor_GroundContactFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Floor_GroundContactFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Floor_GroundContactFields::Length, true);
      OptionalDouble width = oldObject.getDouble(Floor_GroundContactFields::Width, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !width){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *width);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_GroundContactFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_GroundContactFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_GroundContactFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Floor");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Ground");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Floor:Adiabatic
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Floor_Adiabatic)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Floor_AdiabaticFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Floor_AdiabaticFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Floor_AdiabaticFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Floor_AdiabaticFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Floor_AdiabaticFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Floor_AdiabaticFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Floor_AdiabaticFields::Length, true);
      OptionalDouble width = oldObject.getDouble(Floor_AdiabaticFields::Width, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !width){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *width);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_AdiabaticFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_AdiabaticFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_AdiabaticFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Floor");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Adiabatic");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    // Floor:Interzone
    BOOST_FOREACH(WorkspaceObject oldObject, m_workspace.getObjectsByType(IddObjectType::Floor_Interzone)){

      // find zone
      OptionalWorkspaceObject zone = oldObject.getTarget(Floor_InterzoneFields::ZoneName);
      if (!zone){
        LOG(Error, "Could not find zone");
        continue;
      }

      // get simple params
      OptionalDouble azimuth = oldObject.getDouble(Floor_InterzoneFields::AzimuthAngle, true);
      OptionalDouble tilt = oldObject.getDouble(Floor_InterzoneFields::TiltAngle, true);
      OptionalDouble x0 = oldObject.getDouble(Floor_InterzoneFields::StartingXCoordinate, true);
      OptionalDouble y0 = oldObject.getDouble(Floor_InterzoneFields::StartingYCoordinate, true);
      OptionalDouble z0 = oldObject.getDouble(Floor_InterzoneFields::StartingZCoordinate, true);
      OptionalDouble length = oldObject.getDouble(Floor_InterzoneFields::Length, true);
      OptionalDouble width = oldObject.getDouble(Floor_InterzoneFields::Width, true);

      if (!azimuth || !tilt || !x0 || !y0 || !z0 || !length || !width){
        LOG(Error, "Missing required fields");
        continue;
      }

      // get transformation
      Transformation t;
      if (coordChange == CoordinateChange::AbsoluteToRelative){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = zoneTransformation.inverse()*buildingTransformation.inverse();
      }else if(coordChange == CoordinateChange::RelativeToAbsolute){
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        t = buildingTransformation*zoneTransformation;
      }

      // get vertices
      Point3dVector vertices = t*verticesForAzimuthTiltXYZLengthWidthOrHeight(*azimuth, *tilt, *x0, *y0, *z0, *length, *width);

      // add new object
      IdfObject newObject(IddObjectType::BuildingSurface_Detailed);

      // fields which map directly
      std::vector<std::pair<unsigned, unsigned> > fieldMap;
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_InterzoneFields::Name, BuildingSurface_DetailedFields::Name));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_InterzoneFields::ConstructionName, BuildingSurface_DetailedFields::ConstructionName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_InterzoneFields::ZoneName, BuildingSurface_DetailedFields::ZoneName));
      fieldMap.push_back(std::pair<unsigned, unsigned>(Floor_InterzoneFields::OutsideBoundaryConditionObject, BuildingSurface_DetailedFields::OutsideBoundaryConditionObject));

      // copy fields over directly
      mapFields(oldObject, newObject, fieldMap);

      // set other fields
      newObject.setString(BuildingSurface_DetailedFields::SurfaceType, "Floor");
      newObject.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Zone");
      newObject.setString(BuildingSurface_DetailedFields::SunExposure, "NoSun");
      newObject.setString(BuildingSurface_DetailedFields::WindExposure, "NoWind");
      newObject.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
      newObject.setDouble(BuildingSurface_DetailedFields::NumberofVertices, 4);

      // swap old object with new one
      bool ok = m_workspace.swap(oldObject, newObject);
      if (ok){
        OS_ASSERT(oldObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
        // set vertices
        setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, oldObject, vertices);
      }else{
        LOG(Error, "Workspace object swap failed");
        continue;
      }

      // remove old object
      //m_workspace.removeObject(oldObject);
    }

    return true;
  }

  // transform daylighting geometry from the current system to the new system
  bool GeometryTranslator::convertDaylightingGeometry(const CoordinateChange& daylightingCoordChange)
  {
    bool result = true;

    Transformation buildingTransformation = this->buildingTransformation();

    // daylighting controls
    BOOST_FOREACH(WorkspaceObject daylightingControl, m_workspace.getObjectsByType(IddObjectType::Daylighting_Controls)){
      if (daylightingCoordChange != CoordinateChange::NoChange){
        OptionalWorkspaceObject zone = daylightingControl.getTarget(Daylighting_ControlsFields::ZoneName);
        if (!zone){
          LOG(Error, "Could not find zone");
          continue;
        }

        Transformation t;
        if (daylightingCoordChange == CoordinateChange::AbsoluteToRelative){
          Transformation zoneTransformation = this->zoneTransformation(*zone);
          t = zoneTransformation.inverse()*buildingTransformation.inverse();
        }else if(daylightingCoordChange == CoordinateChange::RelativeToAbsolute){
          Transformation zoneTransformation = this->zoneTransformation(*zone);
          t = buildingTransformation*zoneTransformation;
        }

        // transform sensor points
        OptionalDouble x = daylightingControl.getDouble(Daylighting_ControlsFields::XCoordinateofFirstReferencePoint, true);
        OptionalDouble y = daylightingControl.getDouble(Daylighting_ControlsFields::YCoordinateofFirstReferencePoint, true);
        OptionalDouble z = daylightingControl.getDouble(Daylighting_ControlsFields::ZCoordinateofFirstReferencePoint, true);
        if (x && y && z){
          Point3d point(*x, *y, *z);
          point = t*point;
          result = result && daylightingControl.setDouble(Daylighting_ControlsFields::XCoordinateofFirstReferencePoint, point.x());
          result = result && daylightingControl.setDouble(Daylighting_ControlsFields::YCoordinateofFirstReferencePoint, point.y());
          result = result && daylightingControl.setDouble(Daylighting_ControlsFields::ZCoordinateofFirstReferencePoint, point.z());
        }

        x = daylightingControl.getDouble(Daylighting_ControlsFields::XCoordinateofSecondReferencePoint, true);
        y = daylightingControl.getDouble(Daylighting_ControlsFields::YCoordinateofSecondReferencePoint, true);
        z = daylightingControl.getDouble(Daylighting_ControlsFields::ZCoordinateofSecondReferencePoint, true);
        if (x && y && z){
          Point3d point(*x, *y, *z);
          point = t*point;
          result = result && daylightingControl.setDouble(Daylighting_ControlsFields::XCoordinateofSecondReferencePoint, point.x());
          result = result && daylightingControl.setDouble(Daylighting_ControlsFields::YCoordinateofSecondReferencePoint, point.y());
          result = result && daylightingControl.setDouble(Daylighting_ControlsFields::ZCoordinateofSecondReferencePoint, point.z());
        }

        // todo: transform glare angle

      }
    }

    // output illuminance map
    BOOST_FOREACH(WorkspaceObject illuminanceMap, m_workspace.getObjectsByType(IddObjectType::Output_IlluminanceMap)){
      if (daylightingCoordChange != CoordinateChange::NoChange){
        OptionalWorkspaceObject zone = illuminanceMap.getTarget(Output_IlluminanceMapFields::ZoneName);
        if (!zone){
          LOG(Error, "Could not find zone");
          continue;
        }

        Transformation t;
        if (daylightingCoordChange == CoordinateChange::AbsoluteToRelative){
          Transformation zoneTransformation = this->zoneTransformation(*zone);
          t = zoneTransformation.inverse()*buildingTransformation.inverse();
        }else if(daylightingCoordChange == CoordinateChange::RelativeToAbsolute){
          Transformation zoneTransformation = this->zoneTransformation(*zone);
          t = buildingTransformation*zoneTransformation;
        }

        // transform min/max points
        OptionalDouble xmin = illuminanceMap.getDouble(Output_IlluminanceMapFields::XMinimumCoordinate, true);
        OptionalDouble ymin = illuminanceMap.getDouble(Output_IlluminanceMapFields::YMinimumCoordinate, true);
        OptionalDouble xmax = illuminanceMap.getDouble(Output_IlluminanceMapFields::XMaximumCoordinate, true);
        OptionalDouble ymax = illuminanceMap.getDouble(Output_IlluminanceMapFields::YMaximumCoordinate, true);
        OptionalDouble z = illuminanceMap.getDouble(Output_IlluminanceMapFields::Zheight, true);
        if (xmin && ymin && xmax && ymax && z){

          // do not know which point will be min and max in new coord system
          Point3d p1(*xmin, *ymin, *z);
          p1 = t*p1;
          Point3d p2(*xmax, *ymin, *z);
          p2 = t*p2;
          Point3d p3(*xmin, *ymax, *z);
          p3 = t*p3;
          Point3d p4(*xmax, *ymax, *z);
          p4 = t*p4;

          xmin = std::min(std::min(std::min(p1.x(), p2.x()), p3.x()), p4.x());
          ymin = std::min(std::min(std::min(p1.y(), p2.y()), p3.y()), p4.y());
          xmax = std::max(std::max(std::max(p1.x(), p2.x()), p3.x()), p4.x());
          ymax = std::max(std::max(std::max(p1.y(), p2.y()), p3.y()), p4.y());

          result = result && illuminanceMap.setDouble(Output_IlluminanceMapFields::XMinimumCoordinate, *xmin);
          result = result && illuminanceMap.setDouble(Output_IlluminanceMapFields::YMinimumCoordinate, *ymin);
          result = result && illuminanceMap.setDouble(Output_IlluminanceMapFields::XMaximumCoordinate, *xmax);
          result = result && illuminanceMap.setDouble(Output_IlluminanceMapFields::YMaximumCoordinate, *ymax);
          result = result && illuminanceMap.setDouble(Output_IlluminanceMapFields::Zheight, p1.z());
        }
      }
    }

    return result;
  }

  // transform detailed geometry from the current system to the new system
  bool GeometryTranslator::convertDetailedGeometry(const CoordinateChange& detailedCoordChange)
  {
    bool result = true;

    Transformation buildingTransformation = this->buildingTransformation();

    BOOST_FOREACH(WorkspaceObject surface, m_workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)){
      Point3dVector vertices = getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, surface);
      surface.setString(BuildingSurface_DetailedFields::NumberofVertices, "Autocalculate");
      if (detailedCoordChange != CoordinateChange::NoChange){
        OptionalWorkspaceObject zone = surface.getTarget(BuildingSurface_DetailedFields::ZoneName);
        if (!zone){
          LOG(Error, "Could not find zone");
          continue;
        }

        Transformation zoneTransformation = this->zoneTransformation(*zone);

        if (detailedCoordChange == CoordinateChange::AbsoluteToRelative){
          vertices = zoneTransformation.inverse()*buildingTransformation.inverse()*vertices;
          setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, surface, vertices);
        }else if(detailedCoordChange == CoordinateChange::RelativeToAbsolute){
          vertices = buildingTransformation*zoneTransformation*vertices;
          setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, surface, vertices);
        }
      }
    }

    BOOST_FOREACH(WorkspaceObject subsurface, m_workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed)){
      Point3dVector vertices = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, subsurface);
      subsurface.setString(FenestrationSurface_DetailedFields::NumberofVertices, "Autocalculate");
      if (detailedCoordChange != CoordinateChange::NoChange){
        OptionalWorkspaceObject surface = subsurface.getTarget(FenestrationSurface_DetailedFields::BuildingSurfaceName);
        if (!surface){
          LOG(Error, "Could not find surface");
          continue;
        }
        OptionalWorkspaceObject zone = surface->getTarget(BuildingSurface_DetailedFields::ZoneName);
        if (!zone){
          LOG(Error, "Could not find zone");
          continue;
        }
        Transformation zoneTransformation = this->zoneTransformation(*zone);
        if (detailedCoordChange == CoordinateChange::AbsoluteToRelative){
          vertices = zoneTransformation.inverse()*buildingTransformation.inverse()*vertices;
          setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, subsurface, vertices);
        }else if(detailedCoordChange == CoordinateChange::RelativeToAbsolute){
          vertices = buildingTransformation*zoneTransformation*vertices;
          setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, subsurface, vertices);
        }
      }
    }

    BOOST_FOREACH(WorkspaceObject zoneShading, m_workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed)){
      Point3dVector vertices = getVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, zoneShading);
      zoneShading.setString(Shading_Zone_DetailedFields::NumberofVertices, "Autocalculate");
      if (detailedCoordChange != CoordinateChange::NoChange){
        OptionalWorkspaceObject zoneSurface = zoneShading.getTarget(Shading_Zone_DetailedFields::BaseSurfaceName);
        if (!zoneSurface){
          LOG(Error, "Could not find zone surface");
          continue;
        }
        OptionalWorkspaceObject zone = zoneSurface->getTarget(BuildingSurface_DetailedFields::ZoneName);
        if (!zone){
          LOG(Error, "Could not find zone");
          continue;
        }

        Transformation zoneTransformation = this->zoneTransformation(*zone);

        if (detailedCoordChange == CoordinateChange::AbsoluteToRelative){
          vertices = zoneTransformation.inverse()*buildingTransformation.inverse()*vertices;
          setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, zoneShading, vertices);
        }else if(detailedCoordChange == CoordinateChange::RelativeToAbsolute){
          vertices = buildingTransformation*zoneTransformation*vertices;
          setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, zoneShading, vertices);
        }
      }
    }

    BOOST_FOREACH(WorkspaceObject buildingShading, m_workspace.getObjectsByType(IddObjectType::Shading_Building_Detailed)){
      Point3dVector vertices = getVertices(Shading_Building_DetailedFields::NumberofVertices + 1, buildingShading);
      buildingShading.setString(Shading_Building_DetailedFields::NumberofVertices, "Autocalculate");
      if (detailedCoordChange != CoordinateChange::NoChange){
        if (detailedCoordChange == CoordinateChange::AbsoluteToRelative){
          vertices = buildingTransformation.inverse()*vertices;
          setVertices(Shading_Building_DetailedFields::NumberofVertices + 1, buildingShading, vertices);
        }else if(detailedCoordChange == CoordinateChange::RelativeToAbsolute){
          vertices = buildingTransformation*vertices;
          setVertices(Shading_Building_DetailedFields::NumberofVertices + 1, buildingShading, vertices);
        }
      }
    }

    BOOST_FOREACH(WorkspaceObject siteShading, m_workspace.getObjectsByType(IddObjectType::Shading_Site_Detailed)){
      siteShading.setString(Shading_Site_DetailedFields::NumberofVertices, "Autocalculate");
    }

    return result;
  }

  // get vertices for a surface
  openstudio::Point3dVector getVertices(unsigned firstVertex, const IdfObject& surface)
  {
    Point3dVector result;
    unsigned numFields = surface.numFields();
    unsigned numVertexFields;
    unsigned numVertices;

    numVertexFields = numFields - firstVertex;
    numVertices = numVertexFields / 3; // integer division

    // check that at least three vertices
    if ( (numVertexFields % 3) == 0 && numVertices >= 3 ){

      for (unsigned vertexIndex = 0; vertexIndex < numVertices; ++vertexIndex){
        OptionalDouble x = surface.getDouble(firstVertex + 3*vertexIndex + 0);
        OptionalDouble y = surface.getDouble(firstVertex + 3*vertexIndex + 1);
        OptionalDouble z = surface.getDouble(firstVertex + 3*vertexIndex + 2);

        if (x && y && z){
          result.push_back(Point3d(*x, *y, *z));
        }else{
          LOG_FREE(Error, "openstudio.energyplus.GeometryTranslator", 
                   "Could not read vertex " << vertexIndex << " at indices " <<
                   firstVertex + 3*vertexIndex + 0 << " to " << firstVertex + 3*vertexIndex + 2 );
        }
      }
    }else{
      LOG_FREE(Error, "openstudio.energyplus.GeometryTranslator",
              "Fewer than 3 vertices detected for surface '" << surface.name() << "'");
    }

    return result;
  }

  // set vertices for a surface, only detailed surfaces are supported
  bool setVertices(unsigned firstVertex, WorkspaceObject& surface, const openstudio::Point3dVector& vertices)
  {
    bool result = true;

    // delete current fields, cannot currently delete all so leave first 3 vertices
    while( !surface.popExtensibleGroup().empty() )
    {}

    // preallocate enough space for all vertices
    while( (surface.numFields()-firstVertex)/3 < vertices.size() )
    {
      surface.pushExtensibleGroup(StringVector(3));
    }

    // set the vertices
    for (unsigned vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex){
      if (!surface.setDouble(firstVertex + 3*vertexIndex + 0, vertices[vertexIndex].x())){
        LOG_FREE(Error, "openstudio.energyplus.GeometryTranslator",
                 "Could not set field index " << firstVertex + 3*vertexIndex + 0 << " to " << vertices[vertexIndex].x());
        result = false;
      }
      if (!surface.setDouble(firstVertex + 3*vertexIndex + 1, vertices[vertexIndex].y())){
        LOG_FREE(Error, "openstudio.energyplus.GeometryTranslator",
                 "Could not set field index " << firstVertex + 3*vertexIndex + 1 << " to " << vertices[vertexIndex].y());
        result = false;
      }
      if (!surface.setDouble(firstVertex + 3*vertexIndex + 2, vertices[vertexIndex].z())){
        LOG_FREE(Error, "openstudio.energyplus.GeometryTranslator",
                 "Could not set field index " << firstVertex + 3*vertexIndex + 2 << " to " << vertices[vertexIndex].z());
        result = false;
      }
    }

    return result;
  }

  // reverse all detailed vertices
  bool GeometryTranslator::reverseAllDetailedVertices()
  {
    // BuildingSurface:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)){
      Point3dVector vertices = getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    // Wall:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Wall_Detailed)){
      Point3dVector vertices = getVertices(Wall_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(Wall_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    // RoofCeiling:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::RoofCeiling_Detailed)){
      Point3dVector vertices = getVertices(RoofCeiling_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(RoofCeiling_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    // Floor:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Floor_Detailed)){
      Point3dVector vertices = getVertices(Floor_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(Floor_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    // FenestrationSurface:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed)){
      Point3dVector vertices = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    // Shading:Site:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Shading_Site_Detailed)){
      Point3dVector vertices = getVertices(Shading_Site_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(Shading_Site_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    // Shading:Building:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Shading_Building_Detailed)){
      Point3dVector vertices = getVertices(Shading_Building_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(Shading_Building_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    // Shading:Zone:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed)){
      Point3dVector vertices = getVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, object);
      std::reverse(vertices.begin(), vertices.end());
      setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, object, vertices);
    }

    return true;
  }

  // apply upper left corner rule to all detailed vertices
  bool GeometryTranslator::applyUpperLeftCornerRule()
  {
    // BuildingSurface:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)){
      Point3dVector vertices = getVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, object);
      setVertices(BuildingSurface_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    // Wall:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Wall_Detailed)){
      Point3dVector vertices = getVertices(Wall_DetailedFields::NumberofVertices + 1, object);
      setVertices(Wall_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    // RoofCeiling:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::RoofCeiling_Detailed)){
      Point3dVector vertices = getVertices(RoofCeiling_DetailedFields::NumberofVertices + 1, object);
      setVertices(RoofCeiling_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    // Floor:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Floor_Detailed)){
      Point3dVector vertices = getVertices(Floor_DetailedFields::NumberofVertices + 1, object);
      setVertices(Floor_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    // FenestrationSurface:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed)){
      Point3dVector vertices = getVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, object);
      setVertices(FenestrationSurface_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    // Shading:Site:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Shading_Site_Detailed)){
      Point3dVector vertices = getVertices(Shading_Site_DetailedFields::NumberofVertices + 1, object);
      setVertices(Shading_Site_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    // Shading:Building:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Shading_Building_Detailed)){
      Point3dVector vertices = getVertices(Shading_Building_DetailedFields::NumberofVertices + 1, object);
      setVertices(Shading_Building_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    // Shading:Zone:Detailed
    BOOST_FOREACH(WorkspaceObject object, m_workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed)){
      Point3dVector vertices = getVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, object);
      setVertices(Shading_Zone_DetailedFields::NumberofVertices + 1, object, reorderULC(vertices));
    }

    return true;
  }

  // convert azimuth, tilt, starting x, y, z, length, and width to vertices
  // azimith and tilt are in degrees
  openstudio::Point3dVector verticesForAzimuthTiltXYZLengthWidthOrHeight(double azimuth, double tilt, double x0, double y0, double z0, double length, double widthOrHeight)
  {
    openstudio::Point3dVector result(4);

    openstudio::Vector xpts(4, 0.0);
    xpts[2] = length;
    xpts[3] = length;

    openstudio::Vector ypts(4, 0.0);
    ypts[0] = widthOrHeight;
    ypts[3] = widthOrHeight;

    // code copied out of EnergyPlus does not seem to make sense given the documentation in
    // InputOutput reference
    double cosAzimuth = cos(degToRad(azimuth));
    double sinAzimuth = sin(degToRad(azimuth));
    double cosTilt = cos(degToRad(tilt));
    double sinTilt = sin(degToRad(tilt));

    for (unsigned i = 0; i < 4; ++i){
      double x = x0 - cosAzimuth*xpts[i] - cosTilt*sinAzimuth*ypts[i];
      double y = y0 + sinAzimuth*xpts[i] - cosTilt*cosAzimuth*ypts[i];
      double z = z0 + sinTilt*ypts[i];
      result[i] = Point3d(x, y, z);
    }

    return result;
  }

} // energyplus
} // openstudio
