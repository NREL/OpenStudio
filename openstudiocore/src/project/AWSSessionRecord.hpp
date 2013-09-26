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

#ifndef PROJECT_AWSSESSIONRECORD_HPP
#define PROJECT_AWSSESSIONRECORD_HPP

#include <project/ProjectAPI.hpp>
#include <project/CloudSessionRecord.hpp>

namespace openstudio {
namespace NAMESPACE {
  class AWSSession;
}
namespace project {

namespace detail {

  class AWSSessionRecord_Impl;

} // detail

// TODO: Populate or delete this enumeration if there are/are not any derived types, respectively.
/** \class AWSSessionRecordType
 *  \brief ObjectRecord types that derive from AWSSessionRecord.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 *  macro call is:
 *  \code
OPENSTUDIO_ENUM(AWSSessionRecordType,
    ((AWSSessionRecordDerivedRecord1))
);
 *  \endcode */
OPENSTUDIO_ENUM(AWSSessionRecordType,
    ((AWSSessionRecordDerivedRecord1))
);

/** AWSSessionRecord is a CloudSessionRecord. */
class PROJECT_API AWSSessionRecord : public CloudSessionRecord {
 public:

  typedef detail::AWSSessionRecord_Impl ImplType;
  // TODO: Check this typedef. The generator script assumes that the hierarchy is only two deep   // from ObjectRecord.
  typedef CloudSessionRecordColumns ColumnsType;
  // TODO: Check this typedef too.
  typedef CloudSessionRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  // TODO: Delete if AWSSession is abstract, make private if AWSSession is concrete and has derived classes.
  // TODO: Replace ProjectDatabase& database (or add another object if it is ok for AWSSessionRecord to be and orphan) with const& to parent Record if the Table contains a parent id.
  // TODO: Find-replace on 'NAMESPACE'.
  AWSSessionRecord(const NAMESPACE::AWSSession& aWSSession, ProjectDatabase& database);

  // TODO: Delete if AWSSession is abstract, make private if AWSSession is concrete and has derived classes.
  AWSSessionRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~AWSSessionRecord() {}

  //@}

  /** Get AWSSessionRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<AWSSessionRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

  // TODO: Delete if no derived classes.
  static AWSSessionRecord factoryFromAWSSession(const NAMESPACE::AWSSession& aWSSession, ProjectDatabase& database);

  static std::vector<AWSSessionRecord> getAWSSessionRecords(ProjectDatabase& database);

  static boost::optional<AWSSessionRecord> getAWSSessionRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  // ADD METHODS FOR RETRIEVING PARENT, CHILD, AND RESOURCE RECORDS AS DESIRED

  // ADD METHODS FOR GETTING/SETTING SPECIFIC DATA FIELDS AS DESIRED

  NAMESPACE::AWSSession aWSSession() const;

  //@}
 protected:
  /// @cond
  typedef detail::AWSSessionRecord_Impl ImplType;

  explicit AWSSessionRecord(boost::shared_ptr<detail::AWSSessionRecord_Impl> impl);

  friend class detail::AWSSessionRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /** Construct from impl. */
  AWSSessionRecord(boost::shared_ptr<detail::AWSSessionRecord_Impl> impl,
                   ProjectDatabase database);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.AWSSessionRecord");
};

/** \relates AWSSessionRecord*/
typedef boost::optional<AWSSessionRecord> OptionalAWSSessionRecord;

/** \relates AWSSessionRecord*/
typedef std::vector<AWSSessionRecord> AWSSessionRecordVector;

} // project
} // openstudio

#endif // PROJECT_AWSSESSIONRECORD_HPP

