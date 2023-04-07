/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_IDDFILE_IMPL_HPP
#define UTILITIES_IDD_IDDFILE_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "IddObject.hpp"
#include "../core/Logger.hpp"

#include <string>
#include <ostream>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace detail {

  /// Implementation of IddFile
  class UTILITIES_API IddFile_Impl
  {
   public:
    /** @name Constructors */
    //@{

    /// default constructor: it's defaulted
    // IddFile_Impl() = default;

    //@}
    /** @name Getters */
    //@{

    /// get version
    std::string version() const;

    /// get build
    std::string build() const;

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

    /// Build string which might be at the top of an IddFile
    std::string m_build;

    /// The first comment block in an IddFile is its header.
    std::string m_header;

    /// The vector of IddObjects that constitute this IddFile.
    std::vector<IddObject> m_objects;

    /// Cache the Version IddObject
    mutable boost::optional<IddObject> m_versionObject;

    /// Configure logging.
    REGISTER_LOGGER("utilities.idd.IddFile");
  };

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_IDD_IDDFILE_IMPL_HPP
