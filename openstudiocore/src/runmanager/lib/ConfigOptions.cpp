/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "ConfigOptions.hpp"
#include "ToolFinder.hpp"
#include "../../utilities/core/Application.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"

#include <OpenStudio.hxx>

#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QIcon>
#include <QMessageBox>
#include <QSettings>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif



namespace openstudio {
namespace runmanager {

  ToolLocationInfo::ToolLocationInfo(const ToolType &t_type,
      const openstudio::path &t_bindir)
   : toolType(t_type), binaryDir(t_bindir)
  {
  }

  bool ToolLocationInfo::operator<(const ToolLocationInfo &rhs) const
  {
    return (rhs.toolType < toolType)
        || (rhs.toolType == toolType && rhs.binaryDir < binaryDir);
  }

  bool ToolLocationInfo::operator==(const ToolLocationInfo &rhs) const
  {
    return rhs.toolType == toolType && rhs.binaryDir == binaryDir;
  }

  std::ostream &operator<<(std::ostream &os, const ToolLocationInfo &epi)
  {
    os << "(" << epi.toolType.valueDescription() << ", " << toString(epi.binaryDir) << ")";
    return os;
  }



  ConfigOptions::ConfigOptions(bool t_loadQSettings)
     : m_maxLocalJobs(std::max(1u, boost::thread::hardware_concurrency()-1)),
       m_simpleName(false)
  {
    if (t_loadQSettings)
    {
      std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
        tools = loadQSettingsTools();
      m_toolLocations.insert(tools.begin(), tools.end());
      loadQSettingsData();
    }
  }

  void ConfigOptions::setToolLocation(const ToolVersion &t_epv, const ToolLocationInfo &t_info)
  {
    m_toolLocations.insert(std::make_pair(t_epv, t_info));
  }

  std::vector<std::pair<ToolVersion, ToolLocationInfo> > ConfigOptions::getToolLocations() const
  {
    return std::vector<std::pair<ToolVersion, ToolLocationInfo> >(m_toolLocations.rbegin(), 
        m_toolLocations.rend());
  }

  openstudio::runmanager::Tools ConfigOptions::getTools() const
  {
    openstudio::runmanager::Tools ret;
    std::vector<std::pair<ToolVersion, ToolLocationInfo> > eps = ConfigOptions::getToolLocations();

    for (std::vector<std::pair<ToolVersion, ToolLocationInfo> >::const_iterator itr = eps.begin();
         itr != eps.end();
         ++itr)
    {
      ret.append(makeTools(itr->second.toolType, *itr));
    }

    return ret;
  }

  openstudio::runmanager::Tools ConfigOptions::makeTools(
      const openstudio::path &t_energyplus, 
      const openstudio::path &t_xmlpreproc, 
      const openstudio::path &t_radiance,
      const openstudio::path &t_ruby, 
      const openstudio::path &t_dakota)
  {
    Tools tools;

    if (!t_energyplus.empty())
    {
      tools.append(makeTools(ToolType::EnergyPlus, t_energyplus, ToolFinder::parseToolVersion(t_energyplus)));
    }

    if (!t_xmlpreproc.empty())
    {
      tools.append(makeTools(ToolType::XMLPreprocessor, t_xmlpreproc, ToolFinder::parseToolVersion(t_xmlpreproc)));
    }

    if (!t_radiance.empty())
    {
      tools.append(makeTools(ToolType::Radiance, t_radiance, ToolFinder::parseToolVersion(t_radiance)));
    }

    if (!t_ruby.empty())
    {
      ToolVersion tv(ToolFinder::parseToolVersion(t_ruby));
      if (tv.empty()) tv = ToolVersion(2,0);
      tools.append(makeTools(ToolType::Ruby, t_ruby, tv));
    }

    if (!t_dakota.empty())
    {
      tools.append(makeTools(ToolType::Dakota, t_dakota, ToolFinder::parseToolVersion(t_dakota)));
    }

    return tools;
  }

  openstudio::runmanager::Tools ConfigOptions::makeTools(
      const ToolType &t_type,
      const std::pair<ToolVersion, ToolLocationInfo> &tool)
  {
    Tools tools;

    switch(t_type.value())
    {
      case ToolType::EnergyPlus:
        tools.append(toExpandObjectsToolInfo(tool));
        tools.append(toEPlusToolInfo(tool));
        tools.append(toSlabToolInfo(tool));
        tools.append(toBasementToolInfo(tool));
        break;

      case ToolType::Radiance:
        tools.append(toIES2RadToolInfo(tool));
        tools.append(toRaImageToolInfo(tool));
        tools.append(toRadToolInfo(tool));
        tools.append(toRTraceToolInfo(tool));
        tools.append(toEpw2Wea(tool));
        break;

      case ToolType::XMLPreprocessor:
        tools.append(toXMLPreprocessorToolInfo(tool));
        break;

      case ToolType::Ruby:
        tools.append(toRubyToolInfo(tool));
        break;

      case ToolType::Dakota :
        tools.append(toDakotaToolInfo(tool));
        break;

      default:
        assert(!"Unhandled tool type");
        break;
    }

    return tools;

  }

  openstudio::runmanager::Tools ConfigOptions::makeTools(
          const ToolType &t_type,
          const openstudio::path &t_local,
          const ToolVersion &t_version)
  {
    return makeTools(t_type, std::make_pair(t_version, ToolLocationInfo(t_type, t_local)));
  }

  openstudio::runmanager::ToolInfo ConfigOptions::toRTraceToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &rtrace) 
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    return openstudio::runmanager::ToolInfo(
        "rtrace",
        rtrace.first,
        change_extension(rtrace.second.binaryDir / toPath("rtrace"), exeext),
        boost::regex(""));
  } 


  openstudio::runmanager::ToolInfo ConfigOptions::toExpandObjectsToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus) 
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    std::string exename = "ExpandObjects";

    if (eplus.first.getMajor() && eplus.first.getMajor() < 8)
    {
      exename = "expandobjects";
    }

    return openstudio::runmanager::ToolInfo(
        "expandobjects",
        eplus.first,
        change_extension(eplus.second.binaryDir / toPath(exename), exeext),
        boost::regex("expanded\\.idf"));
  } 

  openstudio::runmanager::ToolInfo ConfigOptions::toBasementToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus) 
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    std::string basename = "Basement";
    
    if (eplus.first.getMajor() && eplus.first.getMajor() < 8)
    {
      basename = "basename";
    }
    

    openstudio::path basementlocation = change_extension(eplus.second.binaryDir / toPath(basename), exeext);

    if (!boost::filesystem::exists(basementlocation))
    {
      basementlocation = change_extension(eplus.second.binaryDir / toPath("PreProcess/GrndTempCalc/basement"), exeext);
    }

    if (!boost::filesystem::exists(basementlocation))
    {
      basementlocation = change_extension(eplus.second.binaryDir / toPath("PreProcess/GrndTempCalc/Basement"), exeext);
    }


    return openstudio::runmanager::ToolInfo(
        "basement",
        eplus.first,
        basementlocation,
        boost::regex(".*\\.TXT"));
  } 

  openstudio::runmanager::ToolInfo ConfigOptions::toSlabToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus) 
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    std::string basename = "Slab";
    
    if (eplus.first.getMajor() && eplus.first.getMajor() < 8)
    {
      basename = "slab";
    }

    openstudio::path slablocation = change_extension(eplus.second.binaryDir / toPath(basename), exeext);

    if (!boost::filesystem::exists(slablocation))
    {
      slablocation = change_extension(eplus.second.binaryDir / toPath("PreProcess/GrndTempCalc/slab"), exeext);
    }

    if (!boost::filesystem::exists(slablocation))
    {
      slablocation = change_extension(eplus.second.binaryDir / toPath("PreProcess/GrndTempCalc/Slab"), exeext);
    }

    return openstudio::runmanager::ToolInfo(
        "slab",
        eplus.first,
        slablocation,
        boost::regex(".*\\.TXT"));
  } 

  openstudio::runmanager::ToolInfo 
    ConfigOptions::toXMLPreprocessorToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus) 
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    return openstudio::runmanager::ToolInfo(
        "xmlpreprocessor",
        eplus.first,
        change_extension(eplus.second.binaryDir / toPath("EPXMLPreproc2"), exeext),
        boost::regex("out_pp2\\.idf"));
  } 

  openstudio::runmanager::ToolInfo ConfigOptions::toEPlusToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus)
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    std::string exename = "EnergyPlus";

    if (eplus.first.getMajor() && eplus.first.getMajor() < 8)
    {
      exename = "energyplus";
    }
    else if (eplus.first.getMajor() && eplus.first.getMajor() == 8 && eplus.first.getMinor() && eplus.first.getMinor() >= 3)
    {
      exename = "energyplus";
    }
    else if (eplus.first.getMajor() && eplus.first.getMajor() > 8)
    {
      exename = "energyplus";
    }


    return openstudio::runmanager::ToolInfo(
        "energyplus",
        eplus.first,
        change_extension(eplus.second.binaryDir / toPath(exename), exeext),
        boost::regex("eplus.*"));
  }

  openstudio::runmanager::ToolInfo ConfigOptions::toIES2RadToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus)
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    return openstudio::runmanager::ToolInfo(
        "ies2rad",
        eplus.first,
        change_extension(eplus.second.binaryDir / toPath("ies2rad"), exeext),
        boost::regex(".*\\.(rad|dat)"));
  }

  openstudio::runmanager::ToolInfo ConfigOptions::toRaImageToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus)
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
    static const char exename[] = "ra_bmp";
#else
    static const char exeext[] = "";
    static const char exename[] = "ra_tiff";
#endif



    return openstudio::runmanager::ToolInfo(
        "ra_image",
        eplus.first,
        change_extension(eplus.second.binaryDir / toPath(exename), exeext),
        boost::regex(".*\\.(tiff|bmp)"));
  }

  openstudio::runmanager::ToolInfo ConfigOptions::toRadToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus)
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    return openstudio::runmanager::ToolInfo(
        "rad",
        eplus.first,
        change_extension(eplus.second.binaryDir / toPath("rad"), exeext),
        boost::regex(".*\\.hdr"));
  }

  openstudio::runmanager::ToolInfo ConfigOptions::toEpw2Wea(const std::pair<ToolVersion, ToolLocationInfo> &eplus)
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    return openstudio::runmanager::ToolInfo(
        "epw2wea",
        eplus.first,
        change_extension(eplus.second.binaryDir / toPath("epw2wea"), exeext),
        boost::regex(".*\\.wea"));
  }


  openstudio::runmanager::ToolInfo ConfigOptions::toRubyToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &ruby)
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

#ifdef Q_OS_LINUX
    // Also note that this code might ultimately cause us problems in the future, but should
    // be pretty robust for now
    std::string filename = "ruby";

    ToolVersion tv(ruby.first);

    if (tv.getMajor())
    {
      filename += boost::lexical_cast<std::string>(*tv.getMajor());

      if (tv.getMinor())
      {
        filename += "." + boost::lexical_cast<std::string>(*tv.getMinor());
      } else {
        filename += ".0";
      }

      // note that the extra '.' is required to keep the change_extension tool
      // from erasing the .<minorver>
      filename += ".";
      
    }

    openstudio::path toolPath = change_extension(ruby.second.binaryDir / toPath(filename), exeext);

    if (!exists(toolPath)){
      // DLM: check if 2.0 is in the ruby.second.binaryDir?
      // for rbenv this is /usr/local/rbenv/versions/2.0.0-p481/bin/
      filename = "ruby";
      toolPath = change_extension(ruby.second.binaryDir / toPath(filename), exeext);
    }

#else
    static const char filename[] = "ruby";

    openstudio::path toolPath = change_extension(ruby.second.binaryDir / toPath(filename), exeext);
#endif


    return openstudio::runmanager::ToolInfo(
        "ruby",
        ruby.first,
        toolPath,
        boost::regex(".*"));
  }

  openstudio::runmanager::ToolInfo ConfigOptions::toDakotaToolInfo(const std::pair<ToolVersion, ToolLocationInfo>& dakota) 
  {
#ifdef Q_OS_WIN
    static const wchar_t exeext[] = L".exe";
#else
    static const char exeext[] = "";
#endif

    return openstudio::runmanager::ToolInfo(
        "dakota",
        dakota.first,
        change_extension(dakota.second.binaryDir / toPath("dakota"), exeext),
        boost::regex(".*"));
  }


  void ConfigOptions::reset()
  {
    m_toolLocations.clear();
  }

  openstudio::path ConfigOptions::getDefaultIDFLocation() const
  {
    return m_defaultIDFLocation;
  }

  void ConfigOptions::setDefaultIDFLocation(const openstudio::path &t_loc)
  {
    m_defaultIDFLocation = t_loc;
  }

  openstudio::path ConfigOptions::getOutputLocation() const
  {
    return m_outputLocation;
  }

  void ConfigOptions::setOutputLocation(const openstudio::path &t_loc)
  {
    m_outputLocation = t_loc;
  }


  openstudio::path ConfigOptions::getDefaultEPWLocation() const
  {
    return m_defaultEPWLocation;
  }

  void ConfigOptions::setDefaultEPWLocation(const openstudio::path &t_loc)
  {
    m_defaultEPWLocation = t_loc;
  }

  int ConfigOptions::getMaxLocalJobs() const
  {
    return m_maxLocalJobs;
  }

  void ConfigOptions::setMaxLocalJobs(int t_numjobs)
  {
    m_maxLocalJobs = t_numjobs;
  }

  bool ConfigOptions::getSimpleName() const
  {
    return m_simpleName;
  }

  void ConfigOptions::setSimpleName(bool t_simpleName)
  {
    m_simpleName = t_simpleName;
  }

  std::vector<openstudio::path> ConfigOptions::potentialRadianceLocations() const
  {
    std::vector<openstudio::path> potentialpaths;

    boost::optional<openstudio::path> radianceDirectory = getRadianceDirectory();
    if (radianceDirectory){
      potentialpaths.push_back(*radianceDirectory);
    }

    QByteArray qba = qgetenv("RAYPATH");
    if (!qba.isEmpty())
    {
      QString radiancedir(qba);

      QStringList qsl = radiancedir.split(';', QString::SkipEmptyParts);

      for (QStringList::const_iterator itr = qsl.begin();
           itr != qsl.end();
           ++itr)
      {
        if (QFile(*itr).exists())
        {
          openstudio::path p = toPath(*itr);

          potentialpaths.push_back(toPath(*itr));
          if (p.has_parent_path())
          {
            potentialpaths.push_back(p.parent_path());
          }

          if (p.parent_path().has_parent_path())
          {
            potentialpaths.push_back(p.parent_path().parent_path());
          }
        }
      }
    }
      

    std::vector<openstudio::path> searchdirs;
#if defined(Q_OS_WIN)
    searchdirs.push_back(toPath("C:\\Program Files\\"));
    searchdirs.push_back(toPath("C:\\Program Files (x86)\\"));

    if (openstudio::applicationIsRunningFromBuildDirectory())
    {
      searchdirs.push_back(openstudio::getApplicationRunDirectory().parent_path().parent_path().parent_path().parent_path().parent_path());
    } else  {
      searchdirs.push_back(openstudio::getApplicationRunDirectory().parent_path());
    }
#elif defined(Q_OS_MAC)
    searchdirs.push_back(toPath("/Applications"));
    searchdirs.push_back(toPath("/usr/local"));
    searchdirs.push_back(openstudio::getSharedResourcesPath());
#else
    searchdirs.push_back(toPath("/usr/local"));
    searchdirs.push_back(openstudio::getSharedResourcesPath());
#endif

    for (std::vector<openstudio::path>::const_iterator itr = searchdirs.begin();
         itr != searchdirs.end();
         ++itr)
    {
      QDir dir(toQString(*itr), "Radiance*", QDir::Name, QDir::Dirs);

      QStringList qsl = dir.entryList();

      for (QStringList::const_iterator itr = qsl.begin();
           itr != qsl.end();
           ++itr)
      {
        potentialpaths.push_back(toPath(dir.canonicalPath()) / toPath(*itr));
      }
    }

    return potentialpaths;
  }


  std::vector<openstudio::path> ConfigOptions::potentialEnergyPlusLocations() const
  {
    std::vector<openstudio::path> potentialpaths;

    potentialpaths.push_back(getEnergyPlusDirectory());
    potentialpaths.push_back(getEnergyPlusExecutable());

    QByteArray qba = qgetenv("ENERGYPLUSDIR");
    if (!qba.isEmpty())
    {
      QString energyplusdir(qba);

      QStringList qsl = energyplusdir.split(';', QString::SkipEmptyParts);

      for (QStringList::const_iterator itr = qsl.begin();
           itr != qsl.end();
           ++itr)
      {
        if (QFile(*itr).exists())
        {
          potentialpaths.push_back(toPath(*itr));
        }
      }
    }
      

    std::vector<openstudio::path> searchdirs;
#if defined(Q_OS_WIN)
    searchdirs.push_back(toPath("C:\\"));
#elif defined(Q_OS_MAC)
    searchdirs.push_back(toPath("/Applications"));

#else
    searchdirs.push_back(toPath("/usr/local"));
#endif

    for (std::vector<openstudio::path>::const_iterator itr = searchdirs.begin();
         itr != searchdirs.end();
         ++itr)
    {
      QDir dir(toQString(*itr), "EnergyPlus*", QDir::Name, QDir::Dirs);

      QStringList qsl = dir.entryList();

      for (QStringList::const_iterator itr = qsl.begin();
           itr != qsl.end();
           ++itr)
      {
        potentialpaths.push_back(toPath(dir.canonicalPath()) / toPath(*itr));
      }
    }

    return potentialpaths;
  }

  std::vector<openstudio::path> ConfigOptions::potentialRubyLocations() const
  {
    std::vector<openstudio::path> potentialpaths;

    potentialpaths.push_back(getOpenStudioEmbeddedRubyPath());
    potentialpaths.push_back(getOpenStudioAWSRubyPath());

    return potentialpaths;
  }

  void ConfigOptions::fastFindRadiance()
  {
    std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
      foundTools = openstudio::runmanager::ToolFinder::findTools(potentialRadianceLocations(), false);

    m_toolLocations.insert(foundTools.begin(), foundTools.end());
  }


  void ConfigOptions::fastFindEnergyPlus()
  {
    std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
      foundTools = openstudio::runmanager::ToolFinder::findTools(potentialEnergyPlusLocations(), false);

    m_toolLocations.insert(foundTools.begin(), foundTools.end());

    setIDFEPWDefaults();
  }

  void ConfigOptions::fastFindRuby()
  {
    std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
      foundTools = openstudio::runmanager::ToolFinder::findTools(potentialRubyLocations(), false);

    m_toolLocations.insert(foundTools.begin(), foundTools.end());
  }

  void ConfigOptions::loadQSettingsData() 
  {
    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    QSettings settings(organizationName, applicationName);

    QVariant osversion = settings.value("runmanager_settings_openstudio_version");

    if (openstudio::toString(osversion.toString()) != openStudioVersion())
    {
      LOG(Info, "RunManager settings in QSettings is from a previous version of OpenStudio, ignoring");
      return;
    }

    QVariant maxlocaljobs = settings.value("runmanager_maxlocaljobs");
    QVariant defaultidflocation = settings.value("runmanager_defaultidflocation");
    QVariant defaultepwlocation = settings.value("runmanager_defaultepwlocation");
    QVariant outputlocation = settings.value("runmanager_outputlocation");
    QVariant simplename = settings.value("runmanager_simplename");

    if (maxlocaljobs.isValid())
    {
      setMaxLocalJobs(maxlocaljobs.toInt());
    }

    if (defaultidflocation.isValid())
    {
      setDefaultIDFLocation(openstudio::toPath(defaultidflocation.toString()));
    }

    if (defaultepwlocation.isValid())
    {
      setDefaultEPWLocation(openstudio::toPath(defaultepwlocation.toString()));
    }

    if (outputlocation.isValid())
    {
      setOutputLocation(openstudio::toPath(outputlocation.toString()));
    }

    if (simplename.isValid())
    {
      setSimpleName(simplename.toBool());
    }

  }


  void ConfigOptions::saveQSettingsData() const
  {
    
    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    QSettings settings(organizationName, applicationName);

    settings.setValue("runmanager_settings_openstudio_version", openstudio::toQString(openStudioVersion()));

    settings.setValue("runmanager_maxlocaljobs", getMaxLocalJobs());
    settings.setValue("runmanager_defaultidflocation", openstudio::toQString(getDefaultIDFLocation()));
    settings.setValue("runmanager_defaultepwlocation", openstudio::toQString(getDefaultEPWLocation()));
    settings.setValue("runmanager_outputlocation", openstudio::toQString(getOutputLocation()));
    settings.setValue("runmanager_simplename", getSimpleName());

  }


  std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
    ConfigOptions::loadQSettingsTools() const
  {
    std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> > tools;

    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    QSettings settings(organizationName, applicationName);

    QVariant osversion = settings.value("runmanager_tools_openstudio_version");

    if (openstudio::toString(osversion.toString()) != openStudioVersion())
    {
      LOG(Info, "Tools list in QSettings is from a previous version of OpenStudio, ignoring");
      return tools;
    }

    int count = settings.beginReadArray("tools");
    for (int i = 0; i < count; ++i)
    {
      try {
        settings.setArrayIndex(i);
        QVariant vmajor = settings.value("major");
        QVariant vminor = settings.value("minor");
        QVariant vbuild = settings.value("build");
        QVariant vtag = settings.value("tag");

        openstudio::runmanager::ToolVersion tv(
            vmajor.isValid()?vmajor.toInt():boost::optional<int>(),
            vminor.isValid()?vminor.toInt():boost::optional<int>(),
            vbuild.isValid()?vbuild.toInt():boost::optional<int>(),
            vtag.isValid()?openstudio::toString(vtag.toString()):boost::optional<std::string>()
            );

        openstudio::runmanager::ToolLocationInfo tli(
            ToolType(openstudio::toString(settings.value("toolType").toString())),
            openstudio::toPath(settings.value("binaryDir").toString())
            );

        tools.push_back(std::make_pair(tv, tli));
      } catch (const std::exception &e) {
        LOG(Error, "Error loading runmanager tools: " << e.what());
      }
    }

    settings.endArray();


    return tools;
  }

  void ConfigOptions::saveQSettings() const
  {

    std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
      tools(m_toolLocations.begin(), m_toolLocations.end());
    saveQSettings(tools);
    saveQSettingsData();
  }

  void ConfigOptions::saveQSettings(
      const std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> > &t_tools) const
  {
    QString organizationName = QCoreApplication::organizationName();
    QString applicationName = QCoreApplication::applicationName();
    QSettings settings(organizationName, applicationName);

    settings.setValue("runmanager_tools_openstudio_version", openstudio::toQString(openStudioVersion()));

    size_t size = t_tools.size();
    settings.beginWriteArray("tools", size);
    for (size_t i = 0; i < size; ++i)
    {
      settings.setArrayIndex(i);

      QVariant vmajor;
      QVariant vminor;
      QVariant vbuild;
      QVariant vtag;

      boost::optional<int> major = t_tools[i].first.getMajor();
      boost::optional<int> minor = t_tools[i].first.getMinor();
      boost::optional<int> build = t_tools[i].first.getBuild();
      boost::optional<std::string> tag = t_tools[i].first.getTag();

      if (major)
      {
        vmajor = *major;
      }

      if (minor)
      {
        vminor = *minor;
      }

      if (build)
      {
        vbuild = *build;
      }

      if (tag)
      {
        vtag = openstudio::toQString(*tag);
      }


      settings.setValue("major", vmajor);
      settings.setValue("minor", vminor);
      settings.setValue("build", vbuild);
      settings.setValue("tag", vtag);

      settings.setValue("toolType", openstudio::toQString(t_tools[i].second.toolType.valueName()));
      settings.setValue("binaryDir", openstudio::toQString(t_tools[i].second.binaryDir));
    }

    settings.endArray();
  }


  bool ConfigOptions::findTools(bool t_showProgressDialog, bool t_onlyIfZeroTools, bool t_promptUser, bool t_checkQSettings)
  {
    size_t beforesize = getToolLocations().size();

    std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> > 
      mergedtools(m_toolLocations.begin(), m_toolLocations.end());

    if (t_checkQSettings)
    {
      std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
        tools = loadQSettingsTools();
      ToolFinder::mergeTools(mergedtools, tools);
      m_toolLocations 
        = std::set<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >(mergedtools.begin(), mergedtools.end());
    }

    if (t_onlyIfZeroTools)
    {
      if (getToolLocations().size() == 0) {
        if (t_promptUser)
        {
          QMessageBox msgBox;
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setWindowTitle("No Configured Tools");
          if (Application::instance().isDefaultInstance())
          {
            QIcon icon = QIcon(":/images/rm_16.png");
            icon.addPixmap(QPixmap(":/images/rm_32.png"));
            icon.addPixmap(QPixmap(":/images/rm_48.png"));
            icon.addPixmap(QPixmap(":/images/rm_64.png"));
            icon.addPixmap(QPixmap(":/images/rm_128.png"));
            icon.addPixmap(QPixmap(":/images/rm_256.png"));
            msgBox.setWindowIcon(icon);
          }
          msgBox.setText("There are no tools configured. The system will now automatically search for tools");
          msgBox.exec();
        }

      } else {
        return getToolLocations().size() > beforesize;
      }
    }

    std::vector<openstudio::path> search;

#ifdef Q_OS_WIN
    if (openstudio::applicationIsRunningFromBuildDirectory())
    {
      openstudio::path p = openstudio::getApplicationRunDirectory().parent_path().parent_path().parent_path().parent_path().parent_path();
      search.push_back(p);
      p /= openstudio::toPath("Ruby-prefix/src");
      search.push_back(p);
    } else  {
      search.push_back(openstudio::getApplicationRunDirectory().parent_path());
    }

    // Add drive C no matter what
    search.push_back(openstudio::toPath("C:\\"));

    for (char drive = 'D'; drive <= 'Z'; ++drive)
    {
      // Add all normal fixed hard drives
      std::string path = drive + std::string(":\\");
      if (GetDriveType(path.c_str()) == DRIVE_FIXED)
      {
        search.push_back(openstudio::toPath(path));
      }
    }

#else
    search.push_back(openstudio::toPath("/"));
    search.push_back(openstudio::toPath("/usr/local"));

#ifdef Q_OS_MAC
    search.push_back(toPath("/System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/bin"));
#endif

#endif
    
    if (!openstudio::applicationIsRunningFromBuildDirectory())
    {
      search.push_back(openstudio::getSharedResourcesPath());
    }

    search.push_back(toPath(QDir::homePath()));

    std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
      foundTools = openstudio::runmanager::ToolFinder::findTools(search, t_showProgressDialog);

    ToolFinder::mergeTools(mergedtools, foundTools);
    m_toolLocations 
      = std::set<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >(mergedtools.begin(), mergedtools.end());

    setIDFEPWDefaults();


    try {
      // the epw2wea tool should only be located as part of a radiance installation
      ToolInfo epw2wea = getTools().getLastByName("epw2wea");


      bool verpath_found = false;
      try {
        openstudio::path verpath = epw2wea.localBinPath.parent_path().parent_path() / openstudio::toPath("NREL_ver.txt");
        LOG(Debug, "Looking for NREL_ver at: " << openstudio::toString(verpath));
        verpath_found = boost::filesystem::exists(verpath);
      } catch (const std::exception &) {
      }

      if (!verpath_found) {
        LOG(Error, "Radiance found with no NREL_ver.txt");

        if (t_promptUser || t_showProgressDialog) {
          QMessageBox msgBox;
          msgBox.setIcon(QMessageBox::Information);
          msgBox.setWindowTitle("Non-NREL Radiance Found");
          if (Application::instance().isDefaultInstance())
          {
            QIcon icon = QIcon(":/images/rm_16.png");
            icon.addPixmap(QPixmap(":/images/rm_32.png"));
            icon.addPixmap(QPixmap(":/images/rm_48.png"));
            icon.addPixmap(QPixmap(":/images/rm_64.png"));
            icon.addPixmap(QPixmap(":/images/rm_128.png"));
            icon.addPixmap(QPixmap(":/images/rm_256.png"));
            msgBox.setWindowIcon(icon);
          }
          msgBox.setText("Radiance was found, but it was not the NREL provided version.");
          msgBox.exec();
        }
      }
    } catch (const std::exception &) {
      // no radiance tool found
    }

    if (t_promptUser)
    {
      QMessageBox msgBox;
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setWindowTitle("Tools Found");
      if (Application::instance().isDefaultInstance())
      {
        QIcon icon = QIcon(":/images/rm_16.png");
        icon.addPixmap(QPixmap(":/images/rm_32.png"));
        icon.addPixmap(QPixmap(":/images/rm_48.png"));
        icon.addPixmap(QPixmap(":/images/rm_64.png"));
        icon.addPixmap(QPixmap(":/images/rm_128.png"));
        icon.addPixmap(QPixmap(":/images/rm_256.png"));
        msgBox.setWindowIcon(icon);
      }
      msgBox.setText(QString::number(getToolLocations().size()) + " tool" + (getToolLocations().size()!=1?"s":"") + " found");
      msgBox.exec();
    }

    if (t_checkQSettings)
    {
      std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> >
        tools(m_toolLocations.begin(), m_toolLocations.end());
      saveQSettings(tools);
    }

    return getToolLocations().size() > beforesize;
  }

  void ConfigOptions::setIDFEPWDefaults()
  {
    try {
      ToolInfo eplus = getTools().getTool("energyplus");

      openstudio::path weatherdir1 = eplus.localBinPath.parent_path().parent_path() / toPath("WeatherData");
      openstudio::path weatherdir2 = eplus.localBinPath.parent_path() / toPath("WeatherData");

      QFileInfo fi1(openstudio::toQString(weatherdir1));
      QFileInfo fi2(openstudio::toQString(weatherdir2));

      if (fi1.exists() && fi1.isDir())
      {
        m_defaultEPWLocation = weatherdir1;
      } else if (fi2.exists() && fi2.isDir()) {
        m_defaultEPWLocation = weatherdir2;
      }


      openstudio::path exampledir1 = eplus.localBinPath.parent_path().parent_path() / toPath("Examples");
      openstudio::path exampledir2 = eplus.localBinPath.parent_path() / toPath("Examples");
      openstudio::path exampledir3 = eplus.localBinPath.parent_path().parent_path() / toPath("ExampleFiles");
      openstudio::path exampledir4 = eplus.localBinPath.parent_path() / toPath("ExampleFiles");

      QFileInfo fie1(openstudio::toQString(exampledir1));
      QFileInfo fie2(openstudio::toQString(exampledir2));
      QFileInfo fie3(openstudio::toQString(exampledir3));
      QFileInfo fie4(openstudio::toQString(exampledir4));
  
      if (fie1.exists() && fie1.isDir())
      {
        m_defaultIDFLocation = exampledir1;
      } else if (fie2.exists() && fie2.isDir()) {
        m_defaultIDFLocation = exampledir2;
      } else if (fie3.exists() && fie3.isDir()) {
        m_defaultIDFLocation = exampledir3;
      } else if (fie4.exists() && fie4.isDir()) {
        m_defaultIDFLocation = exampledir4;
      }

    } catch (const std::runtime_error &) {
    }
  }

}
}

