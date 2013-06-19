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

#ifndef PROJECT_MODELOBJECTPERTURBATIONRECORD_HPP
#define PROJECT_MODELOBJECTPERTURBATIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/DiscretePerturbationRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

namespace openstudio {

namespace analysis {
  class ModelRulesetPerturbation;
}

namespace project {

class ModelRulesetRecord;

namespace detail {
  class ModelRulesetPerturbationRecord_Impl;
} // detail

/** ModelRulesetPerturbationRecord is a DiscretePerturbationRecord*/
class PROJECT_API ModelRulesetPerturbationRecord : public DiscretePerturbationRecord {
 public:
  typedef detail::ModelRulesetPerturbationRecord_Impl ImplType;
  typedef DiscretePerturbationRecordColumns ColumnsType;
  typedef DiscretePerturbationRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  ModelRulesetPerturbationRecord(const analysis::ModelRulesetPerturbation& modelRulesetPerturbation, 
                                 DiscreteVariableRecord& discreteVariableRecord,
                                 int perturbationVectorIndex);

  ModelRulesetPerturbationRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~ModelRulesetPerturbationRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<ModelRulesetPerturbationRecord> factoryFromQuery(
      const QSqlQuery& query, ProjectDatabase& database);

  /// static constructor from database
  static std::vector<ModelRulesetPerturbationRecord> getModelRulesetPerturbationRecords(
      ProjectDatabase& database);

  /// static constructor from database
  static boost::optional<ModelRulesetPerturbationRecord> getModelRulesetPerturbationRecord(
      int id, ProjectDatabase& database);

  //@}

  /** Returns the ModelRulesetRecord (child) that defines this perturbation. */
  ModelRulesetRecord modelRulesetRecord() const;

  analysis::ModelRulesetPerturbation modelRulesetPerturbation() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::ModelRulesetPerturbationRecord_Impl;

  /** Construct from impl. */
  ModelRulesetPerturbationRecord(boost::shared_ptr<detail::ModelRulesetPerturbationRecord_Impl> impl,
                                 ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit ModelRulesetPerturbationRecord(boost::shared_ptr<detail::ModelRulesetPerturbationRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.ModelRulesetPerturbationRecord");
};

/** \relates ModelRulesetPerturbationRecord*/
typedef boost::optional<ModelRulesetPerturbationRecord> OptionalModelRulesetPerturbationRecord;

/** \relates ModelRulesetPerturbationRecord*/
typedef std::vector<ModelRulesetPerturbationRecord> ModelRulesetPerturbationRecordVector;

} // project
} // openstudio

#endif // PROJECT_MODELOBJECTPERTURBATIONRECORD_HPP
