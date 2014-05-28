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

#ifndef PROJECT_SAMPLINGALGORITHMRECORD_HPP
#define PROJECT_SAMPLINGALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "DakotaAlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class SamplingAlgorithm;
}
namespace project {

namespace detail {

  class SamplingAlgorithmRecord_Impl;

} // detail

/** SamplingAlgorithmRecord is a DakotaAlgorithmRecord. */
class PROJECT_API SamplingAlgorithmRecord : public DakotaAlgorithmRecord {
 public:
  typedef detail::SamplingAlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  SamplingAlgorithmRecord(const analysis::SamplingAlgorithm& samplingAlgorithm,
                         AnalysisRecord& analysisRecord);

  SamplingAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~SamplingAlgorithmRecord() {}

  //@}

  /** Get SamplingAlgorithmRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<SamplingAlgorithmRecord> factoryFromQuery(const QSqlQuery& query,
                                                                  ProjectDatabase& database);

  static std::vector<SamplingAlgorithmRecord> getSamplingAlgorithmRecords(ProjectDatabase& database);

  static boost::optional<SamplingAlgorithmRecord> getSamplingAlgorithmRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::SamplingAlgorithm samplingAlgorithm() const;

  //@}
 protected:
  /** Construct from impl. */
  SamplingAlgorithmRecord(std::shared_ptr<detail::SamplingAlgorithmRecord_Impl> impl,
                         ProjectDatabase database);

  /// @cond
  explicit SamplingAlgorithmRecord(std::shared_ptr<detail::SamplingAlgorithmRecord_Impl> impl);

  friend class detail::SamplingAlgorithmRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.SamplingAlgorithmRecord");
};

/** \relates SamplingAlgorithmRecord*/
typedef boost::optional<SamplingAlgorithmRecord> OptionalSamplingAlgorithmRecord;

/** \relates SamplingAlgorithmRecord*/
typedef std::vector<SamplingAlgorithmRecord> SamplingAlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_SAMPLINGALGORITHMRECORD_HPP

