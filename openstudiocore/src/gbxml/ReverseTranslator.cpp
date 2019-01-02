/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
#include "../model/AdditionalProperties.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include <utilities/idd/IddEnums.hxx>


#include <QDomDocument>
#include <QDomElement>
#include <QThread>

namespace openstudio {
namespace gbxml {

  std::ostream& operator<<(std::ostream& os, const QDomElement& element)
  {
    QString str;
    QTextStream qts(&str);
    element.save(qts, 2);
    os << str.toStdString();
    return os;
  }

  ReverseTranslator::ReverseTranslator()
    : m_nonBaseMultiplier(1.0), m_lengthMultiplier(1.0)
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.gbxml\\.ReverseTranslator"));
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

    m_idToObjectMap.clear();

    boost::optional<openstudio::model::Model> result;

    if (openstudio::filesystem::exists(path)){

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
    if (!name.isEmpty()){
      value = name;
    }

    return value.replace(',', '-').replace(';', '-').toStdString();
  }

  boost::optional<model::Model> ReverseTranslator::convert(const QDomDocument& doc)
  {
    return translateGBXML(doc.documentElement(), doc);
  }

  boost::optional<model::Model> ReverseTranslator::translateGBXML(const QDomElement& element, const QDomDocument& doc)
  {
    openstudio::model::Model model;
    model.setFastNaming(true);

    // gbXML attributes not mapped directly to IDF, but needed to map

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
      m_nonBaseMultiplier = 1000.0;
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }else if (lengthUnit.contains("Centimeters", Qt::CaseInsensitive)){
      m_nonBaseMultiplier = 1.0e-2;
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }else if (lengthUnit.contains("Millimeters", Qt::CaseInsensitive)){
      m_nonBaseMultiplier = 1.0e-3;
      m_lengthUnit = UnitFactory::instance().createUnit("K").get();
    }else if (lengthUnit.contains("Meters", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }else if (lengthUnit.contains("Miles", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("mi").get();
    }else if (lengthUnit.contains("Yards", Qt::CaseInsensitive)){
      m_nonBaseMultiplier = 3.0;
      m_lengthUnit = UnitFactory::instance().createUnit("ft").get();
    }else if (lengthUnit.contains("Feet", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("ft").get();
    }else if (lengthUnit.contains("Inches", Qt::CaseInsensitive)){
      m_lengthUnit = UnitFactory::instance().createUnit("in").get();
    }else{
      LOG(Warn, "No length unit specified, using Meters");
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }

    Quantity unitLength(m_nonBaseMultiplier, m_lengthUnit);
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
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Window Types"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(windowTypeElements.count());
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < windowTypeElements.count(); i++){
      QDomElement windowTypeElement = windowTypeElements.at(i).toElement();
      boost::optional<model::ModelObject> construction = translateWindowType(windowTypeElement, doc, model);
      OS_ASSERT(construction); // Krishnan, what type of error handling do you want?

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do schedules before loads
    QDomNodeList scheduleElements = element.elementsByTagName("Schedule");
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Schedules"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(scheduleElements.count());
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < scheduleElements.count(); i++){
      QDomElement scheduleElement = scheduleElements.at(i).toElement();
      boost::optional<model::ModelObject> schedule = translateSchedule(scheduleElement, doc, model);
      OS_ASSERT(schedule); // Krishnan, what type of error handling do you want?

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do thermal zones before spaces
    QDomNodeList zoneElements = element.elementsByTagName("Zone");
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(zoneElements.count());
      m_progressBar->setValue(0);
    }

    for (int i = 0; i < zoneElements.count(); i++){
      QDomElement zoneElement = zoneElements.at(i).toElement();
      boost::optional<model::ModelObject> zone = translateThermalZone(zoneElement, doc, model);
      OS_ASSERT(zone); // Krishnan, what type of error handling do you want?

      if (m_progressBar){
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

    // import CADObjectId
    QDomNodeList cadObjectIdElements = element.elementsByTagName("CADObjectId");
    if (cadObjectIdElements.size() >= 1){
      // TODO: import multiple CADObjectIds
      translateCADObjectId(cadObjectIdElements.at(0).toElement(), doc, zone);
    }

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
    if (storyIt != m_idToObjectMap.end()){
      boost::optional<model::BuildingStory> story = storyIt->second.optionalCast<model::BuildingStory>();
      if (story){
        space.setBuildingStory(*story);
      }
    }

    // if space doesn't have story assigned should we warn the user?

    QString zoneId = element.attribute("zoneIdRef");
    auto zoneIt = m_idToObjectMap.find(zoneId);
    if (zoneIt != m_idToObjectMap.end()){
      boost::optional<model::ThermalZone> thermalZone = zoneIt->second.optionalCast<model::ThermalZone>();
      if (thermalZone){
        space.setThermalZone(*thermalZone);
      }
    }

    if (!space.thermalZone()){
      // DLM: may want to revisit this
      // create a new thermal zone if none assigned
      openstudio::model::ThermalZone thermalZone(model);
      thermalZone.setName(escapeName(id, name) + " ThermalZone");
      space.setThermalZone(thermalZone);
    }

    // create a stub space type
    // DLM: is this better than nothing?
    QString spaceTypeId = element.attribute("spaceType");
    if (!spaceTypeId.isEmpty()){
      auto spaceTypeIt = m_idToObjectMap.find(spaceTypeId);

      if (spaceTypeIt == m_idToObjectMap.end()){
        model::SpaceType spaceType(model);
        spaceType.setName(escapeName(spaceTypeId, spaceTypeId));
        spaceTypeIt = m_idToObjectMap.insert(m_idToObjectMap.end(), std::make_pair(spaceTypeId, spaceType));
      }

      boost::optional<model::SpaceType> spaceType = spaceTypeIt->second.optionalCast<model::SpaceType>();
      if (spaceType){
        space.setSpaceType(*spaceType);
      }
    }

    // import CADObjectId
    QDomNodeList cadObjectIdElements = element.elementsByTagName("CADObjectId");
    if (cadObjectIdElements.size() >= 1){
      // TODO: import multiple CADObjectIds
      translateCADObjectId(cadObjectIdElements.at(0).toElement(), doc, space);
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
    if (surfaceType.contains("Shade")){

      openstudio::model::ShadingSurface shadingSurface(vertices, model);

      QString shadingSurfaceId = element.attribute("id");
      m_idToObjectMap.insert(std::make_pair(shadingSurfaceId, shadingSurface));

      QString shadingSurfaceName = element.firstChildElement("Name").toElement().text();
      shadingSurface.setName(escapeName(shadingSurfaceId, shadingSurfaceName));

      openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();

      std::vector<openstudio::model::ShadingSurfaceGroup> shadingSurfaceGroups = building.shadingSurfaceGroups();
      if (shadingSurfaceGroups.empty()){
        openstudio::model::ShadingSurfaceGroup shadingSurfaceGroup(model);
        shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup);
      }else{
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
      }else if (surfaceType.contains("ExposedFloor")){
        surface.setSurfaceType("Floor");
      } else if (surfaceType.contains("Air")){
        // this type can be wall, roof, or floor.  just use default surface type based on vertices.
      }

      // set boundary conditions
      if (exposedToSun.contains("true")){
        surface.setOutsideBoundaryCondition("Outdoors");
        surface.setSunExposure("SunExposed");
        surface.setWindExposure("WindExposed");
      }else if (surfaceType.contains("Interior")){
        surface.setOutsideBoundaryCondition("Adiabatic");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      } else if (surfaceType.contains("Ceiling")) {
        surface.setOutsideBoundaryCondition("Adiabatic");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      } else if (surfaceType.contains("SlabOnGrade")) {
        surface.setOutsideBoundaryCondition("Ground");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      } else if (surfaceType.contains("Underground")) {
        surface.setOutsideBoundaryCondition("Ground");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      }

      result = surface;

      // if air wall
      if (surfaceType.contains("Air")){
        boost::optional<model::Construction> airWall;

        for (const auto& construction : model.getConcreteModelObjects<model::Construction>()){
          if ((construction.numLayers() == 1) && (construction.isModelPartition())) {
            model::MaterialVector layers = construction.layers();
            OS_ASSERT(layers.size() == 1u);
            if (layers[0].optionalCast<model::AirWallMaterial>()){
              airWall = construction;
              break;
            }
          }
        }
        if (!airWall){
          airWall = model::Construction(model);
          model::AirWallMaterial airWallMaterial(model);
          airWall->setLayer(airWallMaterial);
        }
        surface.setConstruction(*airWall);

        // don't translate subsurfaces of air walls?

      } else{
        // not air wall

        // translate construction
        QString constructionIdRef = element.attribute("constructionIdRef");
        auto constructionIt = m_idToObjectMap.find(constructionIdRef);
        if (constructionIt != m_idToObjectMap.end()){
          boost::optional<model::ConstructionBase> construction = constructionIt->second.optionalCast<model::ConstructionBase>();
          if (construction){
            surface.setConstruction(*construction);
          }
        }

        // translate subSurfaces
        QDomNodeList subSurfaceElements = element.elementsByTagName("Opening");
        for (int i = 0; i < subSurfaceElements.count(); ++i){
          try {
            boost::optional<model::ModelObject> subSurface = translateSubSurface(subSurfaceElements.at(i).toElement(), doc, surface);
          } catch (const std::exception&){
            LOG(Error, "Could not translate sub surface " << subSurfaceElements.at(i).toElement());
          }
        }
      }

      // adjacent surfaces
      QString spaceId = adjacentSpaceElements.at(0).toElement().attribute("spaceIdRef");
      auto spaceIt = m_idToObjectMap.find(spaceId);
      if (spaceIt != m_idToObjectMap.end()){
        boost::optional<model::Space> space = spaceIt->second.optionalCast<openstudio::model::Space>();
        if (space){
          surface.setSpace(*space);
        }
      }

      boost::optional<openstudio::model::Space> space = surface.space();
      if (!space){
        LOG(Error, "Surface '" << surface.name().get() << "' is not assigned to a space");
      }

      if (space && adjacentSpaceElements.size() == 2){

        // From the GbXML Schema (v6.01):
        //
        // First AdjacentSpaceId entered will determine how the referenced construction layers are ordered with the first construction layer 
        // being in contact with the outside or 2nd space listed and the last layer in contact with the first space listed. 
        //
        // References the ID of a Space that is bounded by this surface. First AdjacentSpaceId entered will determine how the referenced construction 
        // layers are ordered with the first construction layer being in contact with the outside or 2nd space listed and the last layer in contact with 
        // the first space listed. The outward normal of the surface, as defined by the right hand rule of the coordinates in the planar geometry element, 
        // is always pointing away from the first AdjacentSpaceID listed.
        //
        // With interior horizontal surfaces, this attribute can distinguish between ceiling and floor surfaces to avoid double-counting of floor areas, etc. 
        // If not present, the surface type can be assumed based on the description of the surface type enums. When the surfaceTypeEnum is provided and the 
        // surface attributes (i.e. adjacency, tilt angle) do not match the enumeration's description, the enumeration should have precedence.

        QString adjacentSpaceId = adjacentSpaceElements.at(1).toElement().attribute("spaceIdRef");
        auto adjacentSpaceIt = m_idToObjectMap.find(adjacentSpaceId);
        if (adjacentSpaceIt != m_idToObjectMap.end()) {
          boost::optional<model::Space> adjacentSpace = adjacentSpaceIt->second.optionalCast<openstudio::model::Space>();
          if (adjacentSpace) {

            // DLM: we have issues if interior ceilings/floors are mislabeled, override surface type for adjacent surfaces
            // http://code.google.com/p/cbecc/issues/detail?id=471

            // the surfaceType at this point has been set based on the surfaceType attribute of the surface
            // if there are two spaces, we do not know which copy of the surface should go into which space
            // in the case of a floor/ceiling pair, this can lead to putting the floor in the bottom space and the ceiling in the top space rather than vice versa
            // in the case of interior walls, this can lead to incorrect surface normals (e.g. wall with right-pointing normal placed in the right space instead of left)
            // we can try to use the adjacent space attribute surfaceType to put floors and ceilings in the right space
            // we cannot ensure that wall normals are handled correctly, that will require a convention that vertices are not reversed in the first space listed or additional attribute
            std::string gbXMLSurfaceType = surface.surfaceType();

            // we now assign the default surface type based on outward normal
            // this is needed in case floor vertices (i.e. normal down) are provided for a surface labeled ceiling
            surface.assignDefaultSurfaceType();

            // currentSurfaceType is the surface type assigned by vertices
            std::string currentSurfaceType = surface.surfaceType();

            // spaceSurfaceType is the surfaceType that should be in the first space
            QString spaceSurfaceType = adjacentSpaceElements.at(0).toElement().attribute("surfaceType");

            // spaceSurfaceType is the surfaceType that should be in the second space
            QString adjacentSpaceSurfaceType = adjacentSpaceElements.at(1).toElement().attribute("surfaceType");

            // we will mark figuredOut as true if we are sure of our assignment, otherwise we will log a warning
            bool figuredOut = false;

            // set reverseConstruction if construction should be applied to the surface created in adjacent space
            bool reverseConstruction = false;
            
            if (spaceSurfaceType.isEmpty() && adjacentSpaceSurfaceType.isEmpty()) {
              // this is ok but gives us no new information, no warning issued

            } else if (spaceSurfaceType.isEmpty() && !adjacentSpaceSurfaceType.isEmpty()) {

              // if one adjacent space surfaceType is given then the other should be too
              LOG(Warn, "Only one adjacent surfaceType listed for '" << surface.name().get() << "'");

            } else if (!spaceSurfaceType.isEmpty() && adjacentSpaceSurfaceType.isEmpty()) {

              // if one adjacent surfaceType is given then the other should be too
              LOG(Warn, "Only one adjacent surfaceType listed for '" << surface.name().get() << "'");

            } else {

              // have both spaceSurfaceType and adjacentSpaceSurfaceType

              if (currentSurfaceType == "RoofCeiling" || currentSurfaceType == "Floor") {

                // both the spaceSurfaceType and adjacentSpaceSurfaceType should be either Ceiling or InteriorFloor
                if (!((spaceSurfaceType == "InteriorFloor" || spaceSurfaceType == "Ceiling") && (adjacentSpaceSurfaceType == "InteriorFloor" || adjacentSpaceSurfaceType == "Ceiling"))) {

                  // one of the spaces lists this as a wall or some other surfaceType
                  // we could try to reapply the surfaceType from the gbXML back here, but then we would be in the same problem as before
                  // at least now we have a surface type that matches the vertex outward normal
                  LOG(Warn, "Adjacent surfaceTypes '" << toString(spaceSurfaceType) << "' and  '" << toString(adjacentSpaceSurfaceType) << "' listed for '" << surface.name().get() << "' do not match vertices");

                } else if (spaceSurfaceType == adjacentSpaceSurfaceType) {

                  // both spaceSurfaceType and adjacentSpaceSurfaceType are either InteriorFloor or Ceiling, not allowed
                  LOG(Warn, "Duplicate surfaceType '" << toString(spaceSurfaceType) << "' listed for '" << surface.name().get() << "'");

                } else {

                  if (currentSurfaceType == "Floor") {
                    // vertices indicate floor, outward normal is down
                    if (spaceSurfaceType == "InteriorFloor") {
                      // No changes, floor should go into first space
                      figuredOut = true;
                    } else if (spaceSurfaceType == "Ceiling") {
                      // Swap roles of space and adjacentSpace, floor should go into second space
                      surface.setSpace(*adjacentSpace);
                      auto temp = space;
                      space = adjacentSpace;
                      adjacentSpace = temp;
                      figuredOut = true;

                      // Schema says, "The outward normal of the surface, as defined by the right hand rule of the coordinates in the planar geometry element, 
                      // is always pointing away from the first AdjacentSpaceID listed." but this does not match surfaceType in the first AdjacentSpaceID
                      LOG(Warn, "Outward normal for '" << surface.name().get() << "' does not match surfaceType '" << toString(spaceSurfaceType) << "' attribute of first AdjacentSpaceID");

                      // construction listed in order for first space which is now the adjacent space
                      reverseConstruction = true;
                    }
                  } else if (currentSurfaceType == "RoofCeiling") {
                    // vertices indicate ceiling, outward normal is up
                    if (spaceSurfaceType == "Ceiling") {
                      // No changes, ceiling should go into first space
                      figuredOut = true;
                    } else if (spaceSurfaceType == "InteriorFloor") {
                      // Swap roles of space and adjacentSpace, ceiling should go into second space
                      surface.setSpace(*adjacentSpace);
                      auto temp = space;
                      space = adjacentSpace;
                      adjacentSpace = temp;
                      figuredOut = true;

                      // Schema says, "The outward normal of the surface, as defined by the right hand rule of the coordinates in the planar geometry element, 
                      // is always pointing away from the first AdjacentSpaceID listed." but this does not match surfaceType in the first AdjacentSpaceID
                      LOG(Warn, "Outward normal for '" << surface.name().get() << "' does not match surfaceType attribute '" << toString(spaceSurfaceType) << "' of first AdjacentSpaceID");

                      // construction listed in order for first space which is now the adjacent space
                      reverseConstruction = true;
                    }
                  }
                }
              } else {

                // currentSurfaceType is Wall, we can't be sure it is in correct space (with normal pointing out) but don't issue warning
                // need to have either 1) convention that vertices are not reversed in first space, 2) some new attribute (e.g. 'reversed=true/false'), or 3) heuristic checking outward normal
                // if doing a hueristic, best to run at the end after all other geometry is created so you can tell which way is out of the space (all surfaces should point out of the space)
              }
            }
                
            // if we changed surface type and didn't figure out if that is ok, issue warning
            if (!figuredOut) {
              if (currentSurfaceType != gbXMLSurfaceType) {
                LOG(Warn, "Changing surface type from '" << gbXMLSurfaceType << "' to '" << currentSurfaceType << "' for surface '" << surface.name().get() << "'");
              }
            }

            // clone the surface and sub surfaces and reverse vertices
            boost::optional<openstudio::model::Surface> otherSurface = surface.createAdjacentSurface(*adjacentSpace);
            if (otherSurface) {
              if (reverseConstruction) {
                boost::optional<openstudio::model::ConstructionBase> construction = surface.construction();
                if (construction) {
                  otherSurface->setConstruction(*construction);
                  surface.resetConstruction(); // will be inherited from the adjacent surface
                }
              }
            }else{
              LOG(Error, "Could not create adjacent surface in adjacent space '" << adjacentSpace->name().get() << "' for surface '" << surface.name().get() << "' in space '" << space->name().get() << "'");
            }


          }
        } else {
          LOG(Error, "Could not find adjacent space '" << adjacentSpaceId.toStdString() << "' for surface '" << surface.name().get() << "'");
        }
      }
    }

    OS_ASSERT(result);

    // import CADObjectId
    QDomNodeList cadObjectIdElements = element.elementsByTagName("CADObjectId");
    if (cadObjectIdElements.size() >= 1){
      // TODO: import multiple CADObjectIds
      translateCADObjectId(cadObjectIdElements.at(0).toElement(), doc, *result);

      if (result->optionalCast<model::Surface>()){
        boost::optional<openstudio::model::Surface> otherSurface = result->cast<model::Surface>().adjacentSurface();
        if (otherSurface) {
          translateCADObjectId(cadObjectIdElements.at(0).toElement(), doc, *otherSurface);
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
    if (openingType.contains("FixedWindow")){
      subSurface.setSubSurfaceType("FixedWindow");
    }else if (openingType.contains("OperableWindow")){
      subSurface.setSubSurfaceType("OperableWindow");
    }else if (openingType.contains("FixedSkylight")){
      subSurface.setSubSurfaceType("Skylight");
    }else if (openingType.contains("OperableSkylight")){
      subSurface.setSubSurfaceType("Skylight");
    }else if (openingType.contains("NonSlidingDoor")){ // do before testing contains door
      subSurface.setSubSurfaceType("Door");
    }else if (openingType.contains("SlidingDoor")){
      subSurface.setSubSurfaceType("GlassDoor");
    } else if (openingType.contains("Air")){
      // use default sub surface type?
    }

    // if air wall
    if (openingType.contains("Air")){
      boost::optional<model::Construction> airWall;

      for (const auto& construction : model.getConcreteModelObjects<model::Construction>()){
        if ((construction.numLayers() == 1) && (construction.isModelPartition())) {
          model::MaterialVector layers = construction.layers();
          OS_ASSERT(layers.size() == 1u);
          if (layers[0].optionalCast<model::AirWallMaterial>()){
            airWall = construction;
            break;
          }
        }
      }
      if (!airWall){
        airWall = model::Construction(model);
        model::AirWallMaterial airWallMaterial(model);
        airWall->setLayer(airWallMaterial);
      }
      subSurface.setConstruction(*airWall);

    } else{

      // translate construction
      QString constructionIdRef = element.attribute("constructionIdRef");
      if (constructionIdRef.isEmpty()){
        constructionIdRef = element.attribute("windowTypeIdRef");
      }
      auto constructionIt = m_idToObjectMap.find(constructionIdRef);
      if (constructionIt != m_idToObjectMap.end()){
        boost::optional<model::ConstructionBase> construction = constructionIt->second.optionalCast<model::ConstructionBase>();
        if (construction){
          subSurface.setConstruction(*construction);
        }
      }
    }

    // todo: translate "interiorShadeType", "exteriorShadeType", and other properties of the opening

    // import CADObjectId
    QDomNodeList cadObjectIdElements = element.elementsByTagName("CADObjectId");
    if (cadObjectIdElements.size() >= 1){
      // TODO: import multiple CADObjectIds
      translateCADObjectId(cadObjectIdElements.at(0).toElement(), doc, subSurface);

      boost::optional<openstudio::model::SubSurface> otherSubSurface = subSurface.adjacentSubSurface();
      if (otherSubSurface) {
        translateCADObjectId(cadObjectIdElements.at(0).toElement(), doc, *otherSubSurface);
      }
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCADObjectId(const QDomElement& element, const QDomDocument& doc, openstudio::model::ModelObject& modelObject)
  {
    model::AdditionalProperties result = modelObject.additionalProperties();

    QString cadObjectId = element.text();
    if (!cadObjectId.isEmpty()) {
      result.setFeature("CADObjectId", toString(cadObjectId));

      QString programIdRef = element.attribute("programIdRef");
      if (!programIdRef.isEmpty()) {
        result.setFeature("programIdRef", toString(programIdRef));
      }
    }

    return result;
  }

} // gbxml
} // openstudio
