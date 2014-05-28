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

#ifndef PROJECT_DESIGNOFEXPERIMENTSRECORD_HPP
#define PROJECT_DESIGNOFEXPERIMENTSRECORD_HPP

#include "ProjectAPI.hpp"
#include "OpenStudioAlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class DesignOfExperiments;
} // analysis
namespace project {

namespace detail {

  class DesignOfExperimentsRecord_Impl;

} // detail

/** DesignOfExperimentsRecord is an OpenStudioAlgorithmRecord*/
class PROJECT_API DesignOfExperimentsRecord : public OpenStudioAlgorithmRecord {
 public:

  typedef detail::DesignOfExperimentsRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  DesignOfExperimentsRecord(const analysis::DesignOfExperiments& designOfExperiments, 
                            AnalysisRecord& analysisRecord);

  DesignOfExperimentsRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~DesignOfExperimentsRecord() {}

  //@}

  static boost::optional<DesignOfExperimentsRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                     ProjectDatabase& database);

  static std::vector<DesignOfExperimentsRecord> getDesignOfExperimentsRecords(
      ProjectDatabase& database);

  static boost::optional<DesignOfExperimentsRecord> getDesignOfExperimentsRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::DesignOfExperiments designOfExperiments() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::DesignOfExperimentsRecord_Impl;

  /** Construct from impl. */
  DesignOfExperimentsRecord(std::shared_ptr<detail::DesignOfExperimentsRecord_Impl> impl,
                            ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DesignOfExperimentsRecord(std::shared_ptr<detail::DesignOfExperimentsRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.DesignOfExperimentsRecord");
};

/** \relates DesignOfExperimentsRecord*/
typedef boost::optional<DesignOfExperimentsRecord> OptionalDesignOfExperimentsRecord;

/** \relates DesignOfExperimentsRecord*/
typedef std::vector<DesignOfExperimentsRecord> DesignOfExperimentsRecordVector;

} // project
} // openstudio

#endif // PROJECT_DESIGNOFEXPERIMENTSRECORD_HPP
