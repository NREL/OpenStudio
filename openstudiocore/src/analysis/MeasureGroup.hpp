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

