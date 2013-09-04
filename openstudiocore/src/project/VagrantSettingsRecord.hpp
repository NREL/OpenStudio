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

#ifndef PROJECT_VAGRANTSETTINGSRECORD_HPP
#define PROJECT_VAGRANTSETTINGSRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/CloudSettingsRecord.hpp>

namespace openstudio {
namespace NAMESPACE {
  class VagrantSettings;
}
namespace project {

namespace detail {

  class VagrantSettingsRecord_Impl;

} // detail

// TODO: Populate or delete this enumeration if there are/are not any derived types, respectively.
/** \class VagrantSettingsRecordType
 *  \brief ObjectRecord types that derive from VagrantSettingsRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(VagrantSettingsRecordType,
    ((VagrantSettingsRecordDerivedRecord1))
);
 *  \endcode */
OPENSTUDIO_ENUM(VagrantSettingsRecordType,
    ((VagrantSettingsRecordDerivedRecord1))
);

/** VagrantSettingsRecord is a CloudSettingsRecord. */
class PROJECT_API VagrantSettingsRecord : public CloudSettingsRecord {
 public:

  typedef detail::VagrantSettingsRecord_Impl ImplType;
  // TODO: Check this typedef. The generator script assumes that the hierarchy is only two deep   // from ObjectRecord.
  typedef CloudSettingsRecordColumns ColumnsType;
  // TODO: Check this typedef too.
  typedef CloudSettingsRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  // TODO: Delete if VagrantSettings is abstract, make private if VagrantSettings is concrete and has derived classes.
  // TODO: Replace ProjectDatabase& database (or add another object if it is ok for VagrantSettingsRecord to be and orphan) with const& to parent Record if the Table contains a parent id.
  // TODO: Find-replace on 'NAMESPACE'.
  VagrantSettingsRecord(const NAMESPACE::VagrantSettings& vagrantSettings, ProjectDatabase& database);

  // TODO: Delete if VagrantSettings is abstract, make private if VagrantSettings is concrete and has derived classes.
  VagrantSettingsRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~VagrantSettingsRecord() {}

  //@}

  /** Get VagrantSettingsRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<VagrantSettingsRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  // TODO: Delete if no derived classes.
  static VagrantSettingsRecord factoryFromVagrantSettings(const NAMESPACE::VagrantSettings& vagrantSettings, ProjectDatabase& database);

  static std::vector<VagrantSettingsRecord> getVagrantSettingsRecords(ProjectDatabase& database);

  static boost::optional<VagrantSettingsRecord> getVagrantSettingsRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  // ADD METHODS FOR RETRIEVING PARENT, CHILD, AND RESOURCE RECORDS AS DESIRED

  // ADD METHODS FOR GETTING/SETTING SPECIFIC DATA FIELDS AS DESIRED

  NAMESPACE::VagrantSettings vagrantSettings() const;

  //@}
 protected:
  /// @cond
  typedef detail::VagrantSettingsRecord_Impl ImplType;

  explicit VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl);

  friend class detail::VagrantSettingsRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl,
                        ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.VagrantSettingsRecord");
};

/** \relates VagrantSettingsRecord*/
typedef boost::optional<VagrantSettingsRecord> OptionalVagrantSettingsRecord;

/** \relates VagrantSettingsRecord*/
typedef std::vector<VagrantSettingsRecord> VagrantSettingsRecordVector;

} // project
} // openstudio

#endif // PROJECT_VAGRANTSETTINGSRECORD_HPP

