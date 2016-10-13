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

