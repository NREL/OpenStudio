/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ForwardTranslator.hpp"

#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/DaylightingControl.hpp"
#include "../model/GlareSensor.hpp"
#include "../model/InteriorPartitionSurfaceGroup.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFile_Impl.hpp"
#include "../model/RadianceParameters.hpp"
#include "../model/RadianceParameters_Impl.hpp"
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/ShadingControl.hpp"
#include "../model/ShadingControl_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/WindowPropertyFrameAndDivider.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/Timestep.hpp"
#include "../model/Timestep_Impl.hpp"
#include "../model/SpaceItem.hpp"
#include "../model/SpaceItem_Impl.hpp"
#include "../model/SimulationControl.hpp"
#include "../model/SimulationControl_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/People.hpp"
#include "../model/People_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/OutputVariable.hpp"
#include "../model/OutputVariable_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/time/DateTime.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Intersection.hpp"
#include "../utilities/bcl/BCL.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/bcl/LocalBCL.hpp"

#include <thread>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/math/constants/constants.hpp>

#include <radiance/embedded_files.hxx>

#include <cstring>
#include <cmath>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <math.h>

using openstudio::Point3d;
using openstudio::Point3dVector;
using openstudio::Transformation;
using openstudio::model::Building;
using openstudio::model::Space;
using openstudio::model::Surface;
using openstudio::model::SubSurface;
using openstudio::model::ShadingSurface;
using openstudio::model::ShadingSurfaceGroup;
using openstudio::model::InteriorPartitionSurface;
using openstudio::model::InteriorPartitionSurfaceGroup;
using openstudio::model::OptionalBuilding;
using openstudio::model::OptionalSpace;
using openstudio::model::OptionalShadingSurfaceGroup;
using openstudio::model::OptionalInteriorPartitionSurfaceGroup;
using openstudio::model::OptionalSurface;

using OFSTREAM = openstudio::filesystem::basic_ofstream<char>;

namespace openstudio {
namespace radiance {

  // internal method used to format doubles as strings
  std::string formatString(double t_d, unsigned t_prec) {
    std::stringstream ss;
    ss << std::setprecision(t_prec) << std::noshowpoint << std::fixed << t_d;
    std::string s = ss.str();

    /*
    // truncate 0's from the end
    int i = s.size() - 1;
    while (i > 0 && s[i] == '0')
    {
      --i;
    }

    if (i > 0)
    {
      s.erase(i + 1);

      if (*s.rbegin() == '.')
      {
        s.push_back('0');
      }
    }
    */

    return s;
  }

  // internal method used to format all other types as strings
  template <typename T>
  std::string formatString(const T& t) {
    return boost::lexical_cast<std::string>(t);
  }

  // basic constructor
  ForwardTranslator::ForwardTranslator()
    : m_windowGroupId(1)  // m_windowGroupId is reserved for uncontrolled
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.radiance\\.ForwardTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
  }

  std::vector<openstudio::path> ForwardTranslator::translateModel(const openstudio::path& outPath, const openstudio::model::Model& model) {
    m_model = model.clone(true).cast<openstudio::model::Model>();

    m_model.purgeUnusedResourceObjects();

    m_logSink.setThreadId(std::this_thread::get_id());

    m_logSink.resetStringStream();

    clear();

    // TODO: check for shading controls, set m_shadingControls

    std::vector<openstudio::path> outfiles;

    // preprocess the model here
    unsigned numSpacesToSimulate = 0;
    for (openstudio::model::Space space : m_model.getConcreteModelObjects<openstudio::model::Space>()) {

      // remove any spaces not associated with a thermal zone
      boost::optional<openstudio::model::ThermalZone> thermalZone = space.thermalZone();
      if (!thermalZone) {
        LOG(Error, "Model translation failure; space " << space.name().get() << " is not associated with a ThermalZone, could not be translated.");
        space.remove();
        continue;
      }

      // only allow assigned primary daylighting control points (as defined in thermal zone) through
      for (openstudio::model::DaylightingControl daylightingControl : space.daylightingControls()) {
        if (daylightingControl.isPrimaryDaylightingControl()) {
          // ok
        } else if (daylightingControl.isSecondaryDaylightingControl()) {
          LOG(Warn,
              "Secondary DaylightingControl " << daylightingControl.name().get() << " is not supported by Radiance, it will not be translated.");
          daylightingControl.remove();
        } else {
          LOG(Warn, "DaylightingControl " << daylightingControl.name().get()
                                          << " is not associated with this Space's ThermalZone, it will not be translated.");
          daylightingControl.remove();
        }
      }

      std::vector<openstudio::model::DaylightingControl> daylightingControls = space.daylightingControls();
      std::vector<openstudio::model::IlluminanceMap> illuminanceMaps = space.illuminanceMaps();
      std::vector<openstudio::model::GlareSensor> glareSensors = space.glareSensors();

      if (!daylightingControls.empty() && !illuminanceMaps.empty()) {
        numSpacesToSimulate += 1;

        if (glareSensors.empty()) {
          LOG(Warn, "Space " << space.name().get() << " contains Radiance daylighting objects but no GlareSensor objects.");
        }
      }
    }

    // remove unsupported shading controls
    for (auto& shadingControl : model.getConcreteModelObjects<openstudio::model::ShadingControl>()) {
      std::string shadingType = shadingControl.shadingType();
      bool supported = false;
      if (istringEqual("InteriorShade", shadingType)) {
        supported = true;
      } else if (istringEqual("ExteriorShade", shadingType)) {
        supported = false;
      } else if (istringEqual("ExteriorScreen", shadingType)) {
        supported = false;
      } else if (istringEqual("InteriorBlind", shadingType)) {
        supported = true;
      } else if (istringEqual("ExteriorBlind", shadingType)) {
        supported = false;
      } else if (istringEqual("BetweenGlassShade", shadingType)) {
        supported = false;
      } else if (istringEqual("BetweenGlassBlind", shadingType)) {
        supported = false;
      } else if (istringEqual("SwitchableGlazing", shadingType)) {
        supported = true;
      } else if (istringEqual("InteriorDaylightRedirectionDevice", shadingType)) {
        supported = true;
      } else {
        supported = false;
        LOG(Warn, "Unknown shadingType '" << shadingType << "' found for ShadingControl '" << shadingControl.name().get() << "'");
      }

      if (!supported) {
        LOG(Warn, "Removing ShadingControl '" << shadingControl.name().get() << "' with unsupported shadingType '" << shadingType << "'");
        shadingControl.remove();
        continue;
      }

      std::string shadingControlType = shadingControl.shadingControlType();
      if (istringEqual("AlwaysOff", shadingControlType)) {
        LOG(Info, "Removing ShadingControl '" << shadingControl.name().get() << "' with shadingControlType '" << shadingControlType << "'");
        continue;
      }
    }

    if (numSpacesToSimulate == 0) {
      LOG(Error, "Model does not contain any spaces configured for Radiance; check illuminance map and daylighting control point assignments.");
      return outfiles;
    }

    // catch any directory or file related issues here
    try {

      LOG(Debug, "Working Directory: " + openstudio::toString(outPath));

      if (openstudio::filesystem::exists(outPath)) {
        openstudio::filesystem::remove_all(outPath);
      }

      openstudio::filesystem::create_directories(outPath);

      //create radiance dirs for model export
      openstudio::path radDir = outPath;
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("scene"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("scene/glazing"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("scene/shades"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("materials"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("numeric"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("views"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("options"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("bsdf"));
      openstudio::filesystem::create_directory(radDir / openstudio::toPath("skies"));

      // get the building
      openstudio::model::Building building = m_model.getUniqueModelObject<openstudio::model::Building>();

      // get the site
      auto site = m_model.getUniqueModelObject<openstudio::model::Site>();

      // get site and building shading
      LOG(Debug, "Processing site/building shading elements...");

      // get building shading
      buildingShadingSurfaceGroups(radDir, building.shadingSurfaceGroups(), outfiles);

      // get site shading
      siteShadingSurfaceGroups(radDir, site.shadingSurfaceGroups(), outfiles);

      // get spaces
      buildingSpaces(radDir, building.spaces(), outfiles);

      // write options files
      std::string dcmatsStringin;
      std::ifstream dcmatfilein(openstudio::toSystemFilename(radDir / openstudio::toPath("materials/materials_dc.rad")));

      while (dcmatfilein.good()) {
        std::string line;
        std::getline(dcmatfilein, line);
        dcmatsStringin += line;
      }

      std::vector<std::string> dcmatsArray;
      boost::algorithm::split(dcmatsArray, dcmatsStringin, boost::is_any_of(","));

      int totalVectors = dcmatsArray.size();

      LOG(Debug, "Total aperture headings: " << totalVectors);
      if (totalVectors > 8) {
        LOG(Warn, formatString(totalVectors) << " glazing orientations detected (can cause long simulation times if using daylight coefficients).");
      }

      //    std::string dcmatsString = dcmatsStringin.gsub(',', ' ');

      // get Radiance sim settings
      auto radianceParameters = m_model.getUniqueModelObject<openstudio::model::RadianceParameters>();

      // write Radiance options to file(s)

      // view matrix options

      openstudio::path vmxoptpath = radDir / openstudio::toPath("options/vmx.opt");
      OFSTREAM vmxopt(vmxoptpath);
      if (vmxopt.is_open()) {
        outfiles.push_back(vmxoptpath);
        vmxopt << "-ab " << (int)radianceParameters.ambientBouncesVMX() << " " << "-ad " << (int)radianceParameters.ambientDivisionsVMX() << " "
               << "-as " << (int)radianceParameters.ambientSupersamples()
               << " "
               // << "-c " << (int)radianceParameters.accumulatedRaysperRecord() << " "
               << "-dj " << radianceParameters.directJitter() << " " << "-dp " << radianceParameters.directPretest() << " " << "-dt "
               << radianceParameters.directThreshold() << " " << "-dc " << radianceParameters.directCertainty() << " " << "-lw "
               << radianceParameters.limitWeightVMX() << " ";
      } else {

        LOG(Error, "Cannot open file '" << toString(vmxoptpath) << "' for writing");
      }

      // daylight matrix options

      openstudio::path dmxoptpath = radDir / openstudio::toPath("options/dmx.opt");
      OFSTREAM dmxopt(dmxoptpath);
      if (dmxopt.is_open()) {
        outfiles.push_back(dmxoptpath);
        dmxopt << "-ab " << (int)radianceParameters.ambientBouncesDMX() << " " << "-ad " << (int)radianceParameters.ambientDivisionsDMX() << " "
               << "-as " << (int)radianceParameters.ambientSupersamples()
               << " "
               // << "-c " << (int)radianceParameters.accumulatedRaysperRecord() << " "
               << "-dj " << radianceParameters.directJitter() << " " << "-dp " << radianceParameters.directPretest() << " " << "-dt "
               << radianceParameters.directThreshold() << " " << "-dc " << radianceParameters.directCertainty() << " " << "-lw "
               << radianceParameters.limitWeightDMX() << " ";
      } else {

        LOG(Error, "Cannot open file '" << toString(dmxoptpath) << "' for writing");
      }

      // Tregenza/Klems resolution options
      openstudio::path tregoptpath = radDir / openstudio::toPath("options/treg.opt");
      OFSTREAM tregopt(tregoptpath);
      if (tregopt.is_open()) {
        outfiles.push_back(tregoptpath);
        tregopt << "-c " << (int)radianceParameters.klemsSamplingDensity() << " ";
        tregopt << "-e MF:1 -f tregenza.cal -b tbin -bn Ntbins";
        //restricted to Klems, user is warned when writing out window groups. 2015.09.13 RPG
        //if (radianceParameters.skyDiscretizationResolution() == "146"){
        //  tregopt << "-e MF:1 -f tregenza.cal -b tbin -bn Ntbins";
        //} else if (radianceParameters.skyDiscretizationResolution() == "578"){
        //  tregopt << "-e MF:2 -f reinhart.cal -b rbin -bn Nrbins";
        //} else if (radianceParameters.skyDiscretizationResolution() == "2306"){
        //  tregopt << "-e MF:4 -f reinhart.cal -b rbin -bn Nrbins";
        //}
        // TODO: make these values into a pulldown choice, add support for higher resolution sampling bases and out of bounds
      } else {
        LOG(Error, "Cannot open file '" << toString(tregoptpath) << "' for writing");
      }

      // write dc sky sampling file
      openstudio::path dcskyfilepath = radDir / openstudio::toPath("skies/dc_sky.rad");
      OFSTREAM skyfile(dcskyfilepath);
      if (skyfile.is_open()) {
        outfiles.push_back(dcskyfilepath);
        // ground first (P0)!
        skyfile << "#@rfluxmtx h=u u=Y\nvoid glow groundglow\n0\n0\n4\n1 1 1 0\n\ngroundglow source ground\n0\n0\n4\n0 0 -1 180\n";
        skyfile << "#@rfluxmtx h=r1 u=Y\nvoid glow skyglow\n0\n0\n4\n1 1 1 0\n\nskyglow source sky\n0\n0\n4\n0 0 1 180\n";

      } else {
        LOG(Error, "Cannot open file '" << toString(dcskyfilepath) << "' for writing");
      }

      // write sample sky for renderings
      openstudio::path renderskyfilepath = radDir / openstudio::toPath("skies/03211200_clr.sky");
      OFSTREAM renderskyfile(renderskyfilepath);
      if (renderskyfile.is_open()) {
        outfiles.push_back(renderskyfilepath);
        renderskyfile << "# CIE clear sky - Berkeley, CA - March 21 12:00 \n\n";
        renderskyfile << "void light solar\n0\n0\n3 6.92e+06 6.92e+06 6.92e+06\n\n";
        renderskyfile << "solar source sun\n0\n0\n4 0.067845 -0.617210 0.783868 0.5\n\n";
        renderskyfile << "void brightfunc skyfunc\n2 skybr skybright.cal\n0\n7 1 1.16e+01 2.37e+01 6.92e-01 0.067845 -0.617210 0.783868\n\n";
        renderskyfile << "skyfunc glow skyglow\n0\n0\n4 1.000 1.000 1.000 0\n\n";
        renderskyfile << "skyglow source sky\n0\n0\n4 0 0 1 180\n\n";
        renderskyfile << "skyfunc glow groundglow\n0\n0\n4 1.000 1.000 1.000 0\n\n";
        renderskyfile << "groundglow source ground\n0\n0\n4 0 0 -1 180\n\n";

      } else {
        LOG(Error, "Cannot open file '" << toString(renderskyfilepath) << "' for writing");
      }

      // Hi Qual options (illuminance maps)
      openstudio::path mapsoptpath = radDir / openstudio::toPath("options/maps.opt");
      OFSTREAM mapsopt(mapsoptpath);
      if (mapsopt.is_open()) {
        outfiles.push_back(mapsoptpath);
        mapsopt << "-ab 10 -ad 10000 -as 50 -dt 0 -dc 1 -ds 0.05 -lw 0.00001";
      } else {
        LOG(Error, "Cannot open file '" << toString(mapsoptpath) << "' for writing");
      }

      // the end
      LOG(Debug, "Done. Radiance model located at: " << openstudio::toString(radDir) << ".");

    } catch (const std::exception& e) {
      outfiles.clear();
      std::string error = e.what();
      LOG(Error, error);
    }

    return outfiles;
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const {
    std::vector<LogMessage> result;

    for (const LogMessage& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() == Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const {
    std::vector<LogMessage> result;

    for (const LogMessage& logMessage : m_logSink.logMessages()) {
      if (logMessage.logLevel() > Warn) {
        result.push_back(logMessage);
      }
    }

    return result;
  }

  openstudio::Point3dVectorVector ForwardTranslator::getPolygons(const openstudio::model::Surface& surface) {
    openstudio::Point3dVectorVector result;

    Transformation buildingTransformation;
    OptionalBuilding building = surface.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = surface.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    // transformation from space coordinates to face coordinates
    Transformation alignFace = Transformation::alignFace(surface.vertices());

    // get the current vertices and convert to face coordinates
    Point3dVector surfaceFaceVertices = alignFace.inverse() * surface.vertices();

    // boost polygon wants vertices in clockwise order, faceVertices must be reversed, otherFaceVertices already CCW
    std::reverse(surfaceFaceVertices.begin(), surfaceFaceVertices.end());

    // get the current subsurfaces and convert to face coordinates
    std::vector<std::vector<Point3d>> holes;
    for (const SubSurface& subSurface : surface.subSurfaces()) {
      Point3dVector hole = alignFace.inverse() * subSurface.vertices();
      std::reverse(hole.begin(), hole.end());
      holes.push_back(hole);
    }

    // perform the subtraction
    std::vector<std::vector<Point3d>> faceResult = openstudio::subtract(surfaceFaceVertices, holes, 0.01);

    if (faceResult.empty()) {
      // DLM: is this an error (fail simulation) or a warning?  Should we attempt to put the whole surface in here?
      LOG(Warn, "Failed to create surface polygons for Surface '" << surface.nameString() << "'");
    }

    // convert to absolute coordinates
    for (const Point3dVector& face : faceResult) {
      Point3dVector worldFace = buildingTransformation * spaceTransformation * alignFace * face;
      std::reverse(worldFace.begin(), worldFace.end());
      result.push_back(worldFace);
    }

    return result;
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::SubSurface& subSurface) {
    Transformation buildingTransformation;
    OptionalBuilding building = subSurface.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSurface surface = subSurface.surface();
    if (surface) {
      OptionalSpace space = surface->space();
      if (space) {
        spaceTransformation = space->transformation();
      }
    } else {
      LOG(Warn, "Could not retrieve surface for sub surface '" << subSurface.name() << "'");
    }

    openstudio::Point3dVector vertices = subSurface.vertices();

    // apply frame and divider reveal
    boost::optional<openstudio::model::WindowPropertyFrameAndDivider> frameAndDivider = subSurface.windowPropertyFrameAndDivider();
    if (frameAndDivider) {
      if (!frameAndDivider->isOutsideRevealDepthDefaulted()) {
        openstudio::Vector3d offset = -frameAndDivider->outsideRevealDepth() * subSurface.outwardNormal();

        for (openstudio::Point3d& vertex : vertices) {
          vertex = vertex + offset;
        }
      }
    }

    // convert vertices to absolute coordinates
    return buildingTransformation * spaceTransformation * vertices;
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::ShadingSurface& shadingSurface) {
    Transformation buildingTransformation;
    OptionalBuilding building = shadingSurface.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation shadingSurfaceGroupTransformation;
    Transformation spaceTransformation;
    OptionalShadingSurfaceGroup shadingSurfaceGroup = shadingSurface.shadingSurfaceGroup();
    if (shadingSurfaceGroup) {
      shadingSurfaceGroupTransformation = shadingSurfaceGroup->transformation();

      OptionalSpace space = shadingSurfaceGroup->space();
      if (space) {
        spaceTransformation = space->transformation();
      }
    }

    Transformation transformation;
    std::string shadingSurfaceType = shadingSurfaceGroup->shadingSurfaceType();
    if (istringEqual("Site", shadingSurfaceType)) {
      transformation = shadingSurfaceGroupTransformation;
    } else if (istringEqual("Building", shadingSurfaceType)) {
      transformation = buildingTransformation * shadingSurfaceGroupTransformation;
    } else if (istringEqual("Space", shadingSurfaceType)) {
      transformation = buildingTransformation * spaceTransformation * shadingSurfaceGroupTransformation;
    } else {
      LOG(Error, "Unknown shading surface type for shading surface '" << shadingSurface.name() << "'");
      return {};
    }

    // convert vertices to absolute coordinates
    return transformation * shadingSurface.vertices();
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::InteriorPartitionSurface& interiorPartitionSurface) {
    Transformation buildingTransformation;
    OptionalBuilding building = interiorPartitionSurface.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation interiorPartitionSurfaceGroupTransformation;
    Transformation spaceTransformation;
    OptionalInteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup = interiorPartitionSurface.interiorPartitionSurfaceGroup();
    if (interiorPartitionSurfaceGroup) {
      interiorPartitionSurfaceGroupTransformation = interiorPartitionSurfaceGroup->transformation();

      OptionalSpace space = interiorPartitionSurfaceGroup->space();
      if (space) {
        spaceTransformation = space->transformation();
      }
    }

    // convert vertices to absolute coordinates
    return buildingTransformation * spaceTransformation * interiorPartitionSurfaceGroupTransformation * interiorPartitionSurface.vertices();
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::Luminaire& luminaire) {
    Transformation buildingTransformation;
    OptionalBuilding building = luminaire.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = luminaire.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    openstudio::Point3dVector triangle;
    triangle.push_back(Point3d(0, 0, 0));
    triangle.push_back(Point3d(1, 0, 0));
    triangle.push_back(Point3d(0, 1, 0));

    // convert vertices to absolute coordinates
    return buildingTransformation * spaceTransformation * luminaire.transformation() * triangle;
  }

  openstudio::Point3d ForwardTranslator::getReferencePoint(const openstudio::model::DaylightingControl& control) {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    return buildingTransformation * spaceTransformation * control.transformation() * Point3d(0, 0, 0);
  }

  openstudio::Vector3d ForwardTranslator::getSensorVector(const openstudio::model::DaylightingControl& control) {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    Point3d endPoint = buildingTransformation * spaceTransformation * control.transformation() * Point3d(0, 0, 1);
    Point3d startPoint = buildingTransformation * spaceTransformation * control.transformation() * Point3d(0, 0, 0);

    return (endPoint - startPoint);
  }

  openstudio::Vector3dVector ForwardTranslator::getViewVectors(const openstudio::model::DaylightingControl& control) {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    openstudio::Vector3dVector result;

    double pi = boost::math::constants::pi<double>();
    unsigned n = control.numberofDaylightingViews();
    Point3d startPoint = buildingTransformation * spaceTransformation * control.transformation() * Point3d(0, 0, 0);
    for (unsigned i = 0; i < n; ++i) {
      double angle = 2.0 * i * pi / (double(n));
      Point3d endPoint = buildingTransformation * spaceTransformation * control.transformation() * Point3d(sin(angle), cos(angle), 0);
      result.push_back(endPoint - startPoint);
    }

    return result;
  }

  openstudio::Vector3dVector ForwardTranslator::getViewVectors(const openstudio::model::GlareSensor& control) {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    openstudio::Vector3dVector result;

    double pi = boost::math::constants::pi<double>();
    unsigned n = control.numberofGlareViewVectors();
    Point3d startPoint = buildingTransformation * spaceTransformation * control.transformation() * Point3d(0, 0, 0);
    for (unsigned i = 0; i < n; ++i) {
      double angle = 2.0 * i * pi / (double(n));
      Point3d endPoint = buildingTransformation * spaceTransformation * control.transformation() * Point3d(sin(angle), cos(angle), 0);
      result.push_back(endPoint - startPoint);
    }

    return result;
  }

  openstudio::Point3d ForwardTranslator::getReferencePoint(const openstudio::model::GlareSensor& sensor) {
    Transformation buildingTransformation;
    OptionalBuilding building = sensor.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = sensor.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    return buildingTransformation * spaceTransformation * sensor.transformation() * Point3d(0, 0, 0);
  }

  openstudio::Vector3d ForwardTranslator::getSensorVector(const openstudio::model::GlareSensor& sensor) {
    Transformation buildingTransformation;
    OptionalBuilding building = sensor.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = sensor.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    Point3d endPoint = buildingTransformation * spaceTransformation * sensor.transformation() * Point3d(0, 0, 1);
    Point3d startPoint = buildingTransformation * spaceTransformation * sensor.transformation() * Point3d(0, 0, 0);

    return (endPoint - startPoint);
  }

  openstudio::Point3dVector ForwardTranslator::getReferencePoints(const openstudio::model::IlluminanceMap& map) {
    Transformation buildingTransformation;
    OptionalBuilding building = map.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = map.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    // convert vertices to absolute coordinates
    return buildingTransformation * spaceTransformation * map.transformation() * map.referencePoints();
  }

  openstudio::Vector3d ForwardTranslator::getSensorVector(const openstudio::model::IlluminanceMap& map) {
    Transformation buildingTransformation;
    OptionalBuilding building = map.model().getOptionalUniqueModelObject<Building>();
    if (building) {
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = map.space();
    if (space) {
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    return buildingTransformation * spaceTransformation * map.transformation() * Vector3d(0, 0, 1);
  }

  void ForwardTranslator::clear() {
    m_radMaterials.clear();
    m_radMixMaterials.clear();
    m_radMaterialsDC.clear();
    m_radMaterialsWG0.clear();
    m_radMaterialsSwitchableBase.clear();

    m_radDCmats.clear();

    m_radSceneFiles.clear();

    m_radSpaces.clear();
    m_radSensors.clear();
    m_radGlareSensors.clear();
    m_radMaps.clear();
    m_radMapHandles.clear();
    m_radViewPoints.clear();
    m_radWindowGroups.clear();
    m_radWindowGroupShades.clear();
  }

  WindowGroup ForwardTranslator::getWindowGroup(const openstudio::Vector3d& outwardNormal, const model::Space& space,
                                                const model::ConstructionBase& construction,
                                                const boost::optional<model::ShadingControl>& shadingControl,
                                                const openstudio::Point3dVector& polygon) {
    WindowGroup result(outwardNormal, space, construction, shadingControl);

    std::stringstream ss;
    ss << "WG" << m_windowGroupId;
    std::string name = ss.str();

    // set the uncontrolled window group name
    if (!shadingControl) {
      name = "WG0";
    }

    result.setName(name);

    auto it = std::find(m_windowGroups.begin(), m_windowGroups.end(), result);
    if (it != m_windowGroups.end()) {
      it->addWindowPolygon(polygon);
      return *it;
    }

    // if we made a new group
    if (shadingControl) {
      m_windowGroupId += 1;
    }

    result.addWindowPolygon(polygon);
    m_windowGroups.push_back(result);
    return result;
  }

  void ForwardTranslator::siteShadingSurfaceGroups(const openstudio::path& t_radDir,
                                                   const std::vector<openstudio::model::ShadingSurfaceGroup>& radShadingSurfaceGroups,
                                                   std::vector<openstudio::path>& t_outfiles) {
    LOG(Debug, "site shading groups found: " << radShadingSurfaceGroups.size());

    if (!radShadingSurfaceGroups.empty()) {
      std::set<std::string> siteShadingSurfaces;

      for (const auto& shadingSurfaceGroup : radShadingSurfaceGroups) {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup.shadingSurfaces();
        for (const auto& shadingSurface : shadingSurfaces) {
          // clean name
          std::string shadingSurface_name = cleanName(shadingSurface.name().get());

          LOG(Debug, "site shading surface: " << shadingSurface_name);

          // get reflectance
          double interiorVisibleReflectance = 0.25;  // default for site shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()) {
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }

          double exteriorVisibleReflectance = 0.25;  // default
          if (shadingSurface.exteriorVisibleAbsorptance()) {
            double exteriorVisibleAbsorptance = shadingSurface.exteriorVisibleAbsorptance().get();
            exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
          }

          // write (two-sided) material
          // exterior reflectance for front side
          m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                + formatString(exteriorVisibleReflectance, 3) + " " + formatString(exteriorVisibleReflectance, 3) + " "
                                + formatString(exteriorVisibleReflectance, 3) + " 0 0\n\n");

          // interior reflectance for back side
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");

          // roll up into a mixfunc...
          // void mixfunc overhang
          // 4 front back if(Rdot,1,0) .
          // 0
          // 0
          m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                   + formatString(exteriorVisibleReflectance, 3) + "\n4 " + "refl_" + formatString(exteriorVisibleReflectance, 3)
                                   + " " + "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

          // polygon header
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);

          std::string shadingsurface = "reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                       + formatString(exteriorVisibleReflectance, 3) + " polygon " + shadingSurface_name + "\n";
          shadingsurface += "0\n0\n" + formatString(polygon.size() * 3) + "\n";

          for (auto vertex = polygon.begin(); vertex != polygon.end(); ++vertex) {
            shadingsurface += formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n";
          }

          siteShadingSurfaces.insert(shadingsurface);
        }
      }

      openstudio::path filename = t_radDir / openstudio::toPath("/scene/shading_site.rad");
      OFSTREAM f(filename);
      if (f.is_open()) {
        t_outfiles.push_back(filename);
        m_radSceneFiles.push_back(filename);

        for (const auto& line : siteShadingSurfaces) {
          f << line;
        }
      } else {
        LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
      }
    }
  }

  void ForwardTranslator::buildingShadingSurfaceGroups(const openstudio::path& t_radDir,
                                                       const std::vector<openstudio::model::ShadingSurfaceGroup>& t_radShadingSurfaceGroups,
                                                       std::vector<openstudio::path>& t_outfiles) {
    LOG(Debug, "building shading groups found: " << t_radShadingSurfaceGroups.size());

    if (!t_radShadingSurfaceGroups.empty()) {
      std::set<std::string> buildingShadingSurfaces;

      for (const auto& group : t_radShadingSurfaceGroups) {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = group.shadingSurfaces();

        for (const auto& shadingSurface : shadingSurfaces) {
          // clean name
          std::string shadingSurface_name = cleanName(shadingSurface.name().get());

          LOG(Debug, "building shading surface: " << shadingSurface_name);

          // get reflectance
          double interiorVisibleReflectance = 0.25;  // default for building shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()) {
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }

          double exteriorVisibleReflectance = 0.25;  // default
          if (shadingSurface.exteriorVisibleAbsorptance()) {
            double exteriorVisibleAbsorptance = shadingSurface.exteriorVisibleAbsorptance().get();
            exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
          }

          // write (two-sided) material
          // exterior reflectance for front side
          m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                + formatString(exteriorVisibleReflectance, 3) + " " + formatString(exteriorVisibleReflectance, 3) + " "
                                + formatString(exteriorVisibleReflectance, 3) + " 0 0\n\n");

          // interior reflectance for back side
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");

          // roll up into a mixfunc...
          // void mixfunc overhang
          // 4 front back if(Rdot,1,0) .
          // 0
          // 0
          m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                   + formatString(exteriorVisibleReflectance, 3) + "\n4 " + "refl_" + formatString(exteriorVisibleReflectance, 3)
                                   + " " + "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

          // start polygon
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);

          // header
          std::string shadingsurface = "reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                       + formatString(exteriorVisibleReflectance, 3) + " polygon " + shadingSurface_name + "\n";
          shadingsurface += "0\n0\n" + formatString(polygon.size() * 3) + "\n";

          //

          for (const auto& vertex : polygon) {
            shadingsurface += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n\n";
          }

          buildingShadingSurfaces.insert(shadingsurface);
        }
      }

      openstudio::path filename = t_radDir / openstudio::toPath("scene/shading_building.rad");
      OFSTREAM f(filename);
      if (f.is_open()) {
        t_outfiles.push_back(filename);
        m_radSceneFiles.push_back(filename);
        for (const auto& line : buildingShadingSurfaces) {
          f << line;
        }
      } else {
        LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
      }
    }
  }

  void ForwardTranslator::buildingSpaces(const openstudio::path& t_radDir, const std::vector<openstudio::model::Space>& t_spaces,
                                         std::vector<openstudio::path>& t_outfiles) {
    std::vector<std::string> space_names;

    for (const auto& space : t_spaces) {
      std::string space_name = cleanName(space.name().get());

      space_names.push_back(space_name);
      LOG(Debug, "Processing space: " << space_name);

      // split model into zone-based Radiance .rad files
      m_radSpaces[space_name] = "#\n# geometry file for space: " + space_name + "\n#\n\n";

      // loop over surfaces in space

      std::vector<openstudio::model::Surface> surfaces = space.surfaces();

      for (const auto& surface : surfaces) {

        // skip if air wall
        if (surface.isAirWall()) {
          continue;
        }

        std::string surface_name = cleanName(surface.name().get());

        // add surface to space geometry
        m_radSpaces[space_name] += "# surface: " + surface_name + "\n";

        // set construction of surface
        std::string constructionName = surface.getString(2).get();
        m_radSpaces[space_name] += "# construction: " + constructionName + "\n";

        // get reflectances
        double interiorVisibleReflectance = 0.5;  // default for space surfaces
        if (surface.interiorVisibleAbsorptance()) {
          double interiorVisibleAbsorptance = surface.interiorVisibleAbsorptance().get();
          interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
        }
        double exteriorVisibleReflectance = 0.25;  // default for space surfaces (exterior)
        if (surface.exteriorVisibleAbsorptance()) {
          double exteriorVisibleAbsorptance = surface.exteriorVisibleAbsorptance().get();
          exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
        }

        // create polygon object
        openstudio::Point3dVectorVector polygons = openstudio::radiance::ForwardTranslator::getPolygons(surface);
        for (const openstudio::Point3dVector& polygon : polygons) {

          if (!surface.adjacentSurface()) {
            // 2-sided material

            // header
            m_radSpaces[space_name] += "# reflectance (int) = " + formatString(interiorVisibleReflectance, 3)
                                       + "\n# reflectance (ext) = " + formatString(exteriorVisibleReflectance, 3) + "\n";

            // material definition

            //interior
            m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                  + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                  + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
            //exterior
            m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                  + formatString(exteriorVisibleReflectance, 3) + " " + formatString(exteriorVisibleReflectance, 3) + " "
                                  + formatString(exteriorVisibleReflectance, 3) + " 0 0\n\n");
            // mixfunc
            m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                     + formatString(exteriorVisibleReflectance, 3) + "\n4 " + "refl_" + formatString(exteriorVisibleReflectance, 3)
                                     + " " + "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

            // polygon reference
            m_radSpaces[space_name] += "reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                       + formatString(exteriorVisibleReflectance, 3) + " polygon " + surface_name + "\n0\n0\n"
                                       + formatString(polygon.size() * 3) + "\n";
          } else {
            // interior-only material

            // header
            m_radSpaces[space_name] += "# reflectance: " + formatString(interiorVisibleReflectance, 3) + "\n";

            // material definition
            m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                  + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                  + formatString(interiorVisibleReflectance, 3) + " 0 0\n");

            // polygon reference
            m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon " + surface_name + "\n0\n0\n"
                                       + formatString(polygon.size() * 3) + "\n";
          };

          // add polygon vertices
          for (const auto& vertex : polygon) {
            m_radSpaces[space_name] += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n";
          }
          m_radSpaces[space_name] += "\n";
        }
        // end(surface)

        // get sub surfaces
        std::vector<openstudio::model::SubSurface> subSurfaces = surface.subSurfaces();

        for (const auto& subSurface : subSurfaces) {

          std::string rMaterial = "glass";
          std::string matString = "";
          std::string matStringTinted = "";
          std::string shadeBSDF = "";

          boost::optional<model::ConstructionBase> construction = subSurface.construction();
          if (!construction) {
            LOG(Error, "SubSurface " << subSurface.name().get()
                                     << " is not associated with a Construction, it cannot not be translated. Radiance model translation failed.");
            continue;
          }

          boost::optional<model::WindowPropertyFrameAndDivider> frameAndDivider = subSurface.windowPropertyFrameAndDivider();

          // get the polygon
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(subSurface);

          std::string subSurface_name = cleanName(subSurface.name().get());

          std::string subSurfaceUpCase = boost::algorithm::to_upper_copy(subSurface.subSurfaceType());

          if (subSurfaceUpCase == "FIXEDWINDOW" || subSurfaceUpCase == "OPERABLEWINDOW" || subSurfaceUpCase == "GLASSDOOR"
              || subSurfaceUpCase == "SKYLIGHT") {
            if (!subSurface.visibleTransmittance()) {
              LOG(Error, "Cannot determine visible transmittance for SubSurface "
                           << subSurface.name().get() << ", it cannot not be translated. Radiance model translation failed.");
              continue;
            }

            // TODO: This is going to need update!
            boost::optional<model::ShadingControl> shadingControl;
            auto scs = subSurface.shadingControls();
            if (!scs.empty()) {
              shadingControl = scs[0];
            }

            // future support for mullion factor
            double visibleTransmittanceMultiplier = 1.0;
            if (frameAndDivider) {
              // DLM: Rob what should we do here?
              // <punt>
              visibleTransmittanceMultiplier = 1.0;
              // </punt>
            }

            // find window group
            openstudio::Vector3d outwardNormal = surface.outwardNormal();

            WindowGroup windowGroup = getWindowGroup(outwardNormal, space, *construction, shadingControl, polygon);
            std::string windowGroup_name = windowGroup.name();

            // get the control
            WindowGroupControl control = windowGroup.windowGroupControl();

            if (control.outwardNormal) {

              LOG(Info, windowGroup.name() + " outward normal:" + formatString(control.outwardNormal->x()) + " "
                          + formatString(control.outwardNormal->y()) + " " + formatString(control.outwardNormal->z()) + "\n");
            }

            std::string winUpVector = "Z";
            if (subSurfaceUpCase == "SKYLIGHT") {
              winUpVector = "Y";
            }

            if (m_radWindowGroups.find(windowGroup_name) == m_radWindowGroups.end()) {
              m_radWindowGroups[windowGroup_name] = "# OpenStudio Window Group: " + windowGroup_name + "\n";
              if (windowGroup_name == "WG0") {
                m_radWindowGroups[windowGroup_name] +=
                  "# All uncontrolled windows, multiple orientations possible, no hemispherical sampling info.\n\n";
              } else {
                // 3-phase/rfluxmtx support
                // moved to "shade" polygon now 2015.07.23 RPG
              }
            }

            LOG(Info, "found a " + subSurface.subSurfaceType() + " named '" + subSurface_name + "', windowGroup_name = '" + windowGroup_name + "'");

            // set transmittance(s)
            double visibleTransmittance = subSurface.visibleTransmittance().get() * visibleTransmittanceMultiplier;

            // convert transmittance(Tn) to transmissivity(tn) for Radiance material
            // tn = (sqrt(.8402528435+.0072522239*Tn*Tn)-.9166530661)/.0036261119/Tn
            // or: tn = 1.0895 * Tn (Thanks, Axel Jacobs! (http://www.jaloxa.eu/resources/radiance/documentation/docs/radiance_cookbook.pdf, p. 21))
            double tVis = visibleTransmittance;
            double tn = 0;
            if (tVis == 0.0) {
              LOG(Warn, subSurface_name << " has transmittance of zero.");
              tn = 0.0;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
            } else {
              tn = tVis * 1.0895;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
              if (tVis >= 0.92) {
                LOG(Warn, "glazing material definition in " + space_name + "; Tvis =" + formatString(tVis, 3) + " is very high. Suspect.");
              }
            }

            // get optional switchable glazing material
            boost::optional<double> visibleTransmittanceTinted = 0.0;
            double tnTinted = 0.0;
            if (shadingControl) {
              if (shadingControl->construction()) {
                visibleTransmittanceTinted = shadingControl->construction()->visibleTransmittance();
                LOG(Info, "this is switchable glazing");
                double tintFac = visibleTransmittanceTinted.get() / tVis;
                tnTinted = tn * tintFac;
              } else {
                // something else
              }
            }

            // make materials for single phase (AKA two-phase, depends on whom you talk to)
            if (construction->isSolarDiffusing()) {
              // create Radiance trans material based on transmittance, 100% diffuse (to match E+ performance)
              // trans formulae (from "Rendering with Radiance", sec. 5.2.6):
              // A7=Ts / ( Td+Ts )
              // A6=( Td+Ts ) / ( Rd+Td+Ts )
              // A5=Sr
              // A4=Rs
              // A3=Cb / ( (1-Rs)*(1-A6) )
              // A2=Cg / ( (1-Rs)*(1-A6) )
              // A1=Cr / ( (1-Rs)*(1-A6) )

              // set some constants, let's not get crazy
              double tS = 0.0;  // transmitted specularity
              // rD = 0.95 // diffuse reflectance (not used for monochromatic trans mats)
              double sR = 0.0;   // surface roughness
              double rS = 0.05;  // specular reflectance
              double cRGB = (0.95 - tVis);

              // trans parameters
              double transA7 = tS / (tVis + tS);
              // transA6 = (tn + tS) / (rD + tVis + tS);
              double transA6 = (tn + tS) / (cRGB + tn + tS);
              double transA5 = sR;
              double transA4 = rS;
              // transA3 = cRGB / (1-rS)*(1-transA6);
              // transA2 = cRGB / (1-rS)*(1-transA6);
              // transA1 = cRGB / (1-rS)*(1-transA6);
              double transA3 = cRGB;  // monochromatic
              double transA2 = cRGB;  // monochromatic
              double transA1 = cRGB;  // monochromatic

              rMaterial = "trans";
              matString = "0\n0\n7\n" + formatString(transA1, 3) + " " + formatString(transA2, 3) + " " + formatString(transA3, 3) + " "
                          + formatString(transA4, 3) + " " + formatString(transA5, 3) + " " + formatString(transA6, 3) + " "
                          + formatString(transA7, 2) + "\n";
              //double nTs = 0.0; // transmitted specularity

              if (tVis >= 0.6) {
                LOG(Warn, "dubious glazing material definition in " + space_name + "; Tvis =" + formatString(tVis, 2) + ", yet diffuse? Suspect.");
              }

            } else {

              matString = "0\n0\n3\n" + formatString(tn, 3) + " " + formatString(tn, 3) + " " + formatString(tn, 3) + "\n";

              if (shadingControl) {
                if (shadingControl->construction()) {
                  matStringTinted =
                    "0\n0\n3\n" + formatString(tnTinted, 3) + " " + formatString(tnTinted, 3) + " " + formatString(tnTinted, 3) + "\n";
                }
              }
            }

            // write the window

            // write reveal surfaces from window frame and divider, add small (3") inside reveals if no framediv object

            boost::optional<double> outsideRevealDepth;
            boost::optional<double> insideRevealDepth;
            boost::optional<double> insideSillDepth;

            if (frameAndDivider) {

              if (frameAndDivider->isOutsideRevealDepthDefaulted()) {
                outsideRevealDepth = 0.0;
              } else {
                outsideRevealDepth = frameAndDivider->outsideRevealDepth();
              }

              if (frameAndDivider->isInsideRevealDepthDefaulted()) {
                insideRevealDepth = 0.05;
              } else {
                // ensure there's a reasonable minimum to contain the shade/blind
                if (insideRevealDepth < 0.05) {
                  insideRevealDepth = 0.05;
                  LOG(Warn, "inside reveal depth increased to 0.05 m");
                } else {
                  insideRevealDepth = frameAndDivider->insideRevealDepth();
                }
              }

              if (frameAndDivider->isInsideSillDepthDefaulted()) {
                insideSillDepth = 0.05;
              } else {
                if (insideSillDepth < 0.05) {
                  insideSillDepth = 0.05;
                  LOG(Warn, "inside sill depth increased to 0.05 m");
                } else {
                  insideSillDepth = frameAndDivider->insideSillDepth();
                }
              }

            } else {

              outsideRevealDepth = 0.0;
              insideRevealDepth = 0.05;
              insideSillDepth = 0.05;
            }

            openstudio::Vector3d offset;

            // subSurface.outwardNormal not in global coordinate system
            outwardNormal = subSurface.outwardNormal();
            boost::optional<Vector3d> optionalOutwardNormal = openstudio::getOutwardNormal(polygon);
            if (optionalOutwardNormal) {
              Vector3d outwardNormal = *optionalOutwardNormal;

              size_t N = polygon.size();
              for (size_t i = 0; i < N; ++i) {
                size_t index1 = i;
                size_t index2 = (i + 1) % N;

                if (outsideRevealDepth && (*outsideRevealDepth > 0.0)) {
                  // window polygon is already offset from the wall
                  offset = outsideRevealDepth.get() * outwardNormal;
                  Point3d vertex1 = polygon[index1];
                  Point3d vertex2 = polygon[index1] + offset;
                  Point3d vertex3 = polygon[index2] + offset;
                  Point3d vertex4 = polygon[index2];

                  // TODO: get exterior reflectance of surface
                  double interiorVisibleReflectance = 0.5;
                  double exteriorVisibleReflectance = 0.2;
                  //polygon header
                  m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance, 3) + "\n";
                  m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance, 3) + "\n";
                  // write material
                  m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                        + formatString(exteriorVisibleReflectance, 3) + " " + formatString(exteriorVisibleReflectance, 3) + " "
                                        + formatString(exteriorVisibleReflectance, 3) + " 0 0\n\n");
                  // write polygon
                  m_radSpaces[space_name] +=
                    "refl_" + formatString(exteriorVisibleReflectance, 3) + " polygon outside_reveal_" + subSurface_name + std::to_string(i) + "\n";
                  m_radSpaces[space_name] += "0\n0\n" + formatString(4 * 3) + "\n";
                  m_radSpaces[space_name] += formatString(vertex1.x()) + " " + formatString(vertex1.y()) + " " + formatString(vertex1.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex2.x()) + " " + formatString(vertex2.y()) + " " + formatString(vertex2.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex3.x()) + " " + formatString(vertex3.y()) + " " + formatString(vertex3.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex4.x()) + " " + formatString(vertex4.y()) + " " + formatString(vertex4.z()) + "\n\n";
                }

                // make interior sill/reveal surfaces
                if (insideRevealDepth && (*insideRevealDepth > 0.0)) {

                  // window polygon is already offset from the wall
                  offset = -insideRevealDepth.get() * outwardNormal;
                  Point3d vertex1 = polygon[index1];
                  Point3d vertex2 = polygon[index1] + offset;
                  Point3d vertex3 = polygon[index2] + offset;
                  Point3d vertex4 = polygon[index2];

                  // TODO: get exterior reflectance of surface
                  double interiorVisibleReflectance = 0.5;
                  double exteriorVisibleReflectance = 0.2;
                  //polygon header
                  m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance, 3) + "\n";
                  m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance, 3) + "\n";
                  // write material
                  m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                        + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                        + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
                  // write polygon
                  m_radSpaces[space_name] +=
                    "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon inside_reveal_" + subSurface_name + std::to_string(i) + "\n";
                  m_radSpaces[space_name] += "0\n0\n" + formatString(4 * 3) + "\n";
                  m_radSpaces[space_name] += formatString(vertex1.x()) + " " + formatString(vertex1.y()) + " " + formatString(vertex1.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex2.x()) + " " + formatString(vertex2.y()) + " " + formatString(vertex2.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex3.x()) + " " + formatString(vertex3.y()) + " " + formatString(vertex3.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex4.x()) + " " + formatString(vertex4.y()) + " " + formatString(vertex4.z()) + "\n\n";
                }

                if (insideSillDepth && (*insideSillDepth > 0.0)) {

                  // window polygon is already offset from the wall
                  offset = -insideSillDepth.get() * outwardNormal;
                  Point3d vertex1 = polygon[index1];
                  Point3d vertex2 = polygon[index1] + offset;
                  Point3d vertex3 = polygon[index2] + offset;
                  Point3d vertex4 = polygon[index2];

                  // TODO: get exterior reflectance of surface
                  double interiorVisibleReflectance = 0.5;
                  double exteriorVisibleReflectance = 0.2;
                  //polygon header
                  m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance, 3) + "\n";
                  m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance, 3) + "\n";
                  // write material
                  m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                        + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                        + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
                  // write polygon
                  m_radSpaces[space_name] +=
                    "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon inside_sill_" + subSurface_name + std::to_string(i) + "\n";
                  m_radSpaces[space_name] += "0\n0\n" + formatString(4 * 3) + "\n";
                  m_radSpaces[space_name] += formatString(vertex1.x()) + " " + formatString(vertex1.y()) + " " + formatString(vertex1.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex2.x()) + " " + formatString(vertex2.y()) + " " + formatString(vertex2.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex3.x()) + " " + formatString(vertex3.y()) + " " + formatString(vertex3.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex4.x()) + " " + formatString(vertex4.y()) + " " + formatString(vertex4.z()) + "\n\n";
                }
              }
            }

            // finally, write the actual window
            // add polygon header (same for all)
            m_radWindowGroups[windowGroup_name] += "\n# SubSurface = " + subSurface_name + "\n";
            m_radWindowGroups[windowGroup_name] += "# Tvis = " + formatString(tVis, 3) + " (tn = " + formatString(tn, 3) + ")\n";

            if (windowGroup_name == "WG0") {

              //no shades

              // add materials
              m_radMaterials.insert("void " + rMaterial + " glaz_" + rMaterial + "_tn-" + formatString(tn, 3) + "\n" + matString + "\n");
              m_radMaterialsDC.insert("void alias glaz_" + rMaterial + "_tn-" + formatString(tn, 3) + " WG0\n\n");
              m_radMaterialsSwitchableBase.insert("void alias glaz_" + rMaterial + "_tn-" + formatString(tn, 3) + " WG0\n\n");

              // write the window polygon
              m_radWindowGroups[windowGroup_name] += "glaz_" + rMaterial + "_tn-" + formatString(tn, 3) + " polygon " + subSurface_name + "\n";
              m_radWindowGroups[windowGroup_name] += "0\n0\n" + formatString(polygon.size() * 3) + "\n\n";
              for (auto vertex = polygon.rbegin(); vertex != polygon.rend(); ++vertex) {
                m_radWindowGroups[windowGroup_name] +=
                  "" + formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n";
              }

            } else {

              //has shading control

              //add materials
              m_radMaterials.insert("void " + rMaterial + " " + windowGroup_name + "\n" + matString + "\n");
              if (shadingControl) {
                if (shadingControl->construction()) {
                  m_radMaterials.insert("void " + rMaterial + " " + windowGroup_name + "_TINTED\n" + matStringTinted + "\n");

                  // make special mats files for window group
                  switchableGroup_wgMats = "void " + rMaterial + " " + windowGroup_name + "\n" + matString + "\n";

                  openstudio::path filename = t_radDir / openstudio::toPath("materials") / openstudio::toPath(windowGroup_name + "_clear.mat");
                  OFSTREAM file(filename);
                  if (file.is_open()) {
                    t_outfiles.push_back(filename);
                    file << switchableGroup_wgMats;
                  } else {
                    LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
                  }

                  switchableGroup_wgMats = "void " + rMaterial + " " + windowGroup_name + "_TINTED\n" + matStringTinted + "\n\nvoid alias "
                                           + windowGroup_name + " " + windowGroup_name + "_TINTED " + "\n\n";
                  openstudio::path filename2 = t_radDir / openstudio::toPath("materials") / openstudio::toPath(windowGroup_name + "_tinted.mat");
                  OFSTREAM file2(filename2);
                  if (file2.is_open()) {
                    t_outfiles.push_back(filename2);
                    file2 << switchableGroup_wgMats;
                  } else {
                    LOG(Error, "Cannot open file '" << toString(filename2) << "' for writing");
                  }

                } else {

                  // make mat for single window group
                  std::string wgMat = "";
                  wgMat = "void " + rMaterial + " " + windowGroup_name + "\n" + matString + "\n\n";
                  openstudio::path wgSingleFilename = t_radDir / openstudio::toPath("materials") / openstudio::toPath(windowGroup_name + ".mat");
                  OFSTREAM wgSingleFile(wgSingleFilename);
                  if (wgSingleFile.is_open()) {
                    t_outfiles.push_back(wgSingleFilename);
                    wgSingleFile << wgMat;
                  } else {
                    LOG(Error, "Cannot open file '" << toString(wgSingleFilename) << "' for writing");
                  }
                }
              }
              // write the polygon
              m_radWindowGroups[windowGroup_name] += windowGroup_name + " polygon " + subSurface_name + "\n";
              m_radWindowGroups[windowGroup_name] += "0\n0\n" + formatString(polygon.size() * 3) + "\n";
              for (auto vertex = polygon.rbegin(); vertex != polygon.rend(); ++vertex)

              {
                m_radWindowGroups[windowGroup_name] +=
                  "" + formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n";
              }

              if (shadingControl) {
                if (shadingControl->construction()) {

                  // a construction means it's switchable glazing, so no shade needed,
                  // but make special materials files for window group calcs.
                  m_radMaterialsSwitchableBase.insert("void plastic " + windowGroup_name + "\n0\n0\n5\n0 0 0 0 0\n");
                  m_radMaterialsWG0.insert("void plastic " + windowGroup_name + "\n0\n0\n5\n0 0 0 0 0\n");

                } else {

                  // add the shade polygon

                  shadeBSDF = windowGroup.interiorShadeBSDF();

                  rMaterial = "BSDF";
                  matString = "6\n0 bsdf/" + shadeBSDF + " 0 0 1 .\n0\n0\n";

                  m_radMaterials.insert("void " + rMaterial + " " + windowGroup_name + "_SHADE\n" + matString + "\n\n");

                  m_radMaterialsDC.insert("void light " + windowGroup_name + "_SHADE\n0\n0\n3\n1 1 1\n");
                  m_radMaterialsWG0.insert("void plastic " + windowGroup_name + "_SHADE\n0\n0\n5\n0 0 0 0 0\n");
                  m_radMaterialsSwitchableBase.insert("void plastic " + windowGroup_name + "_SHADE\n0\n0\n5\n0 0 0 0 0\n");
                  m_radMaterialsSwitchableBase.insert("void plastic " + windowGroup_name + "\n0\n0\n5\n0 0 0 0 0\n");

                  // polygon header
                  // Forcing Klems basis... RPG 2015.09.13 =(
                  // user is warned as files are written RPG 2015.12.03
                  std::string tempSkyDivs = "kf";

                  m_radWindowGroupShades[windowGroup_name] +=
                    "#@rfluxmtx h=" + tempSkyDivs + " u=" + winUpVector + " o=output/dc/" + windowGroup_name + ".vmx\n";
                  m_radWindowGroupShades[windowGroup_name] += "\n# shade for SubSurface: " + subSurface_name + "\n";

                  // write the polygon
                  m_radWindowGroupShades[windowGroup_name] +=
                    windowGroup_name + "_SHADE" + " polygon " + windowGroup_name + "_SHADE_" + subSurface_name + "\n";
                  m_radWindowGroupShades[windowGroup_name] += "0\n0\n" + formatString(polygon.size() * 3) + "\n";
                  for (auto vertex = polygon.rbegin(); vertex != polygon.rend(); ++vertex)

                  {

                    // offset the shade to the interior side of the window
                    Point3d offsetVertex = *vertex + (-0.01 * outwardNormal);

                    m_radWindowGroupShades[windowGroup_name] +=
                      "" + formatString(offsetVertex.x()) + " " + formatString(offsetVertex.y()) + " " + formatString(offsetVertex.z()) + "\n";
                  }

                  // make mat for single window group shade
                  std::string wgShadeMat = "";
                  wgShadeMat = "void " + rMaterial + " " + windowGroup_name + "_SHADE\n" + matString + "\n\n";
                  openstudio::path wgSingleFilename =
                    t_radDir / openstudio::toPath("materials") / openstudio::toPath(windowGroup_name + "_SHADE.mat");
                  OFSTREAM wgSingleFile(wgSingleFilename);
                  if (wgSingleFile.is_open()) {
                    t_outfiles.push_back(wgSingleFilename);
                    wgSingleFile << wgShadeMat;
                  } else {
                    LOG(Error, "Cannot open file '" << toString(wgSingleFilename) << "' for writing");
                  }

                  // shade BSDF stuff

                  // make dir for BSDF files

                  openstudio::path bsdfoutpath = t_radDir / openstudio::toPath("bsdf");

                  // path to write bsdf

                  openstudio::path shadeBSDFPath = t_radDir / openstudio::toPath("bsdf") / shadeBSDF;

                  if (!exists(shadeBSDFPath)) {

                    // add BSDF file to the collection of crap to copy up
                    t_outfiles.push_back(shadeBSDFPath);

                    // read BSDF from resource dll
                    // must be referenced in openstudiocore/src/radiance/radiance.qrc
                    const auto defaultFile = ::openstudioradiance::embedded_files::getFileAsString(":/resources/" + shadeBSDF);

                    // write shade BSDF
                    openstudio::filesystem::ofstream outfile(shadeBSDFPath);
                    if (!outfile.is_open()) {
                      LOG_AND_THROW("Cannot write file to '" << toString(shadeBSDFPath) << "'");
                    }
                    outfile << defaultFile;
                    outfile.close();
                  }
                }

                // always add an airBSDF

                openstudio::path airBSDFPath = t_radDir / openstudio::toPath("bsdf") / openstudio::toPath("air.xml");

                if (!exists(airBSDFPath)) {

                  // add BSDF file to the collection of crap to copy up
                  t_outfiles.push_back(airBSDFPath);

                  // read BSDF from resource dll
                  // must be in openstudiocore/src/radiance/radiance.qrc
                  const auto defaultFile = ::openstudioradiance::embedded_files::getFileAsString(":/resources/air.xml");

                  // write shade BSDF
                  openstudio::filesystem::ofstream outFileAir(airBSDFPath);
                  if (!outFileAir.is_open()) {
                    LOG_AND_THROW("Cannot write file to '" << toString(airBSDFPath) << "'");
                  }
                  outFileAir << defaultFile;
                  outFileAir.close();
                }
              }
            }

            //store window group entry for mapping.rad
            if (windowGroup_name == "WG0") {

              // simple placeholder for WG0
              m_radDCmats.insert(windowGroup_name + ",n/a,n/a,n/a,n/a\n");

            } else {

              std::string shadeType = "air.xml,";
              if (shadingControl) {
                if (shadingControl->construction()) {
                  shadeType = "SWITCHABLE,";
                }
              }

              // window group name, normal, control type, setpoint, unshaded bsdf, and shaded bsdf
              m_radDCmats.insert(windowGroup_name + "," + formatString((control.outwardNormal->x() * -1), 2) + " "
                                 + formatString((control.outwardNormal->y() * -1), 2) + " " + formatString((control.outwardNormal->z() * -1), 2) + ","
                                 + windowGroup.shadingControlType() + "," + windowGroup.shadingControlSetpoint() + "," + shadeType + shadeBSDF
                                 + "\n");
            }

          } else if (subSurfaceUpCase == "DOOR") {

            LOG(Info, "found a door, will set to interior reflectance");

            double interiorVisibleAbsorptance = subSurface.interiorVisibleAbsorptance().get();
            double exteriorVisibleAbsorptance = subSurface.exteriorVisibleAbsorptance().get();
            double interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
            double exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
            //polygon header
            m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance, 3) + "\n";
            m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
            // write material
            m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                  + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                  + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
            // write polygon
            m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon " + subSurface_name + "\n";
            m_radSpaces[space_name] += "0\n0\n" + formatString(polygon.size() * 3) + "\n\n";

            for (const auto& vertex : polygon) {
              m_radSpaces[space_name] += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n\n";
            }

          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDOME") {

            LOG(Error, "subsurface is a tdd dome (not yet implemented in Radiance), model translation halted.");

          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDIFFUSER") {

            LOG(Error, "subsurface is a tdd diffuser (not yet implemented in Radiance), model translation halted.");
          }

        }  //end reveals

      }  // end surfaces

      // get shading surfaces

      std::vector<openstudio::model::ShadingSurfaceGroup> shadingSurfaceGroups = space.shadingSurfaceGroups();
      for (const auto& shadingSurfaceGroup : shadingSurfaceGroups) {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup.shadingSurfaces();
        for (const auto& shadingSurface : shadingSurfaces) {
          std::string shadingSurface_name = cleanName(shadingSurface.name().get());

          // add surface to zone geometry
          m_radSpaces[space_name] += "# surface: " + shadingSurface_name + "\n";

          // set construction of space shadingSurface
          std::string constructionName = shadingSurface.getString(2).get();
          m_radSpaces[space_name] += "# construction: " + constructionName + "\n";

          // get reflectance
          double interiorVisibleReflectance = 0.25;  // default for space shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()) {
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }
          double exteriorVisibleReflectance = 0.25;  // default for space shading surfaces
          if (shadingSurface.exteriorVisibleAbsorptance()) {
            double exteriorVisibleAbsorptance = shadingSurface.exteriorVisibleAbsorptance().get();
            exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
          }

          // write (two-sided) material
          // exterior reflectance for front side
          m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                + formatString(exteriorVisibleReflectance, 3) + " " + formatString(exteriorVisibleReflectance, 3) + " "
                                + formatString(exteriorVisibleReflectance, 3) + " 0 0\n\n");

          // interior reflectance for back side
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");

          // mixfunc
          m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                   + formatString(exteriorVisibleReflectance, 3) + "\n4 " + "refl_" + formatString(exteriorVisibleReflectance, 3)
                                   + " " + "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

          // polygon header
          m_radSpaces[space_name] += "# exterior visible reflectance: " + formatString(exteriorVisibleReflectance, 3) + "\n";
          m_radSpaces[space_name] += "# interior visible reflectance: " + formatString(interiorVisibleReflectance, 3) + "\n";

          // get / write surface polygon

          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);
          m_radSpaces[space_name] += "reflBACK_" + formatString(interiorVisibleReflectance, 3) + "_reflFRONT_"
                                     + formatString(exteriorVisibleReflectance, 3) + " polygon " + shadingSurface_name + "\n0\n0\n"
                                     + formatString(polygon.size() * 3) + "\n";

          for (const auto& vertex : polygon) {
            m_radSpaces[space_name] += "" + formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n";
          }
          m_radSpaces[space_name] += "\n";
        }
      }  // end shading surfaces

      //get the interior partition surfaces

      std::vector<openstudio::model::InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroups = space.interiorPartitionSurfaceGroups();
      for (const auto& interiorPartitionSurfaceGroup : interiorPartitionSurfaceGroups) {
        std::vector<openstudio::model::InteriorPartitionSurface> interiorPartitionSurfaces =
          interiorPartitionSurfaceGroup.interiorPartitionSurfaces();
        for (const auto& interiorPartitionSurface : interiorPartitionSurfaces) {

          // get nice name

          std::string interiorPartitionSurface_name = cleanName(interiorPartitionSurface.name().get());

          // check for construction

          boost::optional<model::ConstructionBase> construction = interiorPartitionSurface.construction();
          if (!construction) {
            LOG(Warn, "InteriorPartitionSurface " << interiorPartitionSurface.name().get()
                                                  << " is not associated with a Construction, it will not be translated.");
            continue;
          }

          // add surface to zone geometry

          m_radSpaces[space_name] += "# surface: " + interiorPartitionSurface_name + "\n";

          // set construction of interiorPartitionSurface
          std::string constructionName = interiorPartitionSurface.getString(1).get();
          m_radSpaces[space_name] += "# construction: " + constructionName + "\n";

          // get reflectance
          double interiorVisibleReflectance = 0.5;  // set some default
          if (interiorPartitionSurface.interiorVisibleAbsorptance()) {
            double interiorVisibleAbsorptance = interiorPartitionSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }

          double exteriorVisibleReflectance = 0.5;  // set some default
          if (interiorPartitionSurface.exteriorVisibleAbsorptance()) {
            double exteriorVisibleAbsorptance = interiorPartitionSurface.exteriorVisibleAbsorptance().get();
            exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
          }

          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n"
                                + formatString(interiorVisibleReflectance, 3) + " " + formatString(interiorVisibleReflectance, 3) + " "
                                + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
          // polygon header
          m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance, 3) + "\n";
          m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
          // get / write surface polygon

          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(interiorPartitionSurface);
          m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon " + interiorPartitionSurface_name + "\n0\n0\n"
                                     + formatString(polygon.size() * 3) + "\n";
          for (const auto& vertex : polygon) {
            m_radSpaces[space_name] += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n\n";
          }
        }
      }  // end interior partitions

      // get luminaires
      ///  \todo fully implement once luminaires are fully supported in model
      //std::vector<openstudio::model::Luminaire> luminaires = space.luminaires();
      //for (const auto & luminaire : luminaires)
      //{
      //  luminaire_name = luminaire.name.get.gsub(' ', '_').gsub(':', '_')
      //  polygon = OpenStudio::Radiance::ForwardTranslator::getPolygon(luminaire)
      //}

      // get daylighting controls
      std::vector<openstudio::model::DaylightingControl> daylightingControls = space.daylightingControls();
      for (const auto& control : daylightingControls) {

        m_radSensors[space_name] = "";

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(control);
        openstudio::Vector3d sensor_aimVector = openstudio::radiance::ForwardTranslator::getSensorVector(control);
        m_radSensors[space_name] += formatString(sensor_point.x(), 3) + " " + formatString(sensor_point.y(), 3) + " "
                                    + formatString(sensor_point.z(), 3) + " " + formatString(sensor_aimVector.x(), 3) + " "
                                    + formatString(sensor_aimVector.y(), 3) + " " + formatString(sensor_aimVector.z(), 3) + "\n";

        // write daylighting controls
        openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(space_name + ".sns");
        OFSTREAM file(filename);
        if (file.is_open()) {
          t_outfiles.push_back(filename);
          file << m_radSensors[space_name];
        } else {
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        // write daylighting control view file
        m_radSensorViews[space_name] = "";
        m_radSensorViews[space_name] += "rvu -vth -vp " + formatString(sensor_point.x(), 3) + " " + formatString(sensor_point.y(), 3) + " "
                                        + formatString(sensor_point.z(), 3) + " -vd " + formatString(sensor_aimVector.x(), 3) + " "
                                        + formatString(sensor_aimVector.y(), 3) + " " + formatString(sensor_aimVector.z(), 3)
                                        + " -vu 0 1 0 -vh 180 -vv 180 -vo 0 -vs 0 -vl 0\n";

        filename = t_radDir / openstudio::toPath("views") / openstudio::toPath(space_name + "_dc.vfh");
        OFSTREAM file2(filename);
        if (file2.is_open()) {
          t_outfiles.push_back(filename);
          file2 << m_radSensorViews[space_name];
        } else {
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << "_dc.vfh");

      }  // end daylighting controls

      // get glare sensors
      std::vector<openstudio::model::GlareSensor> glareSensors = space.glareSensors();
      for (const auto& sensor : glareSensors) {
        m_radGlareSensors[space_name] = "";
        m_radGlareSensorViewsVTV[space_name] = "";
        m_radGlareSensorViewsVTA[space_name] = "";

        std::string sensor_name;
        if (sensor.name()) {
          sensor_name = cleanName(sensor.name().get());
        }

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(sensor);
        // openstudio::Vector3dVector sensor_viewVector = openstudio::radiance::ForwardTranslator::getViewVectors(*sensor);
        //std::string sensor_name = sensor.name();
        openstudio::Vector3dVector viewVectors = openstudio::radiance::ForwardTranslator::getViewVectors(sensor);
        // reverse the order so primary view is listed last in the file
        std::reverse(std::begin(viewVectors), std::end(viewVectors));
        for (const Vector3d& viewVector : viewVectors) {

          // glare sensor points
          m_radGlareSensors[space_name] += formatString(sensor_point.x(), 3) + " " + formatString(sensor_point.y(), 3) + " "
                                           + formatString(sensor_point.z(), 3) + " " + formatString(viewVector.x(), 3) + " "
                                           + formatString(viewVector.y(), 3) + " " + formatString(viewVector.z(), 3) + "\n";

          // glare sensor views (perspective)
          m_radGlareSensorViewsVTV[space_name] += "rvu -vtv -vp " + formatString(sensor_point.x(), 3) + " " + formatString(sensor_point.y(), 3) + " "
                                                  + formatString(sensor_point.z(), 3) + " -vd " + formatString(viewVector.x(), 3) + " "
                                                  + formatString(viewVector.y(), 3) + " " + formatString(viewVector.z(), 3)
                                                  + " -vu 0 0 1 -vh 90 -vv 60 -vo 0 -vs 0 -vl 0\n";

          // glare sensor views (fisheye)
          m_radGlareSensorViewsVTA[space_name] += "rvu -vth -vp " + formatString(sensor_point.x(), 3) + " " + formatString(sensor_point.y(), 3) + " "
                                                  + formatString(sensor_point.z(), 3) + " -vd " + formatString(viewVector.x(), 3) + " "
                                                  + formatString(viewVector.y(), 3) + " " + formatString(viewVector.z(), 3)
                                                  + " -vu 0 0 1 -vh 180 -vv 180 -vo 0 -vs 0 -vl 0\n";
        }

        // write glare sensors
        openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(space_name + "_" + sensor_name + ".glr");
        OFSTREAM file(filename);
        if (file.is_open()) {
          t_outfiles.push_back(filename);
          file << m_radGlareSensors[space_name];
        } else {
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << ".glr");

        // write glare sensor views (perspective)
        filename = t_radDir / openstudio::toPath("views") / openstudio::toPath(space_name + "_" + sensor_name + "_gs.vfv");
        OFSTREAM file2(filename);
        if (file2.is_open()) {
          t_outfiles.push_back(filename);
          file2 << m_radGlareSensorViewsVTV[space_name];
        } else {
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << "_" << sensor_name << "_gs.vfv");

        // write glare sensor views (fisheye)
        filename = t_radDir / openstudio::toPath("views") / openstudio::toPath(space_name + "_" + sensor_name + "_gs.vfh");
        OFSTREAM file3(filename);
        if (file3.is_open()) {
          t_outfiles.push_back(filename);
          file3 << m_radGlareSensorViewsVTA[space_name];
        } else {
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << "_" << sensor_name << "_gs.vfh");

      }  // end glare sensor

      // get illuminance map points, write to file
      std::vector<openstudio::model::IlluminanceMap> illuminanceMaps = space.illuminanceMaps();
      for (const auto& map : illuminanceMaps) {
        m_radMaps[space_name] = "";
        m_radMapHandles[space_name] = map.handle();

        // write map file
        openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(space_name + ".map");
        OFSTREAM file(filename);
        if (file.is_open()) {
          t_outfiles.push_back(filename);

          std::vector<Point3d> referencePoints = openstudio::radiance::ForwardTranslator::getReferencePoints(map);
          for (const auto& point : referencePoints) {
            m_radMaps[space_name] +=
              "" + formatString(point.x(), 3) + " " + formatString(point.y(), 3) + " " + formatString(point.z(), 3) + " 0.000 0.000 1.000\n";
          }
          file << m_radMaps[space_name];
        } else {
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "wrote " << space_name << ".map");
      }  //end illuminance map

      // write geometry
      openstudio::path filename = t_radDir / openstudio::toPath("scene") / openstudio::toPath(space_name + ".rad");
      OFSTREAM file(filename);
      if (file.is_open()) {
        t_outfiles.push_back(filename);
        m_radSceneFiles.push_back(filename);
        file << m_radSpaces[space_name];
      } else {
        LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
      }

      for (const auto& windowGroup : m_windowGroups) {
        std::string windowGroup_name = windowGroup.name();

        //write windows (and glazed doors)
        if (m_radWindowGroups.find(windowGroup_name) != m_radWindowGroups.end()) {

          // get the Radiance parameters... so we have them.
          auto radianceParameters = m_model.getUniqueModelObject<openstudio::model::RadianceParameters>();
          if (windowGroup_name != "WG0") {
            if (radianceParameters.skyDiscretizationResolution() == "146") {
              LOG(Info, "writing out window group '" + windowGroup_name + "', using Klems sampling basis.");
            } else if (radianceParameters.skyDiscretizationResolution() == "578") {
              LOG(Warn, "writing out window group '" + windowGroup_name + "', but sampling basis was reset to Klems (145).");
            } else if (radianceParameters.skyDiscretizationResolution() == "2306") {
              LOG(Warn, "writing out window group '" + windowGroup_name + "', but sampling basis was reset to Klems (145).");
            }
          }

          openstudio::path glazefilename = t_radDir / openstudio::toPath("scene/glazing") / openstudio::toPath(windowGroup_name + ".rad");
          OFSTREAM glazefile(glazefilename);
          if (glazefile.is_open()) {
            t_outfiles.push_back(glazefilename);
            m_radSceneFiles.push_back(glazefilename);
            glazefile << m_radWindowGroups[windowGroup_name];
          } else {
            LOG(Error, "Cannot open file '" << toString(glazefilename) << "' for writing");
          }

          if (windowGroup_name != "WG0" && !m_radWindowGroupShades[windowGroup_name].empty()) {
            openstudio::path shadefilename = t_radDir / openstudio::toPath("scene/shades") / openstudio::toPath(windowGroup_name + "_SHADE.rad");
            OFSTREAM shadefile(shadefilename);
            if (shadefile.is_open()) {
              t_outfiles.push_back(shadefilename);
              m_radSceneFiles.push_back(shadefilename);
              shadefile << m_radWindowGroupShades[windowGroup_name];
            } else {
              LOG(Error, "Cannot open file '" << toString(shadefilename) << "' for writing");
            }
          }

          // write window group control points
          // only write for controlled window groups
          if (windowGroup_name != "WG0") {
            openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(windowGroup_name + ".pts");
            OFSTREAM file(filename);
            if (file.is_open()) {
              t_outfiles.push_back(filename);
              file << windowGroup.windowGroupPoints();
            } else {
              LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
            }
          }
        }
      }

      // write radiance materials file
      m_radMaterials.insert("# OpenStudio Materials File\n\n");
      openstudio::path materialsfilename = t_radDir / openstudio::toPath("materials/materials.rad");
      OFSTREAM materialsfile(materialsfilename);
      if (materialsfile.is_open()) {
        t_outfiles.push_back(materialsfilename);
        for (const auto& line : m_radMaterials) {
          materialsfile << line;
        };
        for (const auto& line : m_radMixMaterials) {
          materialsfile << line;
        };
      } else {
        LOG(Error, "Cannot open file '" << toString(materialsfilename) << "' for writing");
      }

      // write radiance DC vmx materials (lights) file
      m_radMaterialsDC.insert("# OpenStudio \"vmx\" Materials File\n# controlled windows: material=\"light\", black out all others.\n\nvoid plastic "
                              "WG0\n0\n0\n5\n0 0 0 0 0\n\n");
      openstudio::path materials_vmxfilename = t_radDir / openstudio::toPath("materials/materials_vmx.rad");
      OFSTREAM materials_vmxfile(materials_vmxfilename);
      if (materials_vmxfile.is_open()) {
        t_outfiles.push_back(materials_vmxfilename);
        for (const auto& line : m_radMaterialsDC) {
          materials_vmxfile << line;
        };
      } else {
        LOG(Error, "Cannot open file '" << toString(materials_vmxfilename) << "' for writing");
      }

      // write radiance WG0 vmx materials file (blacks out controlled window groups)
      m_radMaterialsWG0.insert("# OpenStudio \"WG0\" Materials File\n# black out all controlled window groups.\n");
      openstudio::path materials_WG0filename = t_radDir / openstudio::toPath("materials/materials_WG0.rad");
      OFSTREAM materials_WG0file(materials_WG0filename);
      if (materials_WG0file.is_open()) {
        t_outfiles.push_back(materials_WG0filename);
        for (const auto& line : m_radMaterialsWG0) {
          materials_WG0file << line;
        };
      } else {
        LOG(Error, "Cannot open file '" << toString(materials_WG0filename) << "' for writing");
      }

      // write radiance blackout materials file (blacks out everything)
      m_radMaterialsSwitchableBase.insert(
        "# OpenStudio Blackout Materials File\n# black out all window and shade materials.\n\nvoid plastic WG0\n0\n0\n5\n0 0 0 0 0\n\n");
      openstudio::path materials_SwitchableBasefilename = t_radDir / openstudio::toPath("materials/materials_blackout.rad");
      OFSTREAM materials_SwitchableBasefile(materials_SwitchableBasefilename);
      if (materials_SwitchableBasefile.is_open()) {
        t_outfiles.push_back(materials_SwitchableBasefilename);
        for (const auto& line : m_radMaterialsSwitchableBase) {
          materials_SwitchableBasefile << line;
        };
      } else {
        LOG(Error, "Cannot open file '" << toString(materials_SwitchableBasefilename) << "' for writing");
      }

      // write radiance vmx materials list
      // format of this file is: window group, bsdf, bsdf
      m_radDCmats.insert("# OpenStudio windowGroup->BSDF \"Mapping\" File\n# windowGroup,inwardNormal,shade control type,shade control "
                         "setpoint,unshaded bsdf,shaded bsdf\n");
      openstudio::path materials_dcfilename = t_radDir / openstudio::toPath("bsdf/mapping.rad");
      OFSTREAM materials_dcfile(materials_dcfilename);
      if (materials_dcfile.is_open()) {
        t_outfiles.push_back(materials_dcfilename);
        for (const auto& line : m_radDCmats) {
          materials_dcfile << line;
        };
      } else {
        LOG(Error, "Cannot open file '" << toString(materials_dcfilename) << "' for writing");
      }

      // write complete scene
      openstudio::path modelfilename = t_radDir / openstudio::toPath("model.rad");
      OFSTREAM modelfile(modelfilename);

      if (modelfile.is_open()) {
        t_outfiles.push_back(modelfilename);

        std::set<openstudio::path> uniquePaths(m_radSceneFiles.begin(), m_radSceneFiles.end());

        for (const auto& filename : uniquePaths) {
          modelfile << "!xform ./" << openstudio::toString(openstudio::relativePath(filename, t_radDir)) << '\n';
        }
      } else {
        LOG(Error, "Cannot open file '" << toString(modelfilename) << "' for writing");
      }
    }
  }

  boost::optional<openstudio::path> ForwardTranslator::getBSDF(double vlt, double vltSpecular, const std::string& shadeType) {
    std::string searchTerm = "BSDF";
    unsigned tid = 1316;  // "Construction Assembly.Fenestration.Window";

    boost::optional<std::string> result;

    openstudio::LocalBCL& localBCL = openstudio::LocalBCL::instance();
    result = getBSDF(localBCL, vlt, vltSpecular, shadeType, searchTerm, tid);
    if (result) {
      return toPath(*result);
    }

    openstudio::RemoteBCL remoteBCL;
    result = getBSDF(remoteBCL, vlt, vltSpecular, shadeType, searchTerm, tid);
    if (result) {
      return toPath(*result);
    }

    return boost::none;
  }

  boost::optional<std::string> ForwardTranslator::getBSDF(openstudio::LocalBCL& bcl, double vlt, double vltSpecular, const std::string& shadeType,
                                                          const std::string& searchTerm, unsigned tid) {
    std::vector<BCLComponent> results = bcl.searchComponents(searchTerm, tid);
    for (const BCLComponent& result : results) {

      try {

        if (result.files("xml").empty()) {
          continue;
        }

        double vltDiff = std::numeric_limits<double>::max();
        double vltSpecularDiff = std::numeric_limits<double>::max();
        bool shadeTypeMatch = false;
        for (const Attribute& attribute : result.attributes()) {
          std::string attributeName = attribute.name();
          if (istringEqual(attributeName, "Visible Light Transmittance")) {
            vltDiff = std::abs(attribute.valueAsDouble() - vlt);
          } else if (istringEqual(attributeName, "Visible Light Transmittance Specular Percentage")) {
            vltSpecularDiff = std::abs(attribute.valueAsDouble() - vltSpecular);
          } else if (istringEqual(attributeName, "Interior Shade Layer Type")) {
            shadeTypeMatch = istringEqual(attribute.valueAsString(), shadeType);
          }
        }

        // check if meets criteria
        if (vltDiff <= 0.01 && vltSpecularDiff <= 0.01 && shadeTypeMatch) {
          // this one works try to get payload
          std::vector<std::string> files = result.files("xml");
          if (!files.empty()) {
            return files[0];
          }
        }

      } catch (const std::exception&) {
        // not the right one
      }
    }

    return boost::none;
  }

  boost::optional<std::string> ForwardTranslator::getBSDF(openstudio::RemoteBCL& bcl, double vlt, double vltSpecular, const std::string& shadeType,
                                                          const std::string& searchTerm, unsigned tid) {
    boost::optional<BCLMetaSearchResult> metaResult = bcl.metaSearchComponentLibrary(searchTerm, tid);
    if (metaResult) {
      unsigned numResults = metaResult->numResults();
      unsigned resultsPerQuery = bcl.resultsPerQuery();
      unsigned numPages = numResults / resultsPerQuery;
      if ((numResults % resultsPerQuery) > 0) {
        ++numPages;
      }

      for (unsigned page = 0; page < numPages; ++page) {
        std::vector<BCLSearchResult> results = bcl.searchComponentLibrary(searchTerm, tid, page);
        for (const BCLSearchResult& result : results) {

          try {

            bool hasXML = false;
            for (const BCLFile& file : result.files()) {
              if (file.filetype() == "xml") {
                hasXML = true;
              }
            }
            if (!hasXML) {
              continue;
            }

            double vltDiff = std::numeric_limits<double>::max();
            double vltSpecularDiff = std::numeric_limits<double>::max();
            bool shadeTypeMatch = false;
            for (const Attribute& attribute : result.attributes()) {
              std::string attributeName = attribute.name();
              if (istringEqual(attributeName, "Visible Light Transmittance")) {
                vltDiff = std::abs(attribute.valueAsDouble() - vlt);
              } else if (istringEqual(attributeName, "Visible Light Transmittance Specular Percentage")) {
                vltSpecularDiff = std::abs(attribute.valueAsDouble() - vltSpecular);
              } else if (istringEqual(attributeName, "Interior Shade Layer Type")) {
                shadeTypeMatch = istringEqual(attribute.valueAsString(), shadeType);
              }
            }

            // check if meets criteria
            if (vltDiff <= 0.01 && vltSpecularDiff <= 1 && shadeTypeMatch) {
              // this one works try to get payload
              boost::optional<BCLComponent> component = bcl.getComponent(result.uid());
              if (component) {
                std::vector<std::string> files = component->files("xml");
                if (!files.empty()) {
                  return files[0];
                }
              }
            }

          } catch (const std::exception&) {
            // not the right one
          }
        }
      }
    }

    return boost::none;
  }

  std::string cleanName(const std::string& name) {
    std::string result = boost::algorithm::replace_all_regex_copy(name, boost::regex("[\\.\\s:]"), std::string("_"));
    return result;
  }

  openstudio::model::Model modelToRadPreProcess(const openstudio::model::Model& model) {
    auto outmodel = model.clone().cast<model::Model>();
    outmodel.purgeUnusedResourceObjects();
    outmodel.getUniqueModelObject<openstudio::model::Building>();   // implicitly create building object
    outmodel.getUniqueModelObject<openstudio::model::Timestep>();   // implicitly create timestep object
    outmodel.getUniqueModelObject<openstudio::model::RunPeriod>();  // implicitly create runperiod object

    std::map<std::string, openstudio::model::ThermalZone> thermalZones;

    std::vector<openstudio::model::Space> spaces = outmodel.getConcreteModelObjects<openstudio::model::Space>();
    for (auto& space : spaces) {
      space.hardApplyConstructions();
      space.hardApplySpaceType(true);
      space.hardApplySpaceLoadSchedules();

      // make all surfaces with surface boundary condition adiabatic
      std::vector<openstudio::model::Surface> surfaces = space.surfaces();
      for (auto& surf_it : surfaces) {
        boost::optional<openstudio::model::Surface> adjacentSurface = surf_it.adjacentSurface();
        if (adjacentSurface) {

          // make sure to hard apply constructions in other space before messing with surface in other space
          boost::optional<openstudio::model::Space> adjacentSpace = adjacentSurface->space();
          if (adjacentSpace) {
            adjacentSpace->hardApplyConstructions();
          }

          // resets both surfaces
          surf_it.resetAdjacentSurface();

          // set both to adiabatic
          surf_it.setOutsideBoundaryCondition("Adiabatic");
          adjacentSurface->setOutsideBoundaryCondition("Adiabatic");

          // remove interior windows
          for (openstudio::model::SubSurface subSurface : surf_it.subSurfaces()) {
            subSurface.remove();
          }
          for (openstudio::model::SubSurface subSurface : adjacentSurface->subSurfaces()) {
            subSurface.remove();
          }
        }
      }

      openstudio::model::Space new_space = space.clone(outmodel).optionalCast<openstudio::model::Space>().get();

      boost::optional<openstudio::model::ThermalZone> thermalZone = space.thermalZone();

      if (thermalZone && thermalZone->name()) {
        if (thermalZones.find(*thermalZone->name()) == thermalZones.end()) {
          openstudio::model::ThermalZone newThermalZone(outmodel);
          newThermalZone.setName(*thermalZone->name());
          newThermalZone.setUseIdealAirLoads(true);
          thermalZones.insert(std::make_pair(*thermalZone->name(), newThermalZone));
        }
        auto itr = thermalZones.find(*thermalZone->name());
        OS_ASSERT(itr != thermalZones.end());  // We just added it above if we needed it
        new_space.setThermalZone(itr->second);
      } else if (thermalZone && !thermalZone->name()) {
        LOG_FREE(Warn, "radiance::modelToRadPreProcess", "Space discovered in un-named thermalZone, not translating");
      }
    }

    std::vector<openstudio::model::ShadingSurfaceGroup> shadingsurfacegroups =
      outmodel.getConcreteModelObjects<openstudio::model::ShadingSurfaceGroup>();
    for (auto& shadingSurfaceGroup : shadingsurfacegroups) {
      shadingSurfaceGroup.remove();
    }

    std::vector<openstudio::model::SpaceItem> spaceitems = outmodel.getModelObjects<openstudio::model::SpaceItem>();
    for (auto& spaceItem : spaceitems) {
      if (spaceItem.optionalCast<openstudio::model::People>()) {
        // keep people
      } else if (spaceItem.optionalCast<openstudio::model::Lights>()) {
        // keep lights
      } else if (spaceItem.optionalCast<openstudio::model::Luminaire>()) {
        // keep luminaires
      } else {
        spaceItem.remove();
      }
    }

    std::vector<openstudio::model::OutputVariable> outputVariables = outmodel.getConcreteModelObjects<openstudio::model::OutputVariable>();
    for (auto& outputVariable : outputVariables) {
      outputVariable.remove();
    }

    openstudio::model::OutputVariable outputVariable("Site Exterior Horizontal Sky Illuminance", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    outputVariable = openstudio::model::OutputVariable("Site Exterior Beam Normal Illuminance", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    outputVariable = openstudio::model::OutputVariable("Site Solar Altitude Angle", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    outputVariable = openstudio::model::OutputVariable("Site Solar Azimuth Angle", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    outputVariable = openstudio::model::OutputVariable("Site Sky Diffuse Solar Radiation Luminous Efficacy", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    outputVariable = openstudio::model::OutputVariable("Site Beam Solar Radiation Luminous Efficacy", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    outputVariable = openstudio::model::OutputVariable("Zone People Occupant Count", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    outputVariable = openstudio::model::OutputVariable("Zone Lights Electricity Rate", outmodel);
    outputVariable.setReportingFrequency("Hourly");

    // only report weather periods
    auto simulation_control = outmodel.getUniqueModelObject<openstudio::model::SimulationControl>();
    simulation_control.setRunSimulationforSizingPeriods(false);
    simulation_control.setRunSimulationforWeatherFileRunPeriods(true);
    simulation_control.setSolarDistribution("MinimalShadowing");

    // purge unused
    outmodel.purgeUnusedResourceObjects();

    return outmodel;
  }

}  // namespace radiance
}  // namespace openstudio
