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

#ifndef PROJECT_CLOUDSETTINGSRECORD_HPP
#define PROJECT_CLOUDSETTINGSRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/ObjectRecord.hpp>

namespace openstudio {
namespace NAMESPACE {
  class CloudSettings;
}
namespace project {

namespace detail {

  class CloudSettingsRecord_Impl;

} // detail

// TODO: Populate or delete this enumeration if there are/are not any derived types, respectively.
/** \class CloudSettingsRecordType
 *  \brief ObjectRecord types that derive from CloudSettingsRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(CloudSettingsRecordType,
    ((CloudSettingsRecordDerivedRecord1))
);
 *  \endcode */
OPENSTUDIO_ENUM(CloudSettingsRecordType,
    ((CloudSettingsRecordDerivedRecord1))
);

/** \class CloudSettingsRecordColumns
 *  \brief Column definitions for the CloudSettingsRecords table.
 *
 *  \relates CloudSettingsRecord */
OPENSTUDIO_ENUM(CloudSettingsRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  // TODO: Add Columns to Record Class (and Derived-Class)-Specific Data.
  ((cloudSettingsRecordType)(INTEGER)(8))
);

/** CloudSettingsRecord is a ObjectRecord. */
class PROJECT_API CloudSettingsRecord : public ObjectRecord {
 public:

  typedef detail::CloudSettingsRecord_Impl ImplType;
  typedef CloudSettingsRecordColumns ColumnsType;
  typedef CloudSettingsRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  // TODO: Delete if CloudSettings is abstract, make private if CloudSettings is concrete and has derived classes.
  // TODO: Replace ProjectDatabase& database (or add another object if it is ok for CloudSettingsRecord to be and orphan) with const& to parent Record if the Table contains a parent id.
  // TODO: Find-replace on 'NAMESPACE'.
  CloudSettingsRecord(const NAMESPACE::CloudSettings& cloudSettings, ProjectDatabase& database);

  // TODO: Delete if CloudSettings is abstract, make private if CloudSettings is concrete and has derived classes.
  CloudSettingsRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~CloudSettingsRecord() {}

  //@}

  // TODO: Add a call to createTable in ProjectDatabase_Impl::initialize().
  static std::string databaseTableName();

  // TODO: Add a call to this updatePathData method in ProjectDatabase_Impl::updatePathData.
  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get CloudSettingsRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<CloudSettingsRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  // TODO: Delete if no derived classes.
  static CloudSettingsRecord factoryFromCloudSettings(const NAMESPACE::CloudSettings& cloudSettings, ProjectDatabase& database);

  static std::vector<CloudSettingsRecord> getCloudSettingsRecords(ProjectDatabase& database);

  static boost::optional<CloudSettingsRecord> getCloudSettingsRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  // ADD METHODS FOR RETRIEVING PARENT, CHILD, AND RESOURCE RECORDS AS DESIRED

  // ADD METHODS FOR GETTING/SETTING SPECIFIC DATA FIELDS AS DESIRED

  NAMESPACE::CloudSettings cloudSettings() const;

  //@}
 protected:
  /// @cond
  typedef detail::CloudSettingsRecord_Impl ImplType;

  explicit CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl> impl);

  friend class detail::CloudSettingsRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  CloudSettingsRecord(boost::shared_ptr<detail::CloudSettingsRecord_Impl> impl,
                      ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.CloudSettingsRecord");
};

/** \relates CloudSettingsRecord*/
typedef boost::optional<CloudSettingsRecord> OptionalCloudSettingsRecord;

/** \relates CloudSettingsRecord*/
typedef std::vector<CloudSettingsRecord> CloudSettingsRecordVector;

} // project
} // openstudio

#endif // PROJECT_CLOUDSETTINGSRECORD_HPP

