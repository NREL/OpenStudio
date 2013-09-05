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

#ifndef PROJECT_VAGRANTSETTINGSRECORD_IMPL_HPP
#define PROJECT_VAGRANTSETTINGSRECORD_IMPL_HPP

#include <project/ProjectAPI.hpp>
#include <project/CloudSettingsRecord_Impl.hpp>

namespace openstudio {

class VagrantSettings;

namespace project {

class UrlRecord;

namespace detail {

  /** VagrantSettingsRecord_Impl is a CloudSettingsRecord_Impl that is the implementation class for VagrantSettingsRecord.*/
  class PROJECT_API VagrantSettingsRecord_Impl : public CloudSettingsRecord_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    VagrantSettingsRecord_Impl(const VagrantSettings& vagrantSettings,
                               ProjectDatabase& database);

    /** Constructor from query. Throws if bad query. */
    VagrantSettingsRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~VagrantSettingsRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns objects directly owned by this Record. Children are removed when this Record 
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const boost::shared_ptr<QSqlDatabase> &database);

    //@}
    /** @name Getters */
    //@{

    UrlRecord serverUrlRecord() const;

    UrlRecord workerUrlRecord() const;

    virtual CloudSettings cloudSettings() const;

    VagrantSettings vagrantSettings() const;

    //@}
        /** @name Setters */
    //@{

    /** Impl-only method. Reverts record id values back to last state. For use in constructor
     *  so can access old related records for comparison and possible removal. */
    void revertToLastRecordIds();

    /** Impl-only method. */
    void setServerUrlRecordId(int id);

    /** Impl-only method. */
    void setWorkerUrlRecordId(int id);

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
    REGISTER_LOGGER("openstudio.project.VagrantSettingsRecord");

    bool m_userAgreementSigned;
    openstudio::path m_serverPath;
    int m_serverUrlRecordId;
    openstudio::path m_workerPath;
    int m_workerUrlRecordId;
    bool m_haltOnStop;
    std::string m_username;

    bool m_lastUserAgreementSigned;
    openstudio::path m_lastServerPath;
    int m_lastServerUrlRecordId;
    openstudio::path m_lastWorkerPath;
    int m_lastWorkerUrlRecordId;
    bool m_lastHaltOnStop;
    std::string m_lastUsername;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_VAGRANTSETTINGSRECORD_IMPL_HPP

