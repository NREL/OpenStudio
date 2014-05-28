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

