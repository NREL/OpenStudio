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

#ifndef PROJECT_DATAPOINT_MEASURE_JOINRECORD_HPP
#define PROJECT_DATAPOINT_MEASURE_JOINRECORD_HPP

#include "ProjectAPI.hpp"
#include "JoinRecord.hpp"

namespace openstudio {
namespace project {

class DataPointRecord;
class MeasureRecord;

namespace detail {

  class DataPoint_Measure_JoinRecord_Impl;

} // detail

/** DataPoint_Measure_JoinRecord is a JoinRecord*/
class PROJECT_API DataPoint_Measure_JoinRecord : public JoinRecord {
 public:

  typedef detail::DataPoint_Measure_JoinRecord_Impl ImplType;
  typedef JoinRecordColumns ColumnsType;

  /** @name Constructors and Destructors */
  //@{

  DataPoint_Measure_JoinRecord(const DataPointRecord& dataPointRecord,
                               const MeasureRecord& measureRecord);


  virtual ~DataPoint_Measure_JoinRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static std::vector<MeasureRecord> getMeasureRecords(const DataPointRecord& dataPointRecord);

  static std::vector<DataPointRecord> getDataPointRecords(const MeasureRecord& measureRecord);

  //@}

  DataPointRecord dataPointRecord() const;

  MeasureRecord measureRecord() const;

 protected:
  /// @cond
  friend class Record;
  friend class JoinRecord;
  friend class ProjectDatabase;
  friend class detail::DataPoint_Measure_JoinRecord_Impl;

  /** Construct from impl. */
  DataPoint_Measure_JoinRecord(
      std::shared_ptr<detail::DataPoint_Measure_JoinRecord_Impl> impl,
      ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DataPoint_Measure_JoinRecord(
      std::shared_ptr<detail::DataPoint_Measure_JoinRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.DataPoint_Measure_JoinRecord");
};

/** \relates DataPoint_Measure_JoinRecord*/
typedef boost::optional<DataPoint_Measure_JoinRecord>
    OptionalDataPoint_Measure_JoinRecord;

/** \relates DataPoint_Measure_JoinRecord*/
typedef std::vector<DataPoint_Measure_JoinRecord>
    DataPoint_Measure_JoinRecordVector;

} // project
} // openstudio

#endif // PROJECT_DATAPOINT_MEASURE_JOINRECORD_HPP

