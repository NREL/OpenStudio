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

#include "VagrantSessionRecord.hpp"
#include "VagrantSessionRecord_Impl.hpp"

#include "ProjectDatabase.hpp"
#include "UrlRecord.hpp"

#include "../utilities/cloud/VagrantProvider.hpp"
#include "../utilities/cloud/VagrantProvider_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace project {

namespace detail {

  VagrantSessionRecord_Impl::VagrantSessionRecord_Impl(const VagrantSession& vagrantSession,
                                                       ProjectDatabase& database)
    : CloudSessionRecord_Impl(vagrantSession, 
                              CloudSessionRecordType::VagrantSessionRecord, 
                              database)
  {}

  VagrantSessionRecord_Impl::VagrantSessionRecord_Impl(const QSqlQuery& query, ProjectDatabase& database)
    : CloudSessionRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());
  }

  void VagrantSessionRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database) {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<VagrantSessionRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  CloudSession VagrantSessionRecord_Impl::cloudSession() const {
    return vagrantSession().cast<CloudSession>();
  }

  VagrantSession VagrantSessionRecord_Impl::vagrantSession() const {
    OptionalUrl serverUrl;
    if (OptionalUrlRecord sur = serverUrlRecord()) {
      serverUrl = sur->url();
    }
    UrlVector workerUrls;
    for (const UrlRecord& wur : workerUrlRecords()) {
      workerUrls.push_back(wur.url());
    }
    return VagrantSession(handle(),
                          uuidLast(),
                          sessionId(),
                          serverUrl,
                          workerUrls);
  }

  void VagrantSessionRecord_Impl::bindValues(QSqlQuery& query) const {
    CloudSessionRecord_Impl::bindValues(query);
  }

  void VagrantSessionRecord_Impl::setLastValues(const QSqlQuery& query, ProjectDatabase& projectDatabase) {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    CloudSessionRecord_Impl::setLastValues(query,projectDatabase);
  }

  bool VagrantSessionRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = CloudSessionRecord_Impl::compareValues(query);

    return result;
  }

  void VagrantSessionRecord_Impl::saveLastValues() {
    CloudSessionRecord_Impl::saveLastValues();
  }

  void VagrantSessionRecord_Impl::revertToLastValues() {
    CloudSessionRecord_Impl::revertToLastValues();
  }

} // detail

VagrantSessionRecord::VagrantSessionRecord(const VagrantSession& vagrantSession, 
                                           ProjectDatabase& database)
  : CloudSessionRecord(std::shared_ptr<detail::VagrantSessionRecord_Impl>(
        new detail::VagrantSessionRecord_Impl(vagrantSession, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSessionRecord_Impl>());
  constructRelatedRecords(vagrantSession,database);
}

VagrantSessionRecord::VagrantSessionRecord(const QSqlQuery& query, ProjectDatabase& database)
  : CloudSessionRecord(std::shared_ptr<detail::VagrantSessionRecord_Impl>(
        new detail::VagrantSessionRecord_Impl(query, database)),
        database)
{
  OS_ASSERT(getImpl<detail::VagrantSessionRecord_Impl>());
}

boost::optional<VagrantSessionRecord> VagrantSessionRecord::factoryFromQuery(const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalVagrantSessionRecord result;

  try {
    result = VagrantSessionRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct VagrantSessionRecord from query, because '"
        << e.what() << "'.");
  }

  return result;
}

std::vector<VagrantSessionRecord> VagrantSessionRecord::getVagrantSessionRecords(ProjectDatabase& database) {
  std::vector<VagrantSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::VagrantSessionRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(VagrantSessionRecord(query, database));
  }

  return result;
}

boost::optional<VagrantSessionRecord> VagrantSessionRecord::getVagrantSessionRecord(int id, ProjectDatabase& database) {
  boost::optional<VagrantSessionRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + CloudSessionRecord::databaseTableName() + " WHERE cloudSessionRecordType=:cloudSessionRecordType AND id=:id"));
  query.bindValue(":cloudSessionRecordType", CloudSessionRecordType::VagrantSessionRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = VagrantSessionRecord(query, database);
  }

  return result;
}

VagrantSession VagrantSessionRecord::vagrantSession() const {
  return getImpl<detail::VagrantSessionRecord_Impl>()->vagrantSession();
}

/// @cond
VagrantSessionRecord::VagrantSessionRecord(std::shared_ptr<detail::VagrantSessionRecord_Impl> impl)
  : CloudSessionRecord(impl)
{}

VagrantSessionRecord::VagrantSessionRecord(std::shared_ptr<detail::VagrantSessionRecord_Impl> impl,
                                           ProjectDatabase database)
  : CloudSessionRecord(impl, database)
{
  OS_ASSERT(getImpl<detail::VagrantSessionRecord_Impl>());
}
/// @endcond

} // project
} // openstudio

