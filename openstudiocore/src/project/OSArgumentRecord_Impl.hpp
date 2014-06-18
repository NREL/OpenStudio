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

#ifndef PROJECT_OSARGUMENTRECORD_IMPL_HPP
#define PROJECT_OSARGUMENTRECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"

#include "../ruleset/OSArgument.hpp"

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace project {

class RubyMeasureRecord;
class RubyContinuousVariableRecord;

namespace detail {

  /** OSArgumentRecord_Impl is a ObjectRecord_Impl that is the implementation class for OSArgumentRecord.*/
  class PROJECT_API OSArgumentRecord_Impl : public ObjectRecord_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    OSArgumentRecord_Impl(const ruleset::OSArgument& osArgument,
                          RubyMeasureRecord& rubyMeasureRecord);

    OSArgumentRecord_Impl(const ruleset::OSArgument& osArgument,
                          RubyContinuousVariableRecord& rubyContinuousVariableRecord);

    /** Constructor from query. Throws if bad query. */
    OSArgumentRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~OSArgumentRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the name of the database table for this record. Should be implemented by the base
     *  class for the table. */
    virtual std::string databaseTableName() const;

    /** Returns the direct parent of this object, if it exists. */
    virtual boost::optional<ObjectRecord> parent() const;

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Returns join relationships between this object and others. Such relationships will be
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    virtual void updatePathData(const openstudio::path& originalBase,
                                const openstudio::path& newBase);

    //@}
    /** @name Getters */
    //@{

    /** Get the RubyMeasureRecord that parents this OSArgumentRecord. */
    boost::optional<RubyMeasureRecord> rubyMeasureRecord() const;

    /** Get the RubyContinuousVariableRecord that parents this OSArgumentRecord. */
    boost::optional<RubyContinuousVariableRecord> rubyContinuousVariableRecord() const;

    ruleset::OSArgument osArgument() const;

    ruleset::OSArgumentType type() const;

    bool hasValue() const;

    bool hasDefaultValue() const;

    openstudio::path valueAsPath() const;

    void setValue(const openstudio::path& value);

    openstudio::path defaultValueAsPath() const;

    void setDefaultValue(const openstudio::path& defaultValue);

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
    REGISTER_LOGGER("openstudio.project.OSArgumentRecord");

    boost::optional<int> m_rubyMeasureRecordId;
    boost::optional<int> m_rubyContinuousVariableRecordId;
    ruleset::OSArgumentType m_argumentType;
    bool m_required;
    boost::optional<std::string> m_argumentValue;
    boost::optional<std::string> m_defaultArgumentValue;
    ruleset::OSDomainType m_domainType;
    boost::optional<std::string> m_domain;
    std::string m_choices;
    std::string m_choiceDisplayNames;
    bool m_isRead;
    std::string m_extension;

    boost::optional<int> m_lastRubyMeasureRecordId;
    boost::optional<int> m_lastRubyContinuousVariableRecordId;
    ruleset::OSArgumentType m_lastArgumentType;
    bool m_lastRequired;
    boost::optional<std::string> m_lastArgumentValue;
    boost::optional<std::string> m_lastDefaultArgumentValue;
    ruleset::OSDomainType m_lastDomainType;
    boost::optional<std::string> m_lastDomain;
    std::string m_lastChoices;
    std::string m_lastChoiceDisplayNames;
    bool m_lastIsRead;
    std::string m_lastExtension;

    std::string stringVectorToString(const std::vector<std::string>& strs) const;

    std::vector<std::string> stringToStringVector(const std::string& str) const;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_OSARGUMENTRECORD_IMPL_HPP

