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

#include "DesignOfExperimentsRecord.hpp"
#include "DesignOfExperimentsRecord_Impl.hpp"
#include "AnalysisRecord.hpp"
#include "JoinRecord.hpp"

#include "../analysis/DesignOfExperiments.hpp"
#include "../analysis/DesignOfExperiments_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace project {

namespace detail {

  DesignOfExperimentsRecord_Impl::DesignOfExperimentsRecord_Impl(
      const analysis::DesignOfExperiments& designOfExperiments, 
      AnalysisRecord& analysisRecord)
    : OpenStudioAlgorithmRecord_Impl(
          designOfExperiments, 
          OpenStudioAlgorithmRecordType(OpenStudioAlgorithmRecordType::DesignOfExperimentsRecord), 
          analysisRecord),
      m_designOfExperimentsType(designOfExperiments.designOfExperimentsOptions().designType())
  {}

  DesignOfExperimentsRecord_Impl::DesignOfExperimentsRecord_Impl(const QSqlQuery& query, 
                                                                 ProjectDatabase& database)
    : OpenStudioAlgorithmRecord_Impl(query, database)
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    QVariant value;

    value = query.value(AlgorithmRecordColumns::designOfExperimentsType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_designOfExperimentsType = analysis::DesignOfExperimentsType(value.toInt());
  }

  std::vector<ObjectRecord> DesignOfExperimentsRecord_Impl::resources() const {
    ObjectRecordVector result;
    return result;
  }

  std::vector<JoinRecord> DesignOfExperimentsRecord_Impl::joinRecords() const {
    JoinRecordVector result;
    return result;
  }

  void DesignOfExperimentsRecord_Impl::saveRow(const std::shared_ptr<QSqlDatabase> &database)
  {
    QSqlQuery query(*database);
    this->makeUpdateByIdQuery<DesignOfExperimentsRecord>(query);
    this->bindValues(query);
    assertExec(query);
  }

  analysis::Algorithm DesignOfExperimentsRecord_Impl::algorithm() const {
    return designOfExperiments().cast<analysis::Algorithm>();
  }

  analysis::OpenStudioAlgorithm DesignOfExperimentsRecord_Impl::openStudioAlgorithm() const {
    return designOfExperiments().cast<analysis::OpenStudioAlgorithm>();
  }

  analysis::DesignOfExperiments DesignOfExperimentsRecord_Impl::designOfExperiments() const {
    analysis::DesignOfExperimentsOptions opts(m_designOfExperimentsType,
                                              options());

    return analysis::DesignOfExperiments(handle(),
                                         uuidLast(),
                                         displayName(),
                                         description(),
                                         complete(),
                                         failed(),
                                         iter(),
                                         opts);
  }

  void DesignOfExperimentsRecord_Impl::bindValues(QSqlQuery& query) const {
    OpenStudioAlgorithmRecord_Impl::bindValues(query);

    query.bindValue(AlgorithmRecordColumns::designOfExperimentsType, 
                    m_designOfExperimentsType.value());
  }

  void DesignOfExperimentsRecord_Impl::setLastValues(const QSqlQuery& query, 
                                                     ProjectDatabase& projectDatabase) 
  {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    OpenStudioAlgorithmRecord_Impl::setLastValues(query,projectDatabase);

    QVariant value;

    value = query.value(AlgorithmRecordColumns::designOfExperimentsType);
    OS_ASSERT(value.isValid() && !value.isNull());
    m_lastDesignOfExperimentsType = analysis::DesignOfExperimentsType(value.toInt());
  }

  bool DesignOfExperimentsRecord_Impl::compareValues(const QSqlQuery& query) const {
    OS_ASSERT(query.isValid());
    OS_ASSERT(query.isActive());
    OS_ASSERT(query.isSelect());

    bool result = OpenStudioAlgorithmRecord_Impl::compareValues(query);

    QVariant value;

    value = query.value(AlgorithmRecordColumns::designOfExperimentsType);
    OS_ASSERT(value.isValid() && !value.isNull());
    result = result && (m_designOfExperimentsType == analysis::DesignOfExperimentsType(value.toInt()));

    return result;
  }

  void DesignOfExperimentsRecord_Impl::saveLastValues() {
    OpenStudioAlgorithmRecord_Impl::saveLastValues();

    m_lastDesignOfExperimentsType = m_designOfExperimentsType;
  }

  void DesignOfExperimentsRecord_Impl::revertToLastValues() {
    OpenStudioAlgorithmRecord_Impl::revertToLastValues();

    m_designOfExperimentsType = m_lastDesignOfExperimentsType;
  }

} // detail

DesignOfExperimentsRecord::DesignOfExperimentsRecord(
    const analysis::DesignOfExperiments& designOfExperiments, AnalysisRecord& analysisRecord)
  : OpenStudioAlgorithmRecord(std::shared_ptr<detail::DesignOfExperimentsRecord_Impl>(
        new detail::DesignOfExperimentsRecord_Impl(designOfExperiments, analysisRecord)),
        analysisRecord.projectDatabase(),
        designOfExperiments)
{
  OS_ASSERT(getImpl<detail::DesignOfExperimentsRecord_Impl>());
}

DesignOfExperimentsRecord::DesignOfExperimentsRecord(const QSqlQuery& query, 
                                                     ProjectDatabase& database)
  : OpenStudioAlgorithmRecord(std::shared_ptr<detail::DesignOfExperimentsRecord_Impl>(
        new detail::DesignOfExperimentsRecord_Impl(query, database)),
        database,
        boost::optional<analysis::OpenStudioAlgorithm>())
{
  OS_ASSERT(getImpl<detail::DesignOfExperimentsRecord_Impl>());
}

DesignOfExperimentsRecord::DesignOfExperimentsRecord(
    std::shared_ptr<detail::DesignOfExperimentsRecord_Impl> impl,
    ProjectDatabase database)
    : OpenStudioAlgorithmRecord(impl, database, boost::optional<analysis::OpenStudioAlgorithm>())
{
  OS_ASSERT(getImpl<detail::DesignOfExperimentsRecord_Impl>());
}

boost::optional<DesignOfExperimentsRecord> DesignOfExperimentsRecord::factoryFromQuery(
    const QSqlQuery& query, ProjectDatabase& database)
{
  OptionalDesignOfExperimentsRecord result;
  try {
    result = DesignOfExperimentsRecord(query,database);
  }
  catch (const std::exception& e) {
    LOG(Error,"Unable to construct DesignOfExperimentsRecord from query, because '" 
        << e.what() << "'.");
  }
  return result;
}

std::vector<DesignOfExperimentsRecord> DesignOfExperimentsRecord::getDesignOfExperimentsRecords(ProjectDatabase& database) {
  std::vector<DesignOfExperimentsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() + 
                          " WHERE algorithmRecordType=:algorithmRecordType AND " + 
                          "openStudioAlgorithmRecordType=:openStudioAlgorithmRecordType"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::OpenStudioAlgorithmRecord);
  query.bindValue(":openStudioAlgorithmRecordType", OpenStudioAlgorithmRecordType::DesignOfExperimentsRecord);
  assertExec(query);
  while (query.next()) {
    result.push_back(DesignOfExperimentsRecord(query, database));
  }

  return result;
}

boost::optional<DesignOfExperimentsRecord> DesignOfExperimentsRecord::getDesignOfExperimentsRecord(int id, ProjectDatabase& database) {
  boost::optional<DesignOfExperimentsRecord> result;

  QSqlQuery query(*(database.qSqlDatabase()));
  query.prepare(toQString("SELECT * FROM " + AlgorithmRecord::databaseTableName() + 
                          " WHERE algorithmRecordType=:algorithmRecordType AND " + 
                          "openStudioAlgorithmRecordType=:openStudioAlgorithmRecordType AND " +
                          "id=:id"));
  query.bindValue(":algorithmRecordType", AlgorithmRecordType::OpenStudioAlgorithmRecord);
  query.bindValue(":openStudioAlgorithmRecordType", OpenStudioAlgorithmRecordType::DesignOfExperimentsRecord);
  query.bindValue(":id",id);
  assertExec(query);
  if (query.first()) {
    result = DesignOfExperimentsRecord(query, database);
  }

  return result;
}

analysis::DesignOfExperiments DesignOfExperimentsRecord::designOfExperiments() const {
  return getImpl<detail::DesignOfExperimentsRecord_Impl>()->designOfExperiments();
}

/// @cond
DesignOfExperimentsRecord::DesignOfExperimentsRecord(std::shared_ptr<detail::DesignOfExperimentsRecord_Impl> impl)
  : OpenStudioAlgorithmRecord(impl)
{}
/// @endcond

} // project
} // openstudio

