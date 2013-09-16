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

#ifndef PROJECT_SEQUENTIALSEARCHRECORD_IMPL_HPP
#define PROJECT_SEQUENTIALSEARCHRECORD_IMPL_HPP

#include <project/ProjectAPI.hpp>
#include <project/OpenStudioAlgorithmRecord_Impl.hpp>

namespace openstudio {
namespace analysis {
  class SequentialSearch;
}
namespace project {

namespace detail {

  /** SequentialSearchRecord_Impl is an OpenStudioAlgorithmRecord_Impl that is the implementation 
   *  class for SequentialSearchRecord.*/
  class PROJECT_API SequentialSearchRecord_Impl : public OpenStudioAlgorithmRecord_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    SequentialSearchRecord_Impl(const analysis::SequentialSearch& sequentialSearch, 
                                AnalysisRecord& database);

    /** Constructor from query. Throws if bad query. */
    SequentialSearchRecord_Impl(const QSqlQuery& query, ProjectDatabase& database);

    virtual ~SequentialSearchRecord_Impl() {}

    //@}

    /** Returns objects referenced, but not owned, by this Record. */
    virtual std::vector<ObjectRecord> resources() const;

    /** Returns join relationships between this object and others. Such relationships will be 
     *  removed when either record in the relationship is removed. */
    virtual std::vector<JoinRecord> joinRecords() const;

    /** Save the row that corresponds to this record in projectDatabase. */
    virtual void saveRow(const boost::shared_ptr<QSqlDatabase> &database);

    virtual analysis::Algorithm algorithm() const;

    virtual analysis::OpenStudioAlgorithm openStudioAlgorithm() const;

    analysis::SequentialSearch sequentialSearch() const;

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
    REGISTER_LOGGER("openstudio.project.SequentialSearchRecord");
  };

} // detail

} // project
} // openstudio

#endif // PROJECT_SEQUENTIALSEARCHRECORD_IMPL_HPP

