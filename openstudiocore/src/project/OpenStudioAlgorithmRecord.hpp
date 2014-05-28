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

#ifndef PROJECT_OPENSTUDIOALGORITHMRECORD_HPP
#define PROJECT_OPENSTUDIOALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "AlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class OpenStudioAlgorithm;
}

namespace project {

namespace detail {

  class OpenStudioAlgorithmRecord_Impl;

} // detail

/** \class OpenStudioAlgorithmRecordType 
 *
 *  \relates OpenStudioAlgorithmRecord */
OPENSTUDIO_ENUM(OpenStudioAlgorithmRecordType,
  ((DesignOfExperimentsRecord)(DesignOfExperimentsRecord)(0))
  ((SequentialSearchRecord)(SequentialSearchRecord)(1))
);

/** OpenStudioAlgorithmRecord is a AlgorithmRecord. */
class PROJECT_API OpenStudioAlgorithmRecord : public AlgorithmRecord {
 public:
  typedef detail::OpenStudioAlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~OpenStudioAlgorithmRecord() {}

  //@}

  /** Get OpenStudioAlgorithmRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<OpenStudioAlgorithmRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                     ProjectDatabase& database);

  static OpenStudioAlgorithmRecord factoryFromOpenStudioAlgorithm(
      const analysis::OpenStudioAlgorithm& openStudioAlgorithm, AnalysisRecord& analysisRecord);

  static std::vector<OpenStudioAlgorithmRecord> getOpenStudioAlgorithmRecords(ProjectDatabase& database);

  static boost::optional<OpenStudioAlgorithmRecord> getOpenStudioAlgorithmRecord(int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::OpenStudioAlgorithm openStudioAlgorithm() const;

  //@}
 protected:
  /// @cond

  /** Construct from impl. */
  OpenStudioAlgorithmRecord(std::shared_ptr<detail::OpenStudioAlgorithmRecord_Impl> impl,
                            ProjectDatabase database,
                            const boost::optional<analysis::OpenStudioAlgorithm>& algorithm);

  explicit OpenStudioAlgorithmRecord(std::shared_ptr<detail::OpenStudioAlgorithmRecord_Impl> impl);

  friend class detail::OpenStudioAlgorithmRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.OpenStudioAlgorithmRecord");
};

/** \relates OpenStudioAlgorithmRecord*/
typedef boost::optional<OpenStudioAlgorithmRecord> OptionalOpenStudioAlgorithmRecord;

/** \relates OpenStudioAlgorithmRecord*/
typedef std::vector<OpenStudioAlgorithmRecord> OpenStudioAlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_OPENSTUDIOALGORITHMRECORD_HPP

