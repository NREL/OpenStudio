/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_BCL_BCLFILEREFERENCE_HPP
#define UTILITIES_BCL_BCLFILEREFERENCE_HPP

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../UtilitiesAPI.hpp"

namespace pugi {
class xml_node;
}

namespace Json {
class Value;
}

namespace openstudio {

/** BCLFileReference is a class for tracking files that come with BCL components and measures.
  **/
class UTILITIES_API BCLFileReference
{

 public:
  /** @name Constructors */
  //@{

  /// Constructor from file path.
  explicit BCLFileReference(const openstudio::path& measureRootDir, const openstudio::path& relativePath, const bool setMembers = false);

  //@}
  /** @name Destructor */
  //@{

  /// Virtual destructor
  virtual ~BCLFileReference() = default;

  //@}
  /** @name Getters */
  //@{

  /// Returns absolute path to file.
  openstudio::path path() const;

  // Returns path to file, relative to measure root directory (including subdirectory, eg 'docs/subfolder/docs.rb')
  openstudio::path relativePath() const;

  /// Returns the last recorded checksum.
  std::string checksum() const;

  std::string softwareProgram() const;

  std::string softwareProgramVersion() const;

  boost::optional<VersionString> minCompatibleVersion() const;

  boost::optional<VersionString> maxCompatibleVersion() const;

  // This returns filename to write to the XML. It omits the subdirectory based on usageType
  // eg: a file at docs/subfolder/docs.rb has a usageType='doc' and filename() = 'subfolder/docs.rb'
  std::string fileName() const;

  std::string fileType() const;

  std::string usageType() const;

  //@}
  /** @name Setters */
  //@{

  void setChecksum(const std::string& checksum);

  void setSoftwareProgram(const std::string& softwareProgram);

  void setSoftwareProgramVersion(const std::string& softwareProgramVersion);

  void setMinCompatibleVersion(const VersionString& minCompatibleVersion);

  void resetMinCompatibleVersion();

  void setMaxCompatibleVersion(const VersionString& maxCompatibleVersion);

  void resetMaxCompatibleVersion();

  void setUsageType(const std::string& usageType);

  //@}
  /** @name Operators */
  //@{

  void writeValues(pugi::xml_node& element) const;

  Json::Value toJSON() const;
  std::string toJSONString() const;

  /// Check if the file has been updated and return if so.  Will update checksum.
  bool checkForUpdate();

  //@}

 protected:
  // Declaring the equality operator and the spaceship operator (three-way comparison operator) will end up defining all comparison operators
  // We really need only the operator< for sorting a STL container of BCLFileReference, but might as well be consistent
  // Comparison is done on m_path. Sorting is useful for BCLXML to avoid reordering of files (see #4748)
  // TODO: compiler/SWIG support still seems too sparse (need GCC 10+, Apple Clang 13 at least), let's avoid it for now
  // friend bool operator==(const BCLFileReference& lhs, const BCLFileReference& rhs);
  // friend std::strong_ordering operator<=>(const BCLFileReference& lhs, const BCLFileReference& rhs);
  friend inline bool operator==(const BCLFileReference& lhs, const BCLFileReference& rhs) {
    return lhs.m_path == rhs.m_path;
  }
  friend inline bool operator<(const BCLFileReference& lhs, const BCLFileReference& rhs) {
    return lhs.m_path < rhs.m_path;
  }

 private:
  // configure logging
  REGISTER_LOGGER("utilities.bcl.BCLFileReference");

  openstudio::path m_measureRootDir;
  openstudio::path m_path;
  std::string m_checksum;
  std::string m_softwareProgram;
  std::string m_softwareProgramVersion;
  boost::optional<VersionString> m_minCompatibleVersion;
  boost::optional<VersionString> m_maxCompatibleVersion;
  std::string m_fileName;
  std::string m_fileType;
  std::string m_usageType;
};

/** Prints BCLFileReference to os. \relates BCLFileReference */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const BCLFileReference& file);

inline bool operator!=(const BCLFileReference& lhs, const BCLFileReference& rhs) {
  return !operator==(lhs, rhs);
}
inline bool operator>(const BCLFileReference& lhs, const BCLFileReference& rhs) {
  return operator<(rhs, lhs);
}
inline bool operator<=(const BCLFileReference& lhs, const BCLFileReference& rhs) {
  return !operator>(lhs, rhs);
}
inline bool operator>=(const BCLFileReference& lhs, const BCLFileReference& rhs) {
  return !operator<(lhs, rhs);
}

}  // namespace openstudio

#endif  // UTILITIES_BCL_BCLFILEREFERENCE_HPP
