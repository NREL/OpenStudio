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

#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/EulerAngles.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"

#include <boost/math/constants/constants.hpp>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QThread>

namespace openstudio {
namespace gbxml {

  ForwardTranslator::ForwardTranslator()
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.gbxml\\.ForwardTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  ForwardTranslator::~ForwardTranslator()
  {
  }

  bool ForwardTranslator::modelToGbXML(const openstudio::model::Model& model, const openstudio::path& path, ProgressBar* progressBar)
  {
    m_progressBar = progressBar;

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    boost::optional<QDomDocument> doc = this->translateModel(model);
    if (!doc){
      return false;
    }

    QFile file(toQString(path));
    if (file.open(QFile::WriteOnly)){
      QTextStream textStream(&file);
      textStream << doc->toString(2);
      file.close();
      return true;
    }

    return false;
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() > Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  QString ForwardTranslator::escapeName(const std::string& name)
  {
    QString result = toQString(name);
    result.replace(" ", "_");
    result.replace("(", "_");
    result.replace(")", "_");
    return result;
  }

  boost::optional<QDomDocument> ForwardTranslator::translateModel(const openstudio::model::Model& model)
  {
    QDomDocument doc;
    doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    QDomElement gbXMLElement = doc.createElement("gbXML");
    doc.appendChild(gbXMLElement);
    gbXMLElement.setAttribute("xmlns", "http://www.gbxml.org/schema");
    gbXMLElement.setAttribute("xmlns:xhtml", "http://www.w3.org/1999/xhtml");
    gbXMLElement.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    gbXMLElement.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
    gbXMLElement.setAttribute("xsi:schemaLocation", "http://www.gbxml.org/schema http://www.gbxml.org/schema/0-37/GreenBuildingXML.xsd");
    gbXMLElement.setAttribute("temperatureUnit", "C");
    gbXMLElement.setAttribute("lengthUnit", "Meters");
    gbXMLElement.setAttribute("areaUnit", "SquareMeters");
    gbXMLElement.setAttribute("volumeUnit", "CubicMeters");
    gbXMLElement.setAttribute("useSIUnitsForResults", "true");
    gbXMLElement.setAttribute("version", "0.37");

    boost::optional<model::Facility> facility = model.getOptionalUniqueModelObject<model::Facility>();
    if (facility){
      boost::optional<QDomElement> campusElement = translateFacility(*facility, doc);
      if (campusElement){
        gbXMLElement.appendChild(*campusElement);
      }
    }
  /*
    // do constructions
    for (const model::ConstructionBase& constructionBase : model.getModelObjects<model::ConstructionBase>()){
      boost::optional<QDomElement> constructionElement = translateConstructionBase(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }
    }

    // do materials
    for (const model::Material& material : model.getModelObjects<model::Material>()){
      boost::optional<QDomElement> materialElement = translateMaterial(material, doc);
      if (materialElement){
        projectElement.appendChild(*materialElement);
      }
    }
*/
    // do thermal zones
    std::vector<model::ThermalZone> thermalZones = model.getConcreteModelObjects<model::ThermalZone>();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(thermalZones.size()); 
      m_progressBar->setValue(0);
    }

    for (const model::ThermalZone& thermalZone : thermalZones){
      boost::optional<QDomElement> zoneElement = translateThermalZone(thermalZone, doc);
      if (zoneElement){
        gbXMLElement.appendChild(*zoneElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return doc;
  }

  boost::optional<QDomElement> ForwardTranslator::translateFacility(const openstudio::model::Facility& facility, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Campus");
    m_translatedObjects[facility.handle()] = result;

    // id
    result.setAttribute("id", "Facility");

    model::Model model = facility.model();

    // todo: translate location

    // translate building
    boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
    if (building){
      boost::optional<QDomElement> buildingElement = translateBuilding(*building, doc);
      if (buildingElement){
        result.appendChild(*buildingElement);
      }
    }

    // translate surfaces
    std::vector<model::Surface> surfaces = model.getConcreteModelObjects<model::Surface>();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Surfaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(surfaces.size()); 
      m_progressBar->setValue(0);
    }

    for (const model::Surface& surface : surfaces){
      boost::optional<QDomElement> surfaceElement = translateSurface(surface, doc);
      if (surfaceElement){
        result.appendChild(*surfaceElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }
  

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateBuilding(const openstudio::model::Building& building, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Building");
    m_translatedObjects[building.handle()] = result;

    // id
    std::string name = building.name().get();
    result.setAttribute("id", escapeName(name));

    // building type
    //result.setAttribute("buildingType", "Office");
    result.setAttribute("buildingType", "Unknown");

    // space type
    boost::optional<model::SpaceType> spaceType = building.spaceType();
    if (spaceType){
      //std::string spaceTypeName = spaceType->name().get();
      // todo: map to gbXML types
      //result.setAttribute("buildingType", escapeName(spaceTypeName));
    }

    // area
    QDomElement areaElement = doc.createElement("Area");
    result.appendChild(areaElement);
    areaElement.appendChild(doc.createTextNode(QString::number(building.floorArea())));

    // translate spaces
    std::vector<model::Space> spaces = building.spaces();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Spaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(spaces.size()); 
      m_progressBar->setValue(0);
    }

    for (const model::Space& space : spaces){
      boost::optional<QDomElement> spaceElement = translateSpace(space, doc);
      if (spaceElement){
        result.appendChild(*spaceElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateSpace(const openstudio::model::Space& space, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Space");
    m_translatedObjects[space.handle()] = result;

    // name
    std::string name = space.name().get();
    result.setAttribute("id", escapeName(name));

    // space type
    boost::optional<model::SpaceType> spaceType = space.spaceType();
    if (spaceType){
      //std::string spaceTypeName = spaceType->name().get();
      // todo: map to gbXML types
      //result.setAttribute("spaceType", escapeName(spaceTypeName));
    }

    // thermal zone
    boost::optional<model::ThermalZone> thermalZone = space.thermalZone();
    if (thermalZone){
      std::string thermalZoneName = thermalZone->name().get();
      result.setAttribute("zoneIdRef", escapeName(thermalZoneName));
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateSurface(const openstudio::model::Surface& surface, QDomDocument& doc)
  {
    // return if already translated
    if (m_translatedObjects.find(surface.handle()) != m_translatedObjects.end()){
      return boost::none;
    }

    QDomElement result = doc.createElement("Surface");
    m_translatedObjects[surface.handle()] = result;

    // id
    std::string name = surface.name().get();
    result.setAttribute("id", escapeName(name));

    // DLM: currently unhandled
    //Shade
    //FreestandingColumn
    //EmbeddedColumn

    if (surface.isAirWall()){
      result.setAttribute("surfaceType", "Air");
    }else{
      std::string surfaceType = surface.surfaceType();
      std::string outsideBoundaryCondition = surface.outsideBoundaryCondition();
      if (istringEqual("Wall", surfaceType)){
        if (istringEqual("Outdoors", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "ExteriorWall");
        }else if (istringEqual("Surface", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "InteriorWall");
        }else if (surface.isGroundSurface()){
          result.setAttribute("surfaceType", "UndergroundWall");
        }
      }else if (istringEqual("RoofCeiling", surfaceType)){
        if (istringEqual("Outdoors", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "Roof");
        }else if (istringEqual("Surface", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "Ceiling");
        }else if (surface.isGroundSurface()){
          result.setAttribute("surfaceType", "UndergroundCeiling");
        }
      }else if (istringEqual("Floor", surfaceType)){
        if (istringEqual("Outdoors", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "RaisedFloor");
        }else if (surface.isGroundSurface()){
          result.setAttribute("surfaceType", "UndergroundSlab"); // or SlabOnGrade?
        }else if (istringEqual("Surface", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "InteriorFloor");
        }
      }
    }

    // construction
    boost::optional<model::ConstructionBase> construction = surface.construction();
    if (construction){
      //std::string constructionName = construction->name().get();
      // todo:: translate construction
      //result.setAttribute("constructionIdRef", "constructionName");
    }

    // this space
    Transformation transformation;
    boost::optional<model::Space> space = surface.space();
    if (space){
      transformation = space->siteTransformation();

      std::string spaceName = space->name().get();
      QDomElement adjacentSpaceIdElement = doc.createElement("AdjacentSpaceId");
      result.appendChild(adjacentSpaceIdElement);
      adjacentSpaceIdElement.setAttribute("spaceIdRef", escapeName(spaceName));
    }

    // adjacent surface 
    boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
    if (adjacentSurface){
      boost::optional<model::Space> adjacentSpace = adjacentSurface->space();
      if (adjacentSpace){
        std::string adjacentSpaceName = adjacentSpace->name().get();
        QDomElement adjacentSpaceIdElement = doc.createElement("AdjacentSpaceId");
        result.appendChild(adjacentSpaceIdElement);
        adjacentSpaceIdElement.setAttribute("spaceIdRef", escapeName(adjacentSpaceName));

        // count adjacent surface as translated
        m_translatedObjects[adjacentSurface->handle()] = result;
      }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation*surface.vertices();

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = surface.grossArea();
    if (outwardNormal && area > 0){

      // get tilt, duplicate code in planar surface
      Vector3d up(0.0,0.0,1.0);
      double tiltRadians = getAngle(*outwardNormal, up);

      // get azimuth, duplicate code in planar surface
      Vector3d north(0.0,1.0,0.0);
      double azimuthRadians = getAngle(*outwardNormal, north);
      if (outwardNormal->x() < 0.0) { 
        azimuthRadians = -azimuthRadians + 2.0*boost::math::constants::pi<double>(); 
      }

      // transform vertices to face coordinates
      Transformation faceTransformation = Transformation::alignFace(vertices);
      Point3dVector faceVertices = faceTransformation.inverse()*vertices;
      BoundingBox faceBoundingBox;
      faceBoundingBox.addPoints(faceVertices);
      double width = faceBoundingBox.maxX().get() - faceBoundingBox.minX().get();
      double height = faceBoundingBox.maxY().get() - faceBoundingBox.minY().get();
      double areaCorrection = 1.0;
      if (width > 0 && height > 0){
        areaCorrection = sqrt(area/(width*height));
      }

      // pick lower left corner vertex in face coordinates
      double minY = std::numeric_limits<double>::max();
      double minX = std::numeric_limits<double>::max();
      unsigned llcIndex = 0;
      unsigned N = vertices.size();
      for(unsigned i = 0; i < N; ++i){
        OS_ASSERT(std::abs(faceVertices[i].z()) < 0.001);
        if ((minY > faceVertices[i].y()) || ((minY > faceVertices[i].y() - 0.00001) && (minX > faceVertices[i].x()))){
          llcIndex = i;
          minY = faceVertices[i].y();
          minX = faceVertices[i].x();
        }
      }
      Point3d vertex = vertices[llcIndex];

      // rectangular geometry 
      QDomElement rectangularGeometryElement = doc.createElement("RectangularGeometry");
      result.appendChild(rectangularGeometryElement);

      QDomElement azimuthElement = doc.createElement("Azimuth");
      rectangularGeometryElement.appendChild(azimuthElement);
      azimuthElement.appendChild(doc.createTextNode(QString::number(radToDeg(azimuthRadians))));

      QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
      rectangularGeometryElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));

      QDomElement tiltElement = doc.createElement("Tilt");
      rectangularGeometryElement.appendChild(tiltElement);
      tiltElement.appendChild(doc.createTextNode(QString::number(radToDeg(tiltRadians))));

      QDomElement widthElement = doc.createElement("Width");
      rectangularGeometryElement.appendChild(widthElement);
      widthElement.appendChild(doc.createTextNode(QString::number(areaCorrection*width)));

      QDomElement heightElement = doc.createElement("Height");
      rectangularGeometryElement.appendChild(heightElement);
      heightElement.appendChild(doc.createTextNode(QString::number(areaCorrection*height)));
    }

    // planar geometry
    QDomElement planarGeometryElement = doc.createElement("PlanarGeometry");
    result.appendChild(planarGeometryElement);

    QDomElement polyLoopElement = doc.createElement("PolyLoop");
    planarGeometryElement.appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
      QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
      polyLoopElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));
    }

    // translate sub surfaces
    for (const model::SubSurface& subSurface : surface.subSurfaces()){
      boost::optional<QDomElement> openingElement = translateSubSurface(subSurface, transformation, doc);
      if (openingElement){
        result.appendChild(*openingElement);
      }
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateSubSurface(const openstudio::model::SubSurface& subSurface, const openstudio::Transformation& transformation, QDomDocument& doc)
  {
    // return if already translated
    if (m_translatedObjects.find(subSurface.handle()) != m_translatedObjects.end()){
      return boost::none;
    }

    QDomElement result = doc.createElement("Opening");
    m_translatedObjects[subSurface.handle()] = result;

    // id
    std::string name = subSurface.name().get();
    result.setAttribute("id", escapeName(name));

    // construction
    boost::optional<model::ConstructionBase> construction = subSurface.construction();
    if (construction){
      //std::string constructionName = construction->name().get();
      // todo: translate construction
      // result.setAttribute("constructionIdRef", "constructionName");
    }

    // DLM: currently unhandled
    // OperableSkylight
    // SlidingDoor

    if (subSurface.isAirWall()){
      result.setAttribute("openingType", "Air");
    }else{
      std::string subSurfaceType = subSurface.subSurfaceType();
      if (istringEqual("FixedWindow", subSurfaceType)){
        result.setAttribute("openingType", "FixedWindow");
      }else if(istringEqual("OperableWindow", subSurfaceType)){
        result.setAttribute("openingType", "OperableWindow");
      }else if (istringEqual("Skylight", subSurfaceType)){
        result.setAttribute("openingType", "FixedSkylight");
      }else if (istringEqual("Door", subSurfaceType)){
        result.setAttribute("openingType", "NonSlidingDoor");
      }else if (istringEqual("OverheadDoor", subSurfaceType)){
        result.setAttribute("openingType", "NonSlidingDoor");
      }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation*subSurface.vertices();

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = subSurface.grossArea();
    if (outwardNormal && area > 0){

      // get tilt, duplicate code in planar surface
      Vector3d up(0.0,0.0,1.0);
      double tiltRadians = getAngle(*outwardNormal, up);

      // get azimuth, duplicate code in planar surface
      Vector3d north(0.0,1.0,0.0);
      double azimuthRadians = getAngle(*outwardNormal, north);
      if (outwardNormal->x() < 0.0) { 
        azimuthRadians = -azimuthRadians + 2.0*boost::math::constants::pi<double>(); 
      }

      // transform vertices to face coordinates
      Transformation faceTransformation = Transformation::alignFace(vertices);
      Point3dVector faceVertices = faceTransformation.inverse()*vertices;
      BoundingBox faceBoundingBox;
      faceBoundingBox.addPoints(faceVertices);
      double width = faceBoundingBox.maxX().get() - faceBoundingBox.minX().get();
      double height = faceBoundingBox.maxY().get() - faceBoundingBox.minY().get();
      double areaCorrection = 1.0;
      if (width > 0 && height > 0){
        areaCorrection = sqrt(area/(width*height));
      }

      // pick lower left corner vertex in face coordinates
      double minY = std::numeric_limits<double>::max();
      double minX = std::numeric_limits<double>::max();
      unsigned llcIndex = 0;
      unsigned N = vertices.size();
      for(unsigned i = 0; i < N; ++i){
        OS_ASSERT(std::abs(faceVertices[i].z()) < 0.001);
        if ((minY > faceVertices[i].y()) || ((minY > faceVertices[i].y() - 0.00001) && (minX > faceVertices[i].x()))){
          llcIndex = i;
          minY = faceVertices[i].y();
          minX = faceVertices[i].x();
        }
      }
      Point3d vertex = vertices[llcIndex];

      // rectangular geometry 
      QDomElement rectangularGeometryElement = doc.createElement("RectangularGeometry");
      result.appendChild(rectangularGeometryElement);

      QDomElement azimuthElement = doc.createElement("Azimuth");
      rectangularGeometryElement.appendChild(azimuthElement);
      azimuthElement.appendChild(doc.createTextNode(QString::number(radToDeg(azimuthRadians))));

      QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
      rectangularGeometryElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));

      QDomElement tiltElement = doc.createElement("Tilt");
      rectangularGeometryElement.appendChild(tiltElement);
      tiltElement.appendChild(doc.createTextNode(QString::number(radToDeg(tiltRadians))));

      QDomElement widthElement = doc.createElement("Width");
      rectangularGeometryElement.appendChild(widthElement);
      widthElement.appendChild(doc.createTextNode(QString::number(areaCorrection*width)));

      QDomElement heightElement = doc.createElement("Height");
      rectangularGeometryElement.appendChild(heightElement);
      heightElement.appendChild(doc.createTextNode(QString::number(areaCorrection*height)));
    }

    // planar geometry
    QDomElement planarGeometryElement = doc.createElement("PlanarGeometry");
    result.appendChild(planarGeometryElement);

    // translate vertices
    QDomElement polyLoopElement = doc.createElement("PolyLoop");
    planarGeometryElement.appendChild(polyLoopElement);
    for (const Point3d& vertex : vertices){
      QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
      polyLoopElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x())));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y())));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z())));
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateThermalZone(const openstudio::model::ThermalZone& thermalZone, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Zone");
    m_translatedObjects[thermalZone.handle()] = result;

    // id
    std::string name = thermalZone.name().get();
    result.setAttribute("id", escapeName(name));

    return result;
  }

} // gbxml
} // openstudio
