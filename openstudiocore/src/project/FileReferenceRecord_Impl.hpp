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

#ifndef PROJECT_FILEREFERENCERECORD_IMPL_HPP
#define PROJECT_FILEREFERENCERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "ObjectRecord_Impl.hpp"

#include "../utilities/core/FileReference.hpp"

namespace openstudio {
namespace project {

class AttributeRecord;
class TagRecord;

namespace detail {

  /** FileReferenceRecord_Impl is a ObjectRecord_Impl that is the implementation class for FileReferenceRecord.*/
  class PROJECT_API FileReferenceRecord_Impl : public ObjectRecord_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    /** Orphan FileReferenceRecord. Use with caution. */
    FileReferenceRecord_Impl(const FileReference& fileReference,
                             ProjectDatabase& database);

    FileReferenceRecord_Impl(const FileReference& fileReference,
                             ObjectRecord& parentRecord);

    /** Constructor from query. Throws if bad query. */
    FileReferenceRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~FileReferenceRecord_Impl() {}

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

    /** Deserialize to FileReference. */
    FileReference fileReference() const;

    /** Get the file type. */
    FileReferenceType fileType() const;
    
    /// get the path
    openstudio::path path() const;         

    void setPath(const openstudio::path& newPath);

    /// get the checksum at model creation
    std::string checksumCreate() const;     

    /// get the checksum at last edit
    std::string checksumLast() const;

    std::vector<AttributeRecord> attributeRecords() const;

    boost::optional<AttributeRecord> getAttributeRecord(const std::string& name) const;

    std::vector<TagRecord> tagRecords() const;

    boost::optional<TagRecord> getTagRecord(const std::string& name) const;

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
    REGISTER_LOGGER("openstudio.project.FileReferenceRecord");

    boost::optional<std::string> m_parentDatabaseTableName;
    boost::optional<int> m_parentRecordId;
    FileReferenceType m_fileReferenceType;
    openstudio::path m_path;
    std::string m_checksumCreate;
    std::string m_checksumLast;

    boost::optional<std::string> m_lastParentDatabaseTableName;
    boost::optional<int> m_lastParentRecordId;
    FileReferenceType m_lastFileReferenceType;
    openstudio::path m_lastPath;
    std::string m_lastChecksumCreate;
    std::string m_lastChecksumLast;
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_FILEREFERENCERECORD_IMPL_HPP

