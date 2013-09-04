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
namespace NAMESPACE {
  class VagrantSession;
}
namespace project {

namespace detail {

  class VagrantSessionRecord_Impl;

} // detail

// TODO: Populate or delete this enumeration if there are/are not any derived types, respectively.
/** \class VagrantSessionRecordType
 *  \brief ObjectRecord types that derive from VagrantSessionRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(VagrantSessionRecordType,
    ((VagrantSessionRecordDerivedRecord1))
);
 *  \endcode */
OPENSTUDIO_ENUM(VagrantSessionRecordType,
    ((VagrantSessionRecordDerivedRecord1))
);

/** VagrantSessionRecord is a CloudSessionRecord. */
class PROJECT_API VagrantSessionRecord : public CloudSessionRecord {
 public:

  typedef detail::VagrantSessionRecord_Impl ImplType;
  // TODO: Check this typedef. The generator script assumes that the hierarchy is only two deep   // from ObjectRecord.
  typedef CloudSessionRecordColumns ColumnsType;
  // TODO: Check this typedef too.
  typedef CloudSessionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  // TODO: Delete if VagrantSession is abstract, make private if VagrantSession is concrete and has derived classes.
  // TODO: Replace ProjectDatabase& database (or add another object if it is ok for VagrantSessionRecord to be and orphan) with const& to parent Record if the Table contains a parent id.
  // TODO: Find-replace on 'NAMESPACE'.
  VagrantSessionRecord(const NAMESPACE::VagrantSession& vagrantSession, ProjectDatabase& database);

  // TODO: Delete if VagrantSession is abstract, make private if VagrantSession is concrete and has derived classes.
  VagrantSessionRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~VagrantSessionRecord() {}

  //@}

  /** Get VagrantSessionRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<VagrantSessionRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  // TODO: Delete if no derived classes.
  static VagrantSessionRecord factoryFromVagrantSession(const NAMESPACE::VagrantSession& vagrantSession, ProjectDatabase& database);

  static std::vector<VagrantSessionRecord> getVagrantSessionRecords(ProjectDatabase& database);

  static boost::optional<VagrantSessionRecord> getVagrantSessionRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  // ADD METHODS FOR RETRIEVING PARENT, CHILD, AND RESOURCE RECORDS AS DESIRED

  // ADD METHODS FOR GETTING/SETTING SPECIFIC DATA FIELDS AS DESIRED

  NAMESPACE::VagrantSession vagrantSession() const;

  //@}
 protected:
  /// @cond
  typedef detail::VagrantSessionRecord_Impl ImplType;

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

