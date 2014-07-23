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

#include "BCLFileReference.hpp"
#include "../core/Checksum.hpp"
#include "../core/String.hpp"
#include "../core/System.hpp"

#include <QFile>

namespace openstudio{

  BCLFileReference::BCLFileReference(const openstudio::path& path, const bool setMembers)
    : m_path(boost::filesystem::system_complete(path))
  {
    if (setMembers) {
      m_checksum = openstudio::checksum(m_path);

      std::string fileType = this->fileType();
      if (fileType == "osm"){
        m_softwareProgram = "OpenStudio";
        //m_softwareProgramVersion = "";
      }else if (fileType == "osc"){
        m_softwareProgram = "OpenStudio";
        //m_softwareProgramVersion = "";
      }else if (fileType == "idf"){
        m_softwareProgram = "EnergyPlus";
        //m_softwareProgramVersion = "";
      }else if (fileType == "rb"){
        m_softwareProgram = "OpenStudio";
        //m_softwareProgramVersion = "";
      }
    }
  }

  BCLFileReference::~BCLFileReference()
  {
  }

  openstudio::path BCLFileReference::path() const
  {
    return m_path;
  }

  std::string BCLFileReference::checksum() const
  {
    return m_checksum;
  }

  std::string BCLFileReference::softwareProgram() const
  {
    return m_softwareProgram;
  }

  std::string BCLFileReference::softwareProgramVersion() const
  {
    return m_softwareProgramVersion;
  }

  std::string BCLFileReference::fileName() const
  {
    return toString(m_path.filename());
  }

  std::string BCLFileReference::fileType() const
  {
    return toString(toQString(m_path.extension()).replace(".", ""));
  }

  std::string BCLFileReference::usageType() const
  {
    return m_usageType;
  }

  void BCLFileReference::setChecksum(const std::string& checksum)
  {
    m_checksum = checksum;
  }
  
  void BCLFileReference::setSoftwareProgram(const std::string& softwareProgram)
  {
    m_softwareProgram = softwareProgram;
  }

  void BCLFileReference::setSoftwareProgramVersion(const std::string& softwareProgramVersion)
  {
    m_softwareProgramVersion = softwareProgramVersion;
  }

  void BCLFileReference::setUsageType(const std::string& usageType)
  {
    m_usageType = usageType;
  }

  bool BCLFileReference::checkForUpdate()
  {
    std::string newChecksum = openstudio::checksum(this->path());
    if (m_checksum != newChecksum){
      m_checksum = newChecksum;
      return true;
    }
    return false;
  }

} // openstudio
