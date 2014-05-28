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

#ifndef PROJECT_PARAMETERSTUDYALGORITHMRECORD_HPP
#define PROJECT_PARAMETERSTUDYALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "DakotaAlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class ParameterStudyAlgorithm;
}
namespace project {

namespace detail {

  class ParameterStudyAlgorithmRecord_Impl;

} // detail

/** ParameterStudyAlgorithmRecord is a DakotaAlgorithmRecord. */
class PROJECT_API ParameterStudyAlgorithmRecord : public DakotaAlgorithmRecord {
 public:
  typedef detail::ParameterStudyAlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  ParameterStudyAlgorithmRecord(const analysis::ParameterStudyAlgorithm& parameterStudyAlgorithm,
                                AnalysisRecord& analysisRecord);

  ParameterStudyAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~ParameterStudyAlgorithmRecord() {}

  //@}

  /** Get ParameterStudyAlgorithmRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<ParameterStudyAlgorithmRecord> factoryFromQuery(const QSqlQuery& query,
                                                                         ProjectDatabase& database);

  static std::vector<ParameterStudyAlgorithmRecord> getParameterStudyAlgorithmRecords(
      ProjectDatabase& database);

  static boost::optional<ParameterStudyAlgorithmRecord> getParameterStudyAlgorithmRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */

  //@{

  analysis::ParameterStudyAlgorithm parameterStudyAlgorithm() const;

  //@}
 protected:
  /** Construct from impl. */
  ParameterStudyAlgorithmRecord(std::shared_ptr<detail::ParameterStudyAlgorithmRecord_Impl> impl,
                         ProjectDatabase database);

  /// @cond
  explicit ParameterStudyAlgorithmRecord(
      std::shared_ptr<detail::ParameterStudyAlgorithmRecord_Impl> impl);

  friend class detail::ParameterStudyAlgorithmRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.ParameterStudyAlgorithmRecord");
};

/** \relates ParameterStudyAlgorithmRecord*/
typedef boost::optional<ParameterStudyAlgorithmRecord> OptionalParameterStudyAlgorithmRecord;

/** \relates ParameterStudyAlgorithmRecord*/
typedef std::vector<ParameterStudyAlgorithmRecord> ParameterStudyAlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_PARAMETERSTUDYALGORITHMRECORD_HPP

