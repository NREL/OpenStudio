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

#include "DakotaAlgorithm.hpp"
#include "DakotaAlgorithm_Impl.hpp"

#include "DakotaAlgorithmOptions.hpp"
#include "Analysis.hpp"
#include "Problem.hpp"
#include "DataPoint.hpp"
#include "DakotaParametersFile.hpp"

#include "../utilities/core/Assert.hpp"
#include "../runmanager/lib/JSON.hpp"

#include <sstream>

namespace openstudio {
namespace analysis {

namespace detail {

  DakotaAlgorithm_Impl::DakotaAlgorithm_Impl(const std::string& name,
                                             const DakotaAlgorithmOptions& options)
    : Algorithm_Impl(name,options)
  {}

  DakotaAlgorithm_Impl::DakotaAlgorithm_Impl(const UUID& uuid,
                                             const UUID& versionUUID,
                                             const std::string& name,
                                             const std::string& displayName,
                                             const std::string& description,
                                             bool complete,
                                             bool failed,
                                             int iter,
                                             const DakotaAlgorithmOptions& options,
                                             const boost::optional<FileReference>& restartFileReference,
                                             const boost::optional<FileReference>& outFileReference, 
                                             const boost::optional<runmanager::Job>& job)
    : Algorithm_Impl(uuid,versionUUID,name,displayName,description,complete,failed,iter,options),
      m_restartFileReference(restartFileReference),
      m_outFileReference(outFileReference),
      m_job(job)
  {}

  DakotaAlgorithm_Impl::DakotaAlgorithm_Impl(const DakotaAlgorithm_Impl& other)
    : Algorithm_Impl(other),
      m_restartFileReference(other.restartFileReference()),
      m_outFileReference(other.outFileReference())
  {
    // DLM: TODO should we clone job? for now do nothing.
  }

  bool DakotaAlgorithm_Impl::requiresContinuousVariables() const {
    return false;
  }

  bool DakotaAlgorithm_Impl::isCompatible(const UncertaintyDescriptionType& udescType) const {
    return false;
  }

  boost::optional<FileReference> DakotaAlgorithm_Impl::restartFileReference() const {
    return m_restartFileReference;
  }

  boost::optional<FileReference> DakotaAlgorithm_Impl::outFileReference() const {
    return m_outFileReference;
  }

  boost::optional<runmanager::Job> DakotaAlgorithm_Impl::job() const
  {
    return m_job;
  }

  boost::optional<DataPoint> DakotaAlgorithm_Impl::createNextDataPoint(
      Analysis& analysis,const DakotaParametersFile& params)
  {
    OS_ASSERT(analysis.algorithm().get() == getPublicObject<DakotaAlgorithm>());

    // TODO: Update iteration counter.
    OptionalDataPoint result = analysis.problem().createDataPoint(params,
                                                                  getPublicObject<DakotaAlgorithm>());
    if (result) {
      bool added = analysis.addDataPoint(*result);
      if (!added) {
        // get equivalent point already in analysis
        DataPointVector candidates = analysis.getDataPoints(result->variableValues());
        OS_ASSERT(candidates.size() == 1u);
        result = candidates[0];
      }
      std::stringstream ss;
      ss << name() << "_" << m_iter;
      result->addTag(ss.str());
    }
    return result;
  }

  void DakotaAlgorithm_Impl::setRestartFileReference(const FileReference& file) {
    m_restartFileReference = file;
    ++m_iter;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DakotaAlgorithm_Impl::setOutFileReference(const FileReference &file) {
    m_outFileReference = file;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DakotaAlgorithm_Impl::setJob(const runmanager::Job& job) {
    m_job = job;
    onChange(AnalysisObject_Impl::Benign);
  }

  void DakotaAlgorithm_Impl::reset() {
    Algorithm_Impl::reset();
    m_restartFileReference.reset();
    m_outFileReference.reset();
    m_job.reset();
    onChange(AnalysisObject_Impl::Benign);
  }

  QVariant DakotaAlgorithm_Impl::toVariant() const {
    QVariantMap map = Algorithm_Impl::toVariant().toMap();

    if (OptionalFileReference restartFile = restartFileReference()) {
      map["restart_file"] = openstudio::detail::toVariant(*restartFile);
    }
    if (OptionalFileReference outFile = outFileReference()) {
      map["out_file"] = openstudio::detail::toVariant(*outFile);
    }
    if (boost::optional<runmanager::Job> j = job()) {
      map["job"] = runmanager::detail::JSON::toVariant(j.get());
    }

    return QVariant(map);
  }

} // detail

std::string DakotaAlgorithm::dakotaInFileDescription() const {
  return getImpl<detail::DakotaAlgorithm_Impl>()->dakotaInFileDescription();
}

bool DakotaAlgorithm::requiresContinuousVariables() const {
  return getImpl<detail::DakotaAlgorithm_Impl>()->requiresContinuousVariables();
}

bool DakotaAlgorithm::isCompatible(const UncertaintyDescriptionType& udescType) const {
  return getImpl<detail::DakotaAlgorithm_Impl>()->isCompatible(udescType);
}

boost::optional<FileReference> DakotaAlgorithm::restartFileReference() const {
  return getImpl<detail::DakotaAlgorithm_Impl>()->restartFileReference();
}

boost::optional<FileReference> DakotaAlgorithm::outFileReference() const {
  return getImpl<detail::DakotaAlgorithm_Impl>()->outFileReference();
}

boost::optional<runmanager::Job> DakotaAlgorithm::job() const {
  return getImpl<detail::DakotaAlgorithm_Impl>()->job();
}

boost::optional<DataPoint> DakotaAlgorithm::createNextDataPoint(
    Analysis& analysis,const DakotaParametersFile& params)
{
  return getImpl<detail::DakotaAlgorithm_Impl>()->createNextDataPoint(analysis,params);
}

/// @cond
DakotaAlgorithm::DakotaAlgorithm(std::shared_ptr<detail::DakotaAlgorithm_Impl> impl)
  : Algorithm(impl)
{}
/// @endcond

void DakotaAlgorithm::setRestartFileReference(const FileReference &file) {
  getImpl<detail::DakotaAlgorithm_Impl>()->setRestartFileReference(file);
}

void DakotaAlgorithm::setOutFileReference(const FileReference &file) {
  getImpl<detail::DakotaAlgorithm_Impl>()->setOutFileReference(file);
}

void DakotaAlgorithm::markComplete() {
  getImpl<detail::DakotaAlgorithm_Impl>()->markComplete();
}

void DakotaAlgorithm::markFailed() {
  getImpl<detail::DakotaAlgorithm_Impl>()->markFailed();
}

void DakotaAlgorithm::resetFailed() {
  getImpl<detail::DakotaAlgorithm_Impl>()->resetFailed();
}

} // analysis
} // openstudio

