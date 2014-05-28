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

