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

#ifndef PROJECT_AWSSETTINGSRECORD_IMPL_HPP
#define PROJECT_AWSSETTINGSRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "CloudSettingsRecord_Impl.hpp"

namespace openstudio {

class AWSSettings;

namespace project {

namespace detail {

  /** AWSSettingsRecord_Impl is a CloudSettingsRecord_Impl that is the implementation class
   *  for AWSSettingsRecord.*/
  class PROJECT_API AWSSettingsRecord_Impl : public CloudSettingsRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    AWSSettingsRecord_Impl(const AWSSettings& awsSettings,
                           ProjectDatabase& database);

    /** Constructor from query. Throws if bad query. */
    AWSSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~AWSSettingsRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase>& database);

    //@}
    /** @name Getters */
    //@{

    virtual CloudSettings cloudSettings() const;

    AWSSettings awsSettings() const;

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
    REGISTER_LOGGER("openstudio.project.AWSSettingsRecord");

    bool m_userAgreementSigned;
    unsigned m_numWorkers;
    bool m_terminationDelayEnabled;
    unsigned m_terminationDelay;
    std::string m_region;
    std::string m_serverInstanceType;
    std::string m_workerInstanceType;

    bool m_lastUserAgreementSigned;
    unsigned m_lastNumWorkers;
    bool m_lastTerminationDelayEnabled;
    unsigned m_lastTerminationDelay;
    std::string m_lastRegion;
    std::string m_lastServerInstanceType;
    std::string m_lastWorkerInstanceType;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_AWSSETTINGSRECORD_IMPL_HPP

