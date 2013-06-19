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

#ifndef PROJECT_MODELOBJECTFILTERCLAUSERECORD_IMPL_HPP
#define PROJECT_MODELOBJECTFILTERCLAUSERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include <project/FilterClauseRecord_Impl.hpp>
#include <project/ModelObjectFilterClauseRecord.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional/optional.hpp>

class QSqlDatabase;

namespace openstudio {
namespace ruleset {
  class ModelObjectFilterClause;
}
namespace project {

namespace detail {

  class PROJECT_API ModelObjectFilterClauseRecord_Impl : public FilterClauseRecord_Impl
  {

    Q_OBJECT;

    public:

      /// constructors
      ModelObjectFilterClauseRecord_Impl(const openstudio::ruleset::ModelObjectFilterClause& modelObjectFilterClause, const ModelObjectFilterClauseRecordType& ModelObjectFilterClauseRecordType, const ProjectDatabase& projectDatabase);

      /// constructor from query, throws if bad query
      ModelObjectFilterClauseRecord_Impl(const QSqlQuery& query, const ProjectDatabase& projectDatabase);

      virtual ~ModelObjectFilterClauseRecord_Impl() {}
      
    protected:

      /// bind values to a query for saving
      /// override in derived classes
      virtual void bindValues(QSqlQuery& query) const;

      /// set the last state of this object from the query (including id)
      /// override in derived classes
      virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

      /// check that values (except id) are same as query
      /// override in derived classes
      virtual bool compareValues(const QSqlQuery& query) const;

      /// save values to last state
      /// override in derived classes
      virtual void saveLastValues();

      /// revert values back to last state
      /// override in derived classes
      virtual void revertToLastValues();

    private:

      REGISTER_LOGGER("openstudio.project.ModelObjectFilterClauseRecord");

      ModelObjectFilterClauseRecordType m_modelObjectFilterClauseRecordType;

      ModelObjectFilterClauseRecordType m_lastModelObjectFilterClauseRecordType;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_MODELOBJECTFILTERCLAUSERECORD_IMPL_HPP
