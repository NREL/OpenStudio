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

#ifndef PROJECT_STANDARDSFILTERAVAILABLEMODELRECORD_HPP
#define PROJECT_STANDARDSFILTERAVAILABLEMODELRECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/idd/IddFactory.hxx>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterAvailableModel;
}
namespace project {

  namespace detail {
    class StandardsFilterAvailableModelRecord_Impl;
  }

  /** StandardsFilterAvailableModelRecord 
  **/
  class PROJECT_API StandardsFilterAvailableModelRecord : public StandardsFilterClauseRecord {
    public:

      typedef detail::StandardsFilterAvailableModelRecord_Impl ImplType;
      typedef ClauseRecordColumns ColumnsType;
      typedef ClauseRecord ObjectRecordType;

      /// constructors
      StandardsFilterAvailableModelRecord(
          const openstudio::ruleset::StandardsFilterAvailableModel& standardsFilterAvailableModel, 
          ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      StandardsFilterAvailableModelRecord(const QSqlQuery& query, ProjectDatabase& projectDatabase);

      virtual ~StandardsFilterAvailableModelRecord() {}

      static boost::optional<StandardsFilterAvailableModelRecord> factoryFromQuery(
          const QSqlQuery& query,ProjectDatabase& database);

      /// get all model records in database
      static std::vector<StandardsFilterAvailableModelRecord> 
      getStandardsFilterAvailableModelRecords(ProjectDatabase& projectDatabase);

      /// get model record in database
      static boost::optional<StandardsFilterAvailableModelRecord> 
      getStandardsFilterAvailableModelRecord(int id, ProjectDatabase& projectDatabase);

      /// get the filter from the record
      openstudio::ruleset::StandardsFilterAvailableModel standardsFilterAvailableModel() const;

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsFilterAvailableModelRecord(
          boost::shared_ptr<detail::StandardsFilterAvailableModelRecord_Impl> impl, 
          ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsFilterAvailableModelRecord(boost::shared_ptr<detail::StandardsFilterAvailableModelRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsFilterAvailableModelRecord");
  };

  /** \relates StandardsFilterAvailableModelRecord */
  typedef boost::optional<StandardsFilterAvailableModelRecord> OptionalStandardsFilterAvailableModelRecord;

  /** \relates StandardsFilterAvailableModelRecord */
  typedef std::vector<StandardsFilterAvailableModelRecord> StandardsFilterAvailableModelRecordVector;

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTERAVAILABLEMODELRECORD_HPP
