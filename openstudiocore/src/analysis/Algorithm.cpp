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

#include "Algorithm.hpp"
#include "Algorithm_Impl.hpp"

#include "DDACEAlgorithm.hpp"
#include "DDACEAlgorithm_Impl.hpp"
#include "FSUDaceAlgorithm.hpp"
#include "FSUDaceAlgorithm_Impl.hpp"
#include "ParameterStudyAlgorithm.hpp"
#include "ParameterStudyAlgorithm_Impl.hpp"
#include "PSUADEDaceAlgorithm.hpp"
#include "PSUADEDaceAlgorithm_Impl.hpp"
#include "SamplingAlgorithm.hpp"
#include "SamplingAlgorithm_Impl.hpp"
#include "DesignOfExperiments.hpp"
#include "DesignOfExperiments_Impl.hpp"
#include "SequentialSearch.hpp"
#include "SequentialSearch_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  Algorithm_Impl::Algorithm_Impl(const std::string& name,const AlgorithmOptions& options)
    : AnalysisObject_Impl(name),
      m_complete(false),
      m_failed(false),
      m_iter(-1),
      m_options(options)
  {}

  Algorithm_Impl::Algorithm_Impl(const UUID& uuid, 
                                 const UUID& versionUUID, 
                                 const std::string& name,
                                 const std::string& displayName, 
                                 const std::string& description,
                                 bool complete,
                                 bool failed,
                                 int iter,
                                 const AlgorithmOptions& options)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_complete(complete), m_failed(failed), m_iter(iter), m_options(options)
  {}

  Algorithm_Impl::Algorithm_Impl(const Algorithm_Impl& other)
    : AnalysisObject_Impl(other),
      m_complete(other.isComplete()),
      m_failed(other.failed()),
      m_iter(other.iter()),
      m_options(other.options().clone())
  {}

  void Algorithm_Impl::setName(const std::string& newName) {
    return;
  }

  bool Algorithm_Impl::isComplete() const {
    return m_complete;
  }

  bool Algorithm_Impl::failed() const {
    return m_failed;
  }

  int Algorithm_Impl::iter() const {
    return m_iter;
  }

  AlgorithmOptions Algorithm_Impl::options() const {
    return m_options;
  }

  void Algorithm_Impl::markComplete() {
    m_complete = true;
    onChange(AnalysisObject_Impl::Benign);
  }

  void Algorithm_Impl::resetComplete() {
    m_complete = false;
    onChange(AnalysisObject_Impl::Benign);
  }

  void Algorithm_Impl::markFailed() {
    m_failed = true;
    onChange(AnalysisObject_Impl::Benign);
  }

  void Algorithm_Impl::resetFailed() {
    m_failed = false;
    onChange(AnalysisObject_Impl::Benign);
  }

  void Algorithm_Impl::reset() {
    m_complete = false;
    m_failed = false;
    m_iter = -1;
    onChange(AnalysisObject_Impl::Benign);
  }

  QVariant Algorithm_Impl::toVariant() const {
    QVariantMap map = AnalysisObject_Impl::toVariant().toMap();

    map["complete"] = isComplete();
    map["failed"] = failed();
    map["iter"] = iter();
    map["options"] = options().toVariant();

    return QVariant(map);
  }

  Algorithm Algorithm_Impl::factoryFromVariant(const QVariant& variant,
                                               const VersionString& version)
  {
    QVariantMap map = variant.toMap();

    if (!map.contains("algorithm_type")) {
      LOG_AND_THROW("Unable to find Algorithm in expected location.");
    }

    std::string algorithmType = map["algorithm_type"].toString().toStdString();
    if (algorithmType == "DDACEAlgorithm") {
      return DDACEAlgorithm_Impl::fromVariant(variant,version);
    }
    if (algorithmType == "FSUDaceAlgorithm") {
      return FSUDaceAlgorithm_Impl::fromVariant(variant,version);
    }
    if (algorithmType == "ParameterStudyAlgorithm") {
      return ParameterStudyAlgorithm_Impl::fromVariant(variant,version);
    }
    if (algorithmType == "PSUADEDaceAlgorithm") {
      return PSUADEDaceAlgorithm_Impl::fromVariant(variant,version);
    }
    if (algorithmType == "SamplingAlgorithm") {
      return SamplingAlgorithm_Impl::fromVariant(variant,version);
    }
    if (algorithmType == "DesignOfExperiments") {
      return DesignOfExperiments_Impl::fromVariant(variant,version);
    }
    if (algorithmType == "SequentialSearch") {
      return SequentialSearch_Impl::fromVariant(variant,version);
    }

    LOG_AND_THROW("Unexpected algorithm_type " << algorithmType << ".");
  }

} // detail

bool Algorithm::isComplete() const {
  return getImpl<detail::Algorithm_Impl>()->isComplete();
}

bool Algorithm::failed() const {
  return getImpl<detail::Algorithm_Impl>()->failed();
}

int Algorithm::iter() const {
  return getImpl<detail::Algorithm_Impl>()->iter();
}

AlgorithmOptions Algorithm::options() const {
  return getImpl<detail::Algorithm_Impl>()->options();
}

bool Algorithm::isCompatibleProblemType(const Problem& problem) const {
  return getImpl<detail::Algorithm_Impl>()->isCompatibleProblemType(problem);
}

/// @cond
Algorithm::Algorithm(std::shared_ptr<detail::Algorithm_Impl> impl)
  : AnalysisObject(impl)
{}

void Algorithm::createCallbackForOptions() const {
  Algorithm copyOfThis(getImpl<detail::Algorithm_Impl>());
  options().setAlgorithm(copyOfThis);
}
/// @endcond

void Algorithm::reset() {
  getImpl<detail::Algorithm_Impl>()->reset();
}

void Algorithm::resetComplete() {
  getImpl<detail::Algorithm_Impl>()->resetComplete();
}

} // analysis
} // openstudio

