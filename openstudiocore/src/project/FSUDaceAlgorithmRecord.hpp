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

#ifndef PROJECT_FSUDACEALGORITHMRECORD_HPP
#define PROJECT_FSUDACEALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "DakotaAlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class FSUDaceAlgorithm;
}
namespace project {

namespace detail {

  class FSUDaceAlgorithmRecord_Impl;

} // detail

/** FSUDaceAlgorithmRecord is a DakotaAlgorithmRecord. */
class PROJECT_API FSUDaceAlgorithmRecord : public DakotaAlgorithmRecord {
 public:
  typedef detail::FSUDaceAlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  FSUDaceAlgorithmRecord(const analysis::FSUDaceAlgorithm& fsudaceAlgorithm,
                         AnalysisRecord& analysisRecord);

  FSUDaceAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~FSUDaceAlgorithmRecord() {}

  //@}

  /** Get FSUDaceAlgorithmRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<FSUDaceAlgorithmRecord> factoryFromQuery(const QSqlQuery& query,
                                                                  ProjectDatabase& database);

  static std::vector<FSUDaceAlgorithmRecord> getFSUDaceAlgorithmRecords(ProjectDatabase& database);

  static boost::optional<FSUDaceAlgorithmRecord> getFSUDaceAlgorithmRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::FSUDaceAlgorithm fsudaceAlgorithm() const;

  //@}
 protected:
  /** Construct from impl. */
  FSUDaceAlgorithmRecord(std::shared_ptr<detail::FSUDaceAlgorithmRecord_Impl> impl,
                         ProjectDatabase database);

  /// @cond
  explicit FSUDaceAlgorithmRecord(std::shared_ptr<detail::FSUDaceAlgorithmRecord_Impl> impl);

  friend class detail::FSUDaceAlgorithmRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.FSUDaceAlgorithmRecord");
};

/** \relates FSUDaceAlgorithmRecord*/
typedef boost::optional<FSUDaceAlgorithmRecord> OptionalFSUDaceAlgorithmRecord;

/** \relates FSUDaceAlgorithmRecord*/
typedef std::vector<FSUDaceAlgorithmRecord> FSUDaceAlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_FSUDACEALGORITHMRECORD_HPP

