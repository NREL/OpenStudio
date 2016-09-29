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

#ifndef ANALYSIS_DAKOTAALGORITHM_HPP
#define ANALYSIS_DAKOTAALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "Algorithm.hpp"

namespace openstudio {

class FileReference;

namespace runmanager {
class Job;
}

namespace analysis {

class Analysis;
class DataPoint;
class DakotaParametersFile;
class UncertaintyDescriptionType;

namespace detail {

  class DakotaAlgorithm_Impl;
  class Analysis_Impl;

} // detail

/** DakotaAlgorithm is an Algorithm that works with the third-party software package DAKOTA
 *  (http://dakota.sandia.gov/software.html) to generate new data points. */
class ANALYSIS_API DakotaAlgorithm : public Algorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~DakotaAlgorithm() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns true if the algorithm requires continuous variables. The Problem class uses this
   *  method to determine whether to represent OpenStudio DiscreteVariables as continuous
   *  variables when communicating with DAKOTA. If the desired behavior is to fail, rather than
   *  to fake continuity, then isCompatibleProblemType should return false if Problem contains
   *  non-trivial (> 1 perturbation) discrete variables. */
  bool requiresContinuousVariables() const;

  bool isCompatible(const UncertaintyDescriptionType& udescType) const;

  /** Returns a reference to the DAKOTA restart file, if possible. Should exist if this algorithm
   *  is used by an analysis that has been run using the AnalysisDriver. */
  boost::optional<FileReference> restartFileReference() const;

  /** Returns a reference to the DAKOTA out file, if possible. Should exist if this algorithm
   *  is used by an analysis that has been run using the AnalysisDriver. */
  boost::optional<FileReference> outFileReference() const;

  /** Returns the RunManager job if possible. */
  boost::optional<openstudio::runmanager::Job> job() const;

  //@}
  /** @name Actions */
  //@{

  /** Returns a string that describes this algorithm, including its options, in DAKOTA .in file
   *  syntax. */
  std::string dakotaInFileDescription() const;

  /** Tries to create a new data point from dakotaParametersFile, returning the new point, or an
   *  equivalent point already present in analysis. Also updates this algorithm's internal iter
   *  counter. */
  boost::optional<DataPoint> createNextDataPoint(Analysis& analysis,
                                                 const DakotaParametersFile& params);

  //@}
 protected:
  /// @cond
  typedef detail::DakotaAlgorithm_Impl ImplType;

  explicit DakotaAlgorithm(std::shared_ptr<detail::DakotaAlgorithm_Impl> impl);

  friend class detail::DakotaAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  /// @endcond

  friend class detail::Analysis_Impl;

  /** Called after Dakota job is constructed. As a side effect, sets iter to 0. */
  void setRestartFileReference(const FileReference& file);

  /** Called after Dakota job is constructed. As a side effect, sets iter to 0. */
  void setOutFileReference(const FileReference& file);

  void markComplete();

  void markFailed();

  void resetFailed();

 private:

  REGISTER_LOGGER("openstudio.analysis.DakotaAlgorithm");
};

/** \relates DakotaAlgorithm*/
typedef boost::optional<DakotaAlgorithm> OptionalDakotaAlgorithm;

/** \relates DakotaAlgorithm*/
typedef std::vector<DakotaAlgorithm> DakotaAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DAKOTAALGORITHM_HPP

