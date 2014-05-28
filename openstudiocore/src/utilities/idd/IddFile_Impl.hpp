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

#ifndef UTILITIES_IDD_IDDFILE_IMPL_HPP
#define UTILITIES_IDD_IDDFILE_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "IddObject.hpp"
#include "../core/Logger.hpp"

#include <string>
#include <ostream>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace openstudio{
namespace detail{

  /// Implementation of IddFile
  class UTILITIES_API IddFile_Impl {
   public:

    /** @name Constructors */
    //@{

    /// default constructor
    IddFile_Impl();

    //@}
    /** @name Getters */
    //@{

    /// get version
    std::string version() const;

    /// get header
    std::string header() const;

    /// get all objects
    std::vector<IddObject> objects() const;

    /// get all groups in file (e.g. "" and "Simulation Parameters")
    std::vector<std::string> groups() const;

    /// get all objects in group (e.g. "Simulation Parameters")
    std::vector<IddObject> getObjectsInGroup(const std::string& group) const;

    /** Get the IddObjects that match objectRegex. */
    std::vector<IddObject> getObjects(const boost::regex& objectRegex) const;

    /** If possible, returns the version IddObject for this IddFile. */
    boost::optional<IddObject> versionObject() const;

    /// get object by name
    boost::optional<IddObject> getObject(const std::string& objectName) const;

    /// get object by type
    boost::optional<IddObject> getObject(IddObjectType objectType) const;

    /// required objects, occur at least once
    std::vector<IddObject> requiredObjects() const;

    /// unique objects, occur at most one time
    std::vector<IddObject> uniqueObjects() const;

    //@}
    /** @name Setters */
    //@{

    /// set version
    void setVersion(const std::string& version);

    /// set header
    void setHeader(const std::string& header);

    /// add an object
    void addObject(const IddObject& object);

    //@}
    /** @name Serialization */
    //@{

    /// parse text from input stream to construct an IddFile_Impl
    static std::shared_ptr<IddFile_Impl> load(std::istream& is);

    /// print
    std::ostream& print(std::ostream& os) const;

    //@}

   private:

    /// Parse file text to populate this IddFile.
    void parse(std::istream& is);

    /// Version string required to be at top of any IddFile.
    std::string m_version;

    /// The first comment block in an IddFile is its header.
    std::string m_header;

    /// The vector of IddObjects that constitute this IddFile.
    std::vector<IddObject> m_objects; 

    /// Cache the Version IddObject
    mutable boost::optional<IddObject> m_versionObject;

    /// Configure logging.
    REGISTER_LOGGER("utilities.idd.IddFile");
  };

} // detail
} // openstudio

#endif // UTILITIES_IDD_IDDFILE_IMPL_HPP
