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

#ifndef PROJECT_AWSSESSIONRECORD_IMPL_HPP
#define PROJECT_AWSSESSIONRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "CloudSessionRecord_Impl.hpp"

// TODO: Delete this include if no derived classes (and no AWSSessionRecordType enum).
#include "AWSSessionRecord.hpp"

namespace openstudio {

class AWSSession;

namespace project {

namespace detail {

  /** AWSSessionRecord_Impl is a CloudSessionRecord_Impl that is the implementation class for AWSSessionRecord.*/
  class PROJECT_API AWSSessionRecord_Impl : public CloudSessionRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AWSSessionRecord_Impl(const AWSSession& awsSession,
                          ProjectDatabase& database);

    /** Constructor from query. Throws if bad query. */
    AWSSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~AWSSessionRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase>& database);

    //@}
    /** @name Getters */
    //@{

    virtual CloudSession cloudSession() const;

    AWSSession awsSession() const;

    //@}
   protected:
    /** Bind data member values to a query for saving. */
    virtual void bindValues(QSqlQuery& query) const;

    /** Set the last state of this object from the query (including id). */
    virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase);

    /** Check that values (except id) are same as query. */
    virtual bool compareValues(const QSqlQuery& query) const;

    /** Save values to last state. */
    virtual void saveLastValues();

    /** Revert values back to last state. */
    virtual void revertToLastValues();

   private:
    REGISTER_LOGGER("openstudio.project.AWSSessionRecord");

    unsigned m_numServerProcessors;
    unsigned m_numWorkerProcessors;
    std::string m_privateKey;
    std::string m_timestamp;
    std::string m_region;
    std::string m_serverInstanceType;
    std::string m_workerInstanceType;

    unsigned m_lastNumServerProcessors;
    unsigned m_lastNumWorkerProcessors;
    std::string m_lastPrivateKey;
    std::string m_lastTimestamp;
    std::string m_lastRegion;
    std::string m_lastServerInstanceType;
    std::string m_lastWorkerInstanceType;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_AWSSESSIONRECORD_IMPL_HPP

