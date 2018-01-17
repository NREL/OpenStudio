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

#ifndef ANALYSIS_MEASUREGROUP_HPP
#define ANALYSIS_MEASUREGROUP_HPP

#include "AnalysisAPI.hpp"
#include "DiscreteVariable.hpp"

namespace openstudio {
namespace analysis {

class Measure;

namespace detail {

  class MeasureGroup_Impl;
  class Measure_Impl;
  class RubyMeasure_Impl;

} // detail

/** MeasureGroup is an DiscreteVariable that takes on discrete values, each of which is described by
 *  a Measure. Essentially, MeasureGroup is a mapping from 0, 1, ... ,
 *  (numMeasures(false) - 1) to a Measure that defines what setting the variable
 *  value to the given integer means. MeasureGroup forwards any createWorkItem requests along to
 *  the appropriate DiscretePertubation using this mapping. */
class ANALYSIS_API MeasureGroup : public DiscreteVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from a vector of measures (concrete values of the variable). Performs the
   *  following error checking and clean-up operations:
   *
   *  \li Allows at most one NullMeasure (keeping the first and deleting all others).
   *  \li Ensures that the variable always acts on the same type of model, for instance, idf or osm.
   *
   *  createJob is implemented assuming QVariant value is an integer index into the measure
   *  vector.
   */
  MeasureGroup(const std::string& name,
               const std::vector<Measure>& measures);

  /** Constructor provided for deserialization; not for general use. */
  MeasureGroup(const UUID& uuid,
               const UUID& versionUUID,
               const std::string& name,
               const std::string& displayName,
               const std::string& description,
               const boost::optional<UncertaintyDescription>& udesc,
               const std::vector<Measure>& measures);

  virtual ~MeasureGroup() {}

  //@}
  /** @name Getters and Queries */
  //@{

  std::vector<Measure> measures(bool selectedMeasuresOnly) const;

  /** \deprecated */
  std::vector<Measure> perturbations(bool selectedMeasuresOnly) const;

  Measure getMeasure(int index) const;

  /** \deprecated */
  Measure getPerturbation(int index) const;

  boost::optional<Measure> getMeasureByUUID(const UUID& uuid) const;

  /** \deprecated */
  boost::optional<Measure> getPerturbationByUUID(const UUID& uuid) const;

  unsigned numMeasures(bool selectedMeasuresOnly) const;

  /** \deprecated */
  unsigned numPerturbations(bool selectedPerturbationsOnly) const;

  boost::optional<int> getIndexByUUID(const Measure& measure) const;

  Measure getMeasure(const DataPoint& dataPoint) const;

  /** \deprecated */
  Measure getPerturbation(const DataPoint& dataPoint) const;

  //@}
  /** @name Setters */
  //@{

  bool push(const Measure& measure);

  bool insert(int index, const Measure& measure);

  bool erase(const Measure& measure);

  bool swap(const Measure& measure1, const Measure& measure2);

  bool setMeasures(const std::vector<Measure>& measures);

  /** \deprecated */
  bool setPerturbations(const std::vector<Measure>& measures);

  void clearMeasures();

  /** \deprecated */
  void clearPerturbations();

  //@}
 protected:
  /// @cond
  typedef detail::MeasureGroup_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::MeasureGroup_Impl;

  explicit MeasureGroup(std::shared_ptr<detail::MeasureGroup_Impl> impl);

  friend class detail::Measure_Impl;
  friend class detail::RubyMeasure_Impl;

  bool fileTypesAreCompatible(const Measure& childMeasure,
                              const FileReferenceType& proposedInputFileType,
                              const FileReferenceType& proposedOutputFileType) const;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.MeasureGroup");
};

/** \relates MeasureGroup*/
typedef boost::optional<MeasureGroup> OptionalMeasureGroup;

/** \relates MeasureGroup*/
typedef std::vector<MeasureGroup> MeasureGroupVector;

} // analysis
} // openstudio

#endif // ANALYSIS_MEASUREGROUP_HPP

