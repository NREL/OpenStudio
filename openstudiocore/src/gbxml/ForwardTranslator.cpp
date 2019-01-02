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
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/ScheduleConstant.hpp"
#include "../model/ScheduleConstant_Impl.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/AdditionalProperties.hpp"

#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/EulerAngles.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/time/DateTime.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"

#include <OpenStudio.hxx>

#include <boost/math/constants/constants.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QThread>

#include <regex>

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

    openstudio::filesystem::ofstream file(path, std::ios_base::binary);
    if (file.is_open()){
      openstudio::filesystem::write(file, doc->toString(2));
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
    QString result;
    if (std::regex_match(name, std::regex("^\\d.*"))) {
      result = toQString("id_" + name);
    } else {
      result = toQString(name);
    }
    result.replace(" ", "_");
    result.replace("(", "_");
    result.replace(")", "_");
    result.replace("[", "_");
    result.replace("]", "_");
    result.replace("{", "_");
    result.replace("}", "_");
    result.replace("/", "_");
    result.replace("\\", "_");
    //result.replace("-", "_"); // ok
    //result.replace(".", "_"); // ok
    result.replace(":", "_");
    result.replace(";", "_");
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
    gbXMLElement.setAttribute("xsi:schemaLocation", "http://www.gbxml.org/schema http://gbxml.org/schema/6-01/GreenBuildingXML_Ver6.01.xsd");
    gbXMLElement.setAttribute("temperatureUnit", "C");
    gbXMLElement.setAttribute("lengthUnit", "Meters");
    gbXMLElement.setAttribute("areaUnit", "SquareMeters");
    gbXMLElement.setAttribute("volumeUnit", "CubicMeters");
    gbXMLElement.setAttribute("useSIUnitsForResults", "true");
    gbXMLElement.setAttribute("version", "6.01");
    gbXMLElement.setAttribute("SurfaceReferenceLocation", "Centerline");

    boost::optional<model::Facility> facility = model.getOptionalUniqueModelObject<model::Facility>();
    if (facility){
      boost::optional<QDomElement> campusElement = translateFacility(*facility, doc);
      if (campusElement){
        gbXMLElement.appendChild(*campusElement);
      }
    }

    // do constructions
    std::vector<model::ConstructionBase> constructionBases = model.getModelObjects<model::ConstructionBase>();

    // sort by is opaque so we get constructions before window types
    std::sort(constructionBases.begin(), constructionBases.end(), [](const model::ConstructionBase& a, const model::ConstructionBase& b) {
      if (a.isOpaque() && !b.isOpaque()){
        return true;
      }
      return false;
    });
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)constructionBases.size());
      m_progressBar->setValue(0);
    }

    for (const model::ConstructionBase& constructionBase : constructionBases){
      boost::optional<QDomElement> constructionElement = translateConstructionBase(constructionBase, doc);
      if (constructionElement){
        gbXMLElement.appendChild(*constructionElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do materials
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Materials"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)2*m_materials.size());
      m_progressBar->setValue(0);
    }

    for (const model::Material& material : m_materials){
      boost::optional<QDomElement> layerElement = translateLayer(material, doc);
      if (layerElement){
        gbXMLElement.appendChild(*layerElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }
    for (const model::Material& material : m_materials){
      boost::optional<QDomElement> materialElement = translateMaterial(material, doc);
      if (materialElement){
        gbXMLElement.appendChild(*materialElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do thermal zones
    std::vector<model::ThermalZone> thermalZones = model.getConcreteModelObjects<model::ThermalZone>();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)thermalZones.size());
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

    // Document History
    QDomElement documentHistoryElement = doc.createElement("DocumentHistory");
    gbXMLElement.appendChild(documentHistoryElement);

    QDomElement createdByElement = doc.createElement("CreatedBy");
    documentHistoryElement.appendChild(createdByElement);
    createdByElement.setAttribute("programId", "openstudio");
    createdByElement.setAttribute("date",  toQString(DateTime::now().toXsdDateTime()));
    createdByElement.setAttribute("personId", "unknown");

    QDomElement programInfoElement = doc.createElement("ProgramInfo");
    documentHistoryElement.appendChild(programInfoElement);
    programInfoElement.setAttribute("id", "openstudio");

    QDomElement productNameElement = doc.createElement("ProductName");
    programInfoElement.appendChild(productNameElement);
    productNameElement.appendChild(doc.createTextNode("OpenStudio"));

    QDomElement versionElement = doc.createElement("Version");
    programInfoElement.appendChild(versionElement);
    versionElement.appendChild(doc.createTextNode(QString::fromStdString(openStudioVersion())));

    QDomElement platformElement = doc.createElement("Platform");
    programInfoElement.appendChild(platformElement);
    #if _WIN32 || _MSC_VER
      platformElement.appendChild(doc.createTextNode("Windows"));
    #elif __APPLE__
      platformElement.appendChild(doc.createTextNode("Apple"));
    #else
      platformElement.appendChild(doc.createTextNode("Linux"));
    #endif

    QDomElement projectEntityElement = doc.createElement("ProjectEntity");
    programInfoElement.appendChild(projectEntityElement);

    QDomElement personInfoElement = doc.createElement("PersonInfo");
    documentHistoryElement.appendChild(personInfoElement);
    personInfoElement.setAttribute("id", "unknown");

    QDomElement firstNameElement = doc.createElement("FirstName");
    personInfoElement.appendChild(firstNameElement);
    firstNameElement.appendChild(doc.createTextNode("Unknown"));

    QDomElement lastNameElement = doc.createElement("LastName");
    personInfoElement.appendChild(lastNameElement);
    lastNameElement.appendChild(doc.createTextNode("Unknown"));

    // translate results
    boost::optional<SqlFile> sqlFile = model.sqlFile();
    if (sqlFile){

      // thermal zone results
      if (m_progressBar){
        m_progressBar->setWindowTitle(toString("Translating Thermal Zone Results"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum((int)thermalZones.size());
        m_progressBar->setValue(0);
      }

      for (const model::ThermalZone& thermalZone : thermalZones){
        std::string query;
        boost::optional<double> heatLoad;
        boost::optional<double> coolingLoad;
        boost::optional<double> flow;
        QString thermalZoneId = escapeName(thermalZone.name().get());

        // DLM: these queries are taken from the OpenStudio standards, should be ported to Model
        query = "SELECT Value ";
        query += "FROM tabulardatawithstrings ";
        query += "WHERE ReportName='HVACSizingSummary' ";
        query += "AND ReportForString='Entire Facility' ";
        query += "AND TableName='Zone Sensible Heating' ";
        query += "AND ColumnName='User Design Load' ";
        query += "AND RowName='" + boost::to_upper_copy(thermalZone.name().get()) + "' ";
        query += "AND Units='W'";
        heatLoad = sqlFile->execAndReturnFirstDouble(query);

        query = "SELECT Value ";
        query += "FROM tabulardatawithstrings ";
        query += "WHERE ReportName='HVACSizingSummary' ";
        query += "AND ReportForString='Entire Facility' ";
        query += "AND TableName='Zone Sensible Cooling' ";
        query += "AND ColumnName='User Design Load' ";
        query += "AND RowName='" + boost::to_upper_copy(thermalZone.name().get()) + "' ";
        query += "AND Units='W'";
        coolingLoad = sqlFile->execAndReturnFirstDouble(query);

        query = "SELECT Value ";
        query += "FROM tabulardatawithstrings ";
        query += "WHERE ReportName='HVACSizingSummary' ";
        query += "AND ReportForString='Entire Facility' ";
        query += "AND TableName='Zone Sensible Cooling' ";
        query += "AND ColumnName='User Design Air Flow' ";
        query += "AND RowName='" + boost::to_upper_copy(thermalZone.name().get()) + "' ";
        query += "AND Units='m3/s'";
        boost::optional<double> coolingFlow = sqlFile->execAndReturnFirstDouble(query);

        query = "SELECT Value ";
        query += "FROM tabulardatawithstrings ";
        query += "WHERE ReportName='HVACSizingSummary' ";
        query += "AND ReportForString='Entire Facility' ";
        query += "AND TableName='Zone Sensible Heating' ";
        query += "AND ColumnName='User Design Air Flow' ";
        query += "AND RowName='" + boost::to_upper_copy(thermalZone.name().get()) + "' ";
        query += "AND Units='m3/s'";
        boost::optional<double> heatingFlow = sqlFile->execAndReturnFirstDouble(query);

        if (heatingFlow && coolingFlow){
          flow = std::max(*heatingFlow, *coolingFlow);
        } else if (heatingFlow){
          flow = heatingFlow;
        } else if (coolingFlow){
          flow = coolingFlow;
        }

        if (heatLoad){
          QDomElement resultsElement = doc.createElement("Results");
          gbXMLElement.appendChild(resultsElement);
          resultsElement.setAttribute("id", thermalZoneId + "HeatLoad");
          resultsElement.setAttribute("resultsType", "HeatLoad");
          resultsElement.setAttribute("unit", "Kilowatt");

          QDomElement objectIdElement = doc.createElement("ObjectId");
          resultsElement.appendChild(objectIdElement);
          objectIdElement.appendChild(doc.createTextNode(thermalZoneId));

          QDomElement valueElement = doc.createElement("Value");
          resultsElement.appendChild(valueElement);
          valueElement.appendChild(doc.createTextNode(QString::number(*heatLoad/1000.0, 'f')));
        }

        if (coolingLoad){
          QDomElement resultsElement = doc.createElement("Results");
          gbXMLElement.appendChild(resultsElement);
          resultsElement.setAttribute("id", thermalZoneId + "CoolingLoad");
          resultsElement.setAttribute("resultsType", "CoolingLoad");
          resultsElement.setAttribute("unit", "Kilowatt");

          QDomElement objectIdElement = doc.createElement("ObjectId");
          resultsElement.appendChild(objectIdElement);
          objectIdElement.appendChild(doc.createTextNode(thermalZoneId));

          QDomElement valueElement = doc.createElement("Value");
          resultsElement.appendChild(valueElement);
          valueElement.appendChild(doc.createTextNode(QString::number(*coolingLoad/1000.0, 'f')));
        }

        if (flow){
          QDomElement resultsElement = doc.createElement("Results");
          gbXMLElement.appendChild(resultsElement);
          resultsElement.setAttribute("id", thermalZoneId + "Flow");
          resultsElement.setAttribute("resultsType", "Flow");
          resultsElement.setAttribute("unit", "CubicMPerHr");

          QDomElement objectIdElement = doc.createElement("ObjectId");
          resultsElement.appendChild(objectIdElement);
          objectIdElement.appendChild(doc.createTextNode(thermalZoneId));

          QDomElement valueElement = doc.createElement("Value");
          resultsElement.appendChild(valueElement);
          valueElement.appendChild(doc.createTextNode(QString::number(*flow*3600, 'f')));
        }

        if (m_progressBar){
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }
    }

    return doc;
  }

  boost::optional<QDomElement> ForwardTranslator::translateFacility(const openstudio::model::Facility& facility, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Campus");
    m_translatedObjects[facility.handle()] = result;

    boost::optional<std::string> name = facility.name();

    // id
    result.setAttribute("id", "Facility");

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    if (name){
      nameElement.appendChild(doc.createTextNode(QString::fromStdString(name.get())));
    }else{
      nameElement.appendChild(doc.createTextNode("Facility"));
    }

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
      m_progressBar->setMaximum((int)surfaces.size());
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

    // translate shading surfaces
    std::vector<model::ShadingSurface> shadingSurfaces = model.getConcreteModelObjects<model::ShadingSurface>();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Shading Surfaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)shadingSurfaces.size());
      m_progressBar->setValue(0);
    }

    for (const model::ShadingSurface& shadingSurface : shadingSurfaces){
      boost::optional<QDomElement> shadingSurfaceElement = translateShadingSurface(shadingSurface, doc);
      if (shadingSurfaceElement){
        result.appendChild(*shadingSurfaceElement);
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

    boost::optional<std::string> standardsBuildingType = building.standardsBuildingType();
    if (standardsBuildingType){
      // todo: map to gbXML types
      //result.setAttribute("buildingType", escapeName(spaceTypeName));
    }

    // space type
    boost::optional<model::SpaceType> spaceType = building.spaceType();
    if (spaceType){
      //std::string spaceTypeName = spaceType->name().get();
      // todo: map to gbXML types
      //result.setAttribute("buildingType", escapeName(spaceTypeName));
    }

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    // area
    QDomElement areaElement = doc.createElement("Area");
    result.appendChild(areaElement);

    // DLM: we want to use gbXML's definition of floor area which includes area from all spaces with people in them
    //double floorArea = building.floorArea();

    std::vector<model::Space> spaces = building.spaces();

    double floorArea = 0;
    for (const model::Space& space : spaces){
      double numberOfPeople = space.numberOfPeople();
      if (numberOfPeople > 0.0) {
        floorArea += space.multiplier() * space.floorArea();
      }
    }

    areaElement.appendChild(doc.createTextNode(QString::number(floorArea, 'f')));

    // translate spaces
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Spaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)spaces.size());
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

    // translate shading surface groups
    model::ShadingSurfaceGroupVector shadingSurfaceGroups = building.model().getConcreteModelObjects<model::ShadingSurfaceGroup>();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Shading Surface Groups"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)shadingSurfaceGroups.size());
      m_progressBar->setValue(0);
    }

    for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
      boost::optional<QDomElement> shadingSurfaceGroupElement = translateShadingSurfaceGroup(shadingSurfaceGroup, doc);
      if (shadingSurfaceGroupElement){
        result.appendChild(*shadingSurfaceGroupElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate stories
    model::BuildingStoryVector stories = building.model().getConcreteModelObjects<model::BuildingStory>();
    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Stories"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)stories.size());
      m_progressBar->setValue(0);
    }

    for (const model::BuildingStory& story : stories){
      boost::optional<QDomElement> storyElement = translateBuildingStory(story, doc);
      if (storyElement){
        result.appendChild(*storyElement);
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

    // id
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

    // building story
    boost::optional<model::BuildingStory> story = space.buildingStory();
    if (story){
      std::string storyName = story->name().get();
      result.setAttribute("buildingStoreyIdRef", escapeName(storyName));
    }

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    // append floor area
    double area = space.floorArea();
    QDomElement areaElement = doc.createElement("Area");
    areaElement.appendChild(doc.createTextNode(QString::number(area, 'f')));
    result.appendChild(areaElement);

    // append volume
    double volume = space.volume();
    QDomElement volumeElement = doc.createElement("Volume");
    volumeElement.appendChild(doc.createTextNode(QString::number(volume, 'f')));
    result.appendChild(volumeElement);

    // Lighting

    // LightingControl

    // InfiltrationFlow

    // PeopleNumber
    double numberOfPeople = space.numberOfPeople();
    if (numberOfPeople > 0){
      double floorAreaPerPerson = space.floorAreaPerPerson();
      QDomElement peopleNumberElement = doc.createElement("PeopleNumber");
      peopleNumberElement.setAttribute("unit", "SquareMPerPerson");
      peopleNumberElement.appendChild(doc.createTextNode(QString::number(floorAreaPerPerson, 'f')));
      result.appendChild(peopleNumberElement);
    }

    // PeopleHeatGain
    // unit "WattPerPerson", BtuPerHourPerson"
    // heatGainType "Total",  "Sensible", "Latent"

    // LightPowerPerArea
    double lightingPowerPerFloorArea = space.lightingPowerPerFloorArea();
    if (lightingPowerPerFloorArea > 0){
      QDomElement lightPowerPerAreaElement = doc.createElement("LightPowerPerArea");
      lightPowerPerAreaElement.setAttribute("unit", "WattPerSquareMeter");
      lightPowerPerAreaElement.appendChild(doc.createTextNode(QString::number(lightingPowerPerFloorArea, 'f')));
      result.appendChild(lightPowerPerAreaElement);
    }

    // EquipPowerPerArea
    double electricEquipmentPowerPerFloorArea = space.electricEquipmentPowerPerFloorArea();
    if (electricEquipmentPowerPerFloorArea > 0){
      QDomElement equipPowerPerAreaElement = doc.createElement("EquipPowerPerArea");
      equipPowerPerAreaElement.setAttribute("unit", "WattPerSquareMeter");
      equipPowerPerAreaElement.appendChild(doc.createTextNode(QString::number(electricEquipmentPowerPerFloorArea, 'f')));
      result.appendChild(equipPowerPerAreaElement);
    }

    //  Temperature
    // unit "F", "C", "K", "R"

    // export CADObjectId if present
    translateCADObjectId(space, result, doc);

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateShadingSurfaceGroup(const openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup, QDomDocument& doc)
  {
    if (shadingSurfaceGroup.space()){
      return boost::none;
    }

    QDomElement result = doc.createElement("Space");
    m_translatedObjects[shadingSurfaceGroup.handle()] = result;

    // id
    std::string name = shadingSurfaceGroup.name().get();
    result.setAttribute("id", escapeName(name));

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateBuildingStory(const openstudio::model::BuildingStory& story, QDomDocument& doc)
  {
    boost::optional<double> zLevel = story.nominalZCoordinate();

    // z-level not set, attempt to find it
    if (!zLevel){
      for (const auto& space : story.spaces()){
        Transformation t = space.buildingTransformation();
        for (const auto& surface : space.surfaces()){
          for (const auto& vertex : surface.vertices()){
            if (zLevel){
              zLevel = std::min(*zLevel, vertex.z());
            } else{
              zLevel = vertex.z();
            }
          }
        }
      }
    }

    if (!zLevel){
      zLevel = 0;
    }

    QDomElement result = doc.createElement("BuildingStorey");
    m_translatedObjects[story.handle()] = result;

    // id
    std::string name = story.name().get();
    result.setAttribute("id", escapeName(name));

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    // append level
    QDomElement levelElement = doc.createElement("Level");
    levelElement.appendChild(doc.createTextNode(QString::number(*zLevel, 'f')));
    result.appendChild(levelElement);

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
    //FreestandingColumn
    //EmbeddedColumn

    bool checkSlabOnGrade = false;

    if (surface.isAirWall()){
      result.setAttribute("surfaceType", "Air");
    } else{
      std::string surfaceType = surface.surfaceType();
      std::string outsideBoundaryCondition = surface.outsideBoundaryCondition();
      if (istringEqual("Wall", surfaceType)){
        if (istringEqual("Outdoors", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "ExteriorWall");
        } else if (istringEqual("Surface", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "InteriorWall");
        } else if (surface.isGroundSurface()){
          result.setAttribute("surfaceType", "UndergroundWall");
        } else if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
          result.setAttribute("surfaceType", "InteriorWall");
        }
      } else if (istringEqual("RoofCeiling", surfaceType)){
        if (istringEqual("Outdoors", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "Roof");
        } else if (istringEqual("Surface", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "Ceiling");
        } else if (surface.isGroundSurface()){
          result.setAttribute("surfaceType", "UndergroundCeiling");
        } else if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
          result.setAttribute("surfaceType", "InteriorWall");
        }
      } else if (istringEqual("Floor", surfaceType)){
        if (istringEqual("Outdoors", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "RaisedFloor");
        } else if (surface.isGroundSurface()){
          checkSlabOnGrade = true;
          result.setAttribute("surfaceType", "UndergroundSlab"); // might be SlabOnGrade, check vertices later
        } else if (istringEqual("Surface", outsideBoundaryCondition)){
          result.setAttribute("surfaceType", "InteriorFloor");
        } else if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
          result.setAttribute("surfaceType", "InteriorWall");
        }
      }
    }

    // construction
    boost::optional<model::ConstructionBase> construction = surface.construction();
    if (construction){
      std::string constructionName = construction->name().get();
      if (construction->isOpaque()){
        result.setAttribute("constructionIdRef", escapeName(constructionName));
      } else{
        result.setAttribute("constructionIdRef", escapeName(constructionName));
      }
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

    if (checkSlabOnGrade){
      double minZ = std::numeric_limits<double>::max();
      double maxZ = std::numeric_limits<double>::min();
      for (const auto& vertex : vertices){
        minZ = std::min(minZ, vertex.z());
        maxZ = std::max(maxZ, vertex.z());
      }
      if ((maxZ <= 0.01) && (minZ >= -0.01)){
        result.setAttribute("surfaceType", "SlabOnGrade");
      }
    }


    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = surface.grossArea();
    if (outwardNormal && area > 0){

      // get tilt, duplicate code in planar surface
      Vector3d up(0.0, 0.0, 1.0);
      double tiltRadians = getAngle(*outwardNormal, up);

      // get azimuth, duplicate code in planar surface
      Vector3d north(0.0, 1.0, 0.0);
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
        areaCorrection = sqrt(area / (width*height));
      }

      // pick lower left corner vertex in face coordinates
      double minY = std::numeric_limits<double>::max();
      double minX = std::numeric_limits<double>::max();
      size_t llcIndex = 0;
      size_t N = vertices.size();
      for (size_t i = 0; i < N; ++i){
        double z = faceVertices[i].z();
        OS_ASSERT(std::abs(z) < 0.001);
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
      azimuthElement.appendChild(doc.createTextNode(QString::number(radToDeg(azimuthRadians), 'g')));

      QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
      rectangularGeometryElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x(), 'f')));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y(), 'f')));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z(), 'f')));

      QDomElement tiltElement = doc.createElement("Tilt");
      rectangularGeometryElement.appendChild(tiltElement);
      tiltElement.appendChild(doc.createTextNode(QString::number(radToDeg(tiltRadians), 'g')));

      QDomElement widthElement = doc.createElement("Width");
      rectangularGeometryElement.appendChild(widthElement);
      widthElement.appendChild(doc.createTextNode(QString::number(areaCorrection*width, 'f')));

      QDomElement heightElement = doc.createElement("Height");
      rectangularGeometryElement.appendChild(heightElement);
      heightElement.appendChild(doc.createTextNode(QString::number(areaCorrection*height, 'f')));
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
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x(), 'f')));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y(), 'f')));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z(), 'f')));
    }

    // export CADObjectId if present
    if (!translateCADObjectId(surface, result, doc)) {
      boost::optional<model::Surface> otherSurface = surface.adjacentSurface();
      if (otherSurface) {
        translateCADObjectId(*otherSurface, result, doc);
      }
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
      std::string constructionName = construction->name().get();
      if (construction->isOpaque()){
        result.setAttribute("constructionIdRef", escapeName(constructionName));
      } else{
        result.setAttribute("windowTypeIdRef", escapeName(constructionName));
      }
    }

    // DLM: currently unhandled, nothing in os maps to these
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
      }else if (istringEqual("Door", subSurfaceType)){
        result.setAttribute("openingType", "NonSlidingDoor");
      }else if (istringEqual("GlassDoor", subSurfaceType)){
        result.setAttribute("openingType", "SlidingDoor");
      }else if (istringEqual("OverheadDoor", subSurfaceType)){
        result.setAttribute("openingType", "NonSlidingDoor");
      }else if (istringEqual("Skylight", subSurfaceType)){
        result.setAttribute("openingType", "FixedSkylight");
      }else if (istringEqual("TubularDaylightDome", subSurfaceType)){
        result.setAttribute("openingType", "FixedSkylight");
      }else if (istringEqual("TubularDaylightDiffuser", subSurfaceType)){
        result.setAttribute("openingType", "FixedSkylight");
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
      size_t llcIndex = 0;
      size_t N = vertices.size();
      for (size_t i = 0; i < N; ++i){
        double z = faceVertices[i].z();
        OS_ASSERT(std::abs(z) < 0.001);
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
      azimuthElement.appendChild(doc.createTextNode(QString::number(radToDeg(azimuthRadians), 'g')));

      QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
      rectangularGeometryElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x(), 'f')));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y(), 'f')));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z(), 'f')));

      QDomElement tiltElement = doc.createElement("Tilt");
      rectangularGeometryElement.appendChild(tiltElement);
      tiltElement.appendChild(doc.createTextNode(QString::number(radToDeg(tiltRadians), 'g')));

      QDomElement widthElement = doc.createElement("Width");
      rectangularGeometryElement.appendChild(widthElement);
      widthElement.appendChild(doc.createTextNode(QString::number(areaCorrection*width, 'f')));

      QDomElement heightElement = doc.createElement("Height");
      rectangularGeometryElement.appendChild(heightElement);
      heightElement.appendChild(doc.createTextNode(QString::number(areaCorrection*height, 'f')));
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
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x(), 'f')));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y(), 'f')));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z(), 'f')));
    }

    // export CADObjectId if present
    if (!translateCADObjectId(subSurface, result, doc)){
      boost::optional<model::SubSurface> otherSubSurface = subSurface.adjacentSubSurface();
      if (otherSubSurface) {
        translateCADObjectId(*otherSubSurface, result, doc);
      }
    }
    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateShadingSurface(const openstudio::model::ShadingSurface& shadingSurface, QDomDocument& doc)
  {
    // return if already translated
    if (m_translatedObjects.find(shadingSurface.handle()) != m_translatedObjects.end()){
      return boost::none;
    }

    QDomElement result = doc.createElement("Surface");
    m_translatedObjects[shadingSurface.handle()] = result;

    // id
    std::string name = shadingSurface.name().get();
    result.setAttribute("id", escapeName(name));

    result.setAttribute("surfaceType", "Shade");

    // construction
    boost::optional<model::ConstructionBase> construction = shadingSurface.construction();
    if (construction){
      std::string constructionName = construction->name().get();
      if (construction->isOpaque()){
        result.setAttribute("constructionIdRef", escapeName(constructionName));
      } else{
        result.setAttribute("windowTypeIdRef", escapeName(constructionName));
      }
    }

    // this space
    Transformation transformation;
    boost::optional<model::Space> space = shadingSurface.space();
    if (space){
      boost::optional<model::ShadingSurfaceGroup> shadingSurfaceGroup = shadingSurface.shadingSurfaceGroup();
      if (shadingSurfaceGroup) {
        transformation = shadingSurfaceGroup->siteTransformation();
      } else {
        transformation = space->siteTransformation();
      }

      std::string spaceName = space->name().get();
      QDomElement adjacentSpaceIdElement = doc.createElement("AdjacentSpaceId");
      result.appendChild(adjacentSpaceIdElement);
      adjacentSpaceIdElement.setAttribute("spaceIdRef", escapeName(spaceName));
    } else {
      boost::optional<model::ShadingSurfaceGroup> shadingSurfaceGroup = shadingSurface.shadingSurfaceGroup();
      if (shadingSurfaceGroup){
        transformation = shadingSurfaceGroup->siteTransformation();

        std::string spaceName = shadingSurfaceGroup->name().get();
        QDomElement adjacentSpaceIdElement = doc.createElement("AdjacentSpaceId");
        result.appendChild(adjacentSpaceIdElement);
        adjacentSpaceIdElement.setAttribute("spaceIdRef", escapeName(spaceName));
      }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation*shadingSurface.vertices();

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = shadingSurface.grossArea();
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
      size_t llcIndex = 0;
      size_t N = vertices.size();
      for (size_t i = 0; i < N; ++i){
        double z = faceVertices[i].z();
        OS_ASSERT(std::abs(z) < 0.001);
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
      azimuthElement.appendChild(doc.createTextNode(QString::number(radToDeg(azimuthRadians), 'g')));

      QDomElement cartesianPointElement = doc.createElement("CartesianPoint");
      rectangularGeometryElement.appendChild(cartesianPointElement);

      QDomElement coordinateXElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateXElement);
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x(), 'f')));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y(), 'f')));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z(), 'f')));

      QDomElement tiltElement = doc.createElement("Tilt");
      rectangularGeometryElement.appendChild(tiltElement);
      tiltElement.appendChild(doc.createTextNode(QString::number(radToDeg(tiltRadians), 'g')));

      QDomElement widthElement = doc.createElement("Width");
      rectangularGeometryElement.appendChild(widthElement);
      widthElement.appendChild(doc.createTextNode(QString::number(areaCorrection*width, 'f')));

      QDomElement heightElement = doc.createElement("Height");
      rectangularGeometryElement.appendChild(heightElement);
      heightElement.appendChild(doc.createTextNode(QString::number(areaCorrection*height, 'f')));
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
      coordinateXElement.appendChild(doc.createTextNode(QString::number(vertex.x(), 'f')));

      QDomElement coordinateYElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateYElement);
      coordinateYElement.appendChild(doc.createTextNode(QString::number(vertex.y(), 'f')));

      QDomElement coordinateZElement = doc.createElement("Coordinate");
      cartesianPointElement.appendChild(coordinateZElement);
      coordinateZElement.appendChild(doc.createTextNode(QString::number(vertex.z(), 'f')));
    }

    // export CADObjectId if present
    translateCADObjectId(shadingSurface, result, doc);

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateThermalZone(const openstudio::model::ThermalZone& thermalZone, QDomDocument& doc)
  {
    QDomElement result = doc.createElement("Zone");
    m_translatedObjects[thermalZone.handle()] = result;

    // id
    std::string name = thermalZone.name().get();
    result.setAttribute("id", escapeName(name));

    // name
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    // heating setpoint
    boost::optional<double> designHeatT;
    boost::optional<double> designCoolT;
    boost::optional<model::Thermostat> thermostat = thermalZone.thermostat();
    if (thermostat && thermostat->optionalCast<model::ThermostatSetpointDualSetpoint>()){
      model::ThermostatSetpointDualSetpoint thermostatDualSetpoint = thermostat->cast<model::ThermostatSetpointDualSetpoint>();

      boost::optional<model::Schedule> heatingSchedule = thermostatDualSetpoint.heatingSetpointTemperatureSchedule();
      if (heatingSchedule){
        if (heatingSchedule->optionalCast<model::ScheduleRuleset>()){
          model::ScheduleRuleset scheduleRuleset = heatingSchedule->cast<model::ScheduleRuleset>();
          model::ScheduleDay winterDesignDaySchedule = scheduleRuleset.winterDesignDaySchedule();
          std::vector<double> values = winterDesignDaySchedule.values();
          if (!values.empty()){
            designHeatT = *std::max_element(values.begin(), values.end());
          }
        }
      }

      boost::optional<model::Schedule> coolingSchedule = thermostatDualSetpoint.coolingSetpointTemperatureSchedule();
      if (coolingSchedule){
        if (coolingSchedule->optionalCast<model::ScheduleRuleset>()){
          model::ScheduleRuleset scheduleRuleset = coolingSchedule->cast<model::ScheduleRuleset>();
          model::ScheduleDay summerDesignDaySchedule = scheduleRuleset.summerDesignDaySchedule();
          std::vector<double> values = summerDesignDaySchedule.values();
          if (!values.empty()){
            designCoolT = *std::min_element(values.begin(), values.end());
          }
        }
      }
    }

    if (designHeatT){
      QDomElement designHeatTElement = doc.createElement("DesignHeatT");
      designHeatTElement.setAttribute("unit", "C");
      designHeatTElement.appendChild(doc.createTextNode(QString::number(*designHeatT, 'f')));
      result.appendChild(designHeatTElement);
    }

    if (designCoolT){
      QDomElement designCoolTElement = doc.createElement("DesignCoolT");
      designCoolTElement.setAttribute("unit", "C");
      designCoolTElement.appendChild(doc.createTextNode(QString::number(*designCoolT, 'f')));
      result.appendChild(designCoolTElement);
    }

    // export CADObjectId if present
    translateCADObjectId(thermalZone, result, doc);

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateCADObjectId(const openstudio::model::ModelObject& modelObject, QDomElement& parentElement, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    if (modelObject.hasAdditionalProperties()) {
      model::AdditionalProperties additionalProperties = modelObject.additionalProperties();
      if (additionalProperties.hasFeature("CADObjectId")) {
        boost::optional<std::string> cadObjectId = additionalProperties.getFeatureAsString("CADObjectId");
        if (cadObjectId) {
          QDomElement cadObjectIdElement = doc.createElement("CADObjectId");
          cadObjectIdElement.appendChild(doc.createTextNode(toQString(*cadObjectId)));

          if (additionalProperties.hasFeature("programIdRef")) {
            boost::optional<std::string> programIdRef = additionalProperties.getFeatureAsString("programIdRef");
            if (programIdRef){
              cadObjectIdElement.setAttribute("programIdRef", toQString(*programIdRef));
            }
          }

          parentElement.appendChild(cadObjectIdElement);
          result = cadObjectIdElement;
        }
      }
    }
    return result;
  }

} // gbxml
} // openstudio
