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

#ifndef PROJECT_DDACEALGORITHMRECORD_HPP
#define PROJECT_DDACEALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "DakotaAlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class DDACEAlgorithm;
}
namespace project {

namespace detail {

  class DDACEAlgorithmRecord_Impl;

} // detail

/** DDACEAlgorithmRecord is a DakotaAlgorithmRecord. */
class PROJECT_API DDACEAlgorithmRecord : public DakotaAlgorithmRecord {
 public:
  typedef detail::DDACEAlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  DDACEAlgorithmRecord(const analysis::DDACEAlgorithm& ddaceAlgorithm, 
                       AnalysisRecord& analysisRecord);

  DDACEAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~DDACEAlgorithmRecord() {}

  //@}

  /** Get DDACEAlgorithmRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<DDACEAlgorithmRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                ProjectDatabase& database);

  static std::vector<DDACEAlgorithmRecord> getDDACEAlgorithmRecords(ProjectDatabase& database);

  static boost::optional<DDACEAlgorithmRecord> getDDACEAlgorithmRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::DDACEAlgorithm ddaceAlgorithm() const;

  //@}
 protected:
  /** Construct from impl. */
  DDACEAlgorithmRecord(std::shared_ptr<detail::DDACEAlgorithmRecord_Impl> impl,
                       ProjectDatabase database);

  /// @cond
  explicit DDACEAlgorithmRecord(std::shared_ptr<detail::DDACEAlgorithmRecord_Impl> impl);

  friend class detail::DDACEAlgorithmRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.DDACEAlgorithmRecord");
};

/** \relates DDACEAlgorithmRecord*/
typedef boost::optional<DDACEAlgorithmRecord> OptionalDDACEAlgorithmRecord;

/** \relates DDACEAlgorithmRecord*/
typedef std::vector<DDACEAlgorithmRecord> DDACEAlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_DDACEALGORITHMRECORD_HPP

