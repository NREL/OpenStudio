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

#ifndef PROJECT_RUBYMEASURERECORD_HPP
#define PROJECT_RUBYMEASURERECORD_HPP

#include "ProjectAPI.hpp"
#include "MeasureRecord.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional/optional.hpp>

namespace openstudio {

class FileReferenceType;

namespace analysis {
  class RubyMeasure;
}

namespace project {

class FileReferenceRecord;
class OSArgumentRecord;

namespace detail {
  class RubyMeasureRecord_Impl;
} // detail

/** RubyMeasureRecord is a MeasureRecord*/
class PROJECT_API RubyMeasureRecord : public MeasureRecord {
 public:

  typedef detail::RubyMeasureRecord_Impl ImplType;
  typedef MeasureRecordColumns ColumnsType;
  typedef MeasureRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  RubyMeasureRecord(const analysis::RubyMeasure& rubyMeasure,
                    MeasureGroupRecord& measureGroupRecord,
                    int measureVectorIndex);

  /** Constructor for RubyMeasureRecords that are resources of RubyContinuousVariables. */
  RubyMeasureRecord(const analysis::RubyMeasure& rubyMeasure,
                    ProjectDatabase& database);

  RubyMeasureRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~RubyMeasureRecord() {}

  //@}

  static boost::optional<RubyMeasureRecord> factoryFromQuery(const QSqlQuery& query,
                                                             ProjectDatabase& database);

  static std::vector<RubyMeasureRecord> getRubyMeasureRecords(ProjectDatabase& database);

  static boost::optional<RubyMeasureRecord> getRubyMeasureRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns true if this RubyMeasureRecord references a BCLMeasure. Otherwise, a bare Ruby
   *  script is referenced. */
  bool usesBCLMeasure() const;

  /** Returns the FileReferenceRecord pointing to this measure's Ruby script
   *  (if !usesBCLMeasure()), or to a re-purposed FileReferenceRecord that stores a
   *  BCLMeasure's directory path and UUIDs (if usesBCLMeasure()). */
  FileReferenceRecord fileReferenceRecord() const;

  FileReferenceType inputFileType() const;

  FileReferenceType outputFileType() const;

  /** Returns the child \link OSArgumentRecord OSArgumentRecords\endlink associated with this 
   *  RubyMeasureRecord. */
  std::vector<OSArgumentRecord> osArgumentRecords() const;

  /** Deserializes this record to an analysis::RubyMeasure. */
  analysis::RubyMeasure rubyMeasure() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::RubyMeasureRecord_Impl;

  /** Construct from impl. */
  RubyMeasureRecord(std::shared_ptr<detail::RubyMeasureRecord_Impl> impl,
                    ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit RubyMeasureRecord(std::shared_ptr<detail::RubyMeasureRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.RubyMeasureRecord");

  void constructRelatedRecords(const analysis::RubyMeasure& rubyMeasure);

  void removeOSArgumentRecords(const std::vector<UUID>& uuidsToKeep,
                               ProjectDatabase& database);
};

/** \relates RubyMeasureRecord*/
typedef boost::optional<RubyMeasureRecord> OptionalRubyMeasureRecord;

/** \relates RubyMeasureRecord*/
typedef std::vector<RubyMeasureRecord> RubyMeasureRecordVector;

} // project
} // openstudio

#endif // PROJECT_RUBYMEASURERECORD_HPP
