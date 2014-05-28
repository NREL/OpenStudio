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

#ifndef PROJECT_NULLMEASURERECORD_HPP
#define PROJECT_NULLMEASURERECORD_HPP

#include "ProjectAPI.hpp"
#include "MeasureRecord.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional/optional.hpp>

namespace openstudio {

namespace analysis {
  class NullMeasure;
}

namespace project {

namespace detail {

  class NullMeasureRecord_Impl;

} // detail

/** NullMeasureRecord is a MeasureRecord*/
class PROJECT_API NullMeasureRecord : public MeasureRecord {
 public:

  typedef detail::NullMeasureRecord_Impl ImplType;
  typedef MeasureRecordColumns ColumnsType;
  typedef MeasureRecord ObjectRecordType;

  /** @name Constructors and Destructors */
  //@{

  NullMeasureRecord(const analysis::NullMeasure& nullMeasure,
                    MeasureGroupRecord& measureGroupRecord,
                    int measureVectorIndex);

  NullMeasureRecord(const QSqlQuery& query, ProjectDatabase& database);

  virtual ~NullMeasureRecord() {}

  //@}
  /** @name Static Methods */
  //@{

  static boost::optional<NullMeasureRecord> factoryFromQuery(const QSqlQuery& query,
                                                             ProjectDatabase& database);

  static std::vector<NullMeasureRecord> getNullMeasureRecords(ProjectDatabase& database);

  static boost::optional<NullMeasureRecord> getNullMeasureRecord(int id,
                                                                 ProjectDatabase& database);

  //@}
  /** @name Getters */
  //@{

  analysis::NullMeasure nullMeasure() const;

  //@}
 protected:
  /// @cond
  friend class Record;
  friend class ProjectDatabase;
  friend class detail::NullMeasureRecord_Impl;

  /** Construct from impl. */
  NullMeasureRecord(std::shared_ptr<detail::NullMeasureRecord_Impl> impl,
                    ProjectDatabase database);

  /// Construct from impl. Does not register in the database, so use with caution.
  explicit NullMeasureRecord(std::shared_ptr<detail::NullMeasureRecord_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.project.NullMeasureRecord");
};

/** \relates NullMeasureRecord*/
typedef boost::optional<NullMeasureRecord> OptionalNullMeasureRecord;

/** \relates NullMeasureRecord*/
typedef std::vector<NullMeasureRecord> NullMeasureRecordVector;

} // project
} // openstudio

#endif // PROJECT_NULLMEASURERECORD_HPP
