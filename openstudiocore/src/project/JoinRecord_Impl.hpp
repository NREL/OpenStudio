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

#ifndef PROJECT_JOINRECORD_IMPL_HPP
#define PROJECT_JOINRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "Record_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Logger.hpp"

#include <QObject>

class QSqlQuery;

namespace openstudio {
namespace project {
namespace detail {

  class PROJECT_API JoinRecord_Impl : public Record_Impl {

    public:

      /// constructor
      JoinRecord_Impl(int leftId, const UUID& leftHandle, int rightId, const UUID& rightHandle, const ProjectDatabase& database);

      /// constructor
      JoinRecord_Impl(const QSqlQuery& query, const ProjectDatabase& database);

      virtual ~JoinRecord_Impl() {}

      /// get the left id
      int leftId() const;

      /// get the left handle
      UUID leftHandle() const;

      /// get the left object
      virtual ObjectRecord leftObject() const = 0;

      /// get the right id
      int rightId() const;

      /// get the right handle
      UUID rightHandle() const;

      /// get the right object
      virtual ObjectRecord rightObject() const = 0;

      /// get parent object
      virtual boost::optional<ObjectRecord> parent() const;

      /// get child objects
      virtual std::vector<ObjectRecord> children() const;

      /// get resource objects
      virtual std::vector<ObjectRecord> resources() const;

      /// get join record objects
      virtual std::vector<JoinRecord> joinRecords() const;

    protected:

      friend class Record;

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

      REGISTER_LOGGER("openstudio.project.JoinRecord");

      int m_leftId;
      UUID m_leftHandle;
      int m_rightId;
      UUID m_rightHandle;

      int m_lastLeftId;
      UUID m_lastLeftHandle;
      int m_lastRightId;
      UUID m_lastRightHandle;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_JOINRECORD_IMPL_HPP
