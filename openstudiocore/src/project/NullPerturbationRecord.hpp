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

#ifndef PROJECT_NULLPERTURBATIONRECORD_HPP
#define PROJECT_NULLPERTURBATIONRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/DiscretePerturbationRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

namespace openstudio {

namespace analysis {
  class NullPerturbation;
}

namespace project {

namespace detail {

  class NullPerturbationRecord_Impl;

} // detail

/** NullPerturbationRecord is a DiscretePerturbationRecord*/
class PROJECT_API NullPerturbationRecord : public DiscretePerturbationRecord {
 public:

  typedef detail::NullPerturbationRecord_Impl ImplType;
  typedef DiscretePerturbationRecordColumns ColumnsType;
  typedef DiscretePerturbationRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  NullPerturbationRecord(const analysis::NullPerturbation& nullPerturbation, 
                         DiscreteVariableRecord& discreteVariableRecord,
                         int perturbationVectorIndex);

  NullPerturbationRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~NullPerturbationRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<NullPerturbationRecord> factoryFromQuery(const QSqlQuery& query, 
                                                                  ProjectDatabase& database);

  static std::vector<NullPerturbationRecord> getNullPerturbationRecords(ProjectDatabase& database);

  static boost::optional<NullPerturbationRecord> getNullPerturbationRecord(
      int id, ProjectDatabase& database);

  //@}

  analysis::NullPerturbation nullPerturbation() const;

 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::NullPerturbationRecord_Impl;

  /** Construct from impl. */
  NullPerturbationRecord(boost::shared_ptr<detail::NullPerturbationRecord_Impl> impl,
                         ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit NullPerturbationRecord(boost::shared_ptr<detail::NullPerturbationRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.NullPerturbationRecord");
};

/** \relates NullPerturbationRecord*/
typedef boost::optional<NullPerturbationRecord> OptionalNullPerturbationRecord;

/** \relates NullPerturbationRecord*/
typedef std::vector<NullPerturbationRecord> NullPerturbationRecordVector;

} // project
} // openstudio

#endif // PROJECT_NULLPERTURBATIONRECORD_HPP
