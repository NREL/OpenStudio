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

#include "ForwardTranslator.hpp"

#include "../model/DaylightingControl.hpp"
#include "../model/GlareSensor.hpp"
#include "../model/InteriorPartitionSurfaceGroup.hpp"
#include "../model/Luminaire.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/WeatherFile_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
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

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/bcl/BCL.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/bcl/LocalBCL.hpp"

#include <QPolygonF>
#include <QPointF>
#include <QDir>
#include <QDateTime>
#include <QThread>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

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

typedef boost::filesystem::basic_ofstream<char> OFSTREAM;

namespace openstudio {
namespace radiance {

  // internal method used to format doubles as strings
  std::string formatString(double t_d, unsigned t_prec)
  {
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
  template<typename T>
  std::string formatString(const T &t)
  {
    return boost::lexical_cast<std::string>(t);
  }

  // basic constructor
  ForwardTranslator::ForwardTranslator()
    : m_windowGroupId(1) // m_windowGroupId is reserved for uncontrolled
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.radiance\\.ForwardTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  std::vector<openstudio::path> ForwardTranslator::translateModel(const openstudio::path& outPath, const openstudio::model::Model& model)
  {
    m_model = model.clone(true).cast<openstudio::model::Model>();

    m_model.purgeUnusedResourceObjects();

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    clear();

    // TODO: check for shading controls, set m_shadingControls

    std::vector<openstudio::path> outfiles;

    // preprocess the model here
    unsigned numSpacesToSimulate = 0;
    for (openstudio::model::Space space : m_model.getConcreteModelObjects<openstudio::model::Space>()){

      // remove any spaces not associated with a thermal zone
      boost::optional<openstudio::model::ThermalZone> thermalZone = space.thermalZone();
      if (!thermalZone){
        LOG(Warn, "Space " << space.name().get() << " is not associated with a ThermalZone, it will not be translated.");
        space.remove();
        continue;
      }

      // only allow assigned primary and secondary daylighting control points (as defined in thermal zone) through
      for (openstudio::model::DaylightingControl daylightingControl : space.daylightingControls()){
        if (daylightingControl.isPrimaryDaylightingControl()){
          // ok
        }else if (daylightingControl.isSecondaryDaylightingControl()){
          // ok
        }else{
          LOG(Warn, "DaylightingControl " << daylightingControl.name().get() << \
            " is not associated with this Space's ThermalZone, it will not be translated.");
          daylightingControl.remove();
        }
      }

      std::vector<openstudio::model::DaylightingControl> daylightingControls = space.daylightingControls();
      std::vector<openstudio::model::IlluminanceMap> illuminanceMaps = space.illuminanceMaps();
      std::vector<openstudio::model::GlareSensor> glareSensors = space.glareSensors();

      if (daylightingControls.size() > 0 && illuminanceMaps.size() > 0){
        numSpacesToSimulate += 1;

        if (glareSensors.empty()){
          LOG(Warn, "Space " << space.name().get() << \
            " contains Radiance daylighting objects but no GlareSensor objects.");
        }
      }
    }

    if (numSpacesToSimulate == 0){
      LOG(Error, "Model does not contain any Radiance daylighting objects.");
      return outfiles;
    }

    // catch any directory or file related issues here
    try{

      LOG(Debug, "Working Directory: " + openstudio::toString(outPath));

      boost::filesystem::create_directories(outPath);

      //create radiance dirs for model export
      openstudio::path radDir = outPath;
      boost::filesystem::create_directory(radDir / openstudio::toPath("scene"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("scene/glazing"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("scene/shades"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("materials"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("numeric"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("views"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("options"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("bsdf"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("skies"));

      // get the building
      openstudio::model::Building building = m_model.getUniqueModelObject<openstudio::model::Building>();

      // get the site
      openstudio::model::Site site = m_model.getUniqueModelObject<openstudio::model::Site>();

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
      std::ifstream dcmatfilein(openstudio::toString(radDir / openstudio::toPath("materials/materials_dc.rad")).c_str());

      while (dcmatfilein.good())
      {
        std::string line;
        std::getline(dcmatfilein, line);
        dcmatsStringin += line;
      }

      std::vector<std::string> dcmatsArray;
      boost::algorithm::split(dcmatsArray, dcmatsStringin, boost::is_any_of(","));

      int totalVectors = dcmatsArray.size();

      LOG(Debug, "Total aperture headings: " << totalVectors);
      if (totalVectors > 8)
      {
        LOG(Warn, formatString(totalVectors) << " glazing orientations detected (can cause long simulation times if using daylight coefficients).");
      }

      //    std::string dcmatsString = dcmatsStringin.gsub(',', ' ');

      // get Radiance sim settings
      openstudio::model::RadianceParameters radianceParameters = m_model.getUniqueModelObject<openstudio::model::RadianceParameters>();

      // write Radiance options to file(s)

      // 2- or 3-phase?

      std::vector<openstudio::model::ShadingControl> shadingControls = m_model.getModelObjects<openstudio::model::ShadingControl>();
      openstudio::path daylightsimoptpath = radDir / openstudio::toPath("options/daylightsim.opt");
      OFSTREAM daylightsimopt(daylightsimoptpath);
      if (daylightsimopt.is_open()){
        outfiles.push_back(daylightsimoptpath);
        if (shadingControls.empty())
        {
          // not 3-phase
          daylightsimopt << "--x";
        } else {
          // yes 3-phase
          daylightsimopt << "--z";
        }

      }else{
        LOG(Error, "Cannot open file '" << toString(daylightsimoptpath) << "' for writing");
      }


      // view matrix options

      openstudio::path vmxoptpath = radDir / openstudio::toPath("options/vmx.opt");
      OFSTREAM vmxopt(vmxoptpath);
      if (vmxopt.is_open()){
        outfiles.push_back(vmxoptpath);
        vmxopt << "-ab " << (int)radianceParameters.ambientBouncesVMX() << " "
                  << "-ad " << (int)radianceParameters.ambientDivisionsVMX() << " "
                  << "-as " << (int)radianceParameters.ambientSupersamples() << " "
                  // << "-c " << (int)radianceParameters.accumulatedRaysperRecord() << " "
                  << "-dj " << radianceParameters.directJitter() << " "
                  << "-dp " << radianceParameters.directPretest() << " "
                  << "-dt " << radianceParameters.directThreshold() << " "
                  << "-dc " << radianceParameters.directCertainty() << " "
                  << "-lw " << radianceParameters.limitWeightVMX() << " ";
      }else{

        LOG(Error, "Cannot open file '" << toString(vmxoptpath) << "' for writing");

      }


      // daylight matrix options

      openstudio::path dmxoptpath = radDir / openstudio::toPath("options/dmx.opt");
      OFSTREAM dmxopt(dmxoptpath);
      if (dmxopt.is_open()){
        outfiles.push_back(dmxoptpath);
        dmxopt << "-ab " << (int)radianceParameters.ambientBouncesDMX() << " "
                  << "-ad " << (int)radianceParameters.ambientDivisionsDMX() << " "
                  << "-as " << (int)radianceParameters.ambientSupersamples() << " "
                  // << "-c " << (int)radianceParameters.accumulatedRaysperRecord() << " "
                  << "-dj " << radianceParameters.directJitter() << " "
                  << "-dp " << radianceParameters.directPretest() << " "
                  << "-dt " << radianceParameters.directThreshold() << " "
                  << "-dc " << radianceParameters.directCertainty() << " "
                  << "-lw " << radianceParameters.limitWeightDMX() << " ";
      }else{

        LOG(Error, "Cannot open file '" << toString(dmxoptpath) << "' for writing");

      }

      // Tregenza/Klems resolution options
      openstudio::path tregoptpath = radDir / openstudio::toPath("options/treg.opt");
      OFSTREAM tregopt(tregoptpath);
      if (tregopt.is_open()){
        outfiles.push_back(tregoptpath);
        tregopt << "-c " << (int)radianceParameters.klemsSamplingDensity() << " ";

        if (radianceParameters.skyDiscretizationResolution() == "146"){
          tregopt << "-e MF:1 -f tregenza.cal -b tbin -bn Ntbins";
        } else if (radianceParameters.skyDiscretizationResolution() == "578"){
          tregopt << "-e MF:2 -f reinhart.cal -b rbin -bn Nrbins";
        } else if (radianceParameters.skyDiscretizationResolution() == "2306"){
          tregopt << "-e MF:4 -f reinhart.cal -b rbin -bn Nrbins";
        }
        // TODO: make these values into a pulldown choice, add support for out of bounds
      }else{
        LOG(Error, "Cannot open file '" << toString(tregoptpath) << "' for writing");
      }

      // write dc sky sampling file
      openstudio::path dcskyfilepath = radDir / openstudio::toPath("skies/dc_sky.rad");
      OFSTREAM skyfile(dcskyfilepath);
      if (skyfile.is_open()){
        outfiles.push_back(dcskyfilepath);
        // ground first (P0)!
        skyfile << "#@rfluxmtx h=u u=Y\nvoid glow groundglow\n0\n0\n4\n1 1 1 0\n\ngroundglow source ground\n0\n0\n4\n0 0 -1 180\n";
        if (radianceParameters.skyDiscretizationResolution() == "146"){
          skyfile << "#@rfluxmtx h=r1 u=Y\n";
        } else if (radianceParameters.skyDiscretizationResolution() == "578"){
          skyfile << "#@rfluxmtx h=r2 u=Y\n";
        } else if (radianceParameters.skyDiscretizationResolution() == "2306"){
          skyfile << "#@rfluxmtx h=r4 u=Y\n";
        }
        skyfile << "void glow skyglow\n0\n0\n4\n1 1 1 0\n\nskyglow source sky\n0\n0\n4\n0 0 1 180\n";

      }else{
        LOG(Error, "Cannot open file '" << toString(dcskyfilepath) << "' for writing");
      }

      // Hi Qual options (illuminance maps)
      openstudio::path mapsoptpath = radDir / openstudio::toPath("options/maps.opt");
      OFSTREAM mapsopt(mapsoptpath);
      if (mapsopt.is_open()){
        outfiles.push_back(mapsoptpath);
        mapsopt << "-ab 10 -ad 10000 -as 50 -dt 0 -dc 1 -ds 0.05 -lw 0.00001";
      }else{
        LOG(Error, "Cannot open file '" << toString(mapsoptpath) << "' for writing");
      }

      // the end
      LOG(Debug, "Done. Radiance model located at: " << openstudio::toString(radDir) << ".");

    }catch(const std::exception& e){
      outfiles.clear();

      LOG(Error, e.what());
    }

    return outfiles;
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

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::Surface& surface)
  {
    openstudio::Point3dVector result;

    Transformation buildingTransformation;
    OptionalBuilding building = surface.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = surface.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    // transformation from space coordinates to face coordinates
    Transformation alignFace = Transformation::alignFace(surface.vertices());

    // get the current vertices and convert to face coordinates
    Point3dVector surfaceFaceVertices = alignFace.inverse()*surface.vertices();

    // subtract sub surface polygons from surface polygon
    QPolygonF outer;
    for (const Point3d& point : surfaceFaceVertices){
      if (std::abs(point.z()) > 0.001){
        LOG(Warn, "Surface point z not on plane, z =" << point.z());
      }
      outer << QPointF(point.x(),point.y());
    }

    for (const SubSurface& subSurface : surface.subSurfaces()){
      Point3dVector subsurfaceFaceVertices = alignFace.inverse()*subSurface.vertices();
      QPolygonF inner;
      for (const Point3d& point : subsurfaceFaceVertices){
        if (std::abs(point.z()) > 0.001){
          LOG(Warn, "Subsurface point z not on plane, z =" << point.z());
        }
        inner << QPointF(point.x(),point.y());
      }
      outer = outer.subtracted(inner);

    }

    for (const QPointF& point : outer){
      result.push_back(openstudio::Point3d(point.x(),point.y(), 0));
    }

    return buildingTransformation*spaceTransformation*alignFace*result;
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::SubSurface& subSurface)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = subSurface.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSurface surface = subSurface.surface();
    if (surface){
      OptionalSpace space = surface->space();
      if (space){
        spaceTransformation = space->transformation();
      }
    }else{
      LOG(Warn, "Could not retrieve surface for sub surface '" << subSurface.name() << "'");
    }

    openstudio::Point3dVector vertices = subSurface.vertices();

    // apply frame and divider reveal
    boost::optional<openstudio::model::WindowPropertyFrameAndDivider> frameAndDivider = subSurface.windowPropertyFrameAndDivider();
    if (frameAndDivider){
      if (!frameAndDivider->isOutsideRevealDepthDefaulted()){
        openstudio::Vector3d offset = -frameAndDivider->outsideRevealDepth() * subSurface.outwardNormal();

        for (openstudio::Point3d& vertex : vertices){
          vertex = vertex + offset;
        }
      }
    }

    // convert vertices to absolute coordinates
    return buildingTransformation*spaceTransformation*vertices;
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::ShadingSurface& shadingSurface)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = shadingSurface.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation shadingSurfaceGroupTransformation;
    Transformation spaceTransformation;
    OptionalShadingSurfaceGroup shadingSurfaceGroup = shadingSurface.shadingSurfaceGroup();
    if (shadingSurfaceGroup){
      shadingSurfaceGroupTransformation = shadingSurfaceGroup->transformation();

      OptionalSpace space = shadingSurfaceGroup->space();
      if (space){
        spaceTransformation = space->transformation();
      }
    }

    Transformation transformation;
    std::string shadingSurfaceType = shadingSurfaceGroup->shadingSurfaceType();
    if (istringEqual("Site", shadingSurfaceType)){
      transformation = shadingSurfaceGroupTransformation;
    }else if (istringEqual("Building", shadingSurfaceType)){
      transformation = buildingTransformation*shadingSurfaceGroupTransformation;
    }else if (istringEqual("Space", shadingSurfaceType)){
      transformation = buildingTransformation*spaceTransformation*shadingSurfaceGroupTransformation;
    }else{
      LOG(Error, "Unknown shading surface type for shading surface '" << shadingSurface.name() << "'");
      return Point3dVector();
    }

    // convert vertices to absolute coordinates
    return transformation*shadingSurface.vertices();
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::InteriorPartitionSurface& interiorPartitionSurface)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = interiorPartitionSurface.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation interiorPartitionSurfaceGroupTransformation;
    Transformation spaceTransformation;
    OptionalInteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup = interiorPartitionSurface.interiorPartitionSurfaceGroup();
    if (interiorPartitionSurfaceGroup){
      interiorPartitionSurfaceGroupTransformation = interiorPartitionSurfaceGroup->transformation();

      OptionalSpace space = interiorPartitionSurfaceGroup->space();
      if (space){
        spaceTransformation = space->transformation();
      }
    }

    // convert vertices to absolute coordinates
    return buildingTransformation*spaceTransformation*interiorPartitionSurfaceGroupTransformation*interiorPartitionSurface.vertices();
  }

  openstudio::Point3dVector ForwardTranslator::getPolygon(const openstudio::model::Luminaire& luminaire)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = luminaire.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = luminaire.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    openstudio::Point3dVector triangle;
    triangle.push_back(Point3d(0,0,0));
    triangle.push_back(Point3d(1,0,0));
    triangle.push_back(Point3d(0,1,0));

    // convert vertices to absolute coordinates
    return buildingTransformation*spaceTransformation*luminaire.transformation()*triangle;
  }

  openstudio::Point3d ForwardTranslator::getReferencePoint(const openstudio::model::DaylightingControl& control)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    return buildingTransformation*spaceTransformation*control.transformation()*Point3d(0,0,0);
  }

  openstudio::Vector3d ForwardTranslator::getSensorVector(const openstudio::model::DaylightingControl& control)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    Point3d endPoint = buildingTransformation*spaceTransformation*control.transformation()*Point3d(0,0,1);
    Point3d startPoint = buildingTransformation*spaceTransformation*control.transformation()*Point3d(0,0,0);

    return (endPoint - startPoint);
  }

  openstudio::Vector3dVector ForwardTranslator::getViewVectors(const openstudio::model::DaylightingControl& control)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    openstudio::Vector3dVector result;

    double pi = boost::math::constants::pi<double>();
    unsigned n = control.numberofDaylightingViews();
    Point3d startPoint = buildingTransformation*spaceTransformation*control.transformation()*Point3d(0,0,0);
    for (unsigned i = 0; i < n; ++i){
      double angle = 2.0*i*pi/(double(n));
      Point3d endPoint = buildingTransformation*spaceTransformation*control.transformation()*Point3d(sin(angle),cos(angle),0);
      result.push_back(endPoint - startPoint);
    }

    return result;
  }

  openstudio::Vector3dVector ForwardTranslator::getViewVectors(const openstudio::model::GlareSensor& control)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = control.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = control.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    openstudio::Vector3dVector result;

    double pi = boost::math::constants::pi<double>();
    unsigned n = control.numberofGlareViewVectors();
    Point3d startPoint = buildingTransformation*spaceTransformation*control.transformation()*Point3d(0,0,0);
    for (unsigned i = 0; i < n; ++i){
      double angle = 2.0*i*pi/(double(n));
      Point3d endPoint = buildingTransformation*spaceTransformation*control.transformation()*Point3d(sin(angle),cos(angle),0);
      result.push_back(endPoint - startPoint);
    }

    return result;
  }

  openstudio::Point3d ForwardTranslator::getReferencePoint(const openstudio::model::GlareSensor& sensor)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = sensor.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = sensor.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    return buildingTransformation*spaceTransformation*sensor.transformation()*Point3d(0,0,0);
  }

  openstudio::Vector3d ForwardTranslator::getSensorVector(const openstudio::model::GlareSensor& sensor)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = sensor.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = sensor.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    Point3d endPoint = buildingTransformation*spaceTransformation*sensor.transformation()*Point3d(0,0,1);
    Point3d startPoint = buildingTransformation*spaceTransformation*sensor.transformation()*Point3d(0,0,0);

    return (endPoint - startPoint);
  }

  openstudio::Point3dVector ForwardTranslator::getReferencePoints(const openstudio::model::IlluminanceMap& map)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = map.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = map.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    // convert vertices to absolute coordinates
    return buildingTransformation*spaceTransformation*map.transformation()*map.referencePoints();
  }

  openstudio::Vector3d ForwardTranslator::getSensorVector(const openstudio::model::IlluminanceMap& map)
  {
    Transformation buildingTransformation;
    OptionalBuilding building = map.model().getOptionalUniqueModelObject<Building>();
    if (building){
      buildingTransformation = building->transformation();
    }

    Transformation spaceTransformation;
    OptionalSpace space = map.space();
    if (space){
      spaceTransformation = space->transformation();
    }

    // convert to absolute coordinates
    return buildingTransformation*spaceTransformation*map.transformation()*Vector3d(0,0,1);
  }

  void ForwardTranslator::clear()
  {
    m_radMaterials.clear();
    m_radMixMaterials.clear();
    m_radMaterialsDC.clear();
    m_radMaterialsWG0.clear();

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

  WindowGroup ForwardTranslator::getWindowGroup(const openstudio::Vector3d& outwardNormal, const model::Space& space, const model::ConstructionBase& construction,
    const boost::optional<model::ShadingControl>& shadingControl, const openstudio::Point3dVector& polygon)
  {
    WindowGroup result(outwardNormal, space, construction, shadingControl);

    std::stringstream ss;
    ss << "WG" << m_windowGroupId;
    std::string name = ss.str();

    // set the uncontrolled window group name
    if (!shadingControl){
      name = "WG0";
    }

    result.setName(name);

    auto it = std::find(m_windowGroups.begin(), m_windowGroups.end(), result);
    if (it != m_windowGroups.end()){
      it->addWindowPolygon(polygon);
      return *it;
    }

    // if we made a new group
    if (shadingControl){
      m_windowGroupId += 1;
    }

    result.addWindowPolygon(polygon);
    m_windowGroups.push_back(result);
    return result;
  }

  void ForwardTranslator::siteShadingSurfaceGroups(const openstudio::path &t_radDir,
      const std::vector<openstudio::model::ShadingSurfaceGroup> &radShadingSurfaceGroups,
      std::vector<openstudio::path> &t_outfiles)
  {
    LOG(Debug, "site shading groups found: " << radShadingSurfaceGroups.size());

    if (!radShadingSurfaceGroups.empty())
    {
      std::set<std::string> siteShadingSurfaces;

      for (const auto & shadingSurfaceGroup : radShadingSurfaceGroups)
      {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup.shadingSurfaces();
        for (const auto & shadingSurface : shadingSurfaces)
        {
          // clean name
          std::string shadingSurface_name = cleanName(shadingSurface.name().get());

          LOG(Debug, "site shading surface: " << shadingSurface_name );

         // get reflectance
          double interiorVisibleReflectance = 0.25; // default for site shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()){
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }

          double exteriorVisibleReflectance = 0.25; // default
          if (shadingSurface.exteriorVisibleAbsorptance()){
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
          m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + "\n4 " + \
              "refl_" + formatString(exteriorVisibleReflectance, 3) + " " + \
              "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

          // polygon header
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);

          std::string shadingsurface = "reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + " polygon " + shadingSurface_name + "\n";
          shadingsurface += "0\n0\n" + formatString(polygon.size()*3) + "\n";

          for (Point3dVector::const_iterator vertex = polygon.begin();
              vertex != polygon.end();
              ++vertex)
          {
            shadingsurface += formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) +"\n";
          }

          siteShadingSurfaces.insert(shadingsurface);
        }
      }

      openstudio::path filename = t_radDir / openstudio::toPath("/scene/shading_site.rad");
      OFSTREAM f(filename);
      if (f.is_open()){
        t_outfiles.push_back(filename);
        m_radSceneFiles.push_back(filename);

        for (const auto & line : siteShadingSurfaces)
        {
          f << line;
        }
      }else{
        LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
      }

    }

  }

  void ForwardTranslator::buildingShadingSurfaceGroups(const openstudio::path &t_radDir,
      const std::vector<openstudio::model::ShadingSurfaceGroup> &t_radShadingSurfaceGroups,
      std::vector<openstudio::path> &t_outfiles)
  {
    LOG(Debug, "building shading groups found: " << t_radShadingSurfaceGroups.size());

    if (!t_radShadingSurfaceGroups.empty())
    {
      std::set<std::string> buildingShadingSurfaces;

      for (const auto & group : t_radShadingSurfaceGroups)
      {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = group.shadingSurfaces();

        for (const auto & shadingSurface : shadingSurfaces)
        {
          // clean name
          std::string shadingSurface_name = cleanName(shadingSurface.name().get());

          LOG(Debug, "building shading surface: " << shadingSurface_name);

         // get reflectance
          double interiorVisibleReflectance = 0.25; // default for building shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()){
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }

          double exteriorVisibleReflectance = 0.25; // default
          if (shadingSurface.exteriorVisibleAbsorptance()){
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
          m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + "\n4 " + \
              "refl_" + formatString(exteriorVisibleReflectance, 3) + " " + \
              "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

          // start polygon
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);

          // header
          std::string shadingsurface = "reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + " polygon " + shadingSurface_name + "\n";
          shadingsurface += "0\n0\n" + formatString(polygon.size()*3) + "\n";

          //

          for (const auto & vertex : polygon)
          {
            shadingsurface += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) +"\n\n";
          }

          buildingShadingSurfaces.insert(shadingsurface);

        }
      }

      openstudio::path filename = t_radDir / openstudio::toPath("scene/shading_building.rad");
      OFSTREAM f(filename);
      if (f.is_open()){
        t_outfiles.push_back(filename);
        m_radSceneFiles.push_back(filename);
        for (const auto & line : buildingShadingSurfaces)
        {
          f << line;
        }
      }else{
        LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
      }
    }
  }

  void ForwardTranslator::buildingSpaces(const openstudio::path &t_radDir, const std::vector<openstudio::model::Space> &t_spaces,
      std::vector<openstudio::path> &t_outfiles)
  {
    std::vector<std::string> space_names;

    for (const auto & space : t_spaces)
    {
      std::string space_name = cleanName(space.name().get());

      space_names.push_back(space_name);
      LOG(Debug, "Processing space: " << space_name);

      // split model into zone-based Radiance .rad files
      m_radSpaces[space_name] = "#\n# geometry file for space: " + space_name + "\n#\n\n";

      // loop over surfaces in space

      std::vector<openstudio::model::Surface> surfaces = space.surfaces();

      for (const auto & surface : surfaces)
      {

        // skip if air wall
        if (surface.isAirWall()){
          continue;
        }

        std::string surface_name = cleanName(surface.name().get());

        // add surface to space geometry
        m_radSpaces[space_name] += "# surface: " + surface_name + "\n";

        // set construction of surface
        std::string constructionName = surface.getString(2).get();
        m_radSpaces[space_name] += "# construction: " + constructionName + "\n";

        // get reflectances
        double interiorVisibleReflectance = 0.5; // default for space surfaces
        if (surface.interiorVisibleAbsorptance()){
          double interiorVisibleAbsorptance = surface.interiorVisibleAbsorptance().get();
          interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
        }
        double exteriorVisibleReflectance = 0.25; // default for space surfaces (exterior)
        if (surface.exteriorVisibleAbsorptance()){
          double exteriorVisibleAbsorptance = surface.exteriorVisibleAbsorptance().get();
          exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
        }

        // create polygon object
        openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(surface);


        if (!surface.adjacentSurface()){
          // 2-sided material

          // header
          m_radSpaces[space_name] += "# reflectance (int) = " + formatString(interiorVisibleReflectance, 3) + \
          "\n# reflectance (ext) = " + formatString(exteriorVisibleReflectance, 3) + "\n";

          // material definition

          //interior
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3)
            + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance, 3)
            + " " + formatString(interiorVisibleReflectance, 3)
            + " " + formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
          //exterior
          m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance, 3)
            + "\n0\n0\n5\n" + formatString(exteriorVisibleReflectance, 3)
            + " " + formatString(exteriorVisibleReflectance, 3)
            + " " + formatString(exteriorVisibleReflectance, 3) + " 0 0\n\n");
          // mixfunc
          m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + "\n4 " + \
              "refl_" + formatString(exteriorVisibleReflectance, 3) + " " + \
              "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

          // polygon reference
          m_radSpaces[space_name] += "reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + " polygon " + \
              surface_name + "\n0\n0\n" + formatString(polygon.size() * 3) + "\n";
        }else{
          // interior-only material

          // header
          m_radSpaces[space_name] += "# reflectance: " + formatString(interiorVisibleReflectance, 3) + "\n";

          // material definition
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3)
            + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance, 3)
            + " " + formatString(interiorVisibleReflectance, 3)
            + " " + formatString(interiorVisibleReflectance, 3) + " 0 0\n");

          // polygon reference
          m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3)
          + " polygon " + surface_name + "\n0\n0\n" + formatString(polygon.size() * 3) + "\n";

        };


        // add polygon vertices
        for (const auto & vertex : polygon)
        {
          m_radSpaces[space_name] += formatString(vertex.x()) + " "
            + formatString(vertex.y()) + " "
            + formatString(vertex.z()) + "\n";
        }
        m_radSpaces[space_name] += "\n";

        // end(surface)


        // get sub surfaces
        std::vector<openstudio::model::SubSurface> subSurfaces = surface.subSurfaces();

        for (const auto & subSurface : subSurfaces)
        {

          std::string rMaterial = "glass";
          std::string matString = "";
          std::string shadeBSDF = "";

          boost::optional<model::ConstructionBase> construction = subSurface.construction();
          if (!construction){
            LOG(Warn, "SubSurface " << subSurface.name().get() << " is not associated with a Construction, it will not be translated.");
            continue;
          }

          boost::optional<model::WindowPropertyFrameAndDivider> frameAndDivider = subSurface.windowPropertyFrameAndDivider();

          // get the polygon
          polygon = openstudio::radiance::ForwardTranslator::getPolygon(subSurface);

          std::string subSurface_name = cleanName(subSurface.name().get());

          std::string subSurfaceUpCase = boost::algorithm::to_upper_copy(subSurface.subSurfaceType());

          if (subSurfaceUpCase == "FIXEDWINDOW"
              || subSurfaceUpCase == "OPERABLEWINDOW"
              || subSurfaceUpCase == "GLASSDOOR"
              || subSurfaceUpCase == "SKYLIGHT")
          {
            if (!subSurface.visibleTransmittance())
            {
              LOG(Warn, "Cannot determine visible transmittance for SubSurface " << subSurface.name().get() << ", it will not be translated.");
              continue;
            }

            boost::optional<model::ShadingControl> shadingControl = subSurface.shadingControl();

            // future support for mullion factor
            double visibleTransmittanceMultiplier = 1.0;
            if (frameAndDivider){
              // DLM: Rob what should we do here?
              // <punt>
              visibleTransmittanceMultiplier = 1.0;
              // </punt>
            }

            // find window group
            openstudio::Vector3d outwardNormal = surface.outwardNormal();

            WindowGroup windowGroup = getWindowGroup(outwardNormal, space, *construction, shadingControl, polygon);
            std::string windowGroup_name = windowGroup.name();

            // get the normal
            WindowGroupControl control = windowGroup.windowGroupControl();
            if (control.outwardNormal){

            LOG(Info, windowGroup.name() + " outward normal:" + formatString(control.outwardNormal->x()) + " " + formatString(control.outwardNormal->y()) + " " + \
                formatString(control.outwardNormal->z()) + "\n");
            }

            std::string winUpVector = "Z";
            if (subSurfaceUpCase == "SKYLIGHT"){
              winUpVector = "Y";
            }

            if (m_radWindowGroups.find(windowGroup_name) == m_radWindowGroups.end())
            {
              m_radWindowGroups[windowGroup_name] = "# OpenStudio Window Group: " + windowGroup_name + "\n";
              if(windowGroup_name == "WG0"){
                 m_radWindowGroups[windowGroup_name] += "# All uncontrolled windows, multiple orientations possible, no hemispherical sampling info.\n\n";
              }
              else{
                // 3-phase/rfluxmtx support
                // moved to "shade" polygon now 2015.07.23 RPG
              }

            }

            LOG(Info, "found a " + subSurface.subSurfaceType() + " named '" + subSurface_name + "', windowGroup_name = '" + windowGroup_name + "'");

            // set transmittance...
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
              double tS = 0.0; // transmitted specularity
              // rD = 0.95 // diffuse reflectance (not used for monochromatic trans mats)
              double sR = 0.0; // surface roughness
              double rS = 0.05; // specular reflectance
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
              double transA3 = cRGB; // monochromatic
              double transA2 = cRGB; // monochromatic
              double transA1 = cRGB; // monochromatic

              rMaterial = "trans";
              matString = "0\n0\n7\n" + formatString(transA1, 3) + " " + formatString(transA2, 3) + " " + formatString(transA3, 3) + \
                " " + formatString(transA4, 3) + " " + formatString(transA5, 3) + " " + formatString(transA6, 3) + " " + formatString(transA7, 2) + "\n";
              //double nTs = 0.0; // transmitted specularity

              if (tVis >= 0.6) {
                LOG(Warn, "dubious glazing material definition in " + space_name + "; Tvis =" + formatString(tVis, 2) + ", yet diffuse? Suspect.");
              }

            } else {

              matString = "0\n0\n3\n" + formatString(tn, 3) + " " + formatString(tn, 3) + " " + formatString(tn, 3) + "\n";

            }

            // write the window

            // write reveal surfaces from window frame and divider, add small (3") inside reveals if no framediv object

            boost::optional<double> outsideRevealDepth;
            boost::optional<double> insideRevealDepth;
            boost::optional<double> insideSillDepth;

            if (frameAndDivider){

              if (frameAndDivider->isOutsideRevealDepthDefaulted()){
                outsideRevealDepth = 0.0;
              }else{
                outsideRevealDepth = frameAndDivider->outsideRevealDepth();
              }

              if (frameAndDivider->isInsideRevealDepthDefaulted()){
                insideRevealDepth = 0.05;
              }else{
                // ensure there's a reasonable minimum to contain the shade/blind
                if (insideRevealDepth < 0.05){
                  insideRevealDepth = 0.05;
                  LOG(Warn, "inside reveal depth increased to 0.05 m");
                }else{
                  insideRevealDepth = frameAndDivider->insideRevealDepth();
                }
              }

              if (frameAndDivider->isInsideSillDepthDefaulted()){
                insideSillDepth = 0.05;
              }else{
                if (insideSillDepth < 0.05){
                  insideSillDepth = 0.05;
                  LOG(Warn, "inside sill depth increased to 0.05 m");
                }else{
                  insideSillDepth = frameAndDivider->insideSillDepth();
                }
              }

            }else{

              outsideRevealDepth = 0.0;
              insideRevealDepth = 0.05;
              insideSillDepth = 0.05;

            }

            openstudio::Vector3d offset;

            // subSurface.outwardNormal not in global coordinate system
            outwardNormal = subSurface.outwardNormal();
            boost::optional<Vector3d> optionalOutwardNormal = openstudio::getOutwardNormal(polygon);
            if (optionalOutwardNormal){
              Vector3d outwardNormal = *optionalOutwardNormal;

              size_t N = polygon.size();
              for (size_t i = 0; i < N; ++i)
              {
                size_t index1 = i;
                size_t index2 = (i + 1) % N;

                if (outsideRevealDepth && (*outsideRevealDepth > 0.0)){
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
                  m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance, 3) + "\n0\n0\n5\n" + \
                                        formatString(exteriorVisibleReflectance, 3) + " " + \
                                        formatString(exteriorVisibleReflectance, 3) + " " + \
                                        formatString(exteriorVisibleReflectance, 3) + " 0 0\n\n");
                  // write polygon
                  m_radSpaces[space_name] += "refl_" + formatString(exteriorVisibleReflectance, 3) + " polygon outside_reveal_" + subSurface_name + formatString(i, 0) + "\n";
                  m_radSpaces[space_name] += "0\n0\n" + formatString(4 * 3) + "\n";
                  m_radSpaces[space_name] += formatString(vertex1.x()) + " " + formatString(vertex1.y()) + " " + formatString(vertex1.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex2.x()) + " " + formatString(vertex2.y()) + " " + formatString(vertex2.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex3.x()) + " " + formatString(vertex3.y()) + " " + formatString(vertex3.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex4.x()) + " " + formatString(vertex4.y()) + " " + formatString(vertex4.z()) + "\n\n";
                }

                // make interior sill/reveal surfaces
                if (insideRevealDepth && (*insideRevealDepth > 0.0)){

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
                  m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n" + \
                                        formatString(interiorVisibleReflectance, 3) + " " + \
                                        formatString(interiorVisibleReflectance, 3) + " " + \
                                        formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
                  // write polygon
                  m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon inside_reveal_" + subSurface_name + formatString(i, 0) + "\n";
                  m_radSpaces[space_name] += "0\n0\n" + formatString(4 * 3) + "\n";
                  m_radSpaces[space_name] += formatString(vertex1.x()) + " " + formatString(vertex1.y()) + " " + formatString(vertex1.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex2.x()) + " " + formatString(vertex2.y()) + " " + formatString(vertex2.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex3.x()) + " " + formatString(vertex3.y()) + " " + formatString(vertex3.z()) + "\n\n";
                  m_radSpaces[space_name] += formatString(vertex4.x()) + " " + formatString(vertex4.y()) + " " + formatString(vertex4.z()) + "\n\n";
                }

                if (insideSillDepth && (*insideSillDepth > 0.0)){

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
                  m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n" + \
                                        formatString(interiorVisibleReflectance, 3) + " " + \
                                        formatString(interiorVisibleReflectance, 3) + " " + \
                                        formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
                  // write polygon
                  m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon inside_sill_" + subSurface_name + formatString(i, 0) + "\n";
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

            if (windowGroup_name == "WG0"){

            //no shades

              // add materials
              m_radMaterials.insert("void " + rMaterial + " glaz_" + rMaterial + "_tn-" + formatString(tn, 3) + "\n" + matString + "\n");
              m_radMaterialsDC.insert("void alias glaz_" + rMaterial + "_tn-" + formatString(tn, 3) + " WG0\n\n");

              // write the window polygon
              m_radWindowGroups[windowGroup_name] += "glaz_"+rMaterial+"_tn-"+formatString(tn, 3) + " polygon " + subSurface_name + "\n";
              m_radWindowGroups[windowGroup_name] += "0\n0\n" + formatString(polygon.size()*3) + "\n\n";
              for (Point3dVector::const_reverse_iterator vertex = polygon.rbegin();
                vertex != polygon.rend();
                ++vertex)
              {
                m_radWindowGroups[windowGroup_name] += "" + formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n";
              }

            }else{

            //has shades

              //add materials
              m_radMaterials.insert("void " + rMaterial + " " + windowGroup_name + "\n" + matString + "\n");

              // write the polygon
              m_radWindowGroups[windowGroup_name] += windowGroup_name + " polygon " + subSurface_name + "\n";
              m_radWindowGroups[windowGroup_name] += "0\n0\n" + formatString(polygon.size() * 3) + "\n";
              for (Point3dVector::const_reverse_iterator vertex = polygon.rbegin();
                vertex != polygon.rend();
                ++vertex)

              {
                m_radWindowGroups[windowGroup_name] += "" + \
                formatString(vertex->x()) + " " + \
                formatString(vertex->y()) + " " + \
                formatString(vertex->z()) + "\n";
              }

              // add the shade

              rMaterial = "BSDF";
              matString = "6\n0 bsdf/blinds.xml 0 0 1 .\n0\n0\n";

              m_radMaterials.insert("void " + rMaterial + " " + windowGroup_name + "_SHADE\n" + matString + "\n\n");

              m_radMaterialsDC.insert("void light " + windowGroup_name + "_SHADE\n0\n0\n3\n1 1 1\n");
              m_radMaterialsWG0.insert("void plastic " + windowGroup_name + "_SHADE\n0\n0\n5\n0 0 0 0 0\n");


              // polygon header
              m_radWindowGroupShades[windowGroup_name] += "#@rfluxmtx h=kf u=" + winUpVector + " o=output/dc/" + windowGroup_name + ".vmx\n";
              m_radWindowGroupShades[windowGroup_name] += "\n# shade for SubSurface: " + subSurface_name + "\n";

              // write the polygon
              m_radWindowGroupShades[windowGroup_name] += windowGroup_name + "_SHADE" + " polygon " + windowGroup_name + "_SHADE_" + subSurface_name + "\n";
              m_radWindowGroupShades[windowGroup_name] += "0\n0\n" + formatString(polygon.size() * 3) + "\n";
              for (Point3dVector::const_reverse_iterator vertex = polygon.rbegin();
                vertex != polygon.rend();
                ++vertex)

              {

                // offset the shade to the interior side of the window
                Point3d offsetVertex = *vertex + (-0.01*outwardNormal);

                m_radWindowGroupShades[windowGroup_name] += "" + \
                formatString(offsetVertex.x()) + " " + \
                formatString(offsetVertex.y()) + " " + \
                formatString(offsetVertex.z()) + "\n";
              }

            }


            // shade BSDF stuff

            // make dir for BSDF files

            openstudio::path bsdfoutpath = t_radDir / openstudio::toPath("bsdf");

            // Set shade BSDF

            shadeBSDF = "blinds.xml";
            // TODO get shade type from object
                // if shade type = blind, shadeBSDF = blind.xml
                // if shade type = louver, shadeBSDF = 1xliloX.xml
                // if shade type = shadecloth, shadeBSDF = 05_shade_light.xml
                // etc...

            // path to write bsdf

            openstudio::path shadeBSDFPath = t_radDir / openstudio::toPath("bsdf") / shadeBSDF;

            // add BSDF file to the collection of crap to copy up
            t_outfiles.push_back(shadeBSDFPath);

            // read BSDF from resource dll
            // must be referenced in openstudiocore/src/radiance/radiance.qrc
            QString defaultFile;
            QFile inFile(toQString(":/resources/" + shadeBSDF));
            if (inFile.open(QFile::ReadOnly)){
              QTextStream docIn(&inFile);
              defaultFile = docIn.readAll();
              inFile.close();
            }

            // write shade BSDF
            QFile outFile(toQString(shadeBSDFPath));
            bool opened = outFile.open(QIODevice::WriteOnly);
            if (!opened){
              LOG_AND_THROW("Cannot write file to '" << toString(shadeBSDFPath) << "'");
            }
            QTextStream textStream(&outFile);
            textStream << defaultFile;
            outFile.close();

            // add an airBSDF

            shadeBSDFPath = t_radDir / openstudio::toPath("bsdf") / openstudio::toPath("air.xml");

            // add BSDF file to the collection of crap to copy up
            t_outfiles.push_back(shadeBSDFPath);

            // read BSDF from resource dll
            // must be in openstudiocore/src/radiance/radiance.qrc
            QFile inFileAir(":/resources/air.xml");
            if (inFileAir.open(QFile::ReadOnly)){
              QTextStream docIn(&inFileAir);
              defaultFile = docIn.readAll();
              inFileAir.close();
            }

            // write shade BSDF
            QFile outFileAir(toQString(shadeBSDFPath));
            opened = outFileAir.open(QIODevice::WriteOnly);
            if (!opened){
              LOG_AND_THROW("Cannot write file to '" << toString(shadeBSDFPath) << "'");
            }
            QTextStream textStream2(&outFileAir);
            textStream2 << defaultFile;
            outFileAir.close();


            //store window group entry for mapping.rad
            if (windowGroup_name == "WG0"){
              // simple placeholder for WG0
              m_radDCmats.insert(windowGroup_name + ",n/a,n/a,n/a,n/a\n");
            }else{
              // store window group normal
              // hard coded shade algorithm: on if high solar (2), setpoint 2Klx (2000)
              // hard coded shade type: blinds
              m_radDCmats.insert(windowGroup_name + "," + \
                formatString((control.outwardNormal->x() * -1), 2) + " " + \
                formatString((control.outwardNormal->y() * -1), 2) + " " + \
                formatString((control.outwardNormal->z() * -1), 2) + ",2,2000,air.xml," + shadeBSDF + "\n");
            }

//            if (rMaterial == "glass"){
//
//               // path to write bsdf to
//               openstudio::path uncontrolledBSDFOut = t_radDir / openstudio::toPath("bsdf") / openstudio::toPath("/cl_Tn" + formatString(tVis, 2) + ".xml");
//
//               // add xml file to the collection of crap to copy up
//               t_outfiles.push_back(uncontrolledBSDFOut);
//
//               // get BSDF from BCL
//               boost::optional<openstudio::path> uncontrolledBSDF = getBSDF(tVis, 100, "None");
//               if (uncontrolledBSDF){
//                 // copy uncontrolledBSDF
//                 boost::filesystem::copy_file(*uncontrolledBSDF, uncontrolledBSDFOut, boost::filesystem::copy_option::overwrite_if_exists);
//               }else{
//                 LOG(Warn, "Cannot download BSDF for this window group, using default (Tvis=44%)");
//
//                 // read default file
//                 QString defaultFile;
//                 QFile inFile(":/resources/cl_Tn0.44.xml");
//                 if (inFile.open(QFile::ReadOnly)){
//                   QTextStream docIn(&inFile);
//                   defaultFile = docIn.readAll();
//                   inFile.close();
//                 }
//
//                 // write default file
//                 QFile outFile(toQString(uncontrolledBSDFOut));
//                 bool opened = outFile.open(QIODevice::WriteOnly);
//                 if (!opened){
//                   LOG_AND_THROW("Cannot write file to '" << toString(uncontrolledBSDFOut) << "'");
//                 }
//                 QTextStream textStream(&outFile);
//                 textStream << defaultFile;
//                 outFile.close();
//               }
//
//               // path to write bsdf to
//               openstudio::path controlledBSDFOut = t_radDir / openstudio::toPath("bsdf") / openstudio::toPath("/cl_Tn" + formatString(tVis, 2) + "_blinds.xml");
//
//               // add xml file to the collection of crap to copy up
//               t_outfiles.push_back(controlledBSDFOut);
//
//               // get BSDF from BCL
//               boost::optional<openstudio::path> controlledBSDF = getBSDF(tVis, 100, "Blind");
//               if (controlledBSDF){
//                 // copy controlledBSDF
//                 boost::filesystem::copy_file(*controlledBSDF, controlledBSDFOut, boost::filesystem::copy_option::overwrite_if_exists);
//               }else{
//                 LOG(Warn, "Cannot download BSDF, using default.");
//
//                 // read default file
//                 QString defaultFile;
//                 QFile inFile(":/resources/cl_Tn0.44_blinds.xml");
//                 if (inFile.open(QFile::ReadOnly)){
//                   QTextStream docIn(&inFile);
//                   defaultFile = docIn.readAll();
//                   inFile.close();
//                 }
//
//                 // write default file
//                 QFile outFile(toQString(uncontrolledBSDFOut));
//                 bool opened = outFile.open(QIODevice::WriteOnly);
//                 if (!opened){
//                   LOG_AND_THROW("Cannot write file to '" << toString(uncontrolledBSDFOut) << "'");
//                 }
//                 QTextStream textStream(&outFile);
//                 textStream << defaultFile;
//                 outFile.close();
//               }
//
//               // store window group entry for mapping.rad
//               if (windowGroup_name == "WG0"){
//                 // simple placeholder for WG0
//                 m_radDCmats.insert(windowGroup_name + ",n/a,n/a,n/a,n/a\n");
//               }else{
//                 // store window group normal (may not need anymore with rfluxmtx)
//                 // hard coded shade algorithm: on if high solar (2), setpoint 2Klx (2000)
//                 // hard coded shade type: blinds
//                 m_radDCmats.insert(windowGroup_name + "," + \
//                   formatString((control.outwardNormal->x() * -1), 2) + " " + \
//                   formatString((control.outwardNormal->y() * -1), 2) + " " + \
//                   formatString((control.outwardNormal->z() * -1), 2) + ",2,2000,cl_Tn" + \
//                   formatString(tVis, 2) + ".xml,cl_Tn" + \
//                   formatString(tVis, 2) + "_blinds.xml\n");
//                 }

//             } else if (rMaterial == "trans"){
//
//               // copy uncontrolledBSDF
//               openstudio::path uncontrolledBSDFOut = t_radDir / openstudio::toPath("bsdf") / openstudio::toPath("/df_Tn" + formatString(tVis, 2) + ".xml");
//
//               // add xml file to the collection of crap to copy up
//               t_outfiles.push_back(uncontrolledBSDFOut);
//
//               // get BSDF from BCL
//               boost::optional<openstudio::path> uncontrolledBSDF = getBSDF(tVis, 0, "None");
//               if (uncontrolledBSDF){
//                 // copy controlledBSDF
//                 boost::filesystem::copy_file(*uncontrolledBSDF, uncontrolledBSDFOut, boost::filesystem::copy_option::overwrite_if_exists);
//               }else{
//                 LOG(Warn, "Cannot download BSDF, using default.");
//
//                 // read default file
//                 QString defaultFile;
//                 QFile inFile(":/resources/df_Tn0.44.xml");
//                 if (inFile.open(QFile::ReadOnly)){
//                   QTextStream docIn(&inFile);
//                   defaultFile = docIn.readAll();
//                   inFile.close();
//                 }
//
//                 // write default file
//                 QFile outFile(toQString(uncontrolledBSDFOut));
//                 bool opened = outFile.open(QIODevice::WriteOnly);
//                 if (!opened){
//                   LOG_AND_THROW("Cannot write file to '" << toString(uncontrolledBSDFOut) << "'");
//                 }
//                 QTextStream textStream(&outFile);
//                 textStream << defaultFile;
//                 outFile.close();
//               }
//
//               // store window group entry for mapping.rad
//
//               // simple placeholder for WG0
//               if (windowGroup_name == "WG0"){
//                 m_radDCmats.insert(windowGroup_name + ",n/a\n");
//               }else{
//               // include normals for controlled WGs
//                 m_radDCmats.insert(windowGroup_name + "," + \
//                   formatString((control.outwardNormal->x() * -1), 2) + " " + \
//                   formatString((control.outwardNormal->y() * -1), 2) + " " + \
//                   formatString((control.outwardNormal->z() * -1), 2) + ",df_Tn" + formatString(tVis, 2) + ".xml\n");
//               }

            //}

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
            m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n" + \
              formatString(interiorVisibleReflectance, 3) + " " + \
              formatString(interiorVisibleReflectance, 3) + " " + \
              formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
            // write polygon
            m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon " + subSurface_name + "\n";
            m_radSpaces[space_name] += "0\n0\n" + formatString(polygon.size() * 3) + "\n\n";

            for (const auto & vertex : polygon)
            {
              m_radSpaces[space_name] += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n\n";
            }

          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDOME") {

            LOG(Warn, "subsurface is a tdd dome, not translated (not yet implemented).");

          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDIFFUSER") {

            LOG(Warn, "subsurface is a tdd diffuser, not translated (not yet implemented).");

          }

        } //end reveals

      } // end surfaces

      // get shading surfaces

      std::vector<openstudio::model::ShadingSurfaceGroup> shadingSurfaceGroups = space.shadingSurfaceGroups();
      for (const auto & shadingSurfaceGroup : shadingSurfaceGroups)
      {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup.shadingSurfaces();
        for (const auto & shadingSurface : shadingSurfaces)
        {
          std::string shadingSurface_name = cleanName(shadingSurface.name().get());

          // add surface to zone geometry
          m_radSpaces[space_name] += "# surface: " + shadingSurface_name + "\n";

          // set construction of space shadingSurface
          std::string constructionName = shadingSurface.getString(2).get();
          m_radSpaces[space_name] += "# construction: " + constructionName + "\n";

          // get reflectance
          double interiorVisibleReflectance = 0.25; // default for space shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()){
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }
          double exteriorVisibleReflectance = 0.25; // default for space shading surfaces
          if (shadingSurface.exteriorVisibleAbsorptance()){
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
          m_radMixMaterials.insert("void mixfunc reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + "\n4 " + \
              "refl_" + formatString(exteriorVisibleReflectance, 3) + " " + \
              "refl_" + formatString(interiorVisibleReflectance, 3) + " if(Rdot,1,0) .\n0\n0\n\n");

          // polygon header
          m_radSpaces[space_name] += "# exterior visible reflectance: " + formatString(exteriorVisibleReflectance, 3) + "\n";
          m_radSpaces[space_name] += "# interior visible reflectance: " + formatString(interiorVisibleReflectance, 3) + "\n";

          // get / write surface polygon

          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);
          m_radSpaces[space_name] += "reflBACK_" + formatString(interiorVisibleReflectance, 3) + \
              "_reflFRONT_" + formatString(exteriorVisibleReflectance, 3) + " polygon " + \
          shadingSurface_name + "\n0\n0\n" + formatString(polygon.size() * 3) + "\n";

          for (const auto & vertex : polygon)
          {
            m_radSpaces[space_name] += "" + formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n";
          }
          m_radSpaces[space_name] += "\n";

        }
      } // end shading surfaces


      //get the interior partition surfaces

      std::vector<openstudio::model::InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroups = space.interiorPartitionSurfaceGroups();
      for (const auto & interiorPartitionSurfaceGroup : interiorPartitionSurfaceGroups)
      {
        std::vector<openstudio::model::InteriorPartitionSurface> interiorPartitionSurfaces = interiorPartitionSurfaceGroup.interiorPartitionSurfaces();
        for (const auto & interiorPartitionSurface : interiorPartitionSurfaces)
        {

          // get nice name

          std::string interiorPartitionSurface_name = cleanName(interiorPartitionSurface.name().get());

          // check for construction

          boost::optional<model::ConstructionBase> construction = interiorPartitionSurface.construction();
          if (!construction){
            LOG(Warn, "InteriorPartitionSurface " << interiorPartitionSurface.name().get() << " is not associated with a Construction, it will not be translated.");
            continue;
          }

          // add surface to zone geometry

          m_radSpaces[space_name] += "# surface: " + interiorPartitionSurface_name + "\n";

          // set construction of interiorPartitionSurface
          std::string constructionName = interiorPartitionSurface.getString(1).get();
          m_radSpaces[space_name] += "# construction: " + constructionName + "\n";

         // get reflectance
          double interiorVisibleReflectance = 0.5; // set some default
          if (interiorPartitionSurface.interiorVisibleAbsorptance()){
            double interiorVisibleAbsorptance = interiorPartitionSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }

          double exteriorVisibleReflectance = 0.5; // set some default
          if (interiorPartitionSurface.exteriorVisibleAbsorptance()){
            double exteriorVisibleAbsorptance = interiorPartitionSurface.exteriorVisibleAbsorptance().get();
            exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
          }

          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance, 3) + "\n0\n0\n5\n" + \
            formatString(interiorVisibleReflectance, 3) + " " + \
            formatString(interiorVisibleReflectance, 3) + " " + \
            formatString(interiorVisibleReflectance, 3) + " 0 0\n\n");
          // polygon header
          m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance, 3) + "\n";
          m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
          // get / write surface polygon

          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(interiorPartitionSurface);
          m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance, 3) + " polygon " + \
          interiorPartitionSurface_name + "\n0\n0\n" + formatString(polygon.size() * 3) + "\n";
          for (const auto & vertex : polygon)
          {
            m_radSpaces[space_name] += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n\n";
          }
        }
      } // end interior partitions

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
      for (const auto & control : daylightingControls)
      {

        m_radSensors[space_name] = "";

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(control);
        openstudio::Vector3d sensor_aimVector = openstudio::radiance::ForwardTranslator::getSensorVector(control);
        m_radSensors[space_name] += \
        formatString(sensor_point.x(), 3) + " " + \
        formatString(sensor_point.y(), 3) + " " + \
        formatString(sensor_point.z(), 3) + " " + \
        formatString(sensor_aimVector.x(), 3) + " " + \
        formatString(sensor_aimVector.y(), 3) + " " + \
        formatString(sensor_aimVector.z(), 3) + "\n";

        // write daylighting controls
        openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(space_name + ".sns");
        OFSTREAM file(filename);
        if (file.is_open()){
          t_outfiles.push_back(filename);
          file << m_radSensors[space_name];
        } else{
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        // write daylighting control view file
        m_radSensorViews[space_name] = "";
        m_radSensorViews[space_name] += \
        "rvu -vth -vp " + formatString(sensor_point.x(), 3) + " " + formatString(sensor_point.y(), 3) + " " + \
        formatString(sensor_point.z(), 3) + " -vd " + formatString(sensor_aimVector.x(), 3) + " " + formatString(sensor_aimVector.y(), 3) + " " + \
        formatString(sensor_aimVector.z(), 3) + " -vu 0 1 0 -vh 180 -vv 180 -vo 0 -vs 0 -vl 0\n";

        filename = t_radDir / openstudio::toPath("views") / openstudio::toPath(space_name + ".cvf");
        OFSTREAM file2(filename);
        if (file2.is_open()){
          t_outfiles.push_back(filename);
          file2 << m_radSensorViews[space_name];
        } else{
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << ".cvf");

      } // end daylighting controls


      // get glare sensor
      std::vector<openstudio::model::GlareSensor> glareSensors = space.glareSensors();
      for (const auto & sensor : glareSensors)
      {
        m_radGlareSensors[space_name] = "";

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(sensor);
        // openstudio::Vector3dVector sensor_viewVector = openstudio::radiance::ForwardTranslator::getViewVectors(*sensor);
        openstudio::Vector3dVector viewVectors = openstudio::radiance::ForwardTranslator::getViewVectors(sensor);
        for (const Vector3d& viewVector : viewVectors){
          m_radGlareSensors[space_name] += \
          formatString(sensor_point.x(), 3) + " " + \
          formatString(sensor_point.y(), 3) + " " + \
          formatString(sensor_point.z(), 3) + " " + \
          formatString(viewVector.x(), 3) + " " + \
          formatString(viewVector.y(), 3) + " " + \
          formatString(viewVector.z(), 3) + "\n";
        }

        // write glare sensor
        openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(space_name + ".glr");
        OFSTREAM file(filename);
        if (file.is_open()){
          t_outfiles.push_back(filename);
          file << m_radGlareSensors[space_name];
        } else{
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << ".glr");
      } // end glare sensor


      // get illuminance map points, write to file
      std::vector<openstudio::model::IlluminanceMap> illuminanceMaps = space.illuminanceMaps();
      for (const auto & map : illuminanceMaps)
      {
        m_radMaps[space_name] = "";
        m_radMapHandles[space_name] = map.handle();

        // write map file
        openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(space_name + ".map");
        OFSTREAM file(filename);
        if (file.is_open()){
          t_outfiles.push_back(filename);

          std::vector<Point3d> referencePoints = openstudio::radiance::ForwardTranslator::getReferencePoints(map);
          for (const auto & point : referencePoints)
          {
            m_radMaps[space_name] += "" + formatString(point.x(), 3) + " " + formatString(point.y(), 3) + " " + formatString(point.z(), 3) + " 0.000 0.000 1.000\n";
          }
          file << m_radMaps[space_name];
        } else{
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "wrote " << space_name << ".map");
      } //end illuminance map


      // write geometry
      openstudio::path filename = t_radDir / openstudio::toPath("scene") / openstudio::toPath(space_name + ".rad");
      OFSTREAM file(filename);
      if (file.is_open()){
        t_outfiles.push_back(filename);
        m_radSceneFiles.push_back(filename);
        file << m_radSpaces[space_name];
      } else{
        LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
      }

      for (const auto & windowGroup : m_windowGroups)
      {
        std::string windowGroup_name = windowGroup.name();

        //write windows (and glazed doors)
        if (m_radWindowGroups.find(windowGroup_name) != m_radWindowGroups.end())
        {
          openstudio::path glazefilename = t_radDir / openstudio::toPath("scene/glazing") / openstudio::toPath(windowGroup_name + ".rad");
          OFSTREAM glazefile(glazefilename);
          if (glazefile.is_open()){
            t_outfiles.push_back(glazefilename);
            m_radSceneFiles.push_back(glazefilename);
            glazefile << m_radWindowGroups[windowGroup_name];
          } else{
            LOG(Error, "Cannot open file '" << toString(glazefilename) << "' for writing");
          }

          if(windowGroup_name != "WG0"){
            openstudio::path shadefilename = t_radDir / openstudio::toPath("scene/shades") / openstudio::toPath(windowGroup_name + "_SHADE.rad");
            OFSTREAM shadefile(shadefilename);
            if (shadefile.is_open()){
              t_outfiles.push_back(shadefilename);
              m_radSceneFiles.push_back(shadefilename);
              shadefile << m_radWindowGroupShades[windowGroup_name];
            } else{
              LOG(Error, "Cannot open file '" << toString(shadefilename) << "' for writing");
            }
          }

          // write window group control points
          // only write for controlled window groups
          if(windowGroup_name != "WG0"){
            openstudio::path filename = t_radDir / openstudio::toPath("numeric") / openstudio::toPath(windowGroup_name + ".pts");
            OFSTREAM file(filename);
            if (file.is_open()){
              t_outfiles.push_back(filename);
              file << windowGroup.windowGroupPoints();
            } else{
              LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
            }
          }
        }
      }

      // write radiance materials file
      m_radMaterials.insert("# OpenStudio Materials File\n\n");
      openstudio::path materialsfilename = t_radDir / openstudio::toPath("materials/materials.rad");
      OFSTREAM materialsfile(materialsfilename);
      if (materialsfile.is_open()){
        t_outfiles.push_back(materialsfilename);
        for (const auto & line : m_radMaterials)
        {
          materialsfile << line;
        };
        for (const auto & line : m_radMixMaterials)
        {
          materialsfile << line;
        };
      } else{
        LOG(Error, "Cannot open file '" << toString(materialsfilename) << "' for writing");
      }


      // write radiance DC vmx materials (lights) file
      m_radMaterialsDC.insert("# OpenStudio \"vmx\" Materials File\n# controlled windows: material=\"light\", black out all others.\n\nvoid plastic WG0\n0\n0\n5\n0 0 0 0 0\n\n");
      openstudio::path materials_vmxfilename = t_radDir / openstudio::toPath("materials/materials_vmx.rad");
      OFSTREAM materials_vmxfile(materials_vmxfilename);
      if (materials_vmxfile.is_open()){
        t_outfiles.push_back(materials_vmxfilename);
        for (const auto & line : m_radMaterialsDC)
        {
          materials_vmxfile << line;
        };
      } else{
        LOG(Error, "Cannot open file '" << toString(materials_vmxfilename) << "' for writing");
      }


      // write radiance WG0 vmx materials file (blacks out controlled window groups)
      m_radMaterialsWG0.insert("# OpenStudio \"WG0\" Materials File\n# black out all controlled window groups.\n");
      openstudio::path materials_WG0filename = t_radDir / openstudio::toPath("materials/materials_WG0.rad");
      OFSTREAM materials_WG0file(materials_WG0filename);
      if (materials_WG0file.is_open()){
        t_outfiles.push_back(materials_WG0filename);
        for (const auto & line : m_radMaterialsWG0)
        {
          materials_WG0file << line;
        };
      } else{
        LOG(Error, "Cannot open file '" << toString(materials_WG0filename) << "' for writing");
      }


      // write radiance vmx materials list
      // format of this file is: window group, bsdf, bsdf
      m_radDCmats.insert("#OpenStudio windowGroup->BSDF \"Mapping\" File\n# windowGroup,inwardNormal,shade control option,shade control setpoint,etc...\n");
      openstudio::path materials_dcfilename = t_radDir / openstudio::toPath("bsdf/mapping.rad");
      OFSTREAM materials_dcfile(materials_dcfilename);
      if (materials_dcfile.is_open()){
        t_outfiles.push_back(materials_dcfilename);
        for (const auto & line : m_radDCmats)
        {
          materials_dcfile << line;
        };
      } else{
        LOG(Error, "Cannot open file '" << toString(materials_dcfilename) << "' for writing");
      }


      // write complete scene
      openstudio::path modelfilename = t_radDir / openstudio::toPath("model.rad");
      OFSTREAM modelfile(modelfilename);

      if (modelfile.is_open()){
        t_outfiles.push_back(modelfilename);

        std::set<openstudio::path> uniquePaths(m_radSceneFiles.begin(), m_radSceneFiles.end());

        for (const auto & filename : uniquePaths)
        {
          modelfile << "!xform ./" << openstudio::toString(openstudio::relativePath(filename, t_radDir)) << std::endl;
        }
      } else{
        LOG(Error, "Cannot open file '" << toString(modelfilename) << "' for writing");
      }
    }
  }

  boost::optional<openstudio::path> ForwardTranslator::getBSDF(double vlt, double vltSpecular, const std::string& shadeType)
  {
    std::string searchTerm = "BSDF";
    unsigned tid = 1316; // "Construction Assembly.Fenestration.Window";

    boost::optional<std::string> result;

    openstudio::LocalBCL& localBCL = openstudio::LocalBCL::instance();
    result = getBSDF(localBCL, vlt, vltSpecular, shadeType, searchTerm, tid);
    if (result){
      return toPath(*result);
    }

    openstudio::RemoteBCL remoteBCL;
    //remoteBCL.useRemoteDevelopmentUrl(); // TODO: remove
    result = getBSDF(remoteBCL, vlt, vltSpecular, shadeType, searchTerm, tid);
    if (result){
      return toPath(*result);
    }

    return boost::none;
  }

  boost::optional<std::string> ForwardTranslator::getBSDF(openstudio::LocalBCL& bcl, double vlt, double vltSpecular, const std::string& shadeType, const std::string& searchTerm, unsigned tid)
  {
    std::vector<BCLComponent> results = bcl.searchComponents(searchTerm, tid);
    for (const BCLComponent& result : results){

      try{

        if (result.files("xml").empty()){
          continue;
        }

        double vltDiff = std::numeric_limits<double>::max();
        double vltSpecularDiff = std::numeric_limits<double>::max();
        bool shadeTypeMatch = false;
        for (const Attribute& attribute : result.attributes()){
          std::string attributeName = attribute.name();
          if (istringEqual(attributeName, "Visible Light Transmittance")){
            vltDiff = std::abs(attribute.valueAsDouble() - vlt);
          } else if (istringEqual(attributeName, "Visible Light Transmittance Specular Percentage")) {
            vltSpecularDiff = std::abs(attribute.valueAsDouble() - vltSpecular);
          } else if (istringEqual(attributeName, "Interior Shade Layer Type")) {
            shadeTypeMatch = istringEqual(attribute.valueAsString(), shadeType);
          }
        }

        // check if meets criteria
        if (vltDiff <= 0.01 && vltSpecularDiff <= 0.01 && shadeTypeMatch){
          // this one works try to get payload
          std::vector<std::string> files = result.files("xml");
          if (files.size() > 0){
            return files[0];
          }
        }

      } catch (const std::exception&) {
        // not the right one
      }
    }

    return boost::none;
  }

  boost::optional<std::string> ForwardTranslator::getBSDF(openstudio::RemoteBCL& bcl, double vlt, double vltSpecular, const std::string& shadeType, const std::string& searchTerm, unsigned tid)
  {
    boost::optional<BCLMetaSearchResult> metaResult = bcl.metaSearchComponentLibrary(searchTerm, tid);
    if (metaResult){
      unsigned numResults = metaResult->numResults();
      unsigned resultsPerQuery = bcl.resultsPerQuery();
      unsigned numPages = numResults / resultsPerQuery;
      if ((numResults % resultsPerQuery) > 0){
        ++numPages;
      }

      for (unsigned page = 0; page < numPages; ++page){
        std::vector<BCLSearchResult> results = bcl.searchComponentLibrary(searchTerm, tid, page);
        for (const BCLSearchResult& result : results){

          try{

            bool hasXML = false;
            for (BCLFile file : result.files()){
              if (file.filetype() == "xml"){
                hasXML = true;
              }
            }
            if (!hasXML){
              continue;
            }

            double vltDiff = std::numeric_limits<double>::max();
            double vltSpecularDiff = std::numeric_limits<double>::max();
            bool shadeTypeMatch = false;
            for (const Attribute& attribute : result.attributes()){
              std::string attributeName = attribute.name();
              if (istringEqual(attributeName, "Visible Light Transmittance")){
                vltDiff = std::abs(attribute.valueAsDouble() - vlt);
              } else if (istringEqual(attributeName, "Visible Light Transmittance Specular Percentage")) {
                vltSpecularDiff = std::abs(attribute.valueAsDouble() - vltSpecular);
              } else if (istringEqual(attributeName, "Interior Shade Layer Type")) {
                shadeTypeMatch = istringEqual(attribute.valueAsString(), shadeType);
              }
            }

            // check if meets criteria
            if (vltDiff <= 0.01 && vltSpecularDiff <= 1 && shadeTypeMatch){
              // this one works try to get payload
              boost::optional<BCLComponent> component = bcl.getComponent(result.uid());
              if (component){
                std::vector<std::string> files = component->files("xml");
                if (files.size() > 0){
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

  std::string cleanName(const std::string& name)
  {
    std::string result = boost::algorithm::replace_all_regex_copy(name, boost::regex("[\\.\\s:]"),  std::string("_"));
    return result;
  }

} // radiance
} // openstudio
