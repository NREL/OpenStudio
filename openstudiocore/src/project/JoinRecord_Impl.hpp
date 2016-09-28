/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
      virtual boost::optional<ObjectRecord> parent() const override;

      /// get child objects
      virtual std::vector<ObjectRecord> children() const override;

      /// get resource objects
      virtual std::vector<ObjectRecord> resources() const override;

      /// get join record objects
      virtual std::vector<JoinRecord> joinRecords() const override;

    protected:

      friend class Record;

      /// bind values to a query for saving
      /// override in derived classes
      virtual void bindValues(QSqlQuery& query) const override;

      /// set the last state of this object from the query (including id)
      /// override in derived classes
      virtual void setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) override;

      /// check that values (except id) are same as query
      /// override in derived classes
      virtual bool compareValues(const QSqlQuery& query) const override;

      /// save values to last state
      /// override in derived classes
      virtual void saveLastValues() override;

      /// revert values back to last state
      /// override in derived classes
      virtual void revertToLastValues() override;

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
