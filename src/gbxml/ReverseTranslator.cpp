/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
#include "../model/ConstructionAirBoundary.hpp"
#include "../model/ConstructionAirBoundary_Impl.hpp"
#include "../model/AdditionalProperties.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/xml/XMLValidator.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <resources.hxx>

#include <thread>

#include <pugixml.hpp>
#include <algorithm>
#include <locale>

namespace openstudio {
namespace gbxml {

  std::ostream& operator<<(std::ostream& os, const pugi::xml_node& element) {
    pugi::xml_document doc;
    doc.append_copy(element);
    doc.save(os, "  ");
    return os;
  }

  ReverseTranslator::ReverseTranslator() : m_nonBaseMultiplier(1.0), m_lengthMultiplier(1.0) {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.gbxml\\.ReverseTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  ReverseTranslator::~ReverseTranslator() = default;

  boost::optional<openstudio::model::Model> ReverseTranslator::loadModel(const openstudio::path& path, ProgressBar* progressBar) {
    m_progressBar = progressBar;

    m_logSink.setThreadId(std::this_thread::get_id());

    m_logSink.resetStringStream();

    m_idToObjectMap.clear();

    boost::optional<openstudio::model::Model> result;

    if (openstudio::filesystem::exists(path)) {

      openstudio::filesystem::ifstream file(path, std::ios_base::binary);
      if (file.is_open()) {
        pugi::xml_document doc;
        auto load_result = doc.load(file);
        if (load_result) {
          auto root = doc.document_element();
          if (std::string_view{root.name()} != "gbXML") {
            LOG(Error, "Expected the root element to be <gbXML>");
            return boost::none;
          }
          // Scan version of gbxml schema
          std::string version = root.attribute("version").value();
          VersionString schemaVersion(7, 3);
          if (version.empty()) {
            LOG(Warn, "gbXML has no `version` attribute for the schema version, assuming 7.03.");
          } else {
            try {
              schemaVersion = VersionString(version);
            } catch (...) {
              LOG(Warn, "gbXML has `version` '" << version << "' which was not understood, assuming 7.03.");
            }
          }
          if (schemaVersion == VersionString(7, 3)) {
            // validate the gbxml prior to reverse translation
            auto gbxmlValidator = XMLValidator::gbxmlValidator(schemaVersion);
            gbxmlValidator.validate(path);
          } else {
            LOG(Error,
                "Version of schema specified: " << version
                                                << ", expected 7.03. gbXML Schema Validation skipped. Note that ReverseTranslator rules are built "
                                                   "for 7.03 and older versions are not officially supported, check resulting model with care.");
          };

          result = this->convert(root);
        }
        file.close();
      }
      // JWD: Would be nice to add some error handling here
    }

    return result;
  }

  std::vector<LogMessage> ReverseTranslator::warnings() const {
    std::vector<LogMessage> result;
    std::vector<LogMessage> allMessages = m_logSink.logMessages();
    std::copy_if(allMessages.cbegin(), allMessages.cend(), std::back_inserter(result),
                 [](const auto& logMessage) { return logMessage.logLevel() == Warn; });
    return result;
  }

  std::vector<LogMessage> ReverseTranslator::errors() const {
    std::vector<LogMessage> result;
    std::vector<LogMessage> allMessages = m_logSink.logMessages();
    std::copy_if(allMessages.cbegin(), allMessages.cend(), std::back_inserter(result),
                 [](const auto& logMessage) { return logMessage.logLevel() > Warn; });
    return result;
  }

  std::string ReverseTranslator::escapeName(const std::string& id, const std::string& name) {
    std::string value = id;
    if (!name.empty()) {
      value = name;
    }
    std::replace(value.begin(), value.end(), ',', '-');
    std::replace(value.begin(), value.end(), ';', '-');
    return value;
  }

  boost::optional<model::Model> ReverseTranslator::convert(const pugi::xml_node& root) {
    return translateGBXML(root);
  }

  boost::optional<model::Model> ReverseTranslator::translateGBXML(const pugi::xml_node& root) {
    openstudio::model::Model model;
    model.setFastNaming(true);

    // gbXML attributes not mapped directly to IDF, but needed to map

    // {F, C, K, R}
    // JWD: the previous check was not great, this one is better and more true to the schema
    std::string temperatureUnit{root.attribute("temperatureUnit").value()};
    if (temperatureUnit == "F") {
      m_temperatureUnit = UnitFactory::instance().createUnit("F").get();
    } else if (temperatureUnit == "C") {
      m_temperatureUnit = UnitFactory::instance().createUnit("C").get();
    } else if (temperatureUnit == "K") {
      m_temperatureUnit = UnitFactory::instance().createUnit("K").get();
    } else if (temperatureUnit == "R") {
      m_temperatureUnit = UnitFactory::instance().createUnit("R").get();
    } else {
      LOG(Warn, "No recognized temperature unit specified, using C");
      m_temperatureUnit = UnitFactory::instance().createUnit("C").get();
    }

    // {Kilometers, Centimeters, Millimeters, Meters, Miles, Yards, Feet, Inches}
    // JWD: the previous check was not great, this one is better, though still not exactly true to the schema
    std::string lengthUnit{root.attribute("lengthUnit").value()};
    if (istringEqual(lengthUnit, "Kilometers")) {
      m_nonBaseMultiplier = 1000.0;
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    } else if (istringEqual(lengthUnit, "Centimeters")) {
      m_nonBaseMultiplier = 1.0e-2;
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    } else if (istringEqual(lengthUnit, "Millimeters")) {
      m_nonBaseMultiplier = 1.0e-3;
      m_lengthUnit = UnitFactory::instance().createUnit("K").get();
    } else if (istringEqual(lengthUnit, "Meters")) {
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    } else if (istringEqual(lengthUnit, "Miles")) {
      m_lengthUnit = UnitFactory::instance().createUnit("mi").get();
    } else if (istringEqual(lengthUnit, "Yards")) {
      m_nonBaseMultiplier = 3.0;
      m_lengthUnit = UnitFactory::instance().createUnit("ft").get();
    } else if (istringEqual(lengthUnit, "Feet")) {
      m_lengthUnit = UnitFactory::instance().createUnit("ft").get();
    } else if (istringEqual(lengthUnit, "Inches")) {
      m_lengthUnit = UnitFactory::instance().createUnit("in").get();
    } else {
      LOG(Warn, "No length unit specified, using Meters");
      m_lengthUnit = UnitFactory::instance().createUnit("m").get();
    }

    Quantity unitLength(m_nonBaseMultiplier, m_lengthUnit);
    Unit targetUnit = UnitFactory::instance().createUnit("m").get();
    m_lengthMultiplier = QuantityConverter::instance().convert(unitLength, targetUnit)->value();

    // {SquareKilometers, SquareMeters, SquareCentimeters, SquareMillimeters, SquareMiles, SquareYards, SquareFeet, SquareInches}
    // TODO: still need some help with some units
    std::string areaUnit = root.attribute("areaUnit").value();

    // {CubicKilometers, CubicMeters, CubicCentimeters, CubicMillimeters, CubicMiles, CubicYards, CubicFeet, CubicInches}
    // TODO: still need some help with some units
    std::string volumeUnit = root.attribute("volumeUnit").value();

    // {true, false}
    m_useSIUnitsForResults = true;
    std::string useSIUnitsForResults{root.attribute("useSIUnitsForResults").value()};
    if (istringEqual(useSIUnitsForResults, "False")) {
      m_useSIUnitsForResults = false;
    }

    // do materials before constructions
    auto materialElements = root.children("Material");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Materials"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(materialElements.begin(), materialElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& materialElement : materialElements) {
      boost::optional<model::ModelObject> material = translateMaterial(materialElement, model);
      OS_ASSERT(material);  // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do constructions before surfaces
    std::unordered_map<std::string, pugi::xml_node> layerElements;
    for (auto& layerEl : root.children("Layer")) {
      std::string layerId = layerEl.attribute("id").value();
      // Could maybe use some error checking here for empty IDs
      layerElements[layerId] = layerEl;
    }

    auto constructionElements = root.children("Construction");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(constructionElements.begin(), constructionElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& constructionElement : constructionElements) {
      boost::optional<model::ModelObject> construction = translateConstruction(constructionElement, layerElements, model);
      OS_ASSERT(construction);  // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do window type before sub surfaces
    auto windowTypeElements = root.children("WindowType");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Window Types"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(windowTypeElements.begin(), windowTypeElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& windowTypeElement : windowTypeElements) {
      boost::optional<model::ModelObject> construction = translateWindowType(windowTypeElement, model);
      OS_ASSERT(construction);  // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do schedules before loads
    auto scheduleElements = root.children("Schedule");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Schedules"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(scheduleElements.begin(), scheduleElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& scheduleElement : scheduleElements) {
      boost::optional<model::ModelObject> schedule = translateSchedule(scheduleElement, root, model);
      OS_ASSERT(schedule);  // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // do thermal zones before spaces
    auto zoneElements = root.children("Zone");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Zones"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(zoneElements.begin(), zoneElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& zoneElement : zoneElements) {
      boost::optional<model::ModelObject> zone = translateThermalZone(zoneElement, model);
      OS_ASSERT(zone);  // Krishnan, what type of error handling do you want?

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    auto campusElement = root.child("Campus");
    OS_ASSERT(campusElement.next_sibling("Campus").empty());
    boost::optional<model::ModelObject> facility = translateCampus(campusElement, model);
    OS_ASSERT(facility);  // Krishnan, what type of error handling do you want?

    model.setFastNaming(false);

    return model;
  }

  // A 'quick and dirty' method to find and correct surfaces that have incorrect orientations
  // Checks the surface against the space bounding box
  // If the surface is at or near the upper bound of the bounding box it should be a Roof/Ceiling
  // If the surface is at or near the lower bound of the bounding box it should be a Floor
  // Works only for spaces with 3D shapes that are prisms in the sense that they have only two
  // levels where there are horizontal surfaces.
  void ReverseTranslator::validateSpaceSurfaces(openstudio::model::Model& model) {

    double tol = 0.001;

    const auto& spaces = model.getConcreteModelObjects<openstudio::model::Space>();
    for (const auto& space : spaces) {
      std::string spaceName = space.name().value();

      const auto& bounds = space.boundingBox();
      auto surfaces = space.surfaces();
      for (auto& surface : surfaces) {
        std::string surfType = surface.surfaceType();
        std::string surfName = surface.name().value();

        // Look for Roof or Floor surfaces that have adjacent surface (if there's no adjacwent surface
        // then the spaces cannot be in the wrong order and the orientation would have already been fixed)
        boost::optional<openstudio::model::Surface> adjacentSurf = surface.adjacentSurface();
        if ((surfType == "RoofCeiling" || surfType == "Floor") && adjacentSurf) {
          auto vertices = surface.vertices();

          if (std::abs(vertices[0].z() - bounds.maxZ().value()) > tol && std::abs(vertices[0].z() - bounds.minZ().value()) > tol) {

            // Log this because we cant do a face orientation check because the space
            // isnt a prism (it has > 2 levels of horizontal surfaces)
            LOG(Warn, "Skipping surface " << surfName << " of type " << surfType << " because it is not a prism");
            continue;
          }

          if (std::abs(vertices[0].z() - bounds.maxZ().value()) <= tol) {

            // Surface is at the top of the space bounding box so it should be a roof/ceiling
            // and the normal should be up (z should be > 0)
            auto surfType = surface.surfaceType();
            if (surfType != "RoofCeiling") {
              // Log changing surface type
              LOG(Warn, "Changing surface type from " << surfType << " to RoofCeiling. Surface vertices elevation is above the space.");
              surface.setSurfaceType("RoofCeiling");
            }
            const auto& normal = surface.outwardNormal();
            if (normal.z() < 0) {
              // Log reversing surface
              LOG(Warn, "Reversing surface orientation because surface is a RoofCeiling but the surface is oriented down.");
              std::reverse(vertices.begin(), vertices.end());
              surface.setVertices(vertices);
            }
          } else if (std::abs(vertices[0].z() - bounds.minZ().value()) <= tol) {

            // Surface is at the bottom of the space's bounding box and so should be a floor
            // and the normal shuld be down (z < 0)
            auto surfType = surface.surfaceType();
            if (surfType != "Floor") {
              // Log changing surface type
              surface.setSurfaceType("Floor");
            }
            const auto& normal = surface.outwardNormal();
            if (normal.z() > 0) {
              // Log reversing surface
              LOG(Warn, "Reversing surface orientation because surface is a Floor but the surface is oriented up.");
              std::reverse(vertices.begin(), vertices.end());
              surface.setVertices(vertices);
            }
          }
        }
      }
    }
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateCampus(const pugi::xml_node& element, openstudio::model::Model& model) {
    auto facility = model.getUniqueModelObject<openstudio::model::Facility>();

    auto buildingElement = element.child("Building");
    OS_ASSERT(buildingElement.next_sibling("Building").empty());

    boost::optional<model::ModelObject> building = translateBuilding(buildingElement, model);
    OS_ASSERT(building);

    auto surfaceElements = element.children("Surface");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Surfaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(surfaceElements.begin(), surfaceElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& surfEl : surfaceElements) {
      try {
        boost::optional<model::ModelObject> surface = translateSurface(surfEl, model);
      } catch (const std::exception&) {
        LOG(Error, "Could not translate surface " << surfEl);
      }

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    validateSpaceSurfaces(model);
    return facility;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateBuilding(const pugi::xml_node& element, openstudio::model::Model& model) {
    openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();

    translateId(element, building);
    translateName(element, building);

    auto storyElements = element.children("BuildingStorey");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Building Stories"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(storyElements.begin(), storyElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& storyEl : storyElements) {
      boost::optional<model::ModelObject> story = translateBuildingStory(storyEl, model);
      OS_ASSERT(story);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    auto spaceElements = element.children("Space");
    if (m_progressBar) {
      m_progressBar->setWindowTitle(toString("Translating Spaces"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum((int)std::distance(spaceElements.begin(), spaceElements.end()));
      m_progressBar->setValue(0);
    }

    for (auto& spaceEl : spaceElements) {
      boost::optional<model::ModelObject> space = translateSpace(spaceEl, model);
      OS_ASSERT(space);

      if (m_progressBar) {
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    return building;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateBuildingStory(const pugi::xml_node& element, openstudio::model::Model& model) {
    openstudio::model::BuildingStory story(model);

    translateId(element, story);
    translateName(element, story);

    // DLM: we need to better support separate name from id in this translator

    // DLM: todo, translate Level

    return story;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateThermalZone(const pugi::xml_node& element, openstudio::model::Model& model) {
    openstudio::model::ThermalZone zone(model);

    translateId(element, zone);
    translateName(element, zone);

    // DLM: todo, translate setpoints

    // import CADObjectId
    for (auto& cadObjectId : element.children("CADObjectId")) {
      // TODO: import multiple CADObjectIds
      translateCADObjectId(cadObjectId, zone);

      break;  // TODO: import multiple CADObjectIds
    }

    return zone;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateSpace(const pugi::xml_node& element, openstudio::model::Model& model) {
    openstudio::model::Space space(model);

    translateId(element, space);
    translateName(element, space);

    //DLM: we should be using a map of id to model object to get this, not relying on name
    std::string storyId = element.attribute("buildingStoreyIdRef").value();
    auto storyIt = m_idToObjectMap.find(storyId);
    if (storyIt != m_idToObjectMap.end()) {
      boost::optional<model::BuildingStory> story = storyIt->second.optionalCast<model::BuildingStory>();
      if (story) {
        space.setBuildingStory(*story);
      }
    }

    // if space doesn't have story assigned should we warn the user?

    std::string zoneId = element.attribute("zoneIdRef").value();
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
      std::string id = element.attribute("id").value();
      std::string name = element.child("Name").text().as_string();
      thermalZone.setName(id + " ThermalZone");
      thermalZone.additionalProperties().setFeature("gbXMLId", id + " Thermal Zone");
      thermalZone.additionalProperties().setFeature("displayName", name + " Thermal Zone");
      space.setThermalZone(thermalZone);
    }

    // create a stub space type
    // DLM: is this better than nothing?
    std::string spaceTypeId = element.attribute("spaceType").value();
    if (!spaceTypeId.empty()) {
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

    // import CADObjectId
    for (auto& cadObjectId : element.children("CADObjectId")) {
      // TODO: import multiple CADObjectIds
      translateCADObjectId(cadObjectId, space);

      break;  // TODO: import multiple CADObjectIds
    }

    // import Volume
    auto volume = element.child("Volume").text();
    if (!volume.empty()) {
      space.setVolume(volume.as_double());
    }

    return space;
  }

  boost::optional<model::ModelObject> ReverseTranslator::translateSurface(const pugi::xml_node& element, openstudio::model::Model& model) {
    boost::optional<model::ModelObject> result;
    std::vector<openstudio::Point3d> vertices;

    auto planarGeometryElement = element.child("PlanarGeometry");
    auto polyLoopElement = planarGeometryElement.child("PolyLoop");
    auto cartesianPointElements = polyLoopElement.children("CartesianPoint");

    for (auto& cart_el : cartesianPointElements) {
      auto coordinateElements = cart_el.children("Coordinate");
      OS_ASSERT(std::distance(coordinateElements.begin(), coordinateElements.end()) == 3);

      /* Calling these conversions every time is unnecessarily slow

      Unit targetUnit = UnitFactory::instance().createUnit("m").get();
      Quantity xQuantity(coordinateElements.at(0).toElement().text().toDouble(), m_lengthUnit);
      Quantity yQuantity(coordinateElements.at(1).toElement().text().toDouble(), m_lengthUnit);
      Quantity zQuantity(coordinateElements.at(2).toElement().text().toDouble(), m_lengthUnit);

      double x = QuantityConverter::instance().convert(xQuantity, targetUnit)->value();
      double y = QuantityConverter::instance().convert(yQuantity, targetUnit)->value();
      double z = QuantityConverter::instance().convert(zQuantity, targetUnit)->value();
      */

      std::array<double, 3> coords{{0.0, 0.0, 0.0}};
      size_t i{0};
      for (auto& el : coordinateElements) {
        coords[i] = m_lengthMultiplier * el.text().as_double();
        ++i;
        if (i == 3) {
          break;
        }
      }

      vertices.emplace_back(coords[0], coords[1], coords[2]);
    }

    std::string surfaceType = element.attribute("surfaceType").value();
    if (surfaceType.find("Shade") != std::string::npos) {

      openstudio::model::ShadingSurface shadingSurface(vertices, model);

      translateId(element, shadingSurface);
      translateName(element, shadingSurface);

      openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();

      std::vector<openstudio::model::ShadingSurfaceGroup> shadingSurfaceGroups = building.shadingSurfaceGroups();
      if (shadingSurfaceGroups.empty()) {
        openstudio::model::ShadingSurfaceGroup shadingSurfaceGroup(model);
        shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup);
      } else {
        shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroups[0]);
      }

      result = shadingSurface;

    } else if ((surfaceType.find("FreestandingColumn") != std::string::npos) || (surfaceType.find("EmbeddedColumn") != std::string::npos)) {

      // do not handle these
      return boost::none;

    } else {

      // The rest of the function wants array-type access to the elements, so create that
      std::vector<pugi::xml_node> adjacentSpaceElements;
      for (auto& adj : element.children("AdjacentSpaceId")) {
        adjacentSpaceElements.push_back(adj);
      }

      if (adjacentSpaceElements.empty()) {
        LOG(Warn, "Surface has no adjacent spaces, will not be translated.");
        return boost::none;
      } else if (adjacentSpaceElements.size() == 2) {
        std::string spaceId1 = adjacentSpaceElements.at(0).attribute("spaceIdRef").value();
        std::string spaceId2 = adjacentSpaceElements.at(1).attribute("spaceIdRef").value();
        if (spaceId1 == spaceId2) {
          LOG(Warn, "Surface has two adjacent spaces which are the same space '" << spaceId2 << "', will not be translated.");
          return boost::none;
        }
      } else if (adjacentSpaceElements.size() > 2) {
        LOG(Error, "Surface has more than 2 adjacent surfaces, will not be translated.");
        return boost::none;
      }

      openstudio::model::Surface surface(vertices, model);

      translateId(element, surface);
      translateName(element, surface);

      std::string exposedToSun = element.attribute("exposedToSun").value();

      // set surface type
      // wall types
      if (surfaceType.find("ExteriorWall") != std::string::npos) {
        surface.setSurfaceType("Wall");
      } else if (surfaceType.find("InteriorWall") != std::string::npos) {
        surface.setSurfaceType("Wall");
      } else if (surfaceType.find("UndergroundWall") != std::string::npos) {
        surface.setSurfaceType("Wall");
        // roof types
      } else if (surfaceType.find("Roof") != std::string::npos) {
        surface.setSurfaceType("RoofCeiling");
      } else if (surfaceType.find("Ceiling") != std::string::npos) {
        surface.setSurfaceType("RoofCeiling");
      } else if (surfaceType.find("UndergroundCeiling") != std::string::npos) {
        surface.setSurfaceType("RoofCeiling");
        // floor types
      } else if (surfaceType.find("UndergroundSlab") != std::string::npos) {
        surface.setSurfaceType("Floor");
      } else if (surfaceType.find("SlabOnGrade") != std::string::npos) {
        surface.setSurfaceType("Floor");
      } else if (surfaceType.find("InteriorFloor") != std::string::npos) {
        surface.setSurfaceType("Floor");
      } else if (surfaceType.find("RaisedFloor") != std::string::npos) {
        surface.setSurfaceType("Floor");
      } else if (surfaceType.find("ExposedFloor") != std::string::npos) {
        surface.setSurfaceType("Floor");
      } else if (surfaceType.find("Air") != std::string::npos) {
        // this type can be wall, roof, or floor.  just use default surface type based on vertices.
      }

      // set boundary conditions
      if (exposedToSun.find("true") != std::string::npos) {
        surface.setOutsideBoundaryCondition("Outdoors");
        surface.setSunExposure("SunExposed");
        surface.setWindExposure("WindExposed");
      } else if (surfaceType.find("Interior") != std::string::npos) {
        surface.setOutsideBoundaryCondition("Adiabatic");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      } else if (surfaceType.find("Ceiling") != std::string::npos) {
        surface.setOutsideBoundaryCondition("Adiabatic");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      } else if (surfaceType.find("SlabOnGrade") != std::string::npos) {
        surface.setOutsideBoundaryCondition("Ground");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      } else if (surfaceType.find("Underground") != std::string::npos) {
        surface.setOutsideBoundaryCondition("Ground");
        surface.setSunExposure("NoSun");
        surface.setWindExposure("NoWind");
      }

      result = surface;

      // if air wall
      if (surfaceType.find("Air") != std::string::npos) {
        boost::optional<model::ConstructionAirBoundary> airWall;

        for (const auto& construction : model.getConcreteModelObjects<model::ConstructionAirBoundary>()) {
          airWall = construction;
          break;
        }
        if (!airWall) {
          airWall = model::ConstructionAirBoundary(model);
        }
        surface.setConstruction(*airWall);

        // don't translate subsurfaces of air walls?

      } else {
        // not air wall

        // translate construction
        std::string constructionIdRef = element.attribute("constructionIdRef").value();
        auto constructionIt = m_idToObjectMap.find(constructionIdRef);
        if (constructionIt != m_idToObjectMap.end()) {
          boost::optional<model::ConstructionBase> construction = constructionIt->second.optionalCast<model::ConstructionBase>();
          if (construction) {
            surface.setConstruction(*construction);
          }
        }

        // translate subSurfaces
        for (auto& subsurf : element.children("Opening")) {
          try {
            boost::optional<model::ModelObject> subSurface = translateSubSurface(subsurf, surface);
          } catch (const std::exception&) {
            LOG(Error, "Could not translate sub surface " << subsurf);
          }
        }
      }

      // adjacent surfaces
      std::string spaceId = adjacentSpaceElements.at(0).attribute("spaceIdRef").value();
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

        std::string adjacentSpaceId = adjacentSpaceElements.at(1).attribute("spaceIdRef").value();
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
            std::string spaceSurfaceType = adjacentSpaceElements.at(0).attribute("surfaceType").value();

            // spaceSurfaceType is the surfaceType that should be in the second space
            std::string adjacentSpaceSurfaceType = adjacentSpaceElements.at(1).attribute("surfaceType").value();

            // we will mark figuredOut as true if we are sure of our assignment, otherwise we will log a warning
            bool figuredOut = false;

            // set reverseConstruction if construction should be applied to the surface created in adjacent space
            bool reverseConstruction = false;

            if (spaceSurfaceType.empty() && adjacentSpaceSurfaceType.empty()) {

              // this is ok but gives us no new information, no warning issued

            } else if (spaceSurfaceType.empty() && !adjacentSpaceSurfaceType.empty()) {

              // if one adjacent space surfaceType is given then the other should be too
              LOG(Warn, "Only one adjacent surfaceType listed for '" << surface.name().get() << "'");

            } else if (!spaceSurfaceType.empty() && adjacentSpaceSurfaceType.empty()) {

              // if one adjacent surfaceType is given then the other should be too
              LOG(Warn, "Only one adjacent surfaceType listed for '" << surface.name().get() << "'");

            } else {

              // have both spaceSurfaceType and adjacentSpaceSurfaceType

              if (currentSurfaceType == "RoofCeiling" || currentSurfaceType == "Floor") {

                // both the spaceSurfaceType and adjacentSpaceSurfaceType should be either Ceiling or InteriorFloor
                if (!((spaceSurfaceType == "InteriorFloor" || spaceSurfaceType == "Ceiling")
                      && (adjacentSpaceSurfaceType == "InteriorFloor" || adjacentSpaceSurfaceType == "Ceiling"))) {

                  // one of the spaces lists this as a wall or some other surfaceType
                  // we could try to reapply the surfaceType from the gbXML back here, but then we would be in the same problem as before
                  // at least now we have a surface type that matches the vertex outward normal
                  LOG(Warn, "Adjacent surfaceTypes '" << spaceSurfaceType << "' and  '" << adjacentSpaceSurfaceType << "' listed for '"
                                                      << surface.name().get() << "' do not match vertices");

                } else if (spaceSurfaceType == adjacentSpaceSurfaceType) {

                  // both spaceSurfaceType and adjacentSpaceSurfaceType are either InteriorFloor or Ceiling, not allowed
                  LOG(Warn, "Duplicate surfaceType '" << spaceSurfaceType << "' listed for '" << surface.name().get() << "'");

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
                      LOG(Warn, "Outward normal for '" << surface.name().get() << "' does not match surfaceType '" << spaceSurfaceType
                                                       << "' attribute of first AdjacentSpaceID");

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
                      LOG(Warn, "Outward normal for '" << surface.name().get() << "' does not match surfaceType attribute '" << spaceSurfaceType
                                                       << "' of first AdjacentSpaceID");

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
                LOG(Warn, "Changing surface type from '" << gbXMLSurfaceType << "' to '" << currentSurfaceType << "' for surface '"
                                                         << surface.name().get() << "'");
              }
            }

            // clone the surface and sub surfaces and reverse vertices
            boost::optional<openstudio::model::Surface> otherSurface = surface.createAdjacentSurface(*adjacentSpace);
            if (otherSurface) {
              if (reverseConstruction) {
                boost::optional<openstudio::model::ConstructionBase> construction = surface.construction();
                if (construction) {
                  otherSurface->setConstruction(*construction);
                  surface.resetConstruction();  // will be inherited from the adjacent surface
                }
              }
            } else {
              LOG(Error, "Could not create adjacent surface in adjacent space '" << adjacentSpace->name().get() << "' for surface '"
                                                                                 << surface.name().get() << "' in space '" << space->name().get()
                                                                                 << "'");
            }
          }
        } else {
          LOG(Error, "Could not find adjacent space '" << adjacentSpaceId << "' for surface '" << surface.name().get() << "'");
        }
      }
    }

    OS_ASSERT(result);

    // import CADObjectId
    for (auto& cadObjectId : element.children("CADObjectId")) {
      translateCADObjectId(cadObjectId, *result);
    }

    // merge additional properties for reversed surfaces and sub surfaces
    if (boost::optional<model::Surface> _surface = result->optionalCast<model::Surface>()) {

      boost::optional<openstudio::model::Surface> otherSurface = _surface->adjacentSurface();
      if (otherSurface) {
        otherSurface->additionalProperties().merge(result->additionalProperties(), true);
        otherSurface->additionalProperties().setFeature("gbXMLReversed", true);
      }

      for (const auto& subSurface : _surface->subSurfaces()) {
        boost::optional<openstudio::model::SubSurface> otherSubSurface = subSurface.adjacentSubSurface();
        if (otherSubSurface) {
          otherSubSurface->additionalProperties().merge(subSurface.additionalProperties(), true);
          otherSubSurface->additionalProperties().setFeature("gbXMLReversed", true);
        }

        subSurface.additionalProperties().setFeature("gbXMLReversed", false);
      }

      _surface->additionalProperties().setFeature("gbXMLReversed", false);
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateSubSurface(const pugi::xml_node& element,
                                                                                         openstudio::model::Surface& surface) {
    openstudio::model::Model model = surface.model();

    boost::optional<model::ModelObject> result;

    std::vector<openstudio::Point3d> vertices;

    auto planarGeometryElement = element.child("PlanarGeometry");
    auto polyLoopElement = planarGeometryElement.child("PolyLoop");
    auto cartesianPointElements = polyLoopElement.children("CartesianPoint");

    for (auto& cart_el : cartesianPointElements) {
      auto coordinateElements = cart_el.children("Coordinate");
      OS_ASSERT(std::distance(coordinateElements.begin(), coordinateElements.end()) == 3);

      /* Calling these conversions every time is unnecessarily slow

      Unit targetUnit = UnitFactory::instance().createUnit("m").get();
      Quantity xQuantity(coordinateElements.at(0).toElement().text().toDouble(), m_lengthUnit);
      Quantity yQuantity(coordinateElements.at(1).toElement().text().toDouble(), m_lengthUnit);
      Quantity zQuantity(coordinateElements.at(2).toElement().text().toDouble(), m_lengthUnit);

      double x = QuantityConverter::instance().convert(xQuantity, targetUnit)->value();
      double y = QuantityConverter::instance().convert(yQuantity, targetUnit)->value();
      double z = QuantityConverter::instance().convert(zQuantity, targetUnit)->value();
      */
      std::array<double, 3> coords{{0.0, 0.0, 0.0}};
      size_t i{0};
      for (auto& el : coordinateElements) {
        coords[i] = m_lengthMultiplier * el.text().as_double();
        ++i;
        if (i == 3) {
          break;
        }
      }

      vertices.emplace_back(coords[0], coords[1], coords[2]);
    }

    openstudio::model::SubSurface subSurface(vertices, model);
    subSurface.setSurface(surface);

    translateId(element, subSurface);
    translateName(element, subSurface);

    result = subSurface;

    // translate openingType
    std::string openingType = element.attribute("openingType").value();
    if (openingType.find("FixedWindow") != std::string::npos) {
      subSurface.setSubSurfaceType("FixedWindow");
    } else if (openingType.find("OperableWindow") != std::string::npos) {
      subSurface.setSubSurfaceType("OperableWindow");
    } else if (openingType.find("FixedSkylight") != std::string::npos) {
      subSurface.setSubSurfaceType("Skylight");
    } else if (openingType.find("OperableSkylight") != std::string::npos) {
      subSurface.setSubSurfaceType("Skylight");
    } else if (openingType.find("NonSlidingDoor") != std::string::npos) {  // do before testing contains door
      subSurface.setSubSurfaceType("Door");
    } else if (openingType.find("SlidingDoor") != std::string::npos) {
      subSurface.setSubSurfaceType("GlassDoor");
    } else if (openingType.find("Air") != std::string::npos) {
      // use default sub surface type?
    }

    // if air wall
    if (openingType.find("Air") != std::string::npos) {
      boost::optional<model::ConstructionAirBoundary> airWall;

      for (const auto& construction : model.getConcreteModelObjects<model::ConstructionAirBoundary>()) {
        airWall = construction;
        break;
      }
      if (!airWall) {
        airWall = model::ConstructionAirBoundary(model);
      }
      subSurface.setConstruction(*airWall);

    } else {

      // translate construction
      std::string constructionIdRef = element.attribute("constructionIdRef").value();
      if (constructionIdRef.empty()) {
        constructionIdRef = element.attribute("windowTypeIdRef").value();
      }
      auto constructionIt = m_idToObjectMap.find(constructionIdRef);
      if (constructionIt != m_idToObjectMap.end()) {
        boost::optional<model::ConstructionBase> construction = constructionIt->second.optionalCast<model::ConstructionBase>();
        if (construction) {
          subSurface.setConstruction(*construction);
        }
      }
    }

    // todo: translate "interiorShadeType", "exteriorShadeType", and other properties of the opening

    // import CADObjectId
    for (auto& cadObjectId : element.children("CADObjectId")) {
      translateCADObjectId(cadObjectId, subSurface);
    }

    return result;
  }

  void ReverseTranslator::translateId(const pugi::xml_node& element, openstudio::model::ModelObject& modelObject) {
    std::string id = element.attribute("id").value();
    m_idToObjectMap.insert(std::make_pair(id, modelObject));
    modelObject.setName(id);
    modelObject.setGBXMLId(id);
  }

  void ReverseTranslator::translateName(const pugi::xml_node& element, openstudio::model::ModelObject& modelObject) {

    auto name = element.child("Name").text();
    if (!name.empty()) {
      modelObject.setDisplayName(name.as_string());
    }
  }

  void ReverseTranslator::translateCADObjectId(const pugi::xml_node& element, openstudio::model::ModelObject& modelObject) {

    auto cadObjectId = element.text();
    if (!cadObjectId.empty()) {
      modelObject.setCADObjectId(cadObjectId.as_string());

      auto programIdRef = element.attribute("programIdRef");
      if (programIdRef) {
        model::AdditionalProperties result = modelObject.additionalProperties();
        result.setFeature("programIdRef", programIdRef.value());
      }
    }
  }

}  // namespace gbxml
}  // namespace openstudio
