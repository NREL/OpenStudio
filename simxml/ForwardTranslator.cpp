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

#include "ForwardTranslator.hpp"

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

  ::simxml::SimXmlDoc ForwardTranslator::modelToSimXml(const openstudio::model::Model& model)
  {

    ::simxml::SimXmlDoc doc;
    ForwardTranslator forwardTranslator(model);
    return forwardTranslator.convert();
  }

  ForwardTranslator::ForwardTranslator(const openstudio::model::Model& model)
    : m_model(model)
  {
  }

  ::simxml::SimXmlDoc ForwardTranslator::convert()
  {
    // translate building
    boost::optional<openstudio::model::Building> building = m_model.getOptionalUniqueModelObject<openstudio::model::Building>();
    if (building){
      translateBuilding(*building);
    }

    // translate each building story
    BOOST_FOREACH(const openstudio::model::BuildingStory& buildingStory, m_model.getModelObjects<openstudio::model::BuildingStory>()){
      translateBuildingStory(buildingStory);
    }

    // translate each thermal zone
    BOOST_FOREACH(const openstudio::model::ThermalZone& thermalZone, m_model.getModelObjects<openstudio::model::ThermalZone>()){
      translateThermalZone(thermalZone);
    }

    // translate each space
    BOOST_FOREACH(const openstudio::model::Space& space, m_model.getModelObjects<openstudio::model::Space>()){
      translateSpace(space);
    }

    return m_doc;
  }

  boost::optional<std::string> ForwardTranslator::findRefId(const openstudio::model::ModelObject& modelObject) const
  {
    boost::optional<std::string> result;
    
    std::map<openstudio::model::ModelObject, std::string>::const_iterator it = m_modelObjectToRefIdMap.find(modelObject);
    if (it != m_modelObjectToRefIdMap.end()){
      result = it->second;
    }

    return result;
  }

  void ForwardTranslator::registerModelObject(const openstudio::model::ModelObject& modelObject, const std::string& refId)
  {
    m_modelObjectToRefIdMap.insert(std::make_pair<openstudio::model::ModelObject, std::string>(modelObject, refId));
  }

  simres::SimSpaceBoundary ForwardTranslator::makeSpaceBoundaryForVertices(const std::vector<openstudio::Point3d>& vertices)
  {
    std::vector<openstudio::Point3d> result;

    std::vector< ::simxml::SimXmlElement > geomPoints;
    BOOST_FOREACH(const openstudio::Point3d& vertex, vertices){

      std::vector<double> coordinates;
      coordinates.push_back(1000.0*vertex.x());
      coordinates.push_back(1000.0*vertex.y());
      coordinates.push_back(1000.0*vertex.z());

      simgeom::SimGeomPoint geomPoint(m_doc);
      geomPoint.setCoordinates(coordinates);

      geomPoints.push_back(geomPoint);
    }
  
    simgeom::SimGeomCurve geomCurve(m_doc);
    geomCurve.setPoints(geomPoints);

    simgeom::SimGeomSurface geomSurface(m_doc);
    geomSurface.setOuterBoundary(geomCurve);

    simres::SimSpaceBoundary spaceBoundary(m_doc);
    spaceBoundary.setConnectionGeometry(geomSurface);

    return spaceBoundary;
  }

  simbldg::SimBuilding ForwardTranslator::translateBuilding(const openstudio::model::Building& building)
  {
    boost::optional<std::string> refId = findRefId(building);
    if (refId){
      return simbldg::SimBuilding::find(*refId, m_doc);
    }

    simbldg::SimBuilding simBuilding(m_doc);

    simBuilding.setName(building.name().get());

    registerModelObject(building, simBuilding.refId());

    return simBuilding;
  }

  simbldg::SimBuildingStory ForwardTranslator::translateBuildingStory(const openstudio::model::BuildingStory& buildingStory)
  {
    boost::optional<std::string> refId = findRefId(buildingStory);
    if (refId){
      return simbldg::SimBuildingStory::find(*refId, m_doc);
    }

    simbldg::SimBuildingStory simBuildingStory(m_doc);

    simBuildingStory.setName(buildingStory.name().get());

    registerModelObject(buildingStory, simBuildingStory.refId());

    return simBuildingStory;
  }

  simbldg::SimSpatialZone ForwardTranslator::translateThermalZone(const openstudio::model::ThermalZone& thermalZone)
  {
    boost::optional<std::string> refId = findRefId(thermalZone);
    if (refId){
      return simbldg::SimSpatialZone::find(*refId, m_doc);
    }

    simbldg::SimSpatialZone simSpatialZone(m_doc);

    simSpatialZone.setName(thermalZone.name().get());

    registerModelObject(thermalZone, simSpatialZone.refId());

    return simSpatialZone;
  }

  simbldg::SimSpace ForwardTranslator::translateSpace(const openstudio::model::Space& space)
  {
    boost::optional<std::string> refId = findRefId(space);
    if (refId){
      return simbldg::SimSpace::find(*refId, m_doc);
    }

    simbldg::SimSpace simSpace(m_doc);

    simSpace.setName(space.name().get());

    boost::optional<openstudio::model::BuildingStory> buildingStory = space.buildingStory();
    if (buildingStory){
      simbldg::SimBuildingStory simBuildingStory = translateBuildingStory(*buildingStory);
      simSpace.setSpaceInSpatialContainer(simBuildingStory);
    }

    boost::optional<openstudio::model::ThermalZone> thermalZone = space.thermalZone();
    if (thermalZone){
      simbldg::SimSpatialZone simSpatialZone = translateThermalZone(*thermalZone);
      std::vector< ::simxml::SimXmlElement > simSpatialZones;
      simSpatialZones.push_back(simSpatialZone);
      simSpace.setSpaceZoneAssignments(simSpatialZones);
    }

    BOOST_FOREACH(const openstudio::model::Surface& surface, space.surfaces()){
      translateSurface(surface, simSpace);
    }

    registerModelObject(space, simSpace.refId());

    return simSpace;
  }

  ::simxml::SimXmlElement ForwardTranslator::translateSurface(const openstudio::model::Surface& surface, const simbldg::SimSpace& simSpace)
  {
    boost::optional<std::string> refId = findRefId(surface);
    if (refId){
      return m_doc.findElement(*refId);
    }

    std::string surfaceType = surface.surfaceType();

    boost::optional< ::simxml::SimXmlElement > result;
    if (istringEqual("Wall", surfaceType)){
      result = simbldg::SimWall(m_doc);
    }else if (istringEqual("Floor", surfaceType)){
      result = simbldg::SimSlab(m_doc);
    }else if (istringEqual("RoofCeiling", surfaceType)){
      result = simbldg::SimSlab(m_doc);
    }
    BOOST_ASSERT(result);
  
    simres::SimSpaceBoundary spaceBoundary = makeSpaceBoundaryForVertices(surface.vertices());
    spaceBoundary.setRelatingSpace(simSpace);
    spaceBoundary.setRelatedBuildingElement(*result);

    BOOST_FOREACH(const openstudio::model::SubSurface& subSurface, surface.subSurfaces()){
      translateSubSurface(subSurface, *result, simSpace);
    }

    registerModelObject(surface, result->refId());

    return *result;
  }

  ::simxml::SimXmlElement ForwardTranslator::translateSubSurface(const openstudio::model::SubSurface& subSurface, const ::simxml::SimXmlElement& buildingElement, const simbldg::SimSpace& simSpace)
  {
    boost::optional<std::string> refId = findRefId(subSurface);
    if (refId){
      return m_doc.findElement(*refId);
    }

    std::string subSurfaceType = subSurface.subSurfaceType();

    boost::optional< ::simxml::SimXmlElement > result;
    if (istringEqual("Door", subSurfaceType)){
      simgeom::SimFeatureElementSubtraction simFeatureElementSubtraction(m_doc);
      simFeatureElementSubtraction.setVoidedBldgElement(buildingElement);

      simbldg::SimDoor simDoor(m_doc);
      simDoor.setOpeningFilledByDoor(simFeatureElementSubtraction);
      result = simDoor;
    }else{
      simgeom::SimFeatureElementSubtraction simFeatureElementSubtraction(m_doc);
      simFeatureElementSubtraction.setVoidedBldgElement(buildingElement);

      simbldg::SimWindow simWindow(m_doc);
      simWindow.setOpeningFilledByWindow(simFeatureElementSubtraction);
      result = simWindow;
    }
    BOOST_ASSERT(result);

    simres::SimSpaceBoundary spaceBoundary = makeSpaceBoundaryForVertices(subSurface.vertices());
    spaceBoundary.setRelatingSpace(simSpace);
    spaceBoundary.setRelatedBuildingElement(*result);

    registerModelObject(subSurface, result->refId());

    return *result;
  }


} // simxml
} // openstudio
