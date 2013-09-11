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

#ifndef PROJECT_VAGRANTSESSIONRECORD_HPP
#define PROJECT_VAGRANTSESSIONRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/CloudSessionRecord.hpp>

namespace openstudio {

class VagrantSession;

namespace project {

namespace detail {

  class VagrantSessionRecord_Impl;

} // detail

/** VagrantSessionRecord is a CloudSessionRecord. */
class PROJECT_API VagrantSessionRecord : public CloudSessionRecord {
 public:

  typedef detail::VagrantSessionRecord_Impl ImplType;
  typedef CloudSessionRecordColumns ColumnsType;
  typedef CloudSessionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  VagrantSessionRecord(const VagrantSession& vagrantSession, ProjectDatabase& database);

  VagrantSessionRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~VagrantSessionRecord() {}

  //@}

  /** Get VagrantSessionRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<VagrantSessionRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  static std::vector<VagrantSessionRecord> getVagrantSessionRecords(ProjectDatabase& database);

  static boost::optional<VagrantSessionRecord> getVagrantSessionRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  VagrantSession vagrantSession() const;

  //@}
 protected:
  /// @cond
  explicit VagrantSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl> impl);

  friend class detail::VagrantSessionRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  VagrantSessionRecord(boost::shared_ptr<detail::VagrantSessionRecord_Impl> impl,
                       ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.VagrantSessionRecord");
};

/** \relates VagrantSessionRecord*/
typedef boost::optional<VagrantSessionRecord> OptionalVagrantSessionRecord;

/** \relates VagrantSessionRecord*/
typedef std::vector<VagrantSessionRecord> VagrantSessionRecordVector;

} // project
} // openstudio

#endif // PROJECT_VAGRANTSESSIONRECORD_HPP

