/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
