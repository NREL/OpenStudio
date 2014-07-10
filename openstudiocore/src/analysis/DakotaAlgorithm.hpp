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

