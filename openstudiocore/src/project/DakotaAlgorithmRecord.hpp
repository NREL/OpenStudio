/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

