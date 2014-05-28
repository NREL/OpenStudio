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

#ifndef PROJECT_PSUADEDACEALGORITHMRECORD_HPP
#define PROJECT_PSUADEDACEALGORITHMRECORD_HPP

#include "ProjectAPI.hpp"
#include "DakotaAlgorithmRecord.hpp"

namespace openstudio {
namespace analysis {
  class PSUADEDaceAlgorithm;
}
namespace project {

namespace detail {

  class PSUADEDaceAlgorithmRecord_Impl;

} // detail

/** PSUADEDaceAlgorithmRecord is a DakotaAlgorithmRecord. */
class PROJECT_API PSUADEDaceAlgorithmRecord : public DakotaAlgorithmRecord {
 public:
  typedef detail::PSUADEDaceAlgorithmRecord_Impl ImplType;
  typedef AlgorithmRecordColumns ColumnsType;
  typedef AlgorithmRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  PSUADEDaceAlgorithmRecord(const analysis::PSUADEDaceAlgorithm& psuadedaceAlgorithm,
                         AnalysisRecord& analysisRecord);

  PSUADEDaceAlgorithmRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~PSUADEDaceAlgorithmRecord() {}

  //@}

  /** Get PSUADEDaceAlgorithmRecord from query. Returned object will be of the correct
   *  derived type. */
  static boost::optional<PSUADEDaceAlgorithmRecord> factoryFromQuery(const QSqlQuery& query,
                                                                  ProjectDatabase& database);

  static std::vector<PSUADEDaceAlgorithmRecord> getPSUADEDaceAlgorithmRecords(ProjectDatabase& database);

  static boost::optional<PSUADEDaceAlgorithmRecord> getPSUADEDaceAlgorithmRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  analysis::PSUADEDaceAlgorithm psuadedaceAlgorithm() const;

  //@}
 protected:
  /** Construct from impl. */
  PSUADEDaceAlgorithmRecord(std::shared_ptr<detail::PSUADEDaceAlgorithmRecord_Impl> impl,
                         ProjectDatabase database);

  /// @cond
  explicit PSUADEDaceAlgorithmRecord(std::shared_ptr<detail::PSUADEDaceAlgorithmRecord_Impl> impl);

  friend class detail::PSUADEDaceAlgorithmRecord_Impl;
  friend class Record;
  friend class ProjectDatabase;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.project.PSUADEDaceAlgorithmRecord");
};

/** \relates PSUADEDaceAlgorithmRecord*/
typedef boost::optional<PSUADEDaceAlgorithmRecord> OptionalPSUADEDaceAlgorithmRecord;

/** \relates PSUADEDaceAlgorithmRecord*/
typedef std::vector<PSUADEDaceAlgorithmRecord> PSUADEDaceAlgorithmRecordVector;

} // project
} // openstudio

#endif // PROJECT_PSUADEDACEALGORITHMRECORD_HPP

