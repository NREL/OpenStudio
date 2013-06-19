/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include "ReverseTranslator.hpp"

#include <model/Model.hpp>
#include <model/Construction.hpp>
#include <model/AirWallMaterial.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>

#include <SimXmlDoc.hpp>
#include <simbldg.hxx>
#include <simcore.hxx>
#include <simgeom.hxx>
#include <simmep.hxx>
#include <simmodel.hxx>
#include <simres.hxx>

#include <utilities/core/Assert.hpp>

#include <QDomElement>

namespace openstudio {
namespace simxml {

  boost::optional<openstudio::model::Model> ReverseTranslator::SimXmlToModel(const openstudio::path& path)
  {

    ::simxml::SimXmlDoc doc = ::simxml::SimXmlDoc::load(toString(path));

    if (doc.isNull()){
      return boost::none;
    }

    ReverseTranslator reverseTranslator(doc);
    return reverseTranslator.convert();
  }

  ReverseTranslator::ReverseTranslator(const ::simxml::SimXmlDoc& doc)
    : m_doc(doc)
  {
  }

  openstudio::model::Model ReverseTranslator::convert()
  {
    // initialize model
    m_model = openstudio::model::Model();

    // add air wall for virtual space boundaries
    openstudio::model::AirWallMaterial airWallMaterial(m_model);
    m_airWallConstruction = openstudio::model::Construction(airWallMaterial);
    m_airWallConstruction->setName("Virtual Boundary Construction");

    // translate the building
    std::vector<simbldg::SimBuilding> simBuildings = simbldg::SimBuilding::getAll(m_doc);
    if (!simBuildings.empty()){
      translateSimBuilding(simBuildings[0]);
    }

    // translate each building story
    BOOST_FOREACH(const simbldg::SimBuildingStory& simBuildingStory, simbldg::SimBuildingStory::getAll(m_doc)){
      translateSimBuildingStory(simBuildingStory);
    }

    // translate each thermal zone
    BOOST_FOREACH(const simbldg::SimSpatialZone& simSpatialZone, simbldg::SimSpatialZone::getAll(m_doc)){
      translateSimSpatialZone(simSpatialZone);
    }

    // translate each space
    BOOST_FOREACH(const simbldg::SimSpace& simSpace, simbldg::SimSpace::getAll(m_doc)){
      translateSimSpace(simSpace);
    }

    // translate each wall
    BOOST_FOREACH(const simbldg::SimWall& simWall, simbldg::SimWall::getAll(m_doc)){
      translateSimWall(simWall);
    }

    // translate each slab
    BOOST_FOREACH(const simbldg::SimSlab& simSlab, simbldg::SimSlab::getAll(m_doc)){
      translateSimSlab(simSlab);
    }

    // translate each window
    BOOST_FOREACH(const simbldg::SimWindow& simWindow, simbldg::SimWindow::getAll(m_doc)){
      translateSimWindow(simWindow);
    }

    return m_model;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::findModelObject(const std::string& refId) const
  {
    boost::optional<openstudio::model::ModelObject> result;
    
    std::map<std::string, openstudio::model::ModelObject>::const_iterator it = m_refIdToModelObjectMap.find(refId);
    if (it != m_refIdToModelObjectMap.end()){
      result = it->second;
    }

    return result;
  }

  void ReverseTranslator::registerModelObject(const std::string& refId, const openstudio::model::ModelObject& modelObject)
  {
    m_refIdToModelObjectMap.insert(std::make_pair<std::string, openstudio::model::ModelObject>(refId, modelObject));
  }

  std::vector<openstudio::Point3d> ReverseTranslator::getSpaceBoundaryVertices(const simres::SimSpaceBoundary& simSpaceBoundary)
  {
    std::vector<openstudio::Point3d> result;

    if (!simSpaceBoundary.isValid()){
      return result;
    }

    std::string connectionGeometryId = simSpaceBoundary.connectionGeometryId();
    simgeom::SimGeomSurface geomSurface = simgeom::SimGeomSurface::find(connectionGeometryId, m_doc);
    if (!geomSurface.isValid()){
      return result;
    }

    std::string outerBoundaryId = geomSurface.outerBoundaryId();
    simgeom::SimGeomCurve geomCurve = simgeom::SimGeomCurve::find(outerBoundaryId, m_doc);
    if (!geomCurve.isValid()){
      return result;
    }

    BOOST_FOREACH(const std::string& pointId, geomCurve.pointsIds()){

      simgeom::SimGeomPoint geomPoint = simgeom::SimGeomPoint::find(pointId, m_doc);
      if (geomPoint.isNull()){
        continue;
      }

      std::vector<double> coordinates = geomPoint.coordinates();
      if (coordinates.size() != 3){
        continue;
      }

      // convert from mm to m
      result.push_back(openstudio::Point3d(coordinates[0]/1000.0, coordinates[1]/1000.0, coordinates[2]/1000.0));

    }

    return result;
  }

  boost::optional<openstudio::model::Building> ReverseTranslator::translateSimBuilding(const simbldg::SimBuilding& simBuilding)
  {
    if (!simBuilding.isValid()){
      return boost::none;
    }

    boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simBuilding.refId());
    if (modelObject){
      return modelObject->cast<openstudio::model::Building>();
    }

    openstudio::model::Building building = m_model.getUniqueModelObject<openstudio::model::Building>();
    building.setName(simBuilding.refId());

    registerModelObject(simBuilding.refId(), building);

    return building;
  }

  boost::optional<openstudio::model::BuildingStory> ReverseTranslator::translateSimBuildingStory(const simbldg::SimBuildingStory& simBuildingStory)
  {
    if (!simBuildingStory.isValid()){
      return boost::none;
    }
    
    boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simBuildingStory.refId());
    if (modelObject){
      return modelObject->cast<openstudio::model::BuildingStory>();
    }

    openstudio::model::BuildingStory buildingStory(m_model);
    buildingStory.setName(simBuildingStory.refId());

    registerModelObject(simBuildingStory.refId(), buildingStory);

    return buildingStory;
  }

  boost::optional<openstudio::model::ThermalZone> ReverseTranslator::translateSimSpatialZone(const simbldg::SimSpatialZone& simSpatialZone)
  {
    if (!simSpatialZone.isValid()){
      return boost::none;
    }

    boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simSpatialZone.refId());
    if (modelObject){
      return modelObject->cast<openstudio::model::ThermalZone>();
    }

    openstudio::model::ThermalZone thermalZone(m_model);
    thermalZone.setName(simSpatialZone.refId());
 
    registerModelObject(simSpatialZone.refId(), thermalZone);

    return thermalZone;
  }

  boost::optional<openstudio::model::Space> ReverseTranslator::translateSimSpace(const simbldg::SimSpace& simSpace)
  {
    if (!simSpace.isValid()){
      return boost::none;
    }

    boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simSpace.refId());
    if (modelObject){
      return modelObject->cast<openstudio::model::Space>();
    }

    openstudio::model::Space space(m_model);
    space.setName(simSpace.refId());
    space.setZOrigin(simSpace.spaceDatumElevation()/1000.0);

    simbldg::SimBuildingStory simBuildingStory = simbldg::SimBuildingStory::find(simSpace.spaceInSpatialContainerId(), m_doc);
    boost::optional<openstudio::model::BuildingStory> buildingStory = translateSimBuildingStory(simBuildingStory);
    if (buildingStory){
      space.setBuildingStory(*buildingStory);
    }
    
    std::vector<std::string> spaceZoneAssignmentsIds = simSpace.spaceZoneAssignmentsIds();
    BOOST_ASSERT(spaceZoneAssignmentsIds.size() <= 1);
    if (!spaceZoneAssignmentsIds.empty()){

      simbldg::SimSpatialZone simSpatialZone = simbldg::SimSpatialZone::find(spaceZoneAssignmentsIds[0], m_doc);
      boost::optional<openstudio::model::ThermalZone> thermalZone = translateSimSpatialZone(simSpatialZone);
      if (thermalZone){
        space.setThermalZone(*thermalZone);
      }
    }

    registerModelObject(simSpace.refId(), space);

    return space;
  }

  boost::optional<openstudio::model::Surface> ReverseTranslator::translateSimSlab(const simbldg::SimSlab& simSlab)
  {
    if (!simSlab.isValid()){
      return boost::none;
    }

    boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simSlab.refId());
    if (modelObject){
      return modelObject->cast<openstudio::model::Surface>();
    }
    
    std::vector<simres::SimSpaceBoundary> simSpaceBoundaries = simres::SimSpaceBoundary::findWhere("simres:RelatedBuildingElement", simSlab.refId(), m_doc);
    
    // space boundaries should be of size 0, 1, or 2
    // DLM: apparently not?
    //BOOST_ASSERT(simSpaceBoundaries.size() <= 2);

    if (simSpaceBoundaries.empty()){
      return boost::none;
    }

    // map each surface
    std::vector<openstudio::model::Surface> surfaces;
    BOOST_FOREACH(const simres::SimSpaceBoundary& simSpaceBoundary, simSpaceBoundaries){

      openstudio::Point3dVector points = getSpaceBoundaryVertices(simSpaceBoundary);
      BOOST_ASSERT(points.size() >= 2);

      openstudio::model::Surface surface(points, m_model);
      surface.setName(simSpaceBoundary.refId());

      // get the space
      simbldg::SimSpace simSpace = simbldg::SimSpace::find(simSpaceBoundary.relatingSpaceId(), m_doc);
      boost::optional<openstudio::model::Space> space = translateSimSpace(simSpace);
      if (space){
        surface.setSpace(*space);
      }

      if (simSpaceBoundary.physicalOrVirtualBoundary() == "VIRTUAL"){
        surface.setConstruction(*m_airWallConstruction);
      }

      surfaces.push_back(surface);
    }

    //if (surfaces.size() == 2){
    //  surfaces[0].setAdjacentSurface(surfaces[1]);
    //}
 
    registerModelObject(simSlab.refId(), surfaces[0]);

    return surfaces[0];
  }

  boost::optional<openstudio::model::Surface> ReverseTranslator::translateSimWall(const simbldg::SimWall& simWall)
  {
    if (!simWall.isValid()){
      return boost::none;
    }

    boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simWall.refId());
    if (modelObject){
      return modelObject->cast<openstudio::model::Surface>();
    }

    std::vector<simres::SimSpaceBoundary> simSpaceBoundaries = simres::SimSpaceBoundary::findWhere("simres:RelatedBuildingElement", simWall.refId(), m_doc);
    
    // space boundaries should be of size 0, 1, or 2
    // DLM: apparently not?
    //BOOST_ASSERT(simSpaceBoundaries.size() <= 2);

    if (simSpaceBoundaries.empty()){
      return boost::none;
    }

    // map each surface
    std::vector<openstudio::model::Surface> surfaces;
    BOOST_FOREACH(const simres::SimSpaceBoundary& simSpaceBoundary, simSpaceBoundaries){

      openstudio::Point3dVector points = getSpaceBoundaryVertices(simSpaceBoundary);
      BOOST_ASSERT(points.size() >= 2);

      openstudio::model::Surface surface(points, m_model);
      surface.setName(simSpaceBoundary.refId());

      // get the space
      simbldg::SimSpace simSpace = simbldg::SimSpace::find(simSpaceBoundary.relatingSpaceId(), m_doc);
      boost::optional<openstudio::model::Space> space = translateSimSpace(simSpace);
      if (space){
        surface.setSpace(*space);
      }

      if (simSpaceBoundary.physicalOrVirtualBoundary() == "VIRTUAL"){
        surface.setConstruction(*m_airWallConstruction);
      }

      surfaces.push_back(surface);
    }

    //if (surfaces.size() == 2){
    //  surfaces[0].setAdjacentSurface(surfaces[1]);
    //}
 
    registerModelObject(simWall.refId(), surfaces[0]);

    return surfaces[0];
  }

  boost::optional<openstudio::model::SubSurface> ReverseTranslator::translateSimWindow(const simbldg::SimWindow& simWindow)
  {
    if (!simWindow.isValid()){
      return boost::none;
    }

    boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simWindow.refId());
    if (modelObject){
      return modelObject->cast<openstudio::model::SubSurface>();
    }

    std::vector<simres::SimSpaceBoundary> simSpaceBoundaries = simres::SimSpaceBoundary::findWhere("simres:RelatedBuildingElement", simWindow.refId(), m_doc);
    
    // space boundaries should be of size 0, 1, or 2
    // DLM: apparently not?
    //BOOST_ASSERT(simSpaceBoundaries.size() <= 2);

    if (simSpaceBoundaries.empty()){
      return boost::none;
    }

    // map each sub surface
    std::vector<openstudio::model::SubSurface> subSurfaces;
    BOOST_FOREACH(const simres::SimSpaceBoundary& simSpaceBoundary, simSpaceBoundaries){

      openstudio::Point3dVector points = getSpaceBoundaryVertices(simSpaceBoundary);
      BOOST_ASSERT(points.size() >= 2);

      openstudio::model::SubSurface subSurface(points, m_model);
      subSurface.setName(simSpaceBoundary.refId());

      subSurfaces.push_back(subSurface);
    }

    // get the surface
    simgeom::SimFeatureElementSubtraction simFeatureElementSubtraction = simgeom::SimFeatureElementSubtraction::find(simWindow.openingFilledByWindowId(), m_doc);
    if (simFeatureElementSubtraction.isValid()){
      boost::optional<openstudio::model::ModelObject> modelObject = findModelObject(simFeatureElementSubtraction.voidedBldgElementId());
      if (modelObject){
        boost::optional<openstudio::model::Surface> surface = modelObject->optionalCast<openstudio::model::Surface>();
        if (surface){
          
          if (subSurfaces.size() == 1){
            // if one space boundary, should be exterior window
            BOOST_ASSERT(!surface->adjacentSurface());
            subSurfaces[0].setSurface(*surface);

          }else if (subSurfaces.size() == 2){
            // if two space boundary, should be interior window
            /*
            boost::optional<openstudio::model::Surface> adjacentSurface = surface->adjacentSurface();
            BOOST_ASSERT(adjacentSurface);

            // figure out which sub surface goes to which surface
            Vector3d subSurface0Normal = subSurfaces[0].outwardNormal();
            double dot00 = subSurface0Normal.dot(surface->outwardNormal());
            double dot01 = subSurface0Normal.dot(adjacentSurface->outwardNormal());

            if(dot00 > dot01){
              subSurfaces[0].setSurface(*surface);
              subSurfaces[1].setSurface(*adjacentSurface);
            }else{
              subSurfaces[0].setSurface(*adjacentSurface);
              subSurfaces[1].setSurface(*surface);
            }
            */
          }else{
            // ?
          }
        }
      }
    }
 
    registerModelObject(simWindow.refId(), subSurfaces[0]);

    return subSurfaces[0];
  }

} // simxml
} // openstudio
