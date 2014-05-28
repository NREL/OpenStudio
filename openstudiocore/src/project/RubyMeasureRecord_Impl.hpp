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

#ifndef PROJECT_RUBYMEASURERECORD_IMPL_HPP
#define PROJECT_RUBYMEASURERECORD_IMPL_HPP

#include "ProjectAPI.hpp"
#include "MeasureRecord_Impl.hpp"

#include "../utilities/core/FileReference.hpp"
#include "../utilities/core/Logger.hpp"

#include <boost/optional/optional.hpp>

namespace openstudio {

namespace analysis {
  class RubyMeasure;
}

namespace project {

class FileReferenceRecord;
class OSArgumentRecord;

namespace detail {

  /** RubyMeasureRecord_Impl is a MeasureRecord_Impl that is the implementation class for RubyMeasureRecord.*/
  class PROJECT_API RubyMeasureRecord_Impl : public MeasureRecord_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    RubyMeasureRecord_Impl(const analysis::RubyMeasure& rubyMeasure,
                           MeasureGroupRecord& measureGroupRecord,
                           int measureVectorIndex);

    /** Constructor for RubyMeasureRecords that are resources of RubyContinuousVariables. */
    RubyMeasureRecord_Impl(const analysis::RubyMeasure& rubyMeasure,
                           ProjectDatabase& database);

    /** Constructor from query. Throws if bad query. */
    RubyMeasureRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~RubyMeasureRecord_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns objects directly owned by this Record. Children are removed when this Record
     *  is removed. */
    virtual std::vector<ObjectRecord> children() const;

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const std::shared_ptr<QSqlDatabase> &database);

    //@}
    /** @name Getters */
    //@{

    bool usesBCLMeasure() const;

    /** Returns the FileReferenceRecord pointing to this measure's Ruby script
     *  (if !usesBCLMeasure()), or to a re-purposed FileReferenceRecord that stores a
     *  BCLMeasure's directory path and UUIDs (if usesBCLMeasure()). */
    FileReferenceRecord fileReferenceRecord() const;

    FileReferenceType inputFileType() const;

    FileReferenceType outputFileType() const;

    std::vector<OSArgumentRecord> osArgumentRecords() const;

    virtual analysis::Measure measure() const;

    analysis::RubyMeasure rubyMeasure() const;

    //@}
    /** @name Construction Helpers */
    //@{

    /** Reverts record id values back to last state. For use in RubyMeasureRecord constructor
     *  so can access old related records for comparison and possible removal. */
    void revertToLastRecordIds();

    /** Private method in public class. Used to make two-way connection with FileReferences. */
    void setFileReferenceRecordId(int id);

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
    REGISTER_LOGGER("openstudio.project.RubyMeasureRecord");

    int m_fileReferenceRecordId;
    FileReferenceType m_inputFileType;
    FileReferenceType m_outputFileType;
    bool m_isUserScript;
    bool m_usesBCLMeasure;

    int m_lastFileReferenceRecordId;
    FileReferenceType m_lastInputFileType;
    FileReferenceType m_lastOutputFileType;
    bool m_lastIsUserScript;
    bool m_lastUsesBCLMeasure;
  };

} // detail
} // project
} // openstudio

#endif // PROJECT_RUBYMEASURERECORD_IMPL_HPP
