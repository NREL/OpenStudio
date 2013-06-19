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

#ifndef PROJECT_DISCRETEPERTURBATIONRECORD_HPP
#define PROJECT_DISCRETEPERTURBATIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/ObjectRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>

namespace openstudio {
namespace analysis {
  class DiscretePerturbation;
}
namespace project {

class DiscreteVariableRecord;
class VariableRecord;

namespace detail {
  class DiscretePerturbationRecord_Impl;
  class DiscreteVariableRecord_Impl;
} // detail


/** \class DiscretePerturbationRecordType
 *
 *  \relates DiscretePerturbationRecord */
OPENSTUDIO_ENUM( DiscretePerturbationRecordType,
  ((NullPerturbationRecord)(NullPerturbationRecord)(0))
  ((RubyPerturbationRecord)(RubyPerturbationRecord)(1))
  ((ModelRulesetPerturbationRecord)(ModelRulesetPerturbationRecord)(2))
 );

/** \class DiscretePerturbationRecordColumns
 *  \brief Column definitions for the DiscretePerturbationRecords table.
 *
 *  \relates DiscretePerturbationRecord */
OPENSTUDIO_ENUM(DiscretePerturbationRecordColumns,
  ((id)(INTEGER PRIMARY KEY)(0))
  ((handle)(TEXT)(1))
  ((name)(TEXT)(2))
  ((displayName)(TEXT)(3))
  ((description)(TEXT)(4))
  ((timestampCreate)(TEXT)(5))
  ((timestampLast)(TEXT)(6))
  ((uuidLast)(TEXT)(7))
  ((discretePerturbationRecordType)(INTEGER)(8))
  ((variableRecordId)(INTEGER)(9))
  ((isSelected)(BOOLEAN)(10))
  ((perturbationVectorIndex)(INTEGER)(11))
  ((rubyScriptRecordId)(INTEGER)(12))
  ((inputFileType)(INTEGER)(13))
  ((outputFileType)(INTEGER)(14))
  ((isUserScript)(BOOLEAN)(15))
  ((modelRulesetRecordId)(INTEGER)(16))
  ((usesBCLMeasure)(BOOLEAN)(17))
);

/** A DiscretePerturbation is an ObjectRecord that serializes a specific value of a
 *  DiscreteVariable. */
class PROJECT_API DiscretePerturbationRecord : public ObjectRecord {
 public:

  typedef detail::DiscretePerturbationRecord_Impl ImplType;
  typedef DiscretePerturbationRecordColumns ColumnsType;
  typedef DiscretePerturbationRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  virtual ~DiscretePerturbationRecord() {}

  //@}

  /// get name of the database table
  static std::string databaseTableName();

  static UpdateByIdQueryData updateByIdQueryData();

  static void updatePathData(ProjectDatabase database,
                             const openstudio::path& originalBase,
                             const openstudio::path& newBase);

   /// get perturbation from query
  static boost::optional<DiscretePerturbationRecord> factoryFromQuery(
      const QSqlQuery& query,
      ProjectDatabase& database);

  /** Create a DiscretePerturbationRecord from discretePerturbation that belongs to the discrete
   *  variable serialized to discreteVariableRecord. The index is the vector index of
   *  discretePerturbation in the variable. */
  static DiscretePerturbationRecord factoryFromDiscretePerturbation(
      const analysis::DiscretePerturbation& discretePerturbation,
      DiscreteVariableRecord& discreteVariableRecord,
      int perturbationVectorIndex);

  /// get all perturbations
  static std::vector<DiscretePerturbationRecord> getDiscretePerturbationRecords(
      ProjectDatabase& database);

  /// get perturbation by id
  static boost::optional<DiscretePerturbationRecord> getDiscretePerturbationRecord(
      int id, ProjectDatabase& database);

  /** @name Getters */
  //@{

  /** Get the parent DiscreteVariableRecord. */
  boost::optional<DiscreteVariableRecord> discreteVariableRecord() const;

  /// is this perturbation selected
  bool isSelected() const;

  /// set if this perturbation is selected
  bool setIsSelected(bool isSelected);

  /** Index of this DiscretePerturbation in its (parent) DiscreteVariable's vector of
   *  perturbations. */
  boost::optional<int> perturbationVectorIndex() const;

  analysis::DiscretePerturbation discretePerturbation() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::DiscretePerturbationRecord_Impl;
  friend class detail::DiscreteVariableRecord_Impl;

  /// from impl
  DiscretePerturbationRecord(boost::shared_ptr<detail::DiscretePerturbationRecord_Impl> impl,
                             ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit DiscretePerturbationRecord(boost::shared_ptr<detail::DiscretePerturbationRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.DiscretePerturbationRecord");
};

/** \relates DiscretePerturbationRecord*/
typedef boost::optional<DiscretePerturbationRecord> OptionalDiscretePerturbationRecord;

/** \relates DiscretePerturbationRecord*/
typedef std::vector<DiscretePerturbationRecord> DiscretePerturbationRecordVector;

/** Sorts DiscretePerturbationRecords by perturbationVectorIndex. */
struct PROJECT_API DiscretePerturbationRecordPerturbationVectorIndexLess {
  bool operator()(const DiscretePerturbationRecord& left, const DiscretePerturbationRecord& right) const;
};

} // project
} // openstudio

#endif // PROJECT_DISCRETEPERTURBATIONRECORD_HPP
