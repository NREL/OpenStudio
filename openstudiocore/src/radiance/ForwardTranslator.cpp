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

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/geometry/Transformation.hpp"

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
    ss << std::setprecision(t_prec) << std::showpoint << t_d;
    std::string s = ss.str();

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
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.radiance\\.ForwardTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  std::vector<openstudio::path> ForwardTranslator::translateModel(const openstudio::path& outPath, const openstudio::model::Model& model)
  {
    m_model = model.clone(true).cast<openstudio::model::Model>();

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    clear();

    // TODO: check for shading controls, set m_shadingControls

    std::vector<openstudio::path> outfiles;

    // preprocess the model here
    unsigned numSpacesToSimulate = 0;
    for (openstudio::model::Space space : model.getConcreteModelObjects<openstudio::model::Space>()){
      
      // remove any space's not associated with a thermal zone
      boost::optional<openstudio::model::ThermalZone> thermalZone = space.thermalZone();
      if (!thermalZone){
        LOG(Warn, "Space " << space.name().get() << " is not associated with a ThermalZone, it will not be translated.");
        space.remove();
        continue;
      }

      // only allow primary and secondary daylighting control points (as defined in thermal zone) through
      for (openstudio::model::DaylightingControl daylightingControl : space.daylightingControls()){
        if (daylightingControl.isPrimaryDaylightingControl()){
          // ok
        }else if (daylightingControl.isSecondaryDaylightingControl()){
          // is this ok? DaylightSim only seems to write out values for one DaylightingControl
        }else{
          LOG(Warn, "DaylightingControl " << daylightingControl.name().get() << " is not associated with this Space's ThermalZone, it will not be translated.");
          daylightingControl.remove();
        }
      }

      // only allow illuminance maps referenced by thermal zone through
      for (openstudio::model::IlluminanceMap illuminanceMap : space.illuminanceMaps()){
        boost::optional<openstudio::model::IlluminanceMap> thermalZoneIlluminanceMap = thermalZone->illuminanceMap();
        if (thermalZoneIlluminanceMap && (thermalZoneIlluminanceMap->handle() == illuminanceMap.handle())){
          // ok
        }else{
          LOG(Warn, "IlluminanceMap " << illuminanceMap.name().get() << " is not associated with this Space's ThermalZone, it will not be translated.");
          illuminanceMap.remove();
        }
      }

      std::vector<openstudio::model::DaylightingControl> daylightingControls = space.daylightingControls();
      std::vector<openstudio::model::IlluminanceMap> illuminanceMaps = space.illuminanceMaps();
      std::vector<openstudio::model::GlareSensor> glareSensors = space.glareSensors();

      if (daylightingControls.size() > 0 && illuminanceMaps.size() > 0){
        numSpacesToSimulate += 1;

        if (glareSensors.empty()){
          LOG(Warn, "Space " << space.name().get() << " contains Radiance daylighting objects but no GlareSensor objects.");
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
      boost::filesystem::create_directory(radDir / openstudio::toPath("materials"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("numeric"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("views"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("options"));
      boost::filesystem::create_directory(radDir / openstudio::toPath("bsdf"));

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
        LOG(Warn, formatString(totalVectors) << " glazing orientations detected (can cause long simulation times if using daylight coefficient approach)");
      }

      //    std::string dcmatsString = dcmatsStringin.gsub(',', ' ');

      // get Radiance sim settings
      openstudio::model::RadianceParameters radianceParameters = m_model.getUniqueModelObject<openstudio::model::RadianceParameters>();
      
      // write daylightsim options to files
      //
      std::vector<openstudio::model::ShadingControl> shadingControls = model.getModelObjects<openstudio::model::ShadingControl>();
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

          // copy required bsdf files into place
          openstudio::path bsdfoutpath = radDir/ openstudio::toPath("bsdf");

          // TODO: find all the bsdfs associated with the model and copy them all over now

          boost::filesystem::copy_file(getSharedResourcesPath() / openstudio::toPath("radiance/Daylighting/clear_100.xml"), bsdfoutpath / openstudio::toPath("clear_100.xml"), boost::filesystem::copy_option::overwrite_if_exists);
          boost::filesystem::copy_file(getSharedResourcesPath() / openstudio::toPath("radiance/Daylighting/shade_020.xml"), bsdfoutpath / openstudio::toPath("shade_020.xml"), boost::filesystem::copy_option::overwrite_if_exists);

          /// \todo rpg777 do we need reference these files in the materials.rad file?
          //
          /// \todo rgp777 also, we only want to group windows if they share the same shadinggroup
        }
      }else{
        LOG(Error, "Cannot open file '" << toString(daylightsimoptpath) << "' for writing");
      }

      // write Radiance options to file(s)
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

      // Hi Qual options (illuminance maps)
      openstudio::path mapsoptpath = radDir / openstudio::toPath("options/maps.opt");
      OFSTREAM mapsopt(mapsoptpath);
      if (mapsopt.is_open()){
        outfiles.push_back(mapsoptpath);
        mapsopt << "-ab 10 -ad 8000 -as 50 -dt 0 -dc 1 -ds 0.05 -lw 0.00001";
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
    //jgs20100615 added
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

    // convert vertices to absolute coordinates
    return buildingTransformation*spaceTransformation*subSurface.vertices();
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
    m_radMaterialsDC.clear();

    m_radDCmats.clear();
    
    m_radSceneFiles.clear();

    m_radSpaces.clear();
    m_radSensors.clear();
    m_radGlareSensors.clear();
    m_radMaps.clear();
    m_radMapHandles.clear();
    m_radViewPoints.clear();
    m_radWindowGroups.clear(); 
  }

  WindowGroup ForwardTranslator::getWindowGroup(double azimuth, const model::Space& space, const model::ConstructionBase& construction, 
    const boost::optional<model::ShadingControl>& shadingControl, const openstudio::Point3dVector& polygon)
  {
    WindowGroup result(azimuth, space, construction, shadingControl);

    std::vector<WindowGroup>::iterator it = std::find(m_windowGroups.begin(), m_windowGroups.end(), result);
    if (it != m_windowGroups.end()){
      it->addWindowPolygon(polygon);
      return *it;
    }

    result.addWindowPolygon(polygon);
    m_windowGroups.push_back(result);
    return result;
  }

  void ForwardTranslator::siteShadingSurfaceGroups(const openstudio::path &t_radDir,
      const std::vector<openstudio::model::ShadingSurfaceGroup> &radShadingSurfaceGroups,
      std::vector<openstudio::path> &t_outfiles)
  {
    LOG(Debug, "Site shading groups found: " << radShadingSurfaceGroups.size());

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

          LOG(Debug, "Site shading surface: " << shadingSurface_name );

         // get reflectance
          double interiorVisibleReflectance = 0.4; // default for site shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()){
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }
          
          double exteriorVisibleReflectance = 0.4; // default
          if (shadingSurface.exteriorVisibleAbsorptance()){
            double exteriorVisibleAbsorptance = shadingSurface.exteriorVisibleAbsorptance().get();
            exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
          }

          // write material
          m_radMaterials.insert("void plastic refl_"
              + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n"
              + formatString(interiorVisibleReflectance) + " "
              + formatString(interiorVisibleReflectance) + " "
              + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);

          std::string shadingsurface = "refl_" + formatString(interiorVisibleReflectance) + " polygon " + shadingSurface_name + "\n";
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
    LOG(Debug, "Building shading groups found: " << t_radShadingSurfaceGroups.size());

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

          LOG(Debug, "Building shading surface: " << shadingSurface_name);


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

          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(exteriorVisibleReflectance) + "\n0\n0\n5\n"
              + formatString(interiorVisibleReflectance) + " " + formatString(exteriorVisibleReflectance) + " "
              + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);

          std::string shadingsurface = "refl_" + formatString(exteriorVisibleReflectance) + " polygon " + shadingSurface_name + "\n";
          shadingsurface += "0\n0\n" + formatString(polygon.size()*3) + "\n";

          for (const auto & vertex : polygon)
          {
            shadingsurface += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) +"\n";
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
      m_radSpaces[space_name] = "#Space = " + space_name + "\n";

      // loop over surfaces in space

      std::vector<openstudio::model::Surface> surfaces = space.surfaces();

      for (const auto & surface : surfaces)
      {

        // skip if air wall
        if (surface.isAirWall()) continue;

        std::string surface_name = cleanName(surface.name().get());

        // add surface to space geometry
        m_radSpaces[space_name] += "#-Surface = " + surface_name + "\n";

        // set construction of surface
        std::string constructionName = surface.getString(2).get();
        m_radSpaces[space_name] += "#--constructionName = " + constructionName + "\n";

        // get reflectance
        double interiorVisibleReflectance = 0.5; // default for space surfaces
        if (surface.interiorVisibleAbsorptance()){
          double interiorVisibleAbsorptance = surface.interiorVisibleAbsorptance().get();
          interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
        }

        m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";

        // write material to library array
        /// \todo deal with exterior surfaces
        m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance)
            + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance)
            + " " + formatString(interiorVisibleReflectance)
            + " " + formatString(interiorVisibleReflectance) + " 0 0\n");

        // write surface polygon
        openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(surface);

        m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance)
          + " polygon " + surface_name + "\n0\n0\n" + formatString(polygon.size()*3) +"\n";

        for (const auto & vertex : polygon)
        {
          m_radSpaces[space_name] += formatString(vertex.x()) + " "
            + formatString(vertex.y()) + " "
            + formatString(vertex.z()) +"\n";
        }

        // get sub surfaces
        std::vector<openstudio::model::SubSurface> subSurfaces = surface.subSurfaces();

        for (const auto & subSurface : subSurfaces)
        {

          std::string rMaterial = "glass";
          std::string matString = "";

          boost::optional<model::ConstructionBase> construction = subSurface.construction();
          if (!construction){
            LOG(Warn, "SubSurface " << subSurface.name().get() << " is not associated with a Construction, it will not be translated.");
            continue;
          }

          if (!subSurface.visibleTransmittance())
          {
            LOG(Warn, "Cannot determine visible transmittance for SubSurface " << subSurface.name().get() << ", it will not be translated.");
            continue;
          }

          // get the polygon
          polygon = openstudio::radiance::ForwardTranslator::getPolygon(subSurface);

          boost::optional<model::ShadingControl> shadingControl = subSurface.shadingControl();

          // find window group
          // double azi = surface.azimuth() * (180 / PI());
          double azi = surface.azimuth();
          
          WindowGroup windowGroup = getWindowGroup(azi, space, *construction, shadingControl, polygon);
          std::string windowGroup_name = windowGroup.name();

          std::string subSurface_name = cleanName(subSurface.name().get());
        
          m_radSpaces[space_name] += "#--SubSurface = " + subSurface_name + "\n";

          std::string subSurfaceUpCase = boost::algorithm::to_upper_copy(subSurface.subSurfaceType());

          if (subSurfaceUpCase == "FIXEDWINDOW"
              || subSurfaceUpCase == "OPERABLEWINDOW"
              || subSurfaceUpCase == "GLASSDOOR"
              || subSurfaceUpCase == "SKYLIGHT")
          {

            if (m_radWindowGroups.find(windowGroup_name) == m_radWindowGroups.end())
            {
              m_radWindowGroups[windowGroup_name] = "#WindowGroup = " + windowGroup_name + "\n";
            }

            LOG(Info, "found a "+subSurface.subSurfaceType()+", azimuth = "+formatString(azi)+ "("+subSurface_name+")");

            // set transmittance...
            double visibleTransmittance = subSurface.visibleTransmittance().get();

            // convert transmittance (Tn) to transmissivity (tn) for Radiance material
            // tn = (sqrt(.8402528435+.0072522239*Tn*Tn)-.9166530661)/.0036261119/Tn
            // or: tn = 1.0895 * Tn (Thanks, Axel Jacobs! (http://www.jaloxa.eu/resources/radiance/documentation/docs/radiance_cookbook.pdf, p. 21))
            double tVis = visibleTransmittance;
            double tn = 0;
            if (tVis == 0.0) {
              LOG(Warn, subSurface_name << " has transmittance of zero.");
              tn = 0.0;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
            } else {
              // double tn_x = 0.0072522239 * tVis * tVis;
              // double tn_y = sqrt(tn_x + 0.8402528435) - 0.9166530661;
              // simplified thx to Axel...
              tn = tVis * 1.0895;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
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
              double transA7 = tS / (tVis+tS);
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
              matString = "0\n0\n7\n"+formatString(transA1, 4)+" "+formatString(transA2, 4)+" "+formatString(transA3, 4)+" "+formatString(transA4, 4)+" "+formatString(transA5, 4)+" "+formatString(transA6, 4)+" "+formatString(transA7, 4)+"\n";
              //double nTs = 0.0; // transmitted specularity

              if (tVis >= 0.6) {
                LOG( Warn, "dubious glazing material definition in "+space_name+"; Tvis ="+formatString(tVis, 4)+", yet diffuse? Suspect.");  
              }
              
            } else {

              matString = "0\n0\n3\n "+formatString(tn, 4)+" "+formatString(tn, 4)+" "+formatString(tn, 4)+"\n";
              //double nTs = 1.0; // transmitted specularity
            }

            m_radWindowGroups[windowGroup_name] += "# Tvis = " + formatString(tVis) + " (tn = "+ formatString(tn) + ")\n";
            // write material
            m_radMaterials.insert("void "+rMaterial+" glaz_"+rMaterial+"_"+space_name+"_azi-"+formatString(azi, 4)+"_tn-"+formatString(tn, 4)+" "+matString+"");
            m_radMaterialsDC.insert("void light glaz_light_"+space_name+"_azi-"+formatString(azi, 4)+"_tn-"+formatString(tn, 4)+"\n0\n0\n3\n1 1 1\n");
            // if shading control substitute real bsdf names for glazing.xml,glazing_blind.xml
            if (shadingControl){
              m_radDCmats.insert("glaz_"+rMaterial+"_"+space_name+"_azi-"+formatString(azi, 4)+"_tn-"+formatString(tn, 4)+ ".vmx,glazing.xml,glazing_blind.xml,glaz_" + space_name + "_azi-" + formatString(azi, 4) + "_tn-" + formatString(tn, 4) + ".dmx,\n");
            }else{
              m_radDCmats.insert("glaz_"+rMaterial+"_"+space_name+"_azi-"+formatString(azi, 4)+"_tn-"+formatString(tn, 4)+ ".vmx,glazing.xml,glazing_blind.xml,glaz_" + space_name + "_azi-" + formatString(azi, 4) + "_tn-" + formatString(tn, 4) + ".dmx,\n");
            }
            // polygon header
            m_radWindowGroups[windowGroup_name] += "#--SubSurface = " + subSurface_name + "\n";
            m_radWindowGroups[windowGroup_name] += "#---Tvis = " + formatString(tVis, 4) + " (tn = " + formatString(tn, 4) + ")\n";
            // write the polygon
            m_radWindowGroups[windowGroup_name] += "glaz_"+rMaterial+"_"+space_name + "_azi-" + formatString(azi, 4) + "_tn-" + formatString(tn, 4) + " polygon " + subSurface_name + "\n";
            m_radWindowGroups[windowGroup_name] += "0\n0\n" + formatString(polygon.size()*3) + "\n";

            for (Point3dVector::const_reverse_iterator vertex = polygon.rbegin();
                vertex != polygon.rend();
                ++vertex)
            {
              m_radWindowGroups[windowGroup_name] += "" + formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n";
            }

          } else if (subSurfaceUpCase == "DOOR") {

            LOG(Info, "found a door, will set to interior reflectance");

            double interiorVisibleAbsorptance = subSurface.interiorVisibleAbsorptance().get();
            double exteriorVisibleAbsorptance = subSurface.exteriorVisibleAbsorptance().get();
            double interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
            double exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;
            //polygon header
            m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";
            m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
            // write material
            m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " 0 0\n\n");
            // write polygon
            m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance) + " polygon " + subSurface_name + "\n";
            m_radSpaces[space_name] += "0\n0\n" + formatString(polygon.size()*3) + "\n";

            for (const auto & vertex : polygon)
            {
              m_radSpaces[space_name] += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n\n";
            }

          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDOME") {
            LOG(Warn, "subsurface is a tdd dome, not translated (not yet implemented).");
          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDIFFUSER") {
            LOG(Warn, "subsurface is a tdd diffuser, not translated (not yet implemented).");
          }
        }
      } // loop over surfaces

      // get shading surfaces

      std::vector<openstudio::model::ShadingSurfaceGroup> shadingSurfaceGroups = space.shadingSurfaceGroups();
      for (const auto & shadingSurfaceGroup : shadingSurfaceGroups)
      {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup.shadingSurfaces();
        for (const auto & shadingSurface : shadingSurfaces)
        {
          std::string shadingSurface_name = cleanName(shadingSurface.name().get());

          // add surface to zone geometry
          m_radSpaces[space_name] += "#-Surface = " + shadingSurface_name + "\n";

          // set construction of shadingSurface
          std::string constructionName = shadingSurface.getString(1).get();
          m_radSpaces[space_name] += "#--constructionName = " + constructionName + "\n";

          // get reflectance
          double interiorVisibleReflectance = 0.25; // default for space shading surfaces
          if (shadingSurface.interiorVisibleAbsorptance()){
            double interiorVisibleAbsorptance = shadingSurface.interiorVisibleAbsorptance().get();
            interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          }

          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";
          // get / write surface polygon
          //
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(shadingSurface);
          m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance) + " polygon " + shadingSurface_name + "\n0\n0\n" + formatString(polygon.size()*3) + "\n";

          for (const auto & vertex : polygon)
          {
            m_radSpaces[space_name] += "" + formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n";
          }

        }
      } // shading surfaces

      //get the interior partitions

      std::vector<openstudio::model::InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroups = space.interiorPartitionSurfaceGroups();
      for (const auto & interiorPartitionSurfaceGroup : interiorPartitionSurfaceGroups)
      {
        std::vector<openstudio::model::InteriorPartitionSurface> interiorPartitionSurfaces = interiorPartitionSurfaceGroup.interiorPartitionSurfaces();
        for (const auto & interiorPartitionSurface : interiorPartitionSurfaces)
        {
          std::string interiorPartitionSurface_name = cleanName(interiorPartitionSurface.name().get());

          // add surface to zone geometry
          m_radSpaces[space_name] += "#-Surface = " + interiorPartitionSurface_name + "\n";

          // set construction of interiorPartitionSurface
          std::string constructionName = interiorPartitionSurface.getString(1).get();
          m_radSpaces[space_name] += "#--constructionName = " + constructionName + "\n";

          // get reflectance
          double interiorVisibleAbsorptance = interiorPartitionSurface.interiorVisibleAbsorptance().get();
          double exteriorVisibleAbsorptance = interiorPartitionSurface.exteriorVisibleAbsorptance().get();
          double interiorVisibleReflectance = 1.0 - interiorVisibleAbsorptance;
          double exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorptance;

          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";
          m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
          // get / write surface polygon

          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(interiorPartitionSurface);
          m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance) + " polygon " + interiorPartitionSurface_name + "\n0\n0\n" + formatString(polygon.size()*3) + "\n";
          for (const auto & vertex : polygon)
          {
            m_radSpaces[space_name] += formatString(vertex.x()) + " " + formatString(vertex.y()) + " " + formatString(vertex.z()) + "\n\n";
          }
        }
      } //interior partitions

      // get luminaires
      ///  \todo fully implement once luminaires are fully supported in model
      //std::vector<openstudio::model::Luminaire> luminaires = space.luminaires();
      //for (const auto & luminaire : luminaires)
      //{
      //  luminaire_name = luminaire.name.get.gsub(' ', '_').gsub(':', '_')
      //  polygon = OpenStudio::Radiance::ForwardTranslator::getPolygon(luminaire)
      //}

      // get daylighting control points
      std::vector<openstudio::model::DaylightingControl> daylightingControls = space.daylightingControls();
      for (const auto & control : daylightingControls)
      {
        m_radSensors[space_name] = "";

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(control);
        openstudio::Vector3d sensor_aimVector = openstudio::radiance::ForwardTranslator::getSensorVector(control);
        m_radSensors[space_name] += formatString(sensor_point.x()) + " " + formatString(sensor_point.y()) + " " + formatString(sensor_point.z()) + " " + formatString(sensor_aimVector.x()) + " " + formatString(sensor_aimVector.y()) + " " + formatString(sensor_aimVector.z()) + "\n";
       
        // write daylighting controls
        openstudio::path filename = t_radDir/openstudio::toPath("numeric")/openstudio::toPath(space_name + ".sns");
        OFSTREAM file(filename);
        if (file.is_open()){
          t_outfiles.push_back(filename);
          file << m_radSensors[space_name];
        }else{
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << ".sns");
      }
       
      
      // get glare sensor
      std::vector<openstudio::model::GlareSensor> glareSensors = space.glareSensors();
      for (const auto & sensor : glareSensors)
      {
        m_radGlareSensors[space_name] = "";

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(sensor);
        // openstudio::Vector3dVector sensor_viewVector = openstudio::radiance::ForwardTranslator::getViewVectors(*sensor);
        openstudio::Vector3dVector viewVectors = openstudio::radiance::ForwardTranslator::getViewVectors(sensor);
        for (const Vector3d& viewVector : viewVectors){        
          m_radGlareSensors[space_name] += formatString(sensor_point.x()) + " " + formatString(sensor_point.y()) + " " + formatString(sensor_point.z()) + " " + formatString(viewVector.x()) + " " + formatString(viewVector.y()) + " " + formatString(viewVector.z()) + "\n";
        }

        // write glare sensor
        openstudio::path filename = t_radDir/openstudio::toPath("numeric")/openstudio::toPath(space_name + ".glr");
        OFSTREAM file(filename);
        if (file.is_open()){
          t_outfiles.push_back(filename);
          file << m_radGlareSensors[space_name];
        }else{
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "Wrote " << space_name << ".glr");
      }

      //{  
      //  m_radViews[space_name] = "";
      //
      //  openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(*viewpoints);
      //  openstudio::Vector3dVector sensor_viewVector = openstudio::radiance::ForwardTranslator::getViewVectors(*viewpoints);
      //  m_radViews[space_name] += "rvu -vta -vp " + formatString(sensor_point.x()) + " " + formatString(sensor_point.y()) + " " + formatString(sensor_point.z()) + " -vd " + formatString(sensor_viewVector[0].x()) + " " + formatString(sensor_viewVector[0].y()) + " " + formatString(sensor_viewVector[0].z()) + " -vu 0 0 1 -vh 180 -vv 180 -vo 0 -vs 0 -vl 0\n";
      //
      //  // write views
      //  openstudio::path filename = t_radDir/openstudio::toPath("views")/openstudio::toPath(space_name + ".vw");
      //  OFSTREAM file(filename);
      //  t_outfiles.push_back(filename);
      //  file << m_radViews[space_name];
      //
      //  LOG(Debug, "INFO: wrote " << space_name << ".vw");
      //}
      
      // get output illuminance map points, write to file
      std::vector<openstudio::model::IlluminanceMap> illuminanceMaps = space.illuminanceMaps();
      for (const auto & map : illuminanceMaps)
      {
        m_radMaps[space_name] = "";
        m_radMapHandles[space_name] = map.handle();

        // write map file
        openstudio::path filename = t_radDir/openstudio::toPath("numeric")/openstudio::toPath(space_name + ".map");
        OFSTREAM file(filename);
        if (file.is_open()){
          t_outfiles.push_back(filename);

          std::vector<Point3d> referencePoints = openstudio::radiance::ForwardTranslator::getReferencePoints(map);
          for (const auto & point : referencePoints)
          {
            m_radMaps[space_name] += "" + formatString(point.x()) + " " + formatString(point.y()) + " " + formatString(point.z()) + " 0 0 1\n";
          }
          file << m_radMaps[space_name];
        }else{
          LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
        }

        LOG(Debug, "wrote " << space_name << ".map");
      }

      // write geometry
      openstudio::path filename = t_radDir / openstudio::toPath("scene") / openstudio::toPath(space_name + "_geom.rad");
      OFSTREAM file(filename);
      if (file.is_open()){
        t_outfiles.push_back(filename);
        m_radSceneFiles.push_back(filename);
        file << m_radSpaces[space_name];
      }else{
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
          }else{
            LOG(Error, "Cannot open file '" << toString(glazefilename) << "' for writing");
          }

          // write window group control points
          openstudio::path filename = t_radDir/openstudio::toPath("numeric")/openstudio::toPath(windowGroup_name + ".pts");
          OFSTREAM file(filename);
          if (file.is_open()){
            t_outfiles.push_back(filename);
            file << windowGroup.windowGroupPoints();
          }else{
            LOG(Error, "Cannot open file '" << toString(filename) << "' for writing");
          }
        }
      }

      // write radiance materials file
      openstudio::path materialsfilename = t_radDir / openstudio::toPath("materials/materials.rad");
      OFSTREAM materialsfile(materialsfilename);
      if (materialsfile.is_open()){
        t_outfiles.push_back(materialsfilename);
        for (const auto & line : m_radMaterials)
        {
          materialsfile << line;
        };
      }else{
        LOG(Error, "Cannot open file '" << toString(materialsfilename) << "' for writing");
      }

      // write radiance DC vmx materials (lights) file
      openstudio::path materials_vmxfilename = t_radDir / openstudio::toPath("materials/materials_vmx.rad");
      OFSTREAM materials_vmxfile(materials_vmxfilename);
      if (materials_vmxfile.is_open()){
        t_outfiles.push_back(materials_vmxfilename);
        for (const auto & line : m_radMaterialsDC)
        {
          materials_vmxfile << line;
        };
      }else{
        LOG(Error, "Cannot open file '" << toString(materials_vmxfilename) << "' for writing");
      }

      // write radiance vmx materials list
      // DLM: format of this file is, approximately VMX, bsdf1, bsdf2, DMX
      openstudio::path materials_dcfilename = t_radDir / openstudio::toPath("bsdf/mapping.rad");
      OFSTREAM materials_dcfile(materials_dcfilename);
      if (materials_dcfile.is_open()){
        t_outfiles.push_back(materials_dcfilename);
        for (const auto & line : m_radDCmats)
        {
          materials_dcfile << line;
        };
      }else{
        LOG(Error, "Cannot open file '" << toString(materials_dcfilename) << "' for writing");
      }

      // write complete scene
      openstudio::path modelfilename = t_radDir / openstudio::toPath("model.rad");
      OFSTREAM modelfile(modelfilename);
      if (modelfile.is_open()){
        t_outfiles.push_back(modelfilename);

        // materials not included in model.rad (suport for 3-phase method)
        // modelfile << "!xform materials/materials.rad\n";

        for (const auto & filename : m_radSceneFiles)
        {
          modelfile << "!xform ./" << openstudio::toString(openstudio::relativePath(filename, t_radDir)) << std::endl;
        }
      }else{
        LOG(Error, "Cannot open file '" << toString(modelfilename) << "' for writing");
      }
    }
  }

  std::string cleanName(const std::string& name)
  {
    std::string result = boost::algorithm::replace_all_regex_copy(name, boost::regex("[\\.\\s:]"),  std::string("_"));
    return result;
  }

} // radiance
} // openstudio
