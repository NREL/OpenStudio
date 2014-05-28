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

#ifndef PROJECT_SEQUENTIALSEARCHRECORD_HPP
#define PROJECT_SEQUENTIALSEARCHRECORD_HPP

#include "ProjectAPI.hpp"
#include "OpenStudioAlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class SequentialSearch;
}
namespace project {

namespace detail {

  class SequentialSearchRecord_Impl;

} // detail

/** SequentialSearchRecord is an OpenStudioAlgorithmRecord*/
class PROJECT_API SequentialSearchRecord : public OpenStudioAlgorithmRecord {
 public:

  typedef detail::SequentialSearchRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  SequentialSearchRecord(const analysis::SequentialSearch& sequentialSearch, 
                         AnalysisRecord& analysisRecord);

  SequentialSearchRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~SequentialSearchRecord() {}

  //@}

  static boost::optional<SequentialSearchRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                  ProjectDatabase& database);

  static std::vector<SequentialSearchRecord> getSequentialSearchRecords(ProjectDatabase& database);

  static boost::optional<SequentialSearchRecord> getSequentialSearchRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::SequentialSearch sequentialSearch() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::SequentialSearchRecord_Impl;

  /** Construct from impl. */
  SequentialSearchRecord(std::shared_ptr<detail::SequentialSearchRecord_Impl> impl,
                         ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit SequentialSearchRecord(std::shared_ptr<detail::SequentialSearchRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.SequentialSearchRecord");
};

/** \relates SequentialSearchRecord*/
typedef boost::optional<SequentialSearchRecord> OptionalSequentialSearchRecord;

/** \relates SequentialSearchRecord*/
typedef std::vector<SequentialSearchRecord> SequentialSearchRecordVector;

} // project
} // openstudio

#endif // PROJECT_SEQUENTIALSEARCHRECORD_HPP

