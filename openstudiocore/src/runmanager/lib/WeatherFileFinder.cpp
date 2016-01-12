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

#include "WeatherFileFinder.hpp"
#include <boost/regex.hpp>
#include "../../utilities/idf/IdfFile.hpp"
#include <boost/lexical_cast.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Version_FieldEnums.hxx>
#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include <QFileInfo>
#include <QDir>


namespace openstudio {
namespace runmanager {

  void WeatherFileFinder::extractDetails(
      const IdfFile &t_idffile,
      ToolVersion &t_version,
      boost::optional<std::string> &t_filelocationname,
      boost::optional<std::string> &t_weatherfilename)
  {
    IdfObjectVector versionObjects = t_idffile.getObjectsByType(IddObjectType::Version);
    if(versionObjects.size() == 1){
      OptionalString version = versionObjects[0].getString(VersionFields::VersionIdentifier, true);
      if (version){
        boost::regex majorMinorBuildRegex("^(\\d*?)\\.(\\d*?)\\.(\\d*?)$");
        boost::regex majorMinorRegex("^(\\d*?)\\.(\\d*?)$");
        boost::regex majorRegex("^(\\d*?)$");

        boost::smatch matches;
        if (boost::regex_search(*version, matches, majorMinorBuildRegex)){
          unsigned major = boost::lexical_cast<unsigned>(std::string(matches[1].first, matches[1].second));
          unsigned minor = boost::lexical_cast<unsigned>(std::string(matches[2].first, matches[2].second));
          unsigned build = boost::lexical_cast<unsigned>(std::string(matches[3].first, matches[3].second));
          t_version = ToolVersion(major, minor, build);
        }else if(boost::regex_search(*version, matches, majorMinorRegex)){
          unsigned major = boost::lexical_cast<unsigned>(std::string(matches[1].first, matches[1].second));
          unsigned minor = boost::lexical_cast<unsigned>(std::string(matches[2].first, matches[2].second));
          t_version = ToolVersion(major, minor);
        }else if(boost::regex_search(*version, matches, majorRegex)){
          unsigned major = boost::lexical_cast<unsigned>(std::string(matches[1].first, matches[1].second));
          t_version = ToolVersion(major);
        }
      }
    }

    IdfObjectVector locationObjects = t_idffile.getObjectsByType(IddObjectType::Site_Location);
    if(locationObjects.size() == 1){
      OptionalString locationname = locationObjects[0].getString(Site_LocationFields::Name, true);
      if (locationname && !locationname->empty()){
        LOG(Debug, "Location name field from IDF: " << *locationname);
        t_filelocationname = locationname;
      }
    }

    std::string header = t_idffile.header();
    std::vector<std::string> headerlines;
    boost::split(headerlines, header, boost::is_any_of("\n\r"), boost::algorithm::token_compress_on);
    for (std::vector<std::string>::const_iterator itr = headerlines.begin();
         itr != headerlines.end();
         ++itr)
    {
      boost::smatch matches;
     
      if (boost::regex_match(*itr, matches, boost::regex(".*!\\s+WeatherFileName=(.*)"),boost::regex_constants::match_all))
      {
        std::string match = matches[1];
        t_weatherfilename = match;
        break;
      }
    }
  }

  openstudio::path WeatherFileFinder::find(const JobParams &t_params,
      const boost::optional<std::string> &t_filelocationname,
      const boost::optional<std::string> &t_weatherfilename)
  {
    openstudio::path epwdir;

    try {
      if (t_params.has("epwfile"))
      {
        openstudio::path epwfile = toPath(t_params.get("epwfile").children.at(0).value);

        if (!epwfile.empty() && boost::filesystem::exists(epwfile))
        {
          LOG(Info, "Valid epwfile found, returning: " << openstudio::toString(epwfile));

          return epwfile;
        }
      } else {
        LOG(Info, "No epwfile found in params, moving on for other methods of finding weather file");
      }
    } catch (const std::exception &) {
      // No epw dir set in params
      LOG(Info, "Error with epwfile found in params, moving on for other methods of finding weather file");
    }

    try {
      if (t_params.has("epwdir"))
      {
        epwdir = toPath(t_params.get("epwdir").children.at(0).value);
      }
    } catch (const std::exception &) {
      // No epw dir set in params
      LOG(Info, "No EPWDir known finding weather file will be much harder");
    }

    if (epwdir.empty() || !boost::filesystem::exists(epwdir) || !boost::filesystem::is_directory(epwdir))
    {
      LOG(Info, "The configured EPWDir either does not exist or is not a directory: " << toString(epwdir));
    }


    if (t_weatherfilename)
    {
      openstudio::path p = toPath(*t_weatherfilename);

      if (!boost::filesystem::exists(p))
      {
        p = epwdir / toPath(*t_weatherfilename);

        if (!boost::filesystem::exists(p))
        {
          LOG(Info, "Weather file discovered from comment header cannot be found at: " << toString(p));
        } else {
          return p;
        }
        LOG(Info, "Weather file discovered from comment header cannot be found at: " << *t_weatherfilename);
      } else {
        return p;
      }

    }

    if (t_filelocationname)
    {
      LOG(Info, "attempting to find weather file from location name: " << *t_filelocationname);

      // We did not have an epw set, so let's try to find one
      try {
        QDir dir(openstudio::toQString(epwdir), "*.epw");
        QFileInfoList files = dir.entryInfoList();

        std::set<std::string> desiredname = getNameComponents(*t_filelocationname);

        openstudio::path bestmatch;
        size_t matchsize = 0;

        for (QFileInfoList::const_iterator itr = files.begin();
            itr != files.end();
            ++itr)
        {
          std::set<std::string> foundname = getNameComponents(openstudio::toString(itr->fileName()));

          std::vector<std::string> matches;
          std::set_intersection(desiredname.begin(), desiredname.end(), foundname.begin(), foundname.end(),
              std::back_inserter(matches));

          if (matches.size() > matchsize)
          {
            matchsize = matches.size();
            bestmatch = openstudio::toPath(itr->absoluteFilePath());
          }
        }

        if (matchsize > 2)
        {
          LOG(Info, "Adding best match epw from the list found: " << toString(bestmatch));
          return bestmatch;
        } else {
          LOG(Info, "No best match epw file found, continuing with no epw set");
        }

      } catch (const std::exception &) {
        LOG(Info, "No EPw file set and no epwdir provided. We are continuing, but EnergyPlus will likely fail");
      }
    } else {
      LOG(Info, "No EPw file set and no location information parsed from IDF. We are continuing, but EnergyPlus will likely fail");
    }  

    LOG(Info, "No weather file found");

    return openstudio::path();
  }

  /// Return an ordered set of name parts for a weather name
  std::set<std::string> WeatherFileFinder::getNameComponents(const std::string &name)
  {
    std::string upper = boost::to_upper_copy(name);

    boost::regex re("[A-Z]+");
    boost::sregex_token_iterator i(upper.begin(), upper.end(), re);
    boost::sregex_token_iterator j;

    std::set<std::string> ret;

    while (i != j)
    {
      ret.insert(*i);
      ++i;
    }

    return ret;
  }


}
}

