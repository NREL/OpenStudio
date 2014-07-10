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

#ifndef PROJECT_ALGORITHMRECORD_HPP
#define PROJECT_ALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

namespace openstudio {
namespace analysis {
  class Algorithm;
} // analysis
namespace project {

class AnalysisRecord;

namespace detail {
  class AlgorithmRecord_Impl;
} // detail

/** \class AlgorithmRecordType
 *
 *  \relates AlgorithmRecord */
OPENSTUDIO_ENUM(AlgorithmRecordType,
                ((OpenStudioAlgorithmRecord)(OpenStudioAlgorithmRecord)(0))
                ((DakotaAlgorithmRecord)(DakotaAlgorithmRecord)(1))
                );

/** \class AlgorithmRecordColumns
 *  \brief Column definitions for the AlgorithmRecords table.
 *
 *  \relates AlgorithmRecord */
OPENSTUDIO_ENUM(AlgorithmRecordColumns,
                ((id)(INTEGER PRIMARY KEY)(0))
                ((handle)(TEXT)(1))
                ((name)(TEXT)(2))
                ((displayName)(TEXT)(3))
                ((description)(TEXT)(4))
                ((timestampCreate)(TEXT)(5))
                ((timestampLast)(TEXT)(6))
                ((uuidLast)(TEXT)(7))
                ((iter)(INTEGER)(8))
                ((algorithmRecordType)(INTEGER)(9))
                ((openStudioAlgorithmRecordType)(INTEGER)(10))
                ((dakotaAlgorithmRecordType)(INTEGER)(11))
                ((designOfExperimentsType)(INTEGER)(12))
                ((ddaceAlgorithmType)(INTEGER)(13))
                ((fsudaceAlgorithmType)(INTEGER)(14))
                ((fsudaceTrialType)(INTEGER)(15))
                ((parameterStudyAlgorithmType)(INTEGER)(16))
                ((samplingAlgorithmSampleType)(INTEGER)(17))
                ((samplingAlgorithmRNGType)(INTEGER)(18))
                ((complete)(BOOLEAN)(19))
                ((failed)(BOOLEAN)(20))
                ((dakotaRestartFileRecordId)(INTEGER)(21))
                ((dakotaOutFileRecordId)(INTEGER)(22))
                ((jobUUID)(TEXT)(23))
                );

/** AlgorithmRecord is a ObjectRecord*/
class PROJECT_API AlgorithmRecord : public ObjectRecord {
 public:

  typedef detail::AlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~AlgorithmRecord() {}

  //@}

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

  /** Get AlgorithmRecord from query. Returned object will be of the correct derived type. */
  static boost::optional<AlgorithmRecord> factoryFromQuery(const QSqlQuery& query,
                                                           ProjectDatabase& database);

  static AlgorithmRecord factoryFromAlgorithm(const analysis::Algorithm& algorithm,
                                              AnalysisRecord& analysisRecord);

  static std::vector<AlgorithmRecord> getAlgorithmRecords(ProjectDatabase& database);

  static boost::optional<AlgorithmRecord> getAlgorithmRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns the parent AnalysisRecord of this AlgorithmRecord. */
  AnalysisRecord analysisRecord() const;

  bool isComplete() const;

  /** Deserialize to analysis::Algorithm. */
  analysis::Algorithm algorithm() const;

  //@}
  /** @name Setters */
  //@{

  /** Resets algorithm back to its initial state. Provided for callers operating
   *  directly on the database, not holding a copy of this analysis in memory. Use
   *  with caution. Does not do file system cleanup. */
  void reset();

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::AlgorithmRecord_Impl;

  /** Construct from impl. */
  AlgorithmRecord(std::shared_ptr<detail::AlgorithmRecord_Impl> impl,
                  ProjectDatabase database,
                  const boost::optional<analysis::Algorithm>& algorithm);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit AlgorithmRecord(std::shared_ptr<detail::AlgorithmRecord_Impl> impl);

  /// @endcond

  void constructRelatedRecords(const analysis::Algorithm& algorithm);

 private:
  REGISTER_LOGGER("openstudio.project.AlgorithmRecord");
};

/** \relates AlgorithmRecord*/
typedef boost::optional<AlgorithmRecord> OptionalAlgorithmRecord;

/** \relates AlgorithmRecord*/
typedef std::vector<AlgorithmRecord> AlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_ALGORITHMRECORD_HPP

