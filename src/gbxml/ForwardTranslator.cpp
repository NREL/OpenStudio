/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
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
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"
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
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/xml/XMLValidator.hpp"

#include <OpenStudio.hxx>
#include <resources.hxx>

#include <boost/math/constants/constants.hpp>

#include <pugixml.hpp>

#include <regex>
#include <string_view>

namespace openstudio {
namespace gbxml {

  static constexpr std::string_view shadingSurfaceWithoutConstructionName = "Shading_Surface_Without_Construction";

  ForwardTranslator::ForwardTranslator() {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.gbxml\\.ForwardTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  ForwardTranslator::~ForwardTranslator() = default;

  bool ForwardTranslator::modelToGbXML(const openstudio::model::Model& model, const openstudio::path& path, ProgressBar* progressBar) {
    m_progressBar = progressBar;

    m_logSink.setThreadId(std::this_thread::get_id());

    m_logSink.resetStringStream();

    pugi::xml_document doc;
    //doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    bool result = this->translateModel(model, doc);

    openstudio::filesystem::ofstream file(path, std::ios_base::binary);
    if (file.is_open()) {
      doc.save(file, "  ");
      file.close();

      // validate the gbxml after forward translation
      auto gbxmlValidator = XMLValidator::gbxmlValidator();
      gbxmlValidator.validate(path);

      return result;
    }

    return false;
  }

  std::string ForwardTranslator::modelToGbXMLString(const openstudio::model::Model& model, ProgressBar* progressBar) {
    std::string gbXML_str;

    m_progressBar = progressBar;

    m_logSink.setThreadId(std::this_thread::get_id());

    m_logSink.resetStringStream();

    pugi::xml_document doc;
    //doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    bool result = this->translateModel(model, doc);

    if (result) {
      // doc.save allows any ostream, so use a stringstream
      std::stringstream ss;
      doc.save(ss, "  ");
      gbXML_str = ss.str();
    }

    return gbXML_str;
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const {
    std::vector<LogMessage> result;
    std::vector<LogMessage> allMessages = m_logSink.logMessages();
    std::copy_if(allMessages.cbegin(), allMessages.cend(), std::back_inserter(result),
                 [](const auto& logMessage) { return logMessage.logLevel() == Warn; });
    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const {
    std::vector<LogMessage> result;
    std::vector<LogMessage> allMessages = m_logSink.logMessages();
    std::copy_if(allMessages.cbegin(), allMessages.cend(), std::back_inserter(result),
                 [](const auto& logMessage) { return logMessage.logLevel() > Warn; });
    return result;
  }

  std::string ForwardTranslator::escapeName(const std::string& name) {
    std::string result;
    if (std::regex_match(name, std::regex("^\\d.*"))) {
      result = "id_" + name;
    } else {
      result = name;
    }
    std::replace(result.begin(), result.end(), ' ', '_');
    std::replace(result.begin(), result.end(), '(', '_');
    std::replace(result.begin(), result.end(), ')', '_');
    std::replace(result.begin(), result.end(), '[', '_');
    std::replace(result.begin(), result.end(), ']', '_');
    std::replace(result.begin(), result.end(), '{', '_');
    std::replace(result.begin(), result.end(), '}', '_');
    std::replace(result.begin(), result.end(), '/', '_');
    std::replace(result.begin(), result.end(), '\\', '_');
    //std::replace(result.begin(), result.end(),'-', '_'); // ok
    //std::replace(result.begin(), result.end(),'.', '_'); // ok
    std::replace(result.begin(), result.end(), ':', '_');
    std::replace(result.begin(), result.end(), ';', '_');
    return result;
  }

  bool ForwardTranslator::translateModel(const openstudio::model::Model& model, pugi::xml_document& document) {

    // Clear the map & set
    m_translatedObjects.clear();
    m_materials.clear();
    m_placeholderShadingSurfaceConstructionAlreadyCreated = false;

    auto gbXMLElement = document.append_child("gbXML");
    gbXMLElement.append_attribute("xmlns") = "http://www.gbxml.org/schema";
    gbXMLElement.append_attribute("xmlns:xhtml") = "http://www.w3.org/1999/xhtml";
    gbXMLElement.append_attribute("xmlns:xsi") = "http://www.w3.org/2001/XMLSchema-instance";
    gbXMLElement.append_attribute("xmlns:xsd") = "http://www.w3.org/2001/XMLSchema";
    gbXMLElement.append_attribute("xsi:schemaLocation") = "http://www.gbxml.org/schema http://gbxml.org/schema/6-01/GreenBuildingXML_Ver6.01.xsd";
    gbXMLElement.append_attribute("temperatureUnit") = "C";
    gbXMLElement.append_attribute("lengthUnit") = "Meters";
    gbXMLElement.append_attribute("areaUnit") = "SquareMeters";
    gbXMLElement.append_attribute("volumeUnit") = "CubicMeters";
    gbXMLElement.append_attribute("useSIUnitsForResults") = "true";
    gbXMLElement.append_attribute("version") = "6.01";
    gbXMLElement.append_attribute("SurfaceReferenceLocation") = "Centerline";

    // translateFacility is responsible to translate Surfaces, and calls translateBuilding, which is responsible to translate spaces
    // so we do need to call it anyways.
    translateFacility(model, gbXMLElement);

    // do constructions
    std::vector<model::ConstructionBase> constructionBases = model.getModelObjects<model::ConstructionBase>();

    // sort by is opaque so we get constructions before window types
    std::sort(constructionBases.begin(), constructionBases.end(),
              [](const model::ConstructionBase& a, const model::ConstructionBase& b) { return a.isOpaque() && !b.isOpaque(); });
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)constructionBases.size());
      m_progressBar->setValue(0);
    }

    for (const model::ConstructionBase& constructionBase : constructionBases) {
      translateConstructionBase(constructionBase, gbXMLElement);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do materials
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Materials"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)2 * m_materials.size());
      m_progressBar->setValue(0);
    }

    std::vector<model::Material> materials(m_materials.begin(), m_materials.end());
    std::sort(materials.begin(), materials.end(), WorkspaceObjectNameLess());

    for (const model::Material& material : materials) {
      translateLayer(material, gbXMLElement);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }
    for (const model::Material& material : materials) {
      translateMaterial(material, gbXMLElement);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do thermal zones
    std::vector<model::ThermalZone> thermalZones = model.getConcreteModelObjects<model::ThermalZone>();
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Thermal Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)thermalZones.size());
      m_progressBar->setValue(0);
    }

    for (const model::ThermalZone& thermalZone : thermalZones) {
      translateThermalZone(thermalZone, gbXMLElement);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // Document History
    auto documentHistoryElement = gbXMLElement.append_child("DocumentHistory");

    auto createdByElement = documentHistoryElement.append_child("CreatedBy");
    createdByElement.append_attribute("programId") = "openstudio";
    createdByElement.append_attribute("date") = DateTime::now().toXsdDateTime().c_str();
    createdByElement.append_attribute("personId") = "unknown";

    auto programInfoElement = documentHistoryElement.append_child("ProgramInfo");
    programInfoElement.append_attribute("id") = "openstudio";

    auto productNameElement = programInfoElement.append_child("ProductName");
    productNameElement.text() = "OpenStudio";

    auto versionElement = programInfoElement.append_child("Version");
    versionElement.text() = openStudioVersion().c_str();

    auto platformElement = programInfoElement.append_child("Platform");
#if _WIN32 || _MSC_VER
    platformElement.text() = "Windows";
#elif __APPLE__
    platformElement.text() = "Apple";
#else
    platformElement.text() = "Linux";
#endif

    // TODO: not used
    // auto projectEntityElement = programInfoElement.append_child("ProjectEntity");

    auto personInfoElement = documentHistoryElement.append_child("PersonInfo");
    personInfoElement.append_attribute("id") = "unknown";

    auto firstNameElement = personInfoElement.append_child("FirstName");
    firstNameElement.text() = "Unknown";

    auto lastNameElement = personInfoElement.append_child("LastName");
    lastNameElement.text() = "Unknown";

    // translate results
    boost::optional<SqlFile> sqlFile = model.sqlFile();
    if (sqlFile) {

      // thermal zone results
      if (m_progressBar) {
        m_progressBar->setWindowTitle(toString("Translating Thermal Zone Results"));
        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum((int)thermalZones.size());
        m_progressBar->setValue(0);
      }

      for (const model::ThermalZone& thermalZone : thermalZones) {
        std::string query;
        boost::optional<double> heatLoad;
        boost::optional<double> coolingLoad;
        boost::optional<double> flow;
        std::string thermalZoneId = escapeName(thermalZone.name().get());

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

        if (heatingFlow && coolingFlow) {
          flow = std::max(*heatingFlow, *coolingFlow);
        } else if (heatingFlow) {
          flow = heatingFlow;
        } else if (coolingFlow) {
          flow = coolingFlow;
        }

        if (heatLoad) {
          auto resultsElement = gbXMLElement.append_child("Results");
          resultsElement.append_attribute("id") = (thermalZoneId + "HeatLoad").c_str();
          resultsElement.append_attribute("resultsType") = "HeatLoad";
          resultsElement.append_attribute("unit") = "Kilowatt";

          auto objectIdElement = resultsElement.append_child("ObjectId");
          objectIdElement.text() = thermalZoneId.c_str();

          auto valueElement = resultsElement.append_child("Value");
          valueElement.text() = openstudio::string_conversions::number(*heatLoad / 1000.0, FloatFormat::fixed).c_str();
        }

        if (coolingLoad) {
          auto resultsElement = gbXMLElement.append_child("Results");
          resultsElement.append_attribute("id") = (thermalZoneId + "CoolingLoad").c_str();
          resultsElement.append_attribute("resultsType") = "CoolingLoad";
          resultsElement.append_attribute("unit") = "Kilowatt";

          auto objectIdElement = resultsElement.append_child("ObjectId");
          objectIdElement.text() = thermalZoneId.c_str();

          auto valueElement = resultsElement.append_child("Value");
          valueElement.text() = openstudio::string_conversions::number(*coolingLoad / 1000.0, FloatFormat::fixed).c_str();
        }

        if (flow) {
          auto resultsElement = gbXMLElement.append_child("Results");
          resultsElement.append_attribute("id") = (thermalZoneId + "Flow").c_str();
          resultsElement.append_attribute("resultsType") = "Flow";
          resultsElement.append_attribute("unit") = "CubicMPerHr";

          auto objectIdElement = resultsElement.append_child("ObjectId");
          objectIdElement.text() = thermalZoneId.c_str();

          auto valueElement = resultsElement.append_child("Value");
          valueElement.text() = openstudio::string_conversions::number(*flow * 3600, FloatFormat::fixed).c_str();
        }

        if (m_progressBar) {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }
    }

    return true;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateFacility(const openstudio::model::Model& model, pugi::xml_node& parent) {

    // `model` is `const`, so we shouldn't call getUniqueModelObject<model::Facility> which will **create** a new object in there.
    boost::optional<model::Facility> _facility = model.getOptionalUniqueModelObject<model::Facility>();

    auto result = parent.append_child("Campus");
    std::string name = "Facility";

    if (_facility) {
      m_translatedObjects[_facility->handle()] = result;
      if (auto _s = _facility->name()) {
        name = _s.get();
      }
    }

    // id
    result.append_attribute("id") = "Facility";

    // name
    auto nameElement = result.append_child("Name");
    nameElement.text() = name.c_str();

    // todo: translate location

    // translate building: needs to be done even if not explicitly instantiated since that's what translates Spaces in particular.
    translateBuilding(model, result);

    // translate surfaces
    // TODO: JM 2020-06-18 Why is translateSpace not responsible to call this one?
    std::vector<model::Surface> surfaces = model.getConcreteModelObjects<model::Surface>();
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Surfaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)surfaces.size());
      m_progressBar->setValue(0);
    }

    std::vector<model::Space> spaces = model.getConcreteModelObjects<model::Space>();
    std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());
    for (const model::Space& space : spaces) {
      std::vector<model::Surface> spaceSurfaces = space.surfaces();
      std::sort(spaceSurfaces.begin(), spaceSurfaces.end(), WorkspaceObjectNameLess());

      for (const model::Surface& surface : spaceSurfaces) {
        std::string name = surface.name().value();
        translateSurface(surface, result);

        if (m_progressBar) {
          m_progressBar->setValue(m_progressBar->value() + 1);
        }
      }
    }

    // translate shading surfaces
    std::vector<model::ShadingSurface> shadingSurfaces = model.getConcreteModelObjects<model::ShadingSurface>();
    std::sort(shadingSurfaces.begin(), shadingSurfaces.end(), WorkspaceObjectNameLess());

    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Shading Surfaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)shadingSurfaces.size());
      m_progressBar->setValue(0);
    }

    for (const model::ShadingSurface& shadingSurface : shadingSurfaces) {
      translateShadingSurface(shadingSurface, result);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateBuilding(const openstudio::model::Model& model, pugi::xml_node& parent) {
    // `model` is `const`, so we shouldn't call getUniqueModelObject<model::Building> which will **create** a new object in there.
    // model::Building building = model.getUniqueModelObject<model::Building>();
    boost::optional<model::Building> _building = model.getOptionalUniqueModelObject<model::Building>();

    auto result = parent.append_child("Building");
    std::string bName = "Building";
    std::string bType = "Unknown";
    if (_building) {
      m_translatedObjects[_building->handle()] = result;
      bName = _building->nameString();

      if (boost::optional<std::string> _standardsBuildingType = _building->standardsBuildingType()) {
        // TODO: map to gbXML types
        // bType = escapeName(_standardsBuildingType.get()).c_str();
      }

      // space type
      if (boost::optional<model::SpaceType> _spaceType = _building->spaceType()) {
        //std::string spaceTypeName = _spaceType->nameString();
        // TODO: map to gbXML types
        // bType = escapeName(spaceTypeName).c_str();
      }
    }

    // id
    result.append_attribute("id") = escapeName(bName).c_str();

    // building type
    result.append_attribute("buildingType") = bType.c_str();

    if (_building) {
    }

    // name
    auto nameElement = result.append_child("Name");
    nameElement.text() = bName.c_str();

    // area
    auto areaElement = result.append_child("Area");

    // DLM: we want to use gbXML's definition of floor area which includes area from all spaces with people in them
    //double floorArea = building.floorArea();

    std::vector<model::Space> spaces = model.getConcreteModelObjects<model::Space>();

    double floorArea = 0;
    for (const model::Space& space : spaces) {
      double numberOfPeople = space.numberOfPeople();
      if (numberOfPeople > 0.0) {
        floorArea += space.multiplier() * space.floorArea();
      }
    }

    areaElement.text() = openstudio::string_conversions::number(floorArea, FloatFormat::fixed).c_str();

    // translate spaces
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Spaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)spaces.size());
      m_progressBar->setValue(0);
    }

    std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());

    for (const model::Space& space : spaces) {
      translateSpace(space, result);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate shading surface groups
    model::ShadingSurfaceGroupVector shadingSurfaceGroups = model.getConcreteModelObjects<model::ShadingSurfaceGroup>();
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Shading Surface Groups"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)shadingSurfaceGroups.size());
      m_progressBar->setValue(0);
    }

    std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());
    for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups) {
      translateShadingSurfaceGroup(shadingSurfaceGroup, result);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate stories
    model::BuildingStoryVector stories = model.getConcreteModelObjects<model::BuildingStory>();
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Stories"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)stories.size());
      m_progressBar->setValue(0);
    }

    for (const model::BuildingStory& story : stories) {
      translateBuildingStory(story, result);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateSpace(const openstudio::model::Space& space, pugi::xml_node& parent) {
    auto result = parent.append_child("Space");
    m_translatedObjects[space.handle()] = result;

    translateId(space, result);
    translateName(space, result);

    // space type
    //boost::optional<model::SpaceType> spaceType = space.spaceType();
    //if (spaceType) {
    //std::string spaceTypeName = spaceType->name().get();
    // todo: map to gbXML types
    //result.setAttribute("spaceType", escapeName(spaceTypeName));
    //}

    // thermal zone
    boost::optional<model::ThermalZone> thermalZone = space.thermalZone();
    if (thermalZone) {
      std::string thermalZoneName = thermalZone->name().get();
      result.append_attribute("zoneIdRef") = escapeName(thermalZoneName).c_str();
    }

    // building story
    boost::optional<model::BuildingStory> story = space.buildingStory();
    if (story) {
      std::string storyName = story->name().get();
      result.append_attribute("buildingStoreyIdRef") = escapeName(storyName).c_str();
    }

    // append floor area
    double area = space.floorArea();
    auto areaElement = result.append_child("Area");
    areaElement.text() = openstudio::string_conversions::number(area, FloatFormat::fixed).c_str();

    // append volume
    double volume = space.volume();
    auto volumeElement = result.append_child("Volume");
    volumeElement.text() = openstudio::string_conversions::number(volume, FloatFormat::fixed).c_str();

    // Lighting

    // LightingControl

    // InfiltrationFlow

    // PeopleNumber
    double numberOfPeople = space.numberOfPeople();
    if (numberOfPeople > 0) {
      double floorAreaPerPerson = space.floorAreaPerPerson();
      auto peopleNumberElement = result.append_child("PeopleNumber");
      peopleNumberElement.append_attribute("unit") = "SquareMPerPerson";
      peopleNumberElement.text() = openstudio::string_conversions::number(floorAreaPerPerson, FloatFormat::fixed).c_str();
    }

    // PeopleHeatGain
    // unit "WattPerPerson", BtuPerHourPerson"
    // heatGainType "Total",  "Sensible", "Latent"

    // LightPowerPerArea
    double lightingPowerPerFloorArea = space.lightingPowerPerFloorArea();
    if (lightingPowerPerFloorArea > 0) {
      auto lightPowerPerAreaElement = result.append_child("LightPowerPerArea");
      lightPowerPerAreaElement.append_attribute("unit") = "WattPerSquareMeter";
      lightPowerPerAreaElement.text() = openstudio::string_conversions::number(lightingPowerPerFloorArea, FloatFormat::fixed).c_str();
    }

    // EquipPowerPerArea
    double electricEquipmentPowerPerFloorArea = space.electricEquipmentPowerPerFloorArea();
    if (electricEquipmentPowerPerFloorArea > 0) {
      auto equipPowerPerAreaElement = result.append_child("EquipPowerPerArea");
      equipPowerPerAreaElement.append_attribute("unit") = "WattPerSquareMeter";
      equipPowerPerAreaElement.text() = openstudio::string_conversions::number(electricEquipmentPowerPerFloorArea, FloatFormat::fixed).c_str();
    }

    //  Temperature
    // unit "F", "C", "K", "R"

    // export CADObjectId if present
    translateCADObjectId(space, result);

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateShadingSurfaceGroup(const openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup,
                                                                                  pugi::xml_node& parent) {
    if (shadingSurfaceGroup.space()) {
      return boost::none;
    }

    auto result = parent.append_child("Space");
    m_translatedObjects[shadingSurfaceGroup.handle()] = result;

    translateId(shadingSurfaceGroup, result);
    translateName(shadingSurfaceGroup, result);

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateBuildingStory(const openstudio::model::BuildingStory& story, pugi::xml_node& parent) {
    boost::optional<double> zLevel = story.nominalZCoordinate();

    // z-level not set, attempt to find it
    if (!zLevel) {
      for (const auto& space : story.spaces()) {
        Transformation t = space.buildingTransformation();
        for (const auto& surface : space.surfaces()) {
          for (const auto& vertex : surface.vertices()) {
            if (zLevel) {
              zLevel = std::min(*zLevel, vertex.z());
            } else {
              zLevel = vertex.z();
            }
          }
        }
      }
    }

    if (!zLevel) {
      zLevel = 0;
    }

    auto result = parent.append_child("BuildingStorey");
    m_translatedObjects[story.handle()] = result;

    translateId(story, result);
    translateName(story, result);

    // append level
    auto levelElement = result.append_child("Level");
    levelElement.text() = openstudio::string_conversions::number(*zLevel, FloatFormat::fixed).c_str();

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateSurface(const openstudio::model::Surface& surface, pugi::xml_node& parent) {
    // return if already translated
    if (m_translatedObjects.find(surface.handle()) != m_translatedObjects.end()) {
      return boost::none;
    }

    auto result = parent.append_child("Surface");
    m_translatedObjects[surface.handle()] = result;

    translateId(surface, result);
    translateName(surface, result);

    // DLM: currently unhandled
    //FreestandingColumn
    //EmbeddedColumn

    bool checkSlabOnGrade = false;

    if (surface.isAirWall()) {
      result.append_attribute("surfaceType") = "Air";
    } else {
      std::string surfaceType = surface.surfaceType();
      std::string outsideBoundaryCondition = surface.outsideBoundaryCondition();
      if (istringEqual("Wall", surfaceType)) {
        if (istringEqual("Outdoors", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "ExteriorWall";
        } else if (istringEqual("Surface", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "InteriorWall";
        } else if (surface.isGroundSurface()) {
          result.append_attribute("surfaceType") = "UndergroundWall";
        } else if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "InteriorWall";
        }
      } else if (istringEqual("RoofCeiling", surfaceType)) {
        if (istringEqual("Outdoors", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "Roof";
        } else if (istringEqual("Surface", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "Ceiling";
        } else if (surface.isGroundSurface()) {
          result.append_attribute("surfaceType") = "UndergroundCeiling";
        } else if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "InteriorWall";
        }
      } else if (istringEqual("Floor", surfaceType)) {
        if (istringEqual("Outdoors", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "RaisedFloor";
        } else if (surface.isGroundSurface()) {
          checkSlabOnGrade = true;
          // Can be either UndergroundSlab or SlabOnGrade, check vertices later
        } else if (istringEqual("Surface", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "InteriorFloor";
        } else if (istringEqual("Adiabatic", outsideBoundaryCondition)) {
          result.append_attribute("surfaceType") = "InteriorWall";
        }
      }
    }

    // construction
    boost::optional<model::ConstructionBase> construction = surface.construction();
    if (construction) {
      std::string constructionName = construction->name().get();
      if (construction->isOpaque()) {
        result.append_attribute("constructionIdRef") = escapeName(constructionName).c_str();
      } else {
        result.append_attribute("constructionIdRef") = escapeName(constructionName).c_str();  // FIXME: windowTypeIdRef?
      }
    }

    // exposedToSun
    std::string exposedToSun = surface.sunExposure();
    if (istringEqual("NoSun", exposedToSun)) {
      result.append_attribute("exposedToSun") = "false";
    } else if (istringEqual("SunExposed", exposedToSun)) {
      result.append_attribute("exposedToSun") = "true";
    }

    // this space
    Transformation transformation;
    boost::optional<model::Space> space = surface.space();
    if (space) {
      transformation = space->siteTransformation();

      std::string spaceName = space->name().get();
      auto adjacentSpaceIdElement = result.append_child("AdjacentSpaceId");
      adjacentSpaceIdElement.append_attribute("spaceIdRef") = escapeName(spaceName).c_str();
    }

    // adjacent surface
    boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
    if (adjacentSurface) {
      boost::optional<model::Space> adjacentSpace = adjacentSurface->space();
      if (adjacentSpace) {
        std::string adjacentSpaceName = adjacentSpace->name().get();
        auto adjacentSpaceIdElement = result.append_child("AdjacentSpaceId");
        adjacentSpaceIdElement.append_attribute("spaceIdRef") = escapeName(adjacentSpaceName).c_str();

        // count adjacent surface as translated
        m_translatedObjects[adjacentSurface->handle()] = result;
      }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation * surface.vertices();

    if (checkSlabOnGrade) {
      double minZ = std::numeric_limits<double>::max();
      double maxZ = std::numeric_limits<double>::min();
      for (const auto& vertex : vertices) {
        minZ = std::min(minZ, vertex.z());
        maxZ = std::max(maxZ, vertex.z());
      }
      if ((maxZ <= 0.01) && (minZ >= -0.01)) {
        result.append_attribute("surfaceType") = "SlabOnGrade";
      } else {
        result.append_attribute("surfaceType") = "UndergroundSlab";
      }
    }

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = surface.grossArea();
    if (outwardNormal && area > 0) {

      // get tilt, duplicate code in planar surface
      Vector3d up(0.0, 0.0, 1.0);
      double tiltRadians = getAngle(*outwardNormal, up);

      // get azimuth, duplicate code in planar surface
      Vector3d north(0.0, 1.0, 0.0);
      double azimuthRadians = getAngle(*outwardNormal, north);
      if (outwardNormal->x() < 0.0) {
        azimuthRadians = -azimuthRadians + 2.0 * boost::math::constants::pi<double>();
      }

      // transform vertices to face coordinates
      Transformation faceTransformation = Transformation::alignFace(vertices);
      Point3dVector faceVertices = faceTransformation.inverse() * vertices;
      BoundingBox faceBoundingBox;
      faceBoundingBox.addPoints(faceVertices);
      double width = faceBoundingBox.maxX().get() - faceBoundingBox.minX().get();
      double height = faceBoundingBox.maxY().get() - faceBoundingBox.minY().get();
      double areaCorrection = 1.0;
      if (width > 0 && height > 0) {
        areaCorrection = sqrt(area / (width * height));
      }

      // pick lower left corner vertex in face coordinates
      double minY = std::numeric_limits<double>::max();
      double minX = std::numeric_limits<double>::max();
      size_t llcIndex = 0;
      size_t N = vertices.size();
      for (size_t i = 0; i < N; ++i) {
        double z = faceVertices[i].z();
        OS_ASSERT(std::abs(z) < 0.001);
        if ((minY > faceVertices[i].y()) || ((minY > faceVertices[i].y() - 0.00001) && (minX > faceVertices[i].x()))) {
          llcIndex = i;
          minY = faceVertices[i].y();
          minX = faceVertices[i].x();
        }
      }
      Point3d vertex = vertices[llcIndex];

      // rectangular geometry
      auto rectangularGeometryElement = result.append_child("RectangularGeometry");

      auto azimuthElement = rectangularGeometryElement.append_child("Azimuth");
      azimuthElement.text() = openstudio::string_conversions::number(radToDeg(azimuthRadians), FloatFormat::general).c_str();

      auto cartesianPointElement = rectangularGeometryElement.append_child("CartesianPoint");

      auto coordinateXElement = cartesianPointElement.append_child("Coordinate");
      coordinateXElement.text() = openstudio::string_conversions::number(vertex.x(), FloatFormat::fixed).c_str();

      auto coordinateYElement = cartesianPointElement.append_child("Coordinate");
      coordinateYElement.text() = openstudio::string_conversions::number(vertex.y(), FloatFormat::fixed).c_str();

      auto coordinateZElement = cartesianPointElement.append_child("Coordinate");
      coordinateZElement.text() = openstudio::string_conversions::number(vertex.z(), FloatFormat::fixed).c_str();

      auto tiltElement = rectangularGeometryElement.append_child("Tilt");
      tiltElement.text() = openstudio::string_conversions::number(radToDeg(tiltRadians), FloatFormat::general).c_str();

      auto widthElement = rectangularGeometryElement.append_child("Width");
      widthElement.text() = openstudio::string_conversions::number(areaCorrection * width, FloatFormat::fixed).c_str();

      auto heightElement = rectangularGeometryElement.append_child("Height");
      heightElement.text() = openstudio::string_conversions::number(areaCorrection * height, FloatFormat::fixed).c_str();
    }

    // planar geometry
    auto planarGeometryElement = result.append_child("PlanarGeometry");

    auto polyLoopElement = planarGeometryElement.append_child("PolyLoop");
    for (const Point3d& vertex : vertices) {
      auto cartesianPointElement = polyLoopElement.append_child("CartesianPoint");

      auto coordinateXElement = cartesianPointElement.append_child("Coordinate");
      coordinateXElement.text() = openstudio::string_conversions::number(vertex.x(), FloatFormat::fixed).c_str();

      auto coordinateYElement = cartesianPointElement.append_child("Coordinate");
      coordinateYElement.text() = openstudio::string_conversions::number(vertex.y(), FloatFormat::fixed).c_str();

      auto coordinateZElement = cartesianPointElement.append_child("Coordinate");
      coordinateZElement.text() = openstudio::string_conversions::number(vertex.z(), FloatFormat::fixed).c_str();
    }

    // export CADObjectId if present
    if (!translateCADObjectId(surface, result)) {
      boost::optional<model::Surface> otherSurface = surface.adjacentSurface();
      if (otherSurface) {
        translateCADObjectId(*otherSurface, result);
      }
    }

    // translate sub surfaces
    for (const model::SubSurface& subSurface : surface.subSurfaces()) {
      translateSubSurface(subSurface, transformation, result);
    }

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateSubSurface(const openstudio::model::SubSurface& subSurface,
                                                                         const openstudio::Transformation& transformation, pugi::xml_node& parent) {
    // return if already translated
    if (m_translatedObjects.find(subSurface.handle()) != m_translatedObjects.end()) {
      return boost::none;
    }

    auto result = parent.append_child("Opening");
    m_translatedObjects[subSurface.handle()] = result;

    translateId(subSurface, result);
    translateName(subSurface, result);

    // construction
    boost::optional<model::ConstructionBase> construction = subSurface.construction();
    if (construction) {
      std::string constructionName = construction->name().get();
      if (construction->isOpaque()) {
        result.append_attribute("constructionIdRef") = escapeName(constructionName).c_str();
      } else {
        result.append_attribute("windowTypeIdRef") = escapeName(constructionName).c_str();
      }
    }

    // DLM: currently unhandled, nothing in os maps to these
    // OperableSkylight
    // SlidingDoor

    if (subSurface.isAirWall()) {
      result.append_attribute("openingType") = "Air";
    } else {
      std::string subSurfaceType = subSurface.subSurfaceType();
      if (istringEqual("FixedWindow", subSurfaceType)) {
        result.append_attribute("openingType") = "FixedWindow";
      } else if (istringEqual("OperableWindow", subSurfaceType)) {
        result.append_attribute("openingType") = "OperableWindow";
      } else if (istringEqual("Door", subSurfaceType)) {
        result.append_attribute("openingType") = "NonSlidingDoor";
      } else if (istringEqual("GlassDoor", subSurfaceType)) {
        result.append_attribute("openingType") = "SlidingDoor";
      } else if (istringEqual("OverheadDoor", subSurfaceType)) {
        result.append_attribute("openingType") = "NonSlidingDoor";
      } else if (istringEqual("Skylight", subSurfaceType)) {
        result.append_attribute("openingType") = "FixedSkylight";
      } else if (istringEqual("TubularDaylightDome", subSurfaceType)) {
        result.append_attribute("openingType") = "FixedSkylight";
      } else if (istringEqual("TubularDaylightDiffuser", subSurfaceType)) {
        result.append_attribute("openingType") = "FixedSkylight";
      }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation * subSurface.vertices();

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = subSurface.grossArea();
    if (outwardNormal && area > 0) {

      // get tilt, duplicate code in planar surface
      Vector3d up(0.0, 0.0, 1.0);
      double tiltRadians = getAngle(*outwardNormal, up);

      // get azimuth, duplicate code in planar surface
      Vector3d north(0.0, 1.0, 0.0);
      double azimuthRadians = getAngle(*outwardNormal, north);
      if (outwardNormal->x() < 0.0) {
        azimuthRadians = -azimuthRadians + 2.0 * boost::math::constants::pi<double>();
      }

      // transform vertices to face coordinates
      Transformation faceTransformation = Transformation::alignFace(vertices);
      Point3dVector faceVertices = faceTransformation.inverse() * vertices;
      BoundingBox faceBoundingBox;
      faceBoundingBox.addPoints(faceVertices);
      double width = faceBoundingBox.maxX().get() - faceBoundingBox.minX().get();
      double height = faceBoundingBox.maxY().get() - faceBoundingBox.minY().get();
      double areaCorrection = 1.0;
      if (width > 0 && height > 0) {
        areaCorrection = sqrt(area / (width * height));
      }

      // pick lower left corner vertex in face coordinates
      double minY = std::numeric_limits<double>::max();
      double minX = std::numeric_limits<double>::max();
      size_t llcIndex = 0;
      size_t N = vertices.size();
      for (size_t i = 0; i < N; ++i) {
        double z = faceVertices[i].z();
        OS_ASSERT(std::abs(z) < 0.001);
        if ((minY > faceVertices[i].y()) || ((minY > faceVertices[i].y() - 0.00001) && (minX > faceVertices[i].x()))) {
          llcIndex = i;
          minY = faceVertices[i].y();
          minX = faceVertices[i].x();
        }
      }
      Point3d vertex = vertices[llcIndex];

      // rectangular geometry
      auto rectangularGeometryElement = result.append_child("RectangularGeometry");

      auto azimuthElement = rectangularGeometryElement.append_child("Azimuth");
      azimuthElement.text() = openstudio::string_conversions::number(radToDeg(azimuthRadians), FloatFormat::general).c_str();

      auto cartesianPointElement = rectangularGeometryElement.append_child("CartesianPoint");

      auto coordinateXElement = cartesianPointElement.append_child("Coordinate");
      coordinateXElement.text() = openstudio::string_conversions::number(vertex.x(), FloatFormat::fixed).c_str();

      auto coordinateYElement = cartesianPointElement.append_child("Coordinate");
      coordinateYElement.text() = openstudio::string_conversions::number(vertex.y(), FloatFormat::fixed).c_str();

      auto coordinateZElement = cartesianPointElement.append_child("Coordinate");
      coordinateZElement.text() = openstudio::string_conversions::number(vertex.z(), FloatFormat::fixed).c_str();

      auto tiltElement = rectangularGeometryElement.append_child("Tilt");
      tiltElement.text() = openstudio::string_conversions::number(radToDeg(tiltRadians), FloatFormat::general).c_str();

      auto widthElement = rectangularGeometryElement.append_child("Width");
      widthElement.text() = openstudio::string_conversions::number(areaCorrection * width, FloatFormat::fixed).c_str();

      auto heightElement = rectangularGeometryElement.append_child("Height");
      heightElement.text() = openstudio::string_conversions::number(areaCorrection * height, FloatFormat::fixed).c_str();
    }

    // planar geometry
    auto planarGeometryElement = result.append_child("PlanarGeometry");

    // translate vertices
    auto polyLoopElement = planarGeometryElement.append_child("PolyLoop");
    for (const Point3d& vertex : vertices) {
      auto cartesianPointElement = polyLoopElement.append_child("CartesianPoint");

      auto coordinateXElement = cartesianPointElement.append_child("Coordinate");
      coordinateXElement.text() = openstudio::string_conversions::number(vertex.x(), FloatFormat::fixed).c_str();

      auto coordinateYElement = cartesianPointElement.append_child("Coordinate");
      coordinateYElement.text() = openstudio::string_conversions::number(vertex.y(), FloatFormat::fixed).c_str();

      auto coordinateZElement = cartesianPointElement.append_child("Coordinate");
      coordinateZElement.text() = openstudio::string_conversions::number(vertex.z(), FloatFormat::fixed).c_str();
    }

    // export CADObjectId if present
    if (!translateCADObjectId(subSurface, result)) {
      boost::optional<model::SubSurface> otherSubSurface = subSurface.adjacentSubSurface();
      if (otherSubSurface) {
        translateCADObjectId(*otherSubSurface, result);
      }
    }
    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateShadingSurface(const openstudio::model::ShadingSurface& shadingSurface,
                                                                             pugi::xml_node& parent) {
    // return if already translated
    if (m_translatedObjects.find(shadingSurface.handle()) != m_translatedObjects.end()) {
      return boost::none;
    }

    auto result = parent.append_child("Surface");
    m_translatedObjects[shadingSurface.handle()] = result;

    translateId(shadingSurface, result);
    translateName(shadingSurface, result);

    result.append_attribute("surfaceType") = "Shade";

    // construction
    boost::optional<model::ConstructionBase> construction = shadingSurface.construction();
    if (construction) {
      std::string constructionName = construction->name().get();
      if (construction->isOpaque()) {
        result.append_attribute("constructionIdRef") = escapeName(constructionName).c_str();
      } else {
        result.append_attribute("windowTypeIdRef") = escapeName(constructionName).c_str();
      }
    } else {
      // no construction is relevant here, but we don't want to fail validation
      std::string name{shadingSurfaceWithoutConstructionName};

      result.append_attribute("constructionIdRef") = name.c_str();

      if (!m_placeholderShadingSurfaceConstructionAlreadyCreated) {
        // parent is Facility, and its parent is the gbXML element
        auto gbXMLElement = parent.parent();
        auto node = gbXMLElement.append_child("Construction");
        node.append_attribute("id") = name.c_str();
        node.append_child("Name").text() = name.c_str();
        // Switch bool off
        m_placeholderShadingSurfaceConstructionAlreadyCreated = true;
      }
    }

    // this space
    Transformation transformation;
    boost::optional<model::Space> space = shadingSurface.space();
    if (space) {
      boost::optional<model::ShadingSurfaceGroup> shadingSurfaceGroup = shadingSurface.shadingSurfaceGroup();
      if (shadingSurfaceGroup) {
        transformation = shadingSurfaceGroup->siteTransformation();
      } else {
        transformation = space->siteTransformation();
      }

      std::string spaceName = space->name().get();
      auto adjacentSpaceIdElement = result.append_child("AdjacentSpaceId");
      adjacentSpaceIdElement.append_attribute("spaceIdRef") = escapeName(spaceName).c_str();
    } else {
      boost::optional<model::ShadingSurfaceGroup> shadingSurfaceGroup = shadingSurface.shadingSurfaceGroup();
      if (shadingSurfaceGroup) {
        transformation = shadingSurfaceGroup->siteTransformation();

        std::string spaceName = shadingSurfaceGroup->name().get();
        auto adjacentSpaceIdElement = result.append_child("AdjacentSpaceId");
        adjacentSpaceIdElement.append_attribute("spaceIdRef") = escapeName(spaceName).c_str();
      }
    }

    // transform vertices to world coordinates
    Point3dVector vertices = transformation * shadingSurface.vertices();

    // check if we can make rectangular geometry
    OptionalVector3d outwardNormal = getOutwardNormal(vertices);
    double area = shadingSurface.grossArea();
    if (outwardNormal && area > 0) {

      // get tilt, duplicate code in planar surface
      Vector3d up(0.0, 0.0, 1.0);
      double tiltRadians = getAngle(*outwardNormal, up);

      // get azimuth, duplicate code in planar surface
      Vector3d north(0.0, 1.0, 0.0);
      double azimuthRadians = getAngle(*outwardNormal, north);
      if (outwardNormal->x() < 0.0) {
        azimuthRadians = -azimuthRadians + 2.0 * boost::math::constants::pi<double>();
      }

      // transform vertices to face coordinates
      Transformation faceTransformation = Transformation::alignFace(vertices);
      Point3dVector faceVertices = faceTransformation.inverse() * vertices;
      BoundingBox faceBoundingBox;
      faceBoundingBox.addPoints(faceVertices);
      double width = faceBoundingBox.maxX().get() - faceBoundingBox.minX().get();
      double height = faceBoundingBox.maxY().get() - faceBoundingBox.minY().get();
      double areaCorrection = 1.0;
      if (width > 0 && height > 0) {
        areaCorrection = sqrt(area / (width * height));
      }

      // pick lower left corner vertex in face coordinates
      double minY = std::numeric_limits<double>::max();
      double minX = std::numeric_limits<double>::max();
      size_t llcIndex = 0;
      size_t N = vertices.size();
      for (size_t i = 0; i < N; ++i) {
        double z = faceVertices[i].z();
        OS_ASSERT(std::abs(z) < 0.001);
        if ((minY > faceVertices[i].y()) || ((minY > faceVertices[i].y() - 0.00001) && (minX > faceVertices[i].x()))) {
          llcIndex = i;
          minY = faceVertices[i].y();
          minX = faceVertices[i].x();
        }
      }
      Point3d vertex = vertices[llcIndex];

      // rectangular geometry
      auto rectangularGeometryElement = result.append_child("RectangularGeometry");

      auto azimuthElement = rectangularGeometryElement.append_child("Azimuth");
      azimuthElement.text() = openstudio::string_conversions::number(radToDeg(azimuthRadians), FloatFormat::general).c_str();

      auto cartesianPointElement = rectangularGeometryElement.append_child("CartesianPoint");

      auto coordinateXElement = cartesianPointElement.append_child("Coordinate");
      coordinateXElement.text() = openstudio::string_conversions::number(vertex.x(), FloatFormat::fixed).c_str();

      auto coordinateYElement = cartesianPointElement.append_child("Coordinate");
      coordinateYElement.text() = openstudio::string_conversions::number(vertex.y(), FloatFormat::fixed).c_str();

      auto coordinateZElement = cartesianPointElement.append_child("Coordinate");
      coordinateZElement.text() = openstudio::string_conversions::number(vertex.z(), FloatFormat::fixed).c_str();

      auto tiltElement = rectangularGeometryElement.append_child("Tilt");
      tiltElement.text() = openstudio::string_conversions::number(radToDeg(tiltRadians), FloatFormat::general).c_str();

      auto widthElement = rectangularGeometryElement.append_child("Width");
      widthElement.text() = openstudio::string_conversions::number(areaCorrection * width, FloatFormat::fixed).c_str();

      auto heightElement = rectangularGeometryElement.append_child("Height");
      heightElement.text() = openstudio::string_conversions::number(areaCorrection * height, FloatFormat::fixed).c_str();
    }

    // planar geometry
    auto planarGeometryElement = result.append_child("PlanarGeometry");

    auto polyLoopElement = planarGeometryElement.append_child("PolyLoop");
    for (const Point3d& vertex : vertices) {
      auto cartesianPointElement = polyLoopElement.append_child("CartesianPoint");

      auto coordinateXElement = cartesianPointElement.append_child("Coordinate");
      coordinateXElement.text() = openstudio::string_conversions::number(vertex.x(), FloatFormat::fixed).c_str();

      auto coordinateYElement = cartesianPointElement.append_child("Coordinate");
      coordinateYElement.text() = openstudio::string_conversions::number(vertex.y(), FloatFormat::fixed).c_str();

      auto coordinateZElement = cartesianPointElement.append_child("Coordinate");
      coordinateZElement.text() = openstudio::string_conversions::number(vertex.z(), FloatFormat::fixed).c_str();
    }

    // export CADObjectId if present
    translateCADObjectId(shadingSurface, result);

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateThermalZone(const openstudio::model::ThermalZone& thermalZone, pugi::xml_node& parent) {
    auto result = parent.append_child("Zone");
    m_translatedObjects[thermalZone.handle()] = result;

    translateId(thermalZone, result);
    translateName(thermalZone, result);

    // heating setpoint
    boost::optional<double> designHeatT;
    boost::optional<double> designCoolT;
    boost::optional<model::Thermostat> thermostat = thermalZone.thermostat();
    if (thermostat && thermostat->optionalCast<model::ThermostatSetpointDualSetpoint>()) {
      auto thermostatDualSetpoint = thermostat->cast<model::ThermostatSetpointDualSetpoint>();

      boost::optional<model::Schedule> heatingSchedule = thermostatDualSetpoint.heatingSetpointTemperatureSchedule();
      if (heatingSchedule) {
        if (heatingSchedule->optionalCast<model::ScheduleRuleset>()) {
          auto scheduleRuleset = heatingSchedule->cast<model::ScheduleRuleset>();
          model::ScheduleDay winterDesignDaySchedule = scheduleRuleset.winterDesignDaySchedule();
          std::vector<double> values = winterDesignDaySchedule.values();
          if (!values.empty()) {
            designHeatT = *std::max_element(values.begin(), values.end());
          }
        }
      }

      boost::optional<model::Schedule> coolingSchedule = thermostatDualSetpoint.coolingSetpointTemperatureSchedule();
      if (coolingSchedule) {
        if (coolingSchedule->optionalCast<model::ScheduleRuleset>()) {
          auto scheduleRuleset = coolingSchedule->cast<model::ScheduleRuleset>();
          model::ScheduleDay summerDesignDaySchedule = scheduleRuleset.summerDesignDaySchedule();
          std::vector<double> values = summerDesignDaySchedule.values();
          if (!values.empty()) {
            designCoolT = *std::min_element(values.begin(), values.end());
          }
        }
      }
    }

    if (designHeatT) {
      auto designHeatTElement = result.append_child("DesignHeatT");
      designHeatTElement.append_attribute("unit") = "C";
      designHeatTElement.text() = openstudio::string_conversions::number(*designHeatT, FloatFormat::fixed).c_str();
    }

    if (designCoolT) {
      auto designCoolTElement = result.append_child("DesignCoolT");
      designCoolTElement.append_attribute("unit") = "C";
      designCoolTElement.text() = openstudio::string_conversions::number(*designCoolT, FloatFormat::fixed).c_str();
    }

    // export CADObjectId if present
    translateCADObjectId(thermalZone, result);

    return result;
  }

  void ForwardTranslator::translateId(const openstudio::model::ModelObject& modelObject, pugi::xml_node& parentElement) {

    std::string id = modelObject.name().get();
    if (modelObject.gbXMLId()) {
      id = modelObject.gbXMLId().get();
    }
    parentElement.append_attribute("id") = escapeName(id).c_str();
  }

  void ForwardTranslator::translateName(const openstudio::model::ModelObject& modelObject, pugi::xml_node& parentElement) {

    std::string name = modelObject.name().get();
    if (modelObject.displayName()) {
      name = modelObject.displayName().get();
    }
    parentElement.append_child("Name").text() = name.c_str();
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateCADObjectId(const openstudio::model::ModelObject& modelObject,
                                                                          pugi::xml_node& parentElement) {
    boost::optional<pugi::xml_node> result;

    if (modelObject.hasAdditionalProperties()) {
      model::AdditionalProperties additionalProperties = modelObject.additionalProperties();
      if (boost::optional<std::string> cadObjectId = modelObject.cadObjectId()) {
        auto cadObjectIdElement = parentElement.append_child("CADObjectId");
        cadObjectIdElement.text() = (*cadObjectId).c_str();
        result = cadObjectIdElement;
        if (additionalProperties.hasFeature("programIdRef")) {
          boost::optional<std::string> programIdRef = additionalProperties.getFeatureAsString("programIdRef");
          if (programIdRef) {
            cadObjectIdElement.append_attribute("programIdRef") = (*programIdRef).c_str();
          }
        }
      }
    }
    return result;
  }

}  // namespace gbxml
}  // namespace openstudio
