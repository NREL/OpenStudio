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

#ifndef PROJECT_STANDARDSFILTEROBJECTATTRIBUTERECORD_HPP
#define PROJECT_STANDARDSFILTEROBJECTATTRIBUTERECORD_HPP

#include "ProjectAPI.hpp"
#include <project/StandardsFilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class StandardsFilterObjectAttribute;
}
namespace project {

  namespace detail {
    class StandardsFilterObjectAttributeRecord_Impl;
  }

  class StandardsFilterAvailableObjectRecord;

  OPENSTUDIO_ENUM( StandardsFilterObjectAttributeRecordType,
      ((StandardsFilterObjectBooleanAttributeRecord)(StandardsFilterObjectBooleanAttributeRecord)(0))
      ((StandardsFilterObjectDoubleAttributeRecord)(StandardsFilterObjectDoubleAttributeRecord)(1))
      ((StandardsFilterObjectEnumAttributeRecord)(StandardsFilterObjectEnumAttributeRecord)(2))
      ((StandardsFilterObjectIntegerAttributeRecord)(StandardsFilterObjectIntegerAttributeRecord)(3))
      );

  /** StandardsFilterObjectAttributeRecord
   **/
  class PROJECT_API StandardsFilterObjectAttributeRecord : public StandardsFilterClauseRecord {
    public:

      typedef detail::StandardsFilterObjectAttributeRecord_Impl ImplType;
      typedef ClauseRecordColumns ColumnsType;
      typedef ClauseRecord ObjectRecordType;

      virtual ~StandardsFilterObjectAttributeRecord() {} 

      /// get all model object filter clauses 
      static std::vector<StandardsFilterObjectAttributeRecord> getStandardsFilterObjectAttributeRecords(ProjectDatabase& database);

      /// get model object filter clause by id
      static boost::optional<StandardsFilterObjectAttributeRecord> getStandardsFilterObjectAttributeRecord(int id, ProjectDatabase& database);

      /// get model object filter clause from a query
      static boost::optional<StandardsFilterObjectAttributeRecord> factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database);

      /// try to find the filter in the database by UUID, if not found try to create a new record
      static StandardsFilterObjectAttributeRecord factoryFromFilter(const openstudio::ruleset::StandardsFilterObjectAttribute& filter, ProjectDatabase& projectDatabase);

      /// get the corresponding filter clause
      openstudio::ruleset::StandardsFilterObjectAttribute standardsFilterObjectAttribute() const;

      /// get the record the for available object filter
      StandardsFilterAvailableObjectRecord standardsFilterAvailableObjectRecord() const;

    protected:

      friend class Record;
      friend class ProjectDatabase;

      // from impl
      StandardsFilterObjectAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectAttributeRecord_Impl> impl, ProjectDatabase projectDatabase);

      // from impl, does not register in database, use with caution
      StandardsFilterObjectAttributeRecord(boost::shared_ptr<detail::StandardsFilterObjectAttributeRecord_Impl> impl);

    private:

      REGISTER_LOGGER("openstudio.project.StandardsFilterObjectAttributeRecord");
  };

} // project
} // openstudio

#endif // PROJECT_STANDARDSFILTEROBJECTATTRIBUTERECORD_HPP
