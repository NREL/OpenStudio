/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef PROJECT_RUBYPERTURBATIONRECORD_HPP
#define PROJECT_RUBYPERTURBATIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/DiscretePerturbationRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

namespace openstudio {

class FileReferenceType;

namespace analysis {
  class RubyPerturbation;
}

namespace project {

class FileReferenceRecord;
class OSArgumentRecord;

namespace detail {
  class RubyPerturbationRecord_Impl;
} // detail

/** RubyPerturbationRecord is a DiscretePerturbationRecord*/
class PROJECT_API RubyPerturbationRecord : public DiscretePerturbationRecord {
 public:

  typedef detail::RubyPerturbationRecord_Impl ImplType;
  typedef DiscretePerturbationRecordColumns ColumnsType;
  typedef DiscretePerturbationRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  RubyPerturbationRecord(const analysis::RubyPerturbation& rubyPerturbation,
                         DiscreteVariableRecord& discreteVariableRecord,
                         int perturbationVectorIndex);

  /** Constructor for RubyPerturbationRecords that are resources of RubyContinuousVariables. */
  RubyPerturbationRecord(const analysis::RubyPerturbation& rubyPerturbation,
                         ProjectDatabase& database);

  RubyPerturbationRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~RubyPerturbationRecord() {}

  //@}

  static boost::optional<RubyPerturbationRecord> factoryFromQuery(const QSqlQuery& query,
                                                                  ProjectDatabase& database);

  static std::vector<RubyPerturbationRecord> getRubyPerturbationRecords(ProjectDatabase& database);

  static boost::optional<RubyPerturbationRecord> getRubyPerturbationRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Returns true if this RubyPerturbationRecord references a BCLMeasure. Otherwise, a bare Ruby
   *  script is referenced. */
  bool usesBCLMeasure() const;

  /** Returns the FileReferenceRecord pointing to this perturbation's Ruby script
   *  (if !usesBCLMeasure()), or to a re-purposed FileReferenceRecord that stores a
   *  BCLMeasure's directory path and UUIDs (if usesBCLMeasure()). */
  FileReferenceRecord scriptOrBCLMeasureRecord() const;

  FileReferenceType inputFileType() const;

  FileReferenceType outputFileType() const;

  /** Returns the child \link OSArgumentRecord OSArgumentRecords\endlink associated with this 
   *  RubyPerturbationRecord. */
  std::vector<OSArgumentRecord> osArgumentRecords() const;

  /** Deserializes this record to an analysis::RubyPerturbation. */
  analysis::RubyPerturbation rubyPerturbation() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::RubyPerturbationRecord_Impl;

  /** Construct from impl. */
  RubyPerturbationRecord(boost::shared_ptr<detail::RubyPerturbationRecord_Impl> impl,
                         ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit RubyPerturbationRecord(boost::shared_ptr<detail::RubyPerturbationRecord_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.RubyPerturbationRecord");

  void constructRelatedRecords(const analysis::RubyPerturbation& rubyPerturbation);

  void removeOSArgumentRecords(const std::vector<UUID>& uuidsToKeep,
                               ProjectDatabase& database);
};

/** \relates RubyPerturbationRecord*/
typedef boost::optional<RubyPerturbationRecord> OptionalRubyPerturbationRecord;

/** \relates RubyPerturbationRecord*/
typedef std::vector<RubyPerturbationRecord> RubyPerturbationRecordVector;

} // project
} // openstudio

#endif // PROJECT_RUBYPERTURBATIONRECORD_HPP
