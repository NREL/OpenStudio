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

#ifndef PROJECT_DAKOTAALGORITHMRECORD_HPP
#define PROJECT_DAKOTAALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "AlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class DakotaAlgorithm;
} // analysis
namespace project {

class FileReferenceRecord;

namespace detail {

  class DakotaAlgorithmRecord_Impl;

} // detail

OPENSTUDIO_ENUM(DakotaAlgorithmRecordType,
  ((DDACEAlgorithmRecord)(DDACEAlgorithmRecord)(0))
  ((FSUDaceAlgorithmRecord)(FSUDaceAlgorithmRecord)(1))
  ((PSUADEDaceAlgorithmRecord)(PSUADEDaceAlgorithmRecord)(2))
  ((ParameterStudyAlgorithmRecord)(ParameterStudyAlgorithmRecord)(3))
  ((SamplingAlgorithmRecord)(SamplingAlgorithmRecord)(4))
);

/** DakotaAlgorithmRecord is a AlgorithmRecord. */
class PROJECT_API DakotaAlgorithmRecord : public AlgorithmRecord {
 public:
  typedef detail::DakotaAlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{


  virtual ~DakotaAlgorithmRecord() {}

  //@}

  /** Get DakotaAlgorithmRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<DakotaAlgorithmRecord> factoryFromQuery(const QSqlQuery& query,
                                                                 ProjectDatabase& database);

  static DakotaAlgorithmRecord factoryFromDakotaAlgorithm(
      const analysis::DakotaAlgorithm& dakotaAlgorithm, AnalysisRecord& analysisRecord);

  static std::vector<DakotaAlgorithmRecord> getDakotaAlgorithmRecords(ProjectDatabase& database);

  static boost::optional<DakotaAlgorithmRecord> getDakotaAlgorithmRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  boost::optional<FileReferenceRecord> restartFileReferenceRecord() const;

  boost::optional<FileReferenceRecord> outFileReferenceRecord() const;

  boost::optional<openstudio::UUID> jobUUID() const;

  analysis::DakotaAlgorithm dakotaAlgorithm() const;

  //@}
 protected:
  /** Construct from impl. */
  DakotaAlgorithmRecord(std::shared_ptr<detail::DakotaAlgorithmRecord_Impl> impl,
                        ProjectDatabase database,
                        const boost::optional<analysis::DakotaAlgorithm>& algorithm);

  /// @cond
  explicit DakotaAlgorithmRecord(std::shared_ptr<detail::DakotaAlgorithmRecord_Impl> impl);

  friend class detail::DakotaAlgorithmRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;
  /// @endcond

  void constructRelatedRecords(const analysis::DakotaAlgorithm& algorithm);
 private:
  REGISTER_LOGGER("openstudio.project.DakotaAlgorithmRecord");

  void setRestartFileReferenceRecordId(int id);

  void setOutFileReferenceRecordId(int id);
};

/** \relates DakotaAlgorithmRecord*/
typedef boost::optional<DakotaAlgorithmRecord> OptionalDakotaAlgorithmRecord;

/** \relates DakotaAlgorithmRecord*/
typedef std::vector<DakotaAlgorithmRecord> DakotaAlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_DAKOTAALGORITHMRECORD_HPP

