/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/AirWallMaterial.hpp"
#include "../model/AirWallMaterial_Impl.hpp"

#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"


#include <utilities/idd/IddEnums.hxx>



#include <QDomDocument>
#include <QDomElement>
#include <QThread>

namespace openstudio {
namespace bec {

  std::ostream& operator<<(std::ostream& os, const QDomElement& element)
  {
    QString str;
    QTextStream qts(&str);
    element.save(qts, 2);
    os << str.toStdString();
    return os;
  }

  ReverseTranslator::ReverseTranslator()
    : m_lengthMultiplier(1.0)
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.bec\\.ReverseTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  ReverseTranslator::~ReverseTranslator()
  {
  }

  boost::optional<openstudio::model::Model> ReverseTranslator::loadModel(const openstudio::path& path, ProgressBar* progressBar)
  {
    m_progressBar = progressBar;

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    boost::optional<openstudio::model::Model> result;

    if (openstudio::filesystem::exists(path)) {

      openstudio::filesystem::ifstream file(path, std::ios_base::binary);
      if (file.is_open()) {
        QDomDocument doc;
        doc.setContent(openstudio::filesystem::read_as_QByteArray(file));
        file.close();

        result = this->convert(doc);
      }
    }

    return result;
  }


  std::vector<LogMessage> ReverseTranslator::warnings() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ReverseTranslator::errors() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() > Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::string ReverseTranslator::escapeName(const QString& id, const QString& name) 
  {
    QString value = id;
    if (!name.isEmpty()) {
      value = name;
    }

    return value.replace(',', '-').replace(';', '-').toStdString();
  }

  boost::optional<model::Model> ReverseTranslator::convert(const QDomDocument& doc)
  {
    return translateBEC(doc.documentElement(), doc);
  }

  boost::optional<model::Model> ReverseTranslator::translateBEC(const QDomElement& element, const QDomDocument& doc)
  {
    openstudio::model::Model model;
    model.setFastNaming(true);

    // BEC attributes not mapped directly to IDF, but needed to map

    // {F, C, K, R}
    QString temperatureUnit = element.attribute("temperatureUnit"); 
    if (temperatureUnit.contains("F", Qt::CaseInsensitive)){
      m_temperatureUnit = UnitFactory::instance().createUnit("F").get();
    }else if (temperatureUnit.contains("C", Qt::CaseInsensitive)){
      m_temperatureUnit = UnitFactory::instance().createUnit("C").get();
    }else if (temperatureUnit.contains("K", Qt::CaseInsensitive)){
      m_temperatureUnit = UnitFactory::instance().createUnit("K").get();
    }else if (temperatureUnit.contains("R", Qt::CaseInsensitive)){
      m_temperatureUnit = UnitFactory::instance().createUnit("R").get();
    }else{
      LOG(Warn, "No temperature unit specified, using C");
      m_temperatureUnit = UnitFactory::instance().createUnit("C").get();
    }

    // {Kilometers, Centimeters, Millimeters, Meters, Miles, Yards, Feet, Inches}
    // TODO: still need some help with some units
    QString lengthUnit = element.attribute("lengthUnit"); 
    if (lengthUnit.contains("Kilometers", Qt::CaseInsensitive)){
      //m_lengthUnit = UnitFactory::instance().createUnit("F").get();
    }else if (lengthUnit.contains("Centimeters", Qt::CaseInsensitive)){
      //m_lengthUnit = UnitFactory::instance().createUnit("C").get();
    }else if (lengthUnit.contains("Millimeters", Qt::CaseInsensitive)){
      //m_lengthUnit = UnitFactory::instance().createUnit("K").get();
    }else if (lengthUnit.contains("Meters", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }else if (lengthUnit.contains("Miles", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("mi").get();
    }else if (lengthUnit.contains("Yards", Qt::CaseInsensitive)){
      //m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }else if (lengthUnit.contains("Feet", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("ft").get();
    }else if (lengthUnit.contains("Inches", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("in").get();
    }else{
      LOG(Warn, "No length unit specified, using Meters");
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }

    Quantity unitLength(1.0, m_lengthUnit);
    Unit targetUnit = UnitFactory::instance().createUnit("m").get();
    m_lengthMultiplier = QuantityConverter::instance().convert(unitLength, targetUnit)->value();

    // {SquareKilometers, SquareMeters, SquareCentimeters, SquareMillimeters, SquareMiles, SquareYards, SquareFeet, SquareInches}
    // TODO: still need some help with some units
    QString areaUnit = element.attribute("areaUnit"); 

    // {CubicKilometers, CubicMeters, CubicCentimeters, CubicMillimeters, CubicMiles, CubicYards, CubicFeet, CubicInches}
    // TODO: still need some help with some units
    QString volumeUnit = element.attribute("volumeUnit"); 

    // {true, false}
    QString useSIUnitsForResults = element.attribute("useSIUnitsForResults"); 
    if (useSIUnitsForResults.contains("False", Qt::CaseInsensitive)){
      m_useSIUnitsForResults = false;
    }else{
      m_useSIUnitsForResults = true;
    }

    // do materials before constructions 
    QDomNodeList materialElements = element.elementsByTagName("Material");
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Materials"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(materialElements.count()); 
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < materialElements.count(); i++){
      QDomElement materialElement = materialElements.at(i).toElement();
      boost::optional<model::ModelObject> material = translateMaterial(materialElement, doc, model);
      OS_ASSERT(material); // Krishnan, what type of error handling do you want?
      
      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do constructions before surfaces
    QDomNodeList layerElements = element.elementsByTagName("Layer");
    QDomNodeList constructionElements = element.elementsByTagName("Construction");
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(constructionElements.count()); 
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < constructionElements.count(); i++){
      QDomElement constructionElement = constructionElements.at(i).toElement();
      boost::optional<model::ModelObject> construction = translateConstruction(constructionElement, layerElements, doc, model);
      OS_ASSERT(construction); // Krishnan, what type of error handling do you want?
      
      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do window type before sub surfaces
    QDomNodeList windowTypeElements = element.elementsByTagName("WindowType");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Window Types"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(windowTypeElements.count());
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < windowTypeElements.count(); i++) {
      QDomElement windowTypeElement = windowTypeElements.at(i).toElement();
      boost::optional<model::ModelObject> construction = translateWindowType(windowTypeElement, doc, model);
      OS_ASSERT(construction); // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do schedules before loads
    QDomNodeList scheduleElements = element.elementsByTagName("Schedule");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Schedules"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(scheduleElements.count());
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < scheduleElements.count(); i++) {
      QDomElement scheduleElement = scheduleElements.at(i).toElement();
      boost::optional<model::ModelObject> schedule = translateSchedule(scheduleElement, doc, model);
      OS_ASSERT(schedule); // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do thermal zones before spaces
    QDomNodeList zoneElements = element.elementsByTagName("Zone");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(zoneElements.count());
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < zoneElements.count(); i++) {
      QDomElement zoneElement = zoneElements.at(i).toElement();
      boost::optional<model::ModelObject> zone = translateThermalZone(zoneElement, doc, model);
      OS_ASSERT(zone); // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    QDomNodeList campusElements = element.elementsByTagName("Campus");
    OS_ASSERT(campusElements.count() == 1);
    QDomElement campusElement = campusElements.at(0).toElement();
    boost::optional<model::ModelObject> facility = translateCampus(campusElement, doc, model);
    OS_ASSERT(facility); // Krishnan, what type of error handling do you want?

    model.setFastNaming(false);

    return model;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateCampus(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    openstudio::model::Facility facility = model.getUniqueModelObject<openstudio::model::Facility>();

    QDomNodeList buildingElements = element.elementsByTagName("Building");
    OS_ASSERT(buildingElements.count() == 1);

    boost::optional<model::ModelObject> building = translateBuilding(buildingElements.at(0).toElement(), doc, model);
    OS_ASSERT(building);

    QDomNodeList surfaceElements = element.elementsByTagName("Surface");
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Surfaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(surfaceElements.count()); 
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < surfaceElements.count(); ++i){

      try {
        boost::optional<model::ModelObject> surface = translateSurface(surfaceElements.at(i).toElement(), doc, model);
      }catch(const std::exception&){
        LOG(Error, "Could not translate surface " << surfaceElements.at(i).toElement());
      }
      
      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return facility;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateBuilding(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();

    QString id = element.attribute("id");
    m_idToObjectMap.insert(std::make_pair(id, building));

    QString name = element.firstChildElement("Name").toElement().text();
    building.setName(escapeName(id, name));

    QDomNodeList storyElements = element.elementsByTagName("BuildingStorey");
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Building Stories"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(storyElements.count()); 
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < storyElements.count(); ++i){
      boost::optional<model::ModelObject> story = translateBuildingStory(storyElements.at(i).toElement(), doc, model);
      OS_ASSERT(story);

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    QDomNodeList spaceElements = element.elementsByTagName("Space");
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Spaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(spaceElements.count()); 
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < spaceElements.count(); ++i){
      boost::optional<model::ModelObject> space = translateSpace(spaceElements.at(i).toElement(), doc, model);
      OS_ASSERT(space);

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return building;
  }
 
  boost::optional<model::ModelObject> ReverseTranslator::translateBuildingStory(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    openstudio::model::BuildingStory story(model);

    QString id = element.attribute("id");
    m_idToObjectMap.insert(std::make_pair(id, story));

    QString name = element.firstChildElement("Name").toElement().text();
    story.setName(escapeName(id, name));

    // DLM: we need to better support separate name from id in this translator

    // DLM: todo, translate Level

    return story;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateThermalZone(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    openstudio::model::ThermalZone zone(model);

    QString id = element.attribute("id");
    m_idToObjectMap.insert(std::make_pair(id, zone));

    QString name = element.firstChildElement("Name").toElement().text();
    zone.setName(escapeName(id, name));

    // DLM: we need to better support separate name from id in this translator

    // DLM: todo, translate setpoints

    return zone;
  }
  boost::optional<model::ModelObject> ReverseTranslator::translateSpace(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    openstudio::model::Space space(model);

    QString id = element.attribute("id");
    m_idToObjectMap.insert(std::make_pair(id, space));

    QString name = element.firstChildElement("Name").toElement().text();
    space.setName(escapeName(id, name));

    //DLM: we should be using a map of id to model object to get this, not relying on name
    QString storyId = element.attribute("buildingStoreyIdRef");
    auto storyIt = m_idToObjectMap.find(storyId);
    if (storyIt != m_idToObjectMap.end()) {
      boost::optional<model::BuildingStory> story = storyIt->second.optionalCast<model::BuildingStory>();
      if (story) {
        space.setBuildingStory(*story);
      }
    }

    // if space doesn't have story assigned should we warn the user?

    QString zoneId = element.attribute("zoneIdRef");
    auto zoneIt = m_idToObjectMap.find(zoneId);
    if (zoneIt != m_idToObjectMap.end()) {
      boost::optional<model::ThermalZone> thermalZone = zoneIt->second.optionalCast<model::ThermalZone>();
      if (thermalZone) {
        space.setThermalZone(*thermalZone);
      }
    }

    if (!space.thermalZone()) {
      // DLM: may want to revisit this
      // create a new thermal zone if none assigned
      openstudio::model::ThermalZone thermalZone(model);
      thermalZone.setName(escapeName(id, name) + " ThermalZone");
      space.setThermalZone(thermalZone);
    }

    // create a stub space type
    // DLM: is this better than nothing?
    QString spaceTypeId = element.attribute("spaceType");
    if (!spaceTypeId.isEmpty()) {
      auto spaceTypeIt = m_idToObjectMap.find(spaceTypeId);

      if (spaceTypeIt == m_idToObjectMap.end()) {
        model::SpaceType spaceType(model);
        spaceType.setName(escapeName(spaceTypeId, spaceTypeId));
        spaceTypeIt = m_idToObjectMap.insert(m_idToObjectMap.end(), std::make_pair(spaceTypeId, spaceType));
      }

      boost::optional<model::SpaceType> spaceType = spaceTypeIt->second.optionalCast<model::SpaceType>();
      if (spaceType) {
        space.setSpaceType(*spaceType);
      }
    }

    return space;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    boost::optional<model::ModelObject> result;
    std::vector<openstudio::Point3d> vertices;

    QDomNode planarGeometryElement = element.firstChildElement("PlanarGeometry");
    QDomNode polyLoopElement = planarGeometryElement.firstChildElement("PolyLoop");
    QDomNodeList cartesianPointElements = polyLoopElement.toElement().elementsByTagName("CartesianPoint");

    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coordinate");
      OS_ASSERT(coordinateElements.size() == 3);

      /* Calling these conversions every time is unnecessarily slow

      Unit targetUnit = UnitFactory::instance().createUnit("m").get();
      Quantity xQuantity(coordinateElements.at(0).toElement().text().toDouble(), m_lengthUnit);
      Quantity yQuantity(coordinateElements.at(1).toElement().text().toDouble(), m_lengthUnit);
      Quantity zQuantity(coordinateElements.at(2).toElement().text().toDouble(), m_lengthUnit);

      double x = QuantityConverter::instance().convert(xQuantity, targetUnit)->value();
      double y = QuantityConverter::instance().convert(yQuantity, targetUnit)->value();
      double z = QuantityConverter::instance().convert(zQuantity, targetUnit)->value();
      */

      double x = m_lengthMultiplier*coordinateElements.at(0).toElement().text().toDouble();
      double y = m_lengthMultiplier*coordinateElements.at(1).toElement().text().toDouble();
      double z = m_lengthMultiplier*coordinateElements.at(2).toElement().text().toDouble();

      vertices.push_back(openstudio::Point3d(x,y,z));
    }

    QString surfaceType = element.attribute("surfaceType");
    if (surfaceType.contains("Shade")) {

      openstudio::model::ShadingSurface shadingSurface(vertices, model);

      QString shadingSurfaceId = element.attribute("id");
      m_idToObjectMap.insert(std::make_pair(shadingSurfaceId, shadingSurface));

      QString shadingSurfaceName = element.firstChildElement("Name").toElement().text();
      shadingSurface.setName(escapeName(shadingSurfaceId, shadingSurfaceName));

      openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();

      std::vector<openstudio::model::ShadingSurfaceGroup> shadingSurfaceGroups = building.shadingSurfaceGroups();
      if (shadingSurfaceGroups.empty()) {
        openstudio::model::ShadingSurfaceGroup shadingSurfaceGroup(model);
        shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup);
      } else {
        shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroups[0]);
      }

      result = shadingSurface;

    }else if (surfaceType.contains("FreestandingColumn") || surfaceType.contains("EmbeddedColumn")){
  
      // do not handle these
      return boost::none;

    }else{

      QDomNodeList adjacentSpaceElements = element.elementsByTagName("AdjacentSpaceId");
      if (adjacentSpaceElements.size() == 0){
        LOG(Warn, "Surface has no adjacent spaces, will not be translated.");
        return boost::none;
      } else if (adjacentSpaceElements.size() == 2){
        QString spaceId1 = adjacentSpaceElements.at(0).toElement().attribute("spaceIdRef");
        QString spaceId2 = adjacentSpaceElements.at(1).toElement().attribute("spaceIdRef");
        if (spaceId1 == spaceId2){
          LOG(Warn, "Surface has two adjacent spaces which are the same space '" << toString(spaceId2) << "', will not be translated.");
          return boost::none;
        }
      }else if (adjacentSpaceElements.size() > 2){
        LOG(Error, "Surface has more than 2 adjacent surfaces, will not be translated.");
        return boost::none;
      }

      openstudio::model::Surface surface(vertices, model);

      QString surfaceId = element.attribute("id");
      m_idToObjectMap.insert(std::make_pair(surfaceId, surface));

      QString surfaceName = element.firstChildElement("Name").toElement().text();
      surface.setName(escapeName(surfaceId, surfaceName));

      QString exposedToSun = element.attribute("exposedToSun");

      // set surface type
      // wall types
      if (surfaceType.contains("ExteriorWall")){
        surface.setSurfaceType("Wall"); 
      }else if (surfaceType.contains("InteriorWall")){
        surface.setSurfaceType("Wall"); 
      }else if (surfaceType.contains("UndergroundWall")){
        surface.setSurfaceType("Wall"); 
      // roof types
      }else if (surfaceType.contains("Roof")){
        surface.setSurfaceType("RoofCeiling"); 
      }else if (surfaceType.contains("Ceiling")){
        surface.setSurfaceType("RoofCeiling");
      }else if (surfaceType.contains("UndergroundCeiling")){
        surface.setSurfaceType("RoofCeiling");
      // floor types
      }else if (surfaceType.contains("UndergroundSlab")){
        surface.setSurfaceType("Floor"); 
      }else if (surfaceType.contains("SlabOnGrade")){
        surface.setSurfaceType("Floor"); 
      }else if (surfaceType.contains("InteriorFloor")){
        surface.setSurfaceType("Floor"); 
      }else if (surfaceType.contains("RaisedFloor")){
        surface.setSurfaceType("Floor"); 
      } else if (surfaceType.contains("ExposedFloor")) {
        surface.setSurfaceType("Floor");
      } else if (surfaceType.contains("Air")) {
        // this type can be wall, roof, or floor.  just use default surface type based on vertices.
      }


      // this type can be wall, roof, or floor.  just use default surface type.
      if (surfaceType.contains("Air")){
        // TODO: set air wall construction
      }

      // set boundary conditions
      if (exposedToSun.contains("true")){
        surface.setOutsideBoundaryCondition("Outdoors");
        surface.setSunExposure("SunExposed");
        surface.setWindExposure("WindExposed");
      }else if (surfaceType.contains("InteriorWall")){
        surface.setOutsideBoundaryCondition("Adiabatic");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      }else if (surfaceType.contains("SlabOnGrade")){
        surface.setOutsideBoundaryCondition("Ground");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      }

      // if air wall
      if (surfaceType.contains("Air")) {
        boost::optional<model::Construction> airWall;

        for (const auto& construction : model.getConcreteModelObjects<model::Construction>()) {
          if ((construction.numLayers() == 1) && (construction.isModelPartition())) {
            model::MaterialVector layers = construction.layers();
            OS_ASSERT(layers.size() == 1u);
            if (layers[0].optionalCast<model::AirWallMaterial>()) {
              airWall = construction;
              break;
            }
          }
        }
        if (!airWall) {
          airWall = model::Construction(model);
          model::AirWallMaterial airWallMaterial(model);
          airWall->setLayer(airWallMaterial);
        }
        surface.setConstruction(*airWall);

        // don't translate subsurfaces of air walls?

      } else {
        // not air wall

        // translate construction
        QString constructionIdRef = element.attribute("constructionIdRef");
        auto constructionIt = m_idToObjectMap.find(constructionIdRef);
        if (constructionIt != m_idToObjectMap.end()) {
          boost::optional<model::ConstructionBase> construction = constructionIt->second.optionalCast<model::ConstructionBase>();
          if (construction) {
            surface.setConstruction(*construction);
          }
        }

        // translate subSurfaces
        QDomNodeList subSurfaceElements = element.elementsByTagName("Opening");
        for (int i = 0; i < subSurfaceElements.count(); ++i) {
          try {
            boost::optional<model::ModelObject> subSurface = translateSubSurface(subSurfaceElements.at(i).toElement(), doc, surface);
          } catch (const std::exception&) {
            LOG(Error, "Could not translate sub surface " << subSurfaceElements.at(i).toElement());
          }
        }
      }

      // adjacent surfaces
      QString spaceId = adjacentSpaceElements.at(0).toElement().attribute("spaceIdRef");
      auto spaceIt = m_idToObjectMap.find(spaceId);
      if (spaceIt != m_idToObjectMap.end()) {
        boost::optional<model::Space> space = spaceIt->second.optionalCast<openstudio::model::Space>();
        if (space) {
          surface.setSpace(*space);
        }
      }

      boost::optional<openstudio::model::Space> space = surface.space();
      if (!space) {
        LOG(Error, "Surface '" << surface.name().get() << "' is not assigned to a space");
      }

      if (space && adjacentSpaceElements.size() == 2) {

        QString adjacentSpaceId = adjacentSpaceElements.at(1).toElement().attribute("spaceIdRef");
        auto adjacentSpaceIt = m_idToObjectMap.find(adjacentSpaceId);
        if (adjacentSpaceIt != m_idToObjectMap.end()) {
          boost::optional<model::Space> adjacentSpace = adjacentSpaceIt->second.optionalCast<openstudio::model::Space>();
          if (adjacentSpace) {
            // DLM: we have issues if interior ceilings/floors are mislabeled, override surface type for adjacent surfaces 
            // http://code.google.com/p/cbecc/issues/detail?id=471
            std::string currentSurfaceType = surface.surfaceType();
            surface.assignDefaultSurfaceType();
            if (currentSurfaceType != surface.surfaceType()) {
              LOG(Warn, "Changing surface type from '" << currentSurfaceType << "' to '" << surface.surfaceType() << "' for surface '" << surface.name().get() << "'");
            }

            // clone the surface and sub surfaces and reverse vertices
            boost::optional<openstudio::model::Surface> otherSurface = surface.createAdjacentSurface(*adjacentSpace);
            if (!otherSurface) {
              LOG(Error, "Could not create adjacent surface in adjacent space '" << adjacentSpace->name().get() << "' for surface '" << surface.name().get() << "' in space '" << space->name().get() << "'");
            }
          }
        }
      }
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSubSurface(const QDomElement& element, const QDomDocument& doc, openstudio::model::Surface& surface)
  {
    openstudio::model::Model model = surface.model();

    boost::optional<model::ModelObject> result;
      
    std::vector<openstudio::Point3d> vertices;

    QDomNode planarGeometryElement = element.firstChildElement("PlanarGeometry");
    QDomNode polyLoopElement = planarGeometryElement.firstChildElement("PolyLoop");
    QDomNodeList cartesianPointElements = polyLoopElement.toElement().elementsByTagName("CartesianPoint");

    for (int i = 0; i < cartesianPointElements.count(); i++){
      QDomNodeList coordinateElements = cartesianPointElements.at(i).toElement().elementsByTagName("Coordinate");
      OS_ASSERT(coordinateElements.size() == 3);

      /* Calling these conversions every time is unnecessarily slow

      Unit targetUnit = UnitFactory::instance().createUnit("m").get();
      Quantity xQuantity(coordinateElements.at(0).toElement().text().toDouble(), m_lengthUnit);
      Quantity yQuantity(coordinateElements.at(1).toElement().text().toDouble(), m_lengthUnit);
      Quantity zQuantity(coordinateElements.at(2).toElement().text().toDouble(), m_lengthUnit);

      double x = QuantityConverter::instance().convert(xQuantity, targetUnit)->value();
      double y = QuantityConverter::instance().convert(yQuantity, targetUnit)->value();
      double z = QuantityConverter::instance().convert(zQuantity, targetUnit)->value();
      */
          
      double x = m_lengthMultiplier*coordinateElements.at(0).toElement().text().toDouble();
      double y = m_lengthMultiplier*coordinateElements.at(1).toElement().text().toDouble();
      double z = m_lengthMultiplier*coordinateElements.at(2).toElement().text().toDouble();

      vertices.push_back(openstudio::Point3d(x,y,z));
    }

    openstudio::model::SubSurface subSurface(vertices, model);
    subSurface.setSurface(surface);

    QString id = element.attribute("id");
    m_idToObjectMap.insert(std::make_pair(id, subSurface));

    QString name = element.firstChildElement("Name").toElement().text();
    subSurface.setName(escapeName(id, name));

    result = subSurface;

    // translate openingType
    QString openingType = element.attribute("openingType");
    if (openingType.contains("FixedWindow")) {
      subSurface.setSubSurfaceType("FixedWindow");
    } else if (openingType.contains("OperableWindow")) {
      subSurface.setSubSurfaceType("OperableWindow");
    } else if (openingType.contains("FixedSkylight")) {
      subSurface.setSubSurfaceType("Skylight");
    } else if (openingType.contains("OperableSkylight")) {
      subSurface.setSubSurfaceType("Skylight");
    } else if (openingType.contains("SlidingDoor")) {
      subSurface.setSubSurfaceType("GlassDoor");
    } else if (openingType.contains("NonSlidingDoor")) {
      subSurface.setSubSurfaceType("Door");
    } else if (openingType.contains("Air")) {
      // use default sub surface type?
    }

    // if air wall
    if (openingType.contains("Air")) {
      boost::optional<model::Construction> airWall;

      for (const auto& construction : model.getConcreteModelObjects<model::Construction>()) {
        if ((construction.numLayers() == 1) && (construction.isModelPartition())) {
          model::MaterialVector layers = construction.layers();
          OS_ASSERT(layers.size() == 1u);
          if (layers[0].optionalCast<model::AirWallMaterial>()) {
            airWall = construction;
            break;
          }
        }
      }
      if (!airWall) {
        airWall = model::Construction(model);
        model::AirWallMaterial airWallMaterial(model);
        airWall->setLayer(airWallMaterial);
      }
      subSurface.setConstruction(*airWall);

    } else {

      // translate construction
      QString constructionIdRef = element.attribute("constructionIdRef");
      if (constructionIdRef.isEmpty()) {
        constructionIdRef = element.attribute("windowTypeIdRef");
      }
      auto constructionIt = m_idToObjectMap.find(constructionIdRef);
      if (constructionIt != m_idToObjectMap.end()) {
        boost::optional<model::ConstructionBase> construction = constructionIt->second.optionalCast<model::ConstructionBase>();
        if (construction) {
          subSurface.setConstruction(*construction);
        }
      }
    }

    // todo: translate "interiorShadeType", "exteriorShadeType", "windowTypeIdRef", and other properties of the opening


    return result;
  }

} // bec
} // openstudio
