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

class VagrantSettings;

namespace project {

class UrlRecord;

namespace detail {

  class VagrantSettingsRecord_Impl;

} // detail

/** VagrantSettingsRecord is a CloudSettingsRecord. */
class PROJECT_API VagrantSettingsRecord : public CloudSettingsRecord {
 public:

  typedef detail::VagrantSettingsRecord_Impl ImplType;
  typedef CloudSettingsRecordColumns ColumnsType;
  typedef CloudSettingsRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  VagrantSettingsRecord(const VagrantSettings& vagrantSettings, ProjectDatabase& database);

  VagrantSettingsRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~VagrantSettingsRecord() {}

  //@}

  /** Get VagrantSettingsRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<VagrantSettingsRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<VagrantSettingsRecord> getVagrantSettingsRecords(ProjectDatabase& database);

  static boost::optional<VagrantSettingsRecord> getVagrantSettingsRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  UrlRecord serverUrlRecord() const;

  UrlRecord workerUrlRecord() const;

  VagrantSettings vagrantSettings() const;

  //@}
 protected:
  /// @cond
  explicit VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl);

  friend class detail::VagrantSettingsRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  VagrantSettingsRecord(boost::shared_ptr<detail::VagrantSettingsRecord_Impl> impl,
                        ProjectDatabase database);

  /// @endcond

  void constructRelatedRecords(const VagrantSettings& vagrantSettings, ProjectDatabase& database);

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

