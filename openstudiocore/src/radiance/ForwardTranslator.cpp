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

#include <radiance/ForwardTranslator.hpp>

#include <model/DaylightingControl.hpp>
#include <model/GlareSensor.hpp>
#include <model/InteriorPartitionSurfaceGroup.hpp>
#include <model/Luminaire.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/RadianceParameters.hpp>
#include <model/RadianceParameters_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/ShadingSurface.hpp>
#include <model/ShadingSurface_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/ThermalZone.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/geometry/Transformation.hpp>

#include <QtGui/QPolygonF>
#include <QtCore/QPointF>
#include <QDir>
#include <QDateTime>
#include <QThread>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/filesystem.hpp>

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

namespace openstudio {
namespace radiance {

  // internal method used to format doubles as strings
  std::string formatString(double t_d, unsigned t_prec = 15)
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

    std::vector<openstudio::path> outfiles;

    // preprocess the model here
    unsigned numSpacesToSimulate = 0;
    BOOST_FOREACH(openstudio::model::Space space, model.getModelObjects<openstudio::model::Space>()){
      
      // remove any space's not associated with a thermal zone
      boost::optional<openstudio::model::ThermalZone> thermalZone = space.thermalZone();
      if (!thermalZone){
        LOG(Warn, "Space " << space.name().get() << " is not associated with a ThermalZone, it will not be translated.");
        space.remove();
        continue;
      }

      // only allow primary and secondary daylighting control points (as defined in thermal zone) through
      BOOST_FOREACH(openstudio::model::DaylightingControl daylightingControl, space.daylightingControls()){
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
      BOOST_FOREACH(openstudio::model::IlluminanceMap illuminanceMap, space.illuminanceMaps()){
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

      // do we really need both a daylighting control point and an illuminance map?  MakeSchedules seems to take
      // either one or the other?
      if (daylightingControls.size() > 0 && illuminanceMaps.size() > 0){
        numSpacesToSimulate += 1;

        if (glareSensors.empty()){
          LOG(Warn, "Space " << space.name().get() << " contains DaylightingControl and IlluminanceMap objects but no GlareSensor objects.");
        }
      }
    }

    if (numSpacesToSimulate == 0){
      // need better message?
      LOG(Error, "No spaces in model satisfy requirements for daylighting simulation using radiance.");
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

  //     aperture_headings.push_back("WG00");
  //     aperture_headings.push_back("WG01");
  //     aperture_headings.push_back("WG02");
  //     aperture_headings.push_back("WG03");
  //     aperture_headings.push_back("WG04");
  //     aperture_headings.push_back("WG05");
  //     aperture_headings.push_back("WG06");
  //     aperture_headings.push_back("WG07");
  //     aperture_headings.push_back("WG08");
  //     aperture_headings.push_back("WG09");
  //     aperture_headings.push_back("WG10");
  //     aperture_headings.push_back("WG11");
  //     aperture_headings.push_back("WG12");
  //     aperture_headings.push_back("WG13");
  //     aperture_headings.push_back("WG14");
  //     aperture_headings.push_back("WG15");
  //     aperture_headings.push_back("WG16");
  //     aperture_headings.push_back("WG17");
  //     aperture_headings.push_back("WG18");
  //     aperture_headings.push_back("WG19");
  //     aperture_headings.push_back("WG20");
  //     aperture_headings.push_back("WG21");
  //     aperture_headings.push_back("WG22");
  //     aperture_headings.push_back("WG23");
  //
      // get site and building shading
      LOG(Debug, "Processing site/building shading elements...");

      // get building shading
      buildingShadingSurfaceGroups(radDir, building.shadingSurfaceGroups(), outfiles);

      // get site shading
      siteShadingSurfaceGroups(radDir, site.shadingSurfaceGroups(), outfiles);

      // loop through the model, get spaces
      buildingSpaces(radDir, building.spaces(), outfiles);

      // write options files
      // todo generate options based on model
      //
      // get materials list for DC bins

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
      if (totalVectors > 5)
      {
        LOG(Warn, formatString(totalVectors) << " glazing orientations detected (can cause long simulation times if using daylight coefficient approach)");
      }

      //    std::string dcmatsString = dcmatsStringin.gsub(',', ' ');

      // get Radiance sim settings
      openstudio::model::RadianceParameters radianceParameters = m_model.getUniqueModelObject<openstudio::model::RadianceParameters>();

      // write Radiance options to file(s)
      // view matrix options
      openstudio::path vmxoptpath = radDir / openstudio::toPath("options/vmx.opt");
      outfiles.push_back(vmxoptpath);
      std::ofstream vmxopt(openstudio::toString(vmxoptpath).c_str());
      vmxopt << "-ab " << (int)radianceParameters.ambientBouncesVMX() << " " 
                << "-ad " << (int)radianceParameters.ambientDivisionsVMX() << " "
                << "-as " << (int)radianceParameters.ambientSupersamples() << " "
                // << "-c " << (int)radianceParameters.accumulatedRaysperRecord() << " "
                << "-dj " << radianceParameters.directJitter() << " "
                << "-dp " << radianceParameters.directPretest() << " "              
                << "-dt " << radianceParameters.directThreshold() << " "
                << "-dc " << radianceParameters.directCertainty() << " "
                << "-lw " << radianceParameters.limitWeightVMX() << " ";

      // daylight matrix options
      openstudio::path dmxoptpath = radDir / openstudio::toPath("options/dmx.opt");
      outfiles.push_back(dmxoptpath);
      std::ofstream dmxopt(openstudio::toString(dmxoptpath).c_str());
      dmxopt << "-ab " << (int)radianceParameters.ambientBouncesDMX() << " " 
                << "-ad " << (int)radianceParameters.ambientDivisionsDMX() << " "
                << "-as " << (int)radianceParameters.ambientSupersamples() << " "
                // << "-c " << (int)radianceParameters.accumulatedRaysperRecord() << " "
                << "-dj " << radianceParameters.directJitter() << " "
                << "-dp " << radianceParameters.directPretest() << " "              
                << "-dt " << radianceParameters.directThreshold() << " "
                << "-dc " << radianceParameters.directCertainty() << " "
                << "-lw " << radianceParameters.limitWeightDMX() << " ";

      // Tregenza/Klems resolution options

      openstudio::path tregoptpath = radDir / openstudio::toPath("options/treg.opt");
      outfiles.push_back(tregoptpath);
      std::ofstream tregopt(openstudio::toString(tregoptpath).c_str());
      tregopt << "-c " << (int)radianceParameters.klemsSamplingDensity() << " ";

      if (radianceParameters.skyDiscretizationResolution() == "146"){
        tregopt << "-e MF:1 -f tregenza.cal -b tbin -bn Ntbins";
      } else if (radianceParameters.skyDiscretizationResolution() == "578"){
        tregopt << "-e MF:2 -f reinhart.cal -b rbin -bn Nrbins";
      } else if (radianceParameters.skyDiscretizationResolution() == "2306"){
        tregopt << "-e MF:4 -f reinhart.cal -b rbin -bn Nrbins";
      }
      // TODO: make these values into a pulldown choice, add support for out of bounds



      // Hi Qual options (illumimance maps)
      openstudio::path mapsoptpath = radDir / openstudio::toPath("options/maps.opt");
      outfiles.push_back(mapsoptpath);
      std::ofstream mapsopt(openstudio::toString(mapsoptpath).c_str());
      mapsopt << "-ab 10 -ad 8000 -as 50 -dt 0 -dc 1 -ds 0.05 -lw 0.00001";

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

    BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const
  {
    std::vector<LogMessage> result;

    BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
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
    BOOST_FOREACH(const Point3d& point, surfaceFaceVertices){
      if (abs(point.z()) > 0.001){
        LOG(Warn, "Surface point z not on plane, z =" << point.z());
      }
      outer << QPointF(point.x(),point.y());
    }

    BOOST_FOREACH(const SubSurface& subSurface, surface.subSurfaces()){
      Point3dVector subsurfaceFaceVertices = alignFace.inverse()*subSurface.vertices();
      QPolygonF inner;
      BOOST_FOREACH(const Point3d& point, subsurfaceFaceVertices){
        if (abs(point.z()) > 0.001){
          LOG(Warn, "Subsurface point z not on plane, z =" << point.z());
        }
        inner << QPointF(point.x(),point.y());
      }
      outer = outer.subtracted(inner);

    }

    BOOST_FOREACH(const QPointF& point, outer){
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

  void ForwardTranslator::siteShadingSurfaceGroups(const openstudio::path &t_radDir,
      const std::vector<openstudio::model::ShadingSurfaceGroup> &radShadingSurfaceGroups,
      std::vector<openstudio::path> &t_outfiles)
  {
    LOG(Debug, "Site shading groups found: " << radShadingSurfaceGroups.size());

    if (!radShadingSurfaceGroups.empty())
    {
      std::set<std::string> siteShadingSurfaces;

      for (std::vector<openstudio::model::ShadingSurfaceGroup>::const_iterator shadingSurfaceGroup = radShadingSurfaceGroups.begin();
          shadingSurfaceGroup != radShadingSurfaceGroups.end();
          ++shadingSurfaceGroup)
      {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup->shadingSurfaces();
        for (std::vector<openstudio::model::ShadingSurface>::const_iterator shadingSurface = shadingSurfaces.begin();
            shadingSurface != shadingSurfaces.end();
            ++shadingSurface)
        {
          // clean name
          std::string shadingSurface_name = boost::algorithm::replace_all_regex_copy(shadingSurface->name().get(),
              boost::regex("[ :]"),  std::string("_"));

          LOG(Debug, "Site shading surface: " << shadingSurface_name );
          // get reflectance
          // no constructions yet, Rvis = .2 (same as EnergyPlus default)
          double interiorVisibleReflectance = 0.2;
          // write material
          m_radMaterials.insert("void plastic refl_"
              + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n"
              + formatString(interiorVisibleReflectance) + " "
              + formatString(interiorVisibleReflectance) + " "
              + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*shadingSurface);

          std::string shadingsurface = "refl_0.2 polygon " + shadingSurface_name + "\n";
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
      t_outfiles.push_back(filename);
      m_radSceneFiles.push_back(filename);

      std::ofstream f(openstudio::toString(filename).c_str(), std::ios_base::out | std::ios_base::trunc);

      for (std::set<std::string>::const_iterator line = siteShadingSurfaces.begin();
          line != siteShadingSurfaces.end();
          ++line)
      {
        f << *line;
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

      for (std::vector<openstudio::model::ShadingSurfaceGroup>::const_iterator groupitr = t_radShadingSurfaceGroups.begin();
          groupitr != t_radShadingSurfaceGroups.end();
          ++groupitr)
      {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = groupitr->shadingSurfaces();

        for (std::vector<openstudio::model::ShadingSurface>::const_iterator shadingSurface = shadingSurfaces.begin();
            shadingSurface != shadingSurfaces.end();
            ++shadingSurface)
        {
          // clean name
          std::string shadingSurface_name = boost::algorithm::replace_all_regex_copy(shadingSurface->name().get(),
              boost::regex("[ :]"),  std::string("_"));

          LOG(Debug, "Building shading surface: " << shadingSurface_name);
          // get reflectance
          // no constructions yet, Rvis = .2 (same as EnergyPlus default)
          double interiorVisibleReflectance = 0.2;
          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n"
              + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " "
              + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*shadingSurface);

          std::string shadingsurface = "refl_0.2 polygon " + shadingSurface_name + "\n";
          shadingsurface += "0\n0\n" + formatString(polygon.size()*3) + "\n";

          for (Point3dVector::const_iterator vertex = polygon.begin();
              vertex != polygon.end();
              ++vertex)
          {
            shadingsurface += formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) +"\n";
          }

          buildingShadingSurfaces.insert(shadingsurface);

        }
      }

      openstudio::path filename = t_radDir / openstudio::toPath("scene/shading_building.rad");
      t_outfiles.push_back(filename);
      m_radSceneFiles.push_back(filename);

      std::ofstream f(openstudio::toString(filename).c_str(), std::ios_base::out | std::ios_base::trunc);

      for (std::set<std::string>::const_iterator line = buildingShadingSurfaces.begin();
          line != buildingShadingSurfaces.end();
          ++line)
      {
        f << *line;
      }
    }
  }

  void ForwardTranslator::buildingSpaces(const openstudio::path &t_radDir, const std::vector<openstudio::model::Space> &t_spaces,
      std::vector<openstudio::path> &t_outfiles)
  {
    std::vector<std::string> space_names;

    for (std::vector<openstudio::model::Space>::const_iterator space = t_spaces.begin();
        space != t_spaces.end();
        ++space)
    {
      std::string space_name = boost::algorithm::replace_all_regex_copy(space->name().get(),
          boost::regex("[ :]"),  std::string("_"));
      space_names.push_back(space_name);
      LOG(Debug, "Processing space: " << space_name);
      openstudio::Transformation space_transformation = space->transformation();

      // split model into zone-based Radiance .rad files
      m_radSpaces[space_name] = "#Space = " + space_name + "\n";

      // clear out apertures for space

      //  aperture_headings.each {|aperture_heading| m_radApertures[aperture_heading] = nil}
      m_radApertures.clear();

      // loop over surfaces in space

      std::vector<openstudio::model::Surface> surfaces = space->surfaces();

      for (std::vector<openstudio::model::Surface>::const_iterator surface = surfaces.begin();
          surface != surfaces.end();
          ++surface)
      {

        // skip if air wall
        if (surface->isAirWall()) continue;

        std::string surface_name = boost::algorithm::replace_all_regex_copy(surface->name().get(),
            boost::regex("[ :]"),  std::string("_"));

        // add surface to space geometry
        m_radSpaces[space_name] += "#-Surface = " + surface_name + "\n";

        // set construction of surface
        std::string constructionName = surface->getString(2).get();
        m_radSpaces[space_name] += "#--constructionName = " + constructionName + "\n";

        double interiorVisibleAbsorbtance = surface->interiorVisibleAbsorbtance().get();
        double exteriorVisibleAbsorbtance = surface->exteriorVisibleAbsorbtance().get();
        double interiorVisibleReflectance = 1.0 - interiorVisibleAbsorbtance;
        double exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorbtance;

        m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";
        m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";

        // write material to library array
        /// \todo deal with exterior surfaces
        m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance)
            + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance)
            + " " + formatString(interiorVisibleReflectance)
            + " " + formatString(interiorVisibleReflectance) + " 0 0\n");

        // write surface polygon
        openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*surface);

        m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance)
          + " polygon " + surface_name + "\n0\n0\n" + formatString(polygon.size()*3) +"\n";

        for (Point3dVector::const_iterator vertex = polygon.begin();
            vertex != polygon.end();
            ++vertex)
        {
          m_radSpaces[space_name] += formatString(vertex->x()) + " "
            + formatString(vertex->y()) + " "
            + formatString(vertex->z()) +"\n";
        }


        // figure out azimuth for window bins
        // 15 degree bins per IES LM-83, currently under review (2012.01.20)
        // changed to creating separate bins per heading (RPG 2012.02.21)
        // TODO: allow grouping for highly discretized elevations

        // trying to save some time here...
        //
        // double azi = surface->azimuth() * (180 / PI());
        double azi = surface->azimuth();

        // std::string aperture_heading = boost::lexical_cast<std::string>(azi);
        std::string aperture_heading = boost::lexical_cast<std::string>(azi);
          aperture_heading = formatString(azi, 4);

        if (std::find(aperture_headings.begin(),aperture_headings.end(),aperture_heading) == aperture_headings.end())
        {
          aperture_headings.push_back(aperture_heading);
        }

// keep for future feature (offer auto-binning to window groups for highly tesselated facades)
//
//         if (azi >= 352.50 && azi < 7.50)
//         {
//           aperture_heading = "WG00";
//         } else if (azi >= 7.50 && azi < 22.50) {
//           aperture_heading = "WG01";
//         } else if (azi >= 22.50 && azi < 37.50) {
//           aperture_heading = "WG02";
//         } else if (azi >= 37.50 && azi < 52.50) {
//           aperture_heading = "WG03";
//         } else if (azi >= 52.50 && azi < 67.50) {
//           aperture_heading = "WG04";
//         } else if (azi >= 67.50 && azi < 82.50) {
//           aperture_heading = "WG05";
//         } else if (azi >= 82.50 && azi < 97.50) {
//           aperture_heading = "WG06";
//         } else if (azi >= 97.50 && azi < 112.50) {
//           aperture_heading = "WG07";
//         } else if (azi >= 112.50 && azi < 127.50) {
//           aperture_heading = "WG08";
//         } else if (azi >= 127.50 && azi < 142.50) {
//           aperture_heading = "WG09";
//         } else if (azi >= 142.50 && azi < 157.50) {
//           aperture_heading = "WG10";
//         } else if (azi >= 157.51 && azi < 172.50) {
//           aperture_heading = "WG11";
//         } else if (azi >= 172.50 && azi < 187.50) {
//           aperture_heading = "WG12";
//         } else if (azi >= 187.50 && azi < 202.50) {
//           aperture_heading = "WG13";
//         } else if (azi >= 202.50 && azi < 217.50) {
//           aperture_heading = "WG14";
//         } else if (azi >= 217.51 && azi < 232.50) {
//           aperture_heading = "WG15";
//         } else if (azi >= 232.50 && azi < 247.50) {
//           aperture_heading = "WG16";
//         } else if (azi >= 247.50 && azi < 262.50) {
//           aperture_heading = "WG17";
//         } else if (azi >= 262.50 && azi < 277.50) {
//           aperture_heading = "WG18";
//         } else if (azi >= 277.50 && azi < 292.50) {
//           aperture_heading = "WG19";
//         } else if (azi >= 292.50 && azi < 307.50) {
//           aperture_heading = "WG20";
//         } else if (azi >= 207.50 && azi < 322.50) {
//           aperture_heading = "WG21";
//         } else if (azi >= 322.50 && azi < 337.50) {
//           aperture_heading = "WG22";
//         } else {
//           aperture_heading = "WG23";
//         }

        std::vector<openstudio::model::SubSurface> subSurfaces = surface->subSurfaces();

        for (std::vector<openstudio::model::SubSurface>::const_iterator subSurface = subSurfaces.begin();
            subSurface != subSurfaces.end();
            ++subSurface)
        {
          std::string subSurface_name = boost::algorithm::replace_all_regex_copy(subSurface->name().get(),
              boost::regex("[ :]"),  std::string("_"));

          m_radSpaces[space_name] += "#--SubSurface = " + subSurface_name + "\n";

          std::string subSurfaceUpCase = boost::algorithm::to_upper_copy(subSurface->subSurfaceType());

          if (subSurfaceUpCase == "FIXEDWINDOW"
              || subSurfaceUpCase == "OPERABLEWINDOW"
              || subSurfaceUpCase == "GLASSDOOR")
          {

            if (m_radApertures.find(aperture_heading) == m_radApertures.end())
            {
              m_radApertures[aperture_heading] = "#SpaceApertures = " + space_name + "_" + aperture_heading + "\n";
            }

            LOG(Info, "found a "+subSurface->subSurfaceType()+", azimuth = "+formatString(azi)+ "("+subSurface_name+")");

            if (!subSurface->visibleTransmittance())
            {
              LOG(Warn, "Cannot determine visible transmittance for SubSurface " << subSurface_name << ", it will not be translated.");
              continue;
            }

            double visibleTransmittance = subSurface->visibleTransmittance().get();

            // convert transmittance (Tn) to transmissivity (tn) for Radiance material
            // tn = (sqrt(.8402528435+.0072522239*Tn*Tn)-.9166530661)/.0036261119/Tn
            double tVis = visibleTransmittance;
            double tn = 0;
            if (tVis == 0.0) {
              LOG(Warn, subSurface_name << " has transmittance of zero.");
              tn = 0.0;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
            } else {
              double tn_x = 0.0072522239 * tVis * tVis;
              double tn_y = sqrt(tn_x + 0.8402528435) - 0.9166530661;
              tn = tn_y / 0.0036261119 /tVis;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
            }

            /// \todo add support for translucent materials
            m_radApertures[aperture_heading] += "#---Tvis = " + formatString(tVis) + " (tn = "+formatString(tn)+")\n";
            // write material
            m_radMaterials.insert("void glass glaz_"+space_name+"_azi-"+formatString(azi, 4)+"_tn-"+formatString(tn, 4)+"\n0\n0\n3\n"+formatString(tn, 4)+" "+formatString(tn, 4)+" "+ formatString(tn, 4) +"\n");
            m_radMaterialsDC.insert("void light glaz_spc-"+space_name+"_azi-"+formatString(azi, 4)+"_tn-"+formatString(tn, 4)+"\n0\n0\n3\n1 1 1\n");
            m_radDCmats.insert("glaz_"+space_name+"_azi-"+formatString(azi, 4)+"_tn-"+formatString(tn, 4)+ ".vmx,glazing.xml,glazing_blind.xml,glaz_" + space_name + "_azi-" + formatString(azi, 4) + "_tn-" + formatString(tn, 4) + ".dmx,\n");
            // polygon header
            m_radApertures[aperture_heading] += "#--SubSurface = " + subSurface_name + "\n";
            m_radApertures[aperture_heading] += "#---Tvis = " + formatString(tVis, 4) + " (tn = " + formatString(tn, 4) + ")\n";
            // get/write the polygon
            openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*subSurface);
            m_radApertures[aperture_heading] += "glaz_" + space_name + "_azi-" + formatString(azi, 4) + "_tn-" + formatString(tn, 4) + " polygon " + subSurface_name + "\n";
            m_radApertures[aperture_heading] += "0\n0\n" + formatString(polygon.size()*3) + "\n";

            for (Point3dVector::const_reverse_iterator vertex = polygon.rbegin();
                vertex != polygon.rend();
                ++vertex)
            {
              m_radApertures[aperture_heading] += "" + formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n";
            }

          } else if (subSurfaceUpCase == "DOOR") {

            LOG(Info, "found a door, using interior reflectance");

            double interiorVisibleAbsorbtance = subSurface->interiorVisibleAbsorbtance().get();
            double exteriorVisibleAbsorbtance = subSurface->exteriorVisibleAbsorbtance().get();
            double interiorVisibleReflectance = 1.0 - interiorVisibleAbsorbtance;
            double exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorbtance;
            //polygon header
            m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";
            m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
            // write material
            m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " 0 0\n\n");
            // get / write polygon

            openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*subSurface);
            m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance) + " polygon " + subSurface_name + "\n";
            m_radSpaces[space_name] += "0\n0\n" + formatString(polygon.size()*3) + "\n";

            for (Point3dVector::const_iterator vertex = polygon.begin();
                vertex != polygon.end();
                ++vertex)
            {
              m_radSpaces[space_name] += formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n\n";
            }

          } else if (subSurfaceUpCase == "SKYLIGHT") {
            /// \todo place skylights in their own file by space, separate from geometry
            double visibleTransmittance = subSurface->visibleTransmittance().get();
            // convert transmittance (Tn) to transmissivity (tn) for Radiance material
            // tn = (sqrt(.8402528435+.0072522239*Tn*Tn)-.9166530661)/.0036261119/Tn
            double tVis = visibleTransmittance;
            double tn = 0.0;
            if (tVis == 0.0)
            {
              LOG(Warn, subSurface_name << " has transmittance of zero.");
              tn = 0.0;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
            } else  {
              double tn_x = 0.0072522239 * tVis * tVis;
              double tn_y = sqrt(tn_x + 0.8402528435) - 0.9166530661;
              tn = tn_y / 0.0036261119 /tVis;
              LOG(Debug, "Tvis = " << tVis << " (tn = " << tn << ")");
            }

            LOG(Debug, "Found a skylight, (" + subSurface_name + "), using simple glass model (Tvis = " + formatString(tVis) + " (tn = " + formatString(tn) + ")");

            m_radSpaces[space_name] += "#---Tvis = " + formatString(tVis) + " (tn = " + formatString(tn) + ")\n";
            // write material
            m_radMaterials.insert("void glass glaz_skylight_" + space_name + "_" + formatString(tn) + "\n0\n0\n3\n" + formatString(tn) + " " + formatString(tn) + " " + formatString(tn) + "\n");
            m_radMaterialsDC.insert("void light glaz_skylight_" + space_name + "_vmx\n0\n0\n3\n1 1 1\nvoid alias glaz_skylight_" + space_name + "_" + formatString(tn) + " glaz_skylight_" + space_name + "_vmx\n");
            m_radDCmats.insert("glaz_skylight_" + space_name + ",");
            // polygon header
            m_radSpaces[space_name] += "#--SubSurface = " + subSurface_name + "\n";
            m_radSpaces[space_name] += "#---Tvis = " + formatString(tVis) + " (tn = " + formatString(tn) + ")\n";

            // get/write the polygon
            openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*subSurface);

            m_radSpaces[space_name] += "glaz_skylight_" + space_name + "_" + formatString(tn) + " polygon " + space_name + "_" + subSurface_name + "\n";
            m_radSpaces[space_name] += "0\n0\n" + formatString(polygon.size()*3) + "\n";
            for (Point3dVector::const_iterator vertex = polygon.begin();
                vertex != polygon.end();
                ++vertex)
            {
              m_radSpaces[space_name] += "" + formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n";
            }
          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDOME") {
            LOG(Warn, "subsurface is a tdd dome, not translated (not yet implemented).");
          } else if (subSurfaceUpCase == "TUBULARDAYLIGHTDIFFUSER") {
            LOG(Warn, "subsurface is a tdd diffuser, not translated (not yet implemented).");
          }
        }
      } // loop over surfaces

      // get shading surfaces
      /// \note no constructions yet, so surface properties are hard coded to 20% Rvis, 100% opaque

      std::vector<openstudio::model::ShadingSurfaceGroup> shadingSurfaceGroups = space->shadingSurfaceGroups();
      for (std::vector<openstudio::model::ShadingSurfaceGroup>::const_iterator shadingSurfaceGroup = shadingSurfaceGroups.begin();
          shadingSurfaceGroup != shadingSurfaceGroups.end();
          ++shadingSurfaceGroup)
      {
        std::vector<openstudio::model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup->shadingSurfaces();
        for (std::vector<openstudio::model::ShadingSurface>::const_iterator shadingSurface = shadingSurfaces.begin();
            shadingSurface != shadingSurfaces.end();
            ++shadingSurface)
        {
          std::string shadingSurface_name = boost::algorithm::replace_all_regex_copy(shadingSurface->name().get(),
              boost::regex("[ :]"),  std::string("_"));

          //puts "found a shading surface"
          // add surface to zone geometry
          m_radSpaces[space_name] += "#-Surface = " + shadingSurface_name + "\n";

          // set construction of shadingSurface
          std::string constructionName = shadingSurface->getString(1).get();
          m_radSpaces[space_name] += "#--constructionName = " + constructionName + "\n";

          // get reflectance
          // no constructions yet, Rvis = .2 (same as EnergyPlus default)
          double interiorVisibleReflectance = 0.2;
          double exteriorVisibleReflectance = 0.2;
          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";
          m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
          // get / write surface polygon
          //
          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*shadingSurface);
          m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance) + " polygon " + shadingSurface_name + "\n0\n0\n" + formatString(polygon.size()*3) + "\n";

          for (Point3dVector::const_iterator vertex = polygon.begin();
              vertex != polygon.end();
              ++vertex)
          {
            m_radSpaces[space_name] += "" + formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n\n";
          }

        }
      }

      std::vector<openstudio::model::InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroups = space->interiorPartitionSurfaceGroups();
      for (std::vector<openstudio::model::InteriorPartitionSurfaceGroup>::const_iterator interiorPartitionSurfaceGroup
          = interiorPartitionSurfaceGroups.begin();
          interiorPartitionSurfaceGroup != interiorPartitionSurfaceGroups.end();
          ++interiorPartitionSurfaceGroup)
      {
        std::vector<openstudio::model::InteriorPartitionSurface> interiorPartitionSurfaces = interiorPartitionSurfaceGroup->interiorPartitionSurfaces();
        for (std::vector<openstudio::model::InteriorPartitionSurface>::const_iterator interiorPartitionSurface = interiorPartitionSurfaces.begin();
            interiorPartitionSurface != interiorPartitionSurfaces.end();
            ++interiorPartitionSurface)
        {
          std::string interiorPartitionSurface_name = boost::algorithm::replace_all_regex_copy(interiorPartitionSurface->name().get(),
              boost::regex("[ :]"),  std::string("_"));

          // add surface to zone geometry
          m_radSpaces[space_name] += "#-Surface = " + interiorPartitionSurface_name + "\n";

          // set construction of interiorPartitionSurface
          std::string constructionName = interiorPartitionSurface->getString(1).get();
          m_radSpaces[space_name] += "#--constructionName = " + constructionName + "\n";

          // get reflectance
          double interiorVisibleAbsorbtance = interiorPartitionSurface->interiorVisibleAbsorbtance().get();
          double exteriorVisibleAbsorbtance = interiorPartitionSurface->exteriorVisibleAbsorbtance().get();
          double interiorVisibleReflectance = 1.0 - interiorVisibleAbsorbtance;
          double exteriorVisibleReflectance = 1.0 - exteriorVisibleAbsorbtance;

          // write material
          m_radMaterials.insert("void plastic refl_" + formatString(interiorVisibleReflectance) + "\n0\n0\n5\n" + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " " + formatString(interiorVisibleReflectance) + " 0 0\n\n");
          // polygon header
          m_radSpaces[space_name] += "#--interiorVisibleReflectance = " + formatString(interiorVisibleReflectance) + "\n";
          m_radSpaces[space_name] += "#--exteriorVisibleReflectance = " + formatString(exteriorVisibleReflectance) + "\n";
          // get / write surface polygon

          openstudio::Point3dVector polygon = openstudio::radiance::ForwardTranslator::getPolygon(*interiorPartitionSurface);
          m_radSpaces[space_name] += "refl_" + formatString(interiorVisibleReflectance) + " polygon " + interiorPartitionSurface_name + "\n0\n0\n" + formatString(polygon.size()*3) + "\n";
          for (Point3dVector::const_iterator vertex = polygon.begin();
              vertex != polygon.end();
              ++vertex)
          {
            m_radSpaces[space_name] += formatString(vertex->x()) + " " + formatString(vertex->y()) + " " + formatString(vertex->z()) + "\n\n";
          }
        }
      }

      // get luminaires
      ///  \todo fully implement once luminaires are fully supported in model
      std::vector<openstudio::model::Luminaire> luminaires = space->luminaires();
      for (std::vector<openstudio::model::Luminaire>::const_iterator luminaire = luminaires.begin();
          luminaire != luminaires.end();
          ++luminaire)
      {
        //    luminaire_name = luminaire.name.get.gsub(' ', '_').gsub(':', '_')
        //    polygon = OpenStudio::Radiance::ForwardTranslator::getPolygon(luminaire)
      }

      // get daylighting control points
      std::vector<openstudio::model::DaylightingControl> daylightingControls = space->daylightingControls();
      for (std::vector<openstudio::model::DaylightingControl>::const_iterator control = daylightingControls.begin();
          control != daylightingControls.end();
          ++control)
      {
        m_radSensors[space_name] = "";

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(*control);
        openstudio::Vector3d sensor_aimVector = openstudio::radiance::ForwardTranslator::getSensorVector(*control);
        m_radSensors[space_name] += formatString(sensor_point.x()) + " " + formatString(sensor_point.y()) + " " + formatString(sensor_point.z()) + " " + formatString(sensor_aimVector.x()) + " " + formatString(sensor_aimVector.y()) + " " + formatString(sensor_aimVector.z()) + "\n";
       
        // write daylighting controls
        openstudio::path filename = t_radDir/openstudio::toPath("numeric")/openstudio::toPath(space_name + ".sns");
        std::ofstream file(openstudio::toString(filename).c_str());
        t_outfiles.push_back(filename);
        file << m_radSensors[space_name];

        LOG(Debug, "INFO: wrote " << space_name << ".sns");
      }
       
      
      // get glare sensor
      std::vector<openstudio::model::GlareSensor> glareSensors = space->glareSensors();
      for (std::vector<openstudio::model::GlareSensor>::const_iterator sensor = glareSensors.begin();
          sensor != glareSensors.end();
          ++sensor)
      {
        m_radGlareSensors[space_name] = "";

        openstudio::Point3d sensor_point = openstudio::radiance::ForwardTranslator::getReferencePoint(*sensor);
        // openstudio::Vector3dVector sensor_viewVector = openstudio::radiance::ForwardTranslator::getViewVectors(*sensor);
        openstudio::Vector3dVector viewVectors = openstudio::radiance::ForwardTranslator::getViewVectors(*sensor);
        BOOST_FOREACH(const Vector3d& viewVector, viewVectors){        
        m_radGlareSensors[space_name] += formatString(sensor_point.x()) + " " + formatString(sensor_point.y()) + " " + formatString(sensor_point.z()) + " " + formatString(viewVector.x()) + " " + formatString(viewVector.y()) + " " + formatString(viewVector.z()) + "\n";
        }
        // write glare sensor
        openstudio::path filename = t_radDir/openstudio::toPath("numeric")/openstudio::toPath(space_name + ".glr");
        std::ofstream file(openstudio::toString(filename).c_str());
        t_outfiles.push_back(filename);
        file << m_radGlareSensors[space_name];

        LOG(Debug, "INFO: wrote " << space_name << ".glr");
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
      //  std::ofstream file(openstudio::toString(filename).c_str());
      //  t_outfiles.push_back(filename);
      //  file << m_radViews[space_name];
      //
      //  LOG(Debug, "INFO: wrote " << space_name << ".vw");
      //}
      
      // get output illuminance map points, write to file
      std::vector<openstudio::model::IlluminanceMap> illuminanceMaps = space->illuminanceMaps();
      for (std::vector<openstudio::model::IlluminanceMap>::const_iterator map = illuminanceMaps.begin();
          map != illuminanceMaps.end();
          ++map)
      {
        m_radMaps[space_name] = "";
        m_radMapHandles[space_name] = map->handle();

        std::vector<Point3d> referencePoints = openstudio::radiance::ForwardTranslator::getReferencePoints(*map);

        for (std::vector<Point3d>::const_iterator point = referencePoints.begin();
            point != referencePoints.end();
            ++point)
        {
          m_radMaps[space_name] += "" + formatString(point->x()) + " " + formatString(point->y()) + " " + formatString(point->z()) + " 0 0 1\n";
          // write map file
          openstudio::path filename = t_radDir/openstudio::toPath("numeric")/openstudio::toPath(space_name + ".map");
          std::ofstream file(openstudio::toString(filename).c_str());
          t_outfiles.push_back(filename);
          file << m_radMaps[space_name];
        }

        LOG(Debug, "wrote " << space_name << ".map");
      }

      // write geometry
      openstudio::path filename = t_radDir / openstudio::toPath("scene") / openstudio::toPath(space_name + "_geom.rad");
      t_outfiles.push_back(filename);
      m_radSceneFiles.push_back(filename);

      std::ofstream file(toString(filename).c_str());
      file << m_radSpaces[space_name];

      for (std::vector<std::string>::const_iterator aperture_heading = aperture_headings.begin();
          aperture_heading != aperture_headings.end();
          ++aperture_heading)
      {
        //write windows (and glazed doors)
        if (m_radApertures.find(*aperture_heading) != m_radApertures.end())
        {
          openstudio::path glazefilename = t_radDir / openstudio::toPath("scene/glazing") / openstudio::toPath(space_name + "_glaz_" + *aperture_heading + ".rad");
          m_radSceneFiles.push_back(glazefilename);
          std::ofstream glazefile(openstudio::toString(glazefilename).c_str());
          t_outfiles.push_back(glazefilename);
          glazefile << m_radApertures[*aperture_heading];
        }
      }

      // write radiance materials file
      openstudio::path materialsfilename = t_radDir / openstudio::toPath("materials/materials.rad");
      t_outfiles.push_back(materialsfilename);
      std::ofstream materialsfile(openstudio::toString(materialsfilename).c_str());

      for (std::set<std::string>::const_iterator line = m_radMaterials.begin();
          line != m_radMaterials.end();
          ++line)
      {
        materialsfile << *line;
      };

      // write radiance DC vmx materials (lights) file
      openstudio::path materials_vmxfilename = t_radDir / openstudio::toPath("materials/materials_vmx.rad");
      t_outfiles.push_back(materials_vmxfilename);
      std::ofstream materials_vmxfile(openstudio::toString(materials_vmxfilename).c_str());

      for (std::set<std::string>::const_iterator line = m_radMaterialsDC.begin();
          line != m_radMaterialsDC.end();
          ++line)
      {
        materials_vmxfile << *line;
      };

      // write radiance vmx materials list
      openstudio::path materials_dcfilename = t_radDir / openstudio::toPath("bsdf/mapping.rad");
      t_outfiles.push_back(materials_dcfilename);
      std::ofstream materials_dcfile(openstudio::toString(materials_dcfilename).c_str());

      for (std::set<std::string>::const_iterator line = m_radDCmats.begin();
          line != m_radDCmats.end();
          ++line)
      {
        materials_dcfile << *line;
      };


      // write complete scene
      openstudio::path modelfilename= t_radDir / openstudio::toPath("model.rad");
      t_outfiles.push_back(modelfilename);
      std::ofstream modelfile(openstudio::toString(modelfilename).c_str());

      // materials not included in model.rad (suport for 3-phase method)
      // modelfile << "!xform materials/materials.rad\n";

      for (std::vector<openstudio::path>::const_iterator filename = m_radSceneFiles.begin();
          filename != m_radSceneFiles.end();
          ++filename)
      {
        modelfile << "!xform ./" << openstudio::toString(openstudio::relativePath(*filename, t_radDir)) << std::endl;
      }

    }
  }
} // radiance
} // openstudio
