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

#ifndef ANALYSIS_MEASURE_HPP
#define ANALYSIS_MEASURE_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

#include "../utilities/core/Path.hpp"

namespace openstudio {

class FileReferenceType;

namespace runmanager {
  class WorkItem;
}

namespace analysis {

class MeasureGroup;

namespace detail {

  class Measure_Impl;
  class MeasureGroup_Impl;

} // detail

/** Measure is an AnalysisObject that defines one particular value of a
 *  DiscreteVariable. Measure is abstract, implementing generic select/deselect
 *  functionality, and requiring derived types to specify inputFileType, outputFileType, and
 *  to construct the runmanager::Job that will apply the Measure to an input file. */
class ANALYSIS_API Measure : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Measure() {}

  //@}
  /** @name Getters and Quereis */
  //@{

  bool isSelected() const;

  boost::optional<FileReferenceType> inputFileType() const;

  boost::optional<FileReferenceType> outputFileType() const;

  //@}
  /** @name Setters */
  //@{

  void setIsSelected(bool isSelected);

  //@}
  /** @name Actions */
  //@{

  runmanager::WorkItem createWorkItem(const openstudio::path& rubyIncludeDirectory) const;

  //@}
 protected:
  /// @cond
  typedef detail::Measure_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit Measure(std::shared_ptr<detail::Measure_Impl> impl);

  friend class MeasureGroup; // to call setParent
  friend class detail::MeasureGroup_Impl; // to call setParent

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.Measure");
};

/** \relates Measure */
typedef boost::optional<Measure> OptionalMeasure;

/** \relates Measure */
typedef std::vector<Measure> MeasureVector;

// ETH@20130723 - These typedefs are just to preserve the old names for the near to
// medium-term future (to avoid breaking existing scripts).

/** \relates Measure */
typedef Measure DiscretePerturbation;

/** \relates Measure */
typedef boost::optional<Measure> OptionalDiscretePerturbation;

/** \relates Measure */
typedef std::vector<Measure> DiscretePerturbationVector;

} // analysis
} // openstudio

#endif // ANALYSIS_MEASURE_HPP

