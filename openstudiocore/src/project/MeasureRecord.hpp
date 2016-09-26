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

#ifndef PROJECT_MEASURERECORD_HPP
#define PROJECT_MEASURERECORD_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord.hpp"

#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace analysis {
  class Measure;
}
namespace project {

class MeasureGroupRecord;

namespace detail {
  class MeasureRecord_Impl;
  class MeasureGroupRecord_Impl;
} // detail


/** \class MeasureRecordType
 *
 *  \relates MeasureRecord */
OPENSTUDIO_ENUM( MeasureRecordType,
  ((NullMeasureRecord)(NullMeasureRecord)(0))
  ((RubyMeasureRecord)(RubyMeasureRecord)(1))
 );

/** \class MeasureRecordColumns
 *  \brief Column definitions for the MeasureRecords table.
 *
 *  \relates MeasureRecord */
OPENSTUDIO_ENUM(MeasureRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((measureRecordType)(INTEGER)(8))
  ((variableRecordId)(INTEGER)(9))
  ((measureVectorIndex)(INTEGER)(10))
  ((isSelected)(BOOLEAN)(11))
  ((inputFileType)(INTEGER)(12))
  ((outputFileType)(INTEGER)(13))
  ((fileReferenceRecordId)(INTEGER)(14))
  ((isUserScript)(BOOLEAN)(15))
  ((usesBCLMeasure)(BOOLEAN)(16))
);

/** A Measure is an ObjectRecord that serializes a specific value of a
 *  MeasureGroup. */
class PROJECT_API MeasureRecord : public ObjectRecord {
 public:

  typedef detail::MeasureRecord_Impl ImplType;
  typedef MeasureRecordColumns ColumnsType;
  typedef MeasureRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~MeasureRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

   /// get measure from query
  static boost::optional<MeasureRecord> factoryFromQuery(const QSqlQuery& query,
                                                         ProjectDatabase& database);

  /** Create a MeasureRecord from measure that belongs to the discrete
   *  variable serialized to measureGroupRecord. The index is the vector index of
   *  measure in the variable. */
  static MeasureRecord factoryFromMeasure(const analysis::Measure& measure,
                                          MeasureGroupRecord& measureGroupRecord,
                                          int measureVectorIndex);

  /// get all measures
  static std::vector<MeasureRecord> getMeasureRecords(ProjectDatabase& database);

  /// get measure by id
  static boost::optional<MeasureRecord> getMeasureRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  boost::optional<int> variableRecordId() const;

  /** Get the parent MeasureGroupRecord. */
  boost::optional<MeasureGroupRecord> measureGroupRecord() const;

  /// is this measure selected
  bool isSelected() const;

  /// set if this measure is selected
  bool setIsSelected(bool isSelected);

  /** Index of this Measure in its (parent) MeasureGroup's vector of measures. */
  boost::optional<int> measureVectorIndex() const;

  analysis::Measure measure() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::MeasureRecord_Impl;
  friend class detail::MeasureGroupRecord_Impl;

  /// from impl
  MeasureRecord(std::shared_ptr<detail::MeasureRecord_Impl> impl,ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit MeasureRecord(std::shared_ptr<detail::MeasureRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.MeasureRecord");
};

/** \relates MeasureRecord*/
typedef boost::optional<MeasureRecord> OptionalMeasureRecord;

/** \relates MeasureRecord*/
typedef std::vector<MeasureRecord> MeasureRecordVector;

/** Sorts MeasureRecords by measureVectorIndex. */
struct PROJECT_API MeasureRecordMeasureVectorIndexLess {
  bool operator()(const MeasureRecord& left, const MeasureRecord& right) const;
};

inline PROJECT_API bool variableRecordIdEquals(const MeasureRecord& record, int id){
  if (boost::optional<int> val = record.variableRecordId()) {
    return (*val == id);
  }
  return false;
};

} // project
} // openstudio

#endif // PROJECT_MEASURERECORD_HPP
