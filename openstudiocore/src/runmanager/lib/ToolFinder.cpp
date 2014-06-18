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

#include "ToolFinder.hpp"

#include "../../utilities/core/Application.hpp"
#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/idd/IddFile.hpp"

#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>
#include <QProgressDialog>
#include <QFileInfo>
#include <QDirIterator>

#include <boost/filesystem.hpp>

namespace openstudio {
namespace runmanager {

  bool ToolFinder::safeExists(const openstudio::path &t_path)
  {

    try {
      return boost::filesystem::exists(t_path);
    } catch (const std::exception &) {
      return false;
    }
  }

  ToolFinder::ToolFinder()
  {
  }

  ToolFinder::ToolFinder(const std::vector<openstudio::path> &t_searchPaths)
    : m_searchPaths(t_searchPaths)
  {
  }

  void ToolFinder::addSearchPath(const openstudio::path &t_searchPath)
  {
    m_searchPaths.push_back(t_searchPath);
  }


  std::vector<std::pair<ToolVersion, ToolLocationInfo> > ToolFinder::findTools(const std::vector<openstudio::path> &t_searchPaths, bool t_showProgressDialog) 
  {
    ToolFinder tf(t_searchPaths);
    return tf.findTools(t_showProgressDialog);
  }

  ToolVersion ToolFinder::parseToolVersion(const openstudio::path &t_path)
  {
    QSharedPointer<ToolVersion> toolver;

    for (const auto & path : t_path)
    {
      std::string pathstr = openstudio::toString(path);
      LOG(Debug, "Parsing tool version number from string: " << pathstr);

      boost::regex reg(".*?V?([0-9]+)[\\.-]([0-9]+)[\\.-]?([0-9]*)([\\.-][0-9]+)?.*");

      boost::smatch results;
      if (boost::regex_match(pathstr, results, reg))
      {
        int major = atoi(results[1].str().c_str());
        int minor = atoi(results[2].str().c_str());
        std::string build_str = results[3].str();
        if (!build_str.empty()) {
          int build = atoi(build_str.c_str());
          toolver = QSharedPointer<ToolVersion>(new ToolVersion(major,minor,build));
        } else {
          toolver = QSharedPointer<ToolVersion>(new ToolVersion(major,minor));
        }
      } 
    }

    if (!toolver) 
    {
      openstudio::path iddpath = t_path.parent_path() / openstudio::toPath("Energy+.idd");

      if (safeExists(iddpath))
      {
        boost::optional<openstudio::IddFile> f = openstudio::IddFile::load(iddpath);
        LOG(Debug, "Unable to parse version of EnergyPlus from folder name, using idd version: " << openstudio::toString(iddpath));

        if (f)
        {
          std::string version = f->version();
          LOG(Debug, "Version string is: " << version);
          boost::regex reg("([0-9]+)\\.([0-9]+)\\.([0-9]+)(\\.[0-9]+)?.*");

          boost::smatch results;
          if (boost::regex_match(version, results, reg))
          {
            int major = atoi(results[1].str().c_str());
            int minor = atoi(results[2].str().c_str());
            int build = atoi(results[3].str().c_str());

            toolver = QSharedPointer<ToolVersion>(new ToolVersion(major,minor, build));
          }
        }
      }
    }

    // return last version parsed.
    return toolver?*toolver:ToolVersion();
  }


  void ToolFinder::mergeTools(std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_tools,
    const std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_newtools)
  {
    for (const auto & newTools : t_newtools)
    {
      mergeTool(t_tools, newTools);
    }
  }

  void ToolFinder::mergeTool(std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_tools,
      const std::pair<ToolVersion, ToolLocationInfo> &t_tool)
  {
    openstudio::path runbase;
#ifdef Q_OS_WIN32
    if (openstudio::applicationIsRunningFromBuildDirectory())
    {
      runbase = openstudio::getApplicationRunDirectory().parent_path().parent_path().parent_path().parent_path().parent_path();
    } else  {
      runbase = openstudio::getApplicationRunDirectory().parent_path();
    }
#else 
    runbase = openstudio::getSharedResourcesPath();
#endif

    struct ComparePaths
    {
      static openstudio::path bestPath(const openstudio::path &t_path1, const openstudio::path &t_path2, const openstudio::path &t_runbase)
      {
        bool path1_exists = safeExists(t_path1);
        bool path2_exists = safeExists(t_path2);
        bool path1_in_rundir = !t_runbase.empty() && !openstudio::relativePath(t_path1, t_runbase).empty();
        bool path2_in_rundir = !t_runbase.empty() && !openstudio::relativePath(t_path2, t_runbase).empty();
        bool path2_is_shorter = std::distance(t_path1.begin(), t_path1.end())
                    > std::distance(t_path2.begin(), t_path2.end());
        bool path1_is_aws = subPathMatch(t_path1, boost::regex(".*-aws-.*", boost::regex::perl));
        bool path2_is_aws = subPathMatch(t_path2, boost::regex(".*-aws-.*", boost::regex::perl));

#ifdef Q_OS_MAC
#ifdef __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__

#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ == 1090
        bool path1isruby20 = t_path1 == openstudio::toPath("/System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/bin");
        bool path2isruby20 = t_path2 == openstudio::toPath("/System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/bin");

        if (path1isruby20) return t_path1;
        if (path2isruby20) return t_path2;
#else
        bool path1isruby18 = t_path1 == openstudio::toPath("/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/bin");
        bool path2isruby18 = t_path2 == openstudio::toPath("/System/Library/Frameworks/Ruby.framework/Versions/1.8/usr/bin");

        if (path1isruby18) return t_path1;
        if (path2isruby18) return t_path2;
#endif

#endif

#endif

        if (!t_path1.empty() && !t_path2.empty())
        {
          openstudio::path verpath_1 = t_path1.parent_path() / openstudio::toPath("NREL_ver.txt");
          openstudio::path verpath_2 = t_path2.parent_path() / openstudio::toPath("NREL_ver.txt");

          LOG(Debug, "Checking for NREL_ver " << openstudio::toString(verpath_1));
          LOG(Debug, "Checking for NREL_ver " << openstudio::toString(verpath_2));

          // existence of this file in the parent folder indicates that it's a version provided
          // by NREL and we should prefer it 
          bool path1_has_nrel_ver = safeExists(verpath_1);
          bool path2_has_nrel_ver = safeExists(verpath_2);

          if (path1_has_nrel_ver && !path2_has_nrel_ver)
          {
            return t_path1;
          } else if (!path1_has_nrel_ver && path2_has_nrel_ver) {
            return t_path2;
          }
        }


        // return the shortest path that exists. Unless one of the two exists in the run dir,
        // that means it's one we provided and we want to return it
        if (t_path1.empty() 
            || (!t_path2.empty()
                && path2_is_shorter
                && !path1_in_rundir)
            || (!t_runbase.empty() && !t_path2.empty() && path2_in_rundir) 
            || (!t_path2.empty() 
                && !t_path1.empty() 
                && !path1_exists 
                && path2_exists)
            || (!t_path2.empty()
                && path1_is_aws
                && !path2_is_aws)
                )
        {
          return t_path2;
        } else {
          return t_path1;
        }
      }
    };

    LOG(Debug, "Merging Tool: " << t_tool.second.toolType.valueName() << " " << toString(t_tool.second.binaryDir)
        << " " << toString(t_tool.second.linuxBinaryArchive) << " " <<
        t_tool.first.toString());

    for (auto & tool : t_tools)
    {
      if (tool.second.toolType == t_tool.second.toolType
          && tool.first == t_tool.first)
      {
        tool.second.binaryDir = ComparePaths::bestPath(tool.second.binaryDir, t_tool.second.binaryDir, runbase);
        tool.second.linuxBinaryArchive = ComparePaths::bestPath(tool.second.linuxBinaryArchive, t_tool.second.linuxBinaryArchive, runbase);

        LOG(Debug, "Existing tool found with same type and version - merging data");
        return; // we found and updated the appropriate item
      }
    }


    LOG(Debug, "No existing tool found with same type and version - adding data");

    // no existing item found, add it
    t_tools.push_back(t_tool);
  }

  std::vector<std::pair<ToolVersion, ToolLocationInfo> > ToolFinder::filterTools(
    const std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_tools)
  {
    std::vector<std::pair<ToolVersion, ToolLocationInfo> > retval;

    for (const auto & tool : t_tools)
    {
      if (!tool.second.binaryDir.empty())
      {
        retval.push_back(tool);
      }
    }

    return retval;
  }

  std::vector<std::pair<ToolVersion, ToolLocationInfo> > ToolFinder::findTools(bool t_showProgressDialog) const
  {
    std::vector<std::string> names;
#ifdef Q_OS_WIN32
    names.push_back("ies2rad.exe");
    names.push_back("ruby.exe");
    names.push_back("energyplus.exe");
    names.push_back("_energyplus.tar.gz");
    names.push_back("EPXMLPreproc2.exe");
#else
    names.push_back("ies2rad");
    names.push_back("ruby");
    names.push_back("energyplus");
    names.push_back("_energyplus.tar.gz");
    names.push_back("EPXMLPreproc2");
#endif

    std::vector<openstudio::path> searchPaths = m_searchPaths;

    std::sort(searchPaths.begin(), searchPaths.end()); // Sort them

    // DLM: have to sort before calling unique, unique only works on consecutive elements
    searchPaths.erase(std::unique(searchPaths.begin(), searchPaths.end()), searchPaths.end()); // Erase duplicate elements
#ifdef Q_OS_WIN32
    // For Windows, maintain drive order while searching the deepest paths first
    std::vector<openstudio::path> newSearchPaths, temp;
    char drive = 'Z';
    for (const openstudio::path& path : searchPaths) {
      if (drive != path.string()[0]) {
        drive = path.string()[0];
        newSearchPaths.insert(newSearchPaths.end(), temp.rbegin(), temp.rend());
        temp.clear();
      }
      temp.push_back(path);
    }
    newSearchPaths.insert(newSearchPaths.end(), temp.rbegin(), temp.rend());
    searchPaths = newSearchPaths;
#else
    std::reverse(searchPaths.begin(), searchPaths.end()); // Reverse, we want to search the deepest paths first
#endif

    std::vector<openstudio::path> exes = findExecutables(searchPaths, names, t_showProgressDialog);

    std::vector<std::pair<ToolVersion, ToolLocationInfo> > tools;

    for (const auto & exe : exes)
    {
      LOG(Debug, "executable found: " + toString(exe));

      if (boost::iequals(toString(exe.stem()), "ies2rad"))
      {
        mergeTool(tools, std::make_pair(ToolVersion(), ToolLocationInfo(ToolType::Radiance, exe.parent_path(), openstudio::path())));
      } else if (boost::iequals(toString(exe.stem()), "ruby")) {
        mergeTool(tools, std::make_pair(ToolVersion(), ToolLocationInfo(ToolType::Ruby, exe.parent_path(), openstudio::path())));
      } else if (boost::iequals(toString(exe.stem()), "energyplus")) {
        if (safeExists(exe.parent_path() / toPath("WeatherData"))
          || safeExists(exe.parent_path().parent_path() / toPath("WeatherData")))
        {
          mergeTool(tools, std::make_pair(parseToolVersion(exe), ToolLocationInfo(ToolType::EnergyPlus, exe.parent_path(), openstudio::path())));
        }
      } else if (boost::iequals(toString(exe.filename()), "_energyplus.tar.gz")) {
        mergeTool(tools, std::make_pair(parseToolVersion(exe), ToolLocationInfo(ToolType::EnergyPlus, openstudio::path(), exe)));
      } else if (boost::iequals(toString(exe.stem()), "EPXMLPreproc2")
                 && subPathMatch(exe, boost::regex("preprocessor.*", boost::regex::perl|boost::regex::icase))) {
        mergeTool(tools, std::make_pair(ToolVersion(), ToolLocationInfo(ToolType::XMLPreprocessor, exe.parent_path(), openstudio::path())));
      }
    }



    return filterTools(tools);
  }


  std::vector<openstudio::path> ToolFinder::findExecutables(
      const openstudio::path &t_path,
      const std::vector<std::string> &t_names,
      const std::shared_ptr<QProgressDialog> &t_dlg,
      std::map<openstudio::path, int> &t_searchedPaths) const
  {
    std::vector<openstudio::path> results;

    std::deque<std::pair<int, openstudio::path> > pathsToSearch;
    pathsToSearch.push_front(std::make_pair(0, t_path));

    int max_depth = 3;
    QFileInfo basefi(openstudio::toQString(t_path));

    if (!basefi.exists() || !basefi.isDir())
    {
      // Search dir does not exist / is not a directory
      return results;
    }

    while (!pathsToSearch.empty())
    {
      QFileInfoList qfil = QDir(openstudio::toQString(pathsToSearch.front().second), "", QDir::Name, QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden).entryInfoList();

      int cur_depth = pathsToSearch.front().first;
      ++t_searchedPaths[pathsToSearch.front().second];


      for (const auto & infoList : qfil)
      {
        QString p = infoList.absoluteFilePath();
        QFileInfo fi(infoList);

        openstudio::path curPath = openstudio::toPath(p);

        if (t_dlg)
        {
          t_dlg->setLabelText(toQString("Scanning For Tools In: " + shortenPath(curPath)));
        }

        // We only want files, not directories
        if (fi.isReadable() && fi.isDir() && cur_depth < max_depth)
        {
          if (openstudio::toPath("/proc") == curPath
            || openstudio::toPath("/dev") == curPath
            || openstudio::toPath("/etc") == curPath
            || openstudio::toPath("/tmp") == curPath
            || openstudio::toPath("/var") == curPath
            || openstudio::toPath("/sys") == curPath
            || openstudio::toPath("/media") == curPath
            || openstudio::toPath("/mnt") == curPath
            || openstudio::toPath("/Volumes") == curPath
            || subPathMatch(curPath, boost::regex("lib.*", boost::regex::perl))
            || subPathMatch(curPath, boost::regex("share", boost::regex::perl))
            || openstudio::toPath("C:/Windows") == curPath
            || (subPathMatch(curPath, boost::regex("share", boost::regex::perl)) && !subPathMatch(curPath, boost::regex("openstudio", boost::regex::perl)))
            || openstudio::toPath("C:/Windows") == curPath
            || openstudio::toPath("C:/DAYSIM") == curPath
            || openstudio::toPath("C:/$Recycle.Bin") == curPath
            || subPathMatch(curPath, boost::regex("\\..*", boost::regex::perl))
            || subPathMatch(curPath, boost::regex("Temp.*", boost::regex::perl|boost::regex::icase)))
          {
            // don't scan it if it matches these things
          } else {

            // Otherwise queue for scanning
            if (t_searchedPaths[curPath] > 1)
            {
              // we've already scanned it, no reason to add it nowa
            } else {
              pathsToSearch.push_back(std::make_pair(cur_depth + 1, curPath));
            }
          }
        }


        if (fi.isFile() && fi.isReadable() && !fi.isDir())
        {
          for (const auto & name : t_names)
          {
            if (boost::iequals(toString(curPath.filename()), name))
            {
              results.push_back(curPath);
            }
          }
        }
      }


      if (t_dlg)
      {
        if (t_dlg->wasCanceled())
        {
          break;
        }

        t_dlg->setValue(t_dlg->value() + 1);
        openstudio::Application::instance().processEvents();
      }

      pathsToSearch.pop_front();
    }

    return results;
  }

  bool ToolFinder::subPathMatch(const openstudio::path &t_path, const boost::regex &t_regex)
  {
    for (const auto & path : t_path)
    {
      if (boost::regex_match(toString(path), t_regex))
      {
        return true;
      }
    }

    return false;
  }

  std::string ToolFinder::shortenPath(const openstudio::path &t_path) const
  {
    if (std::distance(t_path.begin(), t_path.end()) <= 5)
    {
      return t_path.string();
    } else {
      openstudio::path outpath;

      openstudio::path::const_iterator itr = t_path.begin();
      outpath /= *itr;
      ++itr;
      outpath /= *itr;
      outpath /= toPath("...");

      itr = t_path.end();
      --itr;
      --itr;
      outpath /= *itr;
      ++itr;
      outpath /= *itr;

      return outpath.string();
    }
  }

  std::vector<openstudio::path> ToolFinder::findExecutables(const std::vector<openstudio::path> &t_searchPaths, const std::vector<std::string> &t_names,
    bool t_showProgressDialog) const
  {
    std::set<openstudio::path> files;

    std::shared_ptr<QProgressDialog> dlg;

    if (t_showProgressDialog)
    {
      dlg = std::shared_ptr<QProgressDialog>(new QProgressDialog(nullptr, Qt::WindowFlags(Qt::Dialog | Qt::WindowTitleHint)));
      dlg->setWindowTitle("Scanning For Tools");
      if (Application::instance().isDefaultInstance()){
        QIcon icon = QIcon(":/images/rm_16.png");
        icon.addPixmap(QPixmap(":/images/rm_32.png"));
        icon.addPixmap(QPixmap(":/images/rm_48.png"));
        icon.addPixmap(QPixmap(":/images/rm_64.png"));
        icon.addPixmap(QPixmap(":/images/rm_128.png"));
        icon.addPixmap(QPixmap(":/images/rm_256.png"));
        dlg->setWindowIcon(icon);
      }
      dlg->setMinimumDuration(1000);
      dlg->setMinimum(0);
      dlg->setMaximum(0);
      dlg->setMaximumWidth(400);
      dlg->setMinimumWidth(400);
      dlg->setCancelButtonText("Stop Scan");
      auto lbl = new QLabel(dlg.get());
      
      lbl->setWordWrap(true);
      dlg->setLabel(lbl);
      
      dlg->setLabelText("Scanning For Tools");
      dlg->open();
    }

    std::map<openstudio::path, int> searchedPaths;
    for (const auto & searchPath : t_searchPaths)
    {
      LOG(Info, "Scanning top level directory: " << openstudio::toString(searchPath) << " for tools");
      std::vector<openstudio::path> found = findExecutables(searchPath, t_names, dlg, searchedPaths);
      files.insert(found.begin(), found.end());
    }

    return std::vector<openstudio::path>(files.begin(), files.end());
  }



}
}

