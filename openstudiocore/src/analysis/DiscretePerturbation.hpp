/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef ANALYSIS_DISCRETEPERTURBATION_HPP
#define ANALYSIS_DISCRETEPERTURBATION_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/AnalysisObject.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {

class FileReferenceType;

namespace runmanager {
  class WorkItem;
}

namespace analysis {

class DiscreteVariable;

namespace detail {

  class DiscretePerturbation_Impl;
  class DiscreteVariable_Impl;

} // detail

/** DiscretePerturbation is an AnalysisObject that defines one particular value of a
 *  DiscreteVariable. DiscretePerturbation is abstract, implementing generic select/deselect
 *  functionality, and requiring derived types to specify inputFileType, outputFileType, and
 *  to construct the runmanager::Job that will apply the DiscretePerturbation to an input file. */
class ANALYSIS_API DiscretePerturbation : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~DiscretePerturbation() {}

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
  typedef detail::DiscretePerturbation_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit DiscretePerturbation(boost::shared_ptr<detail::DiscretePerturbation_Impl> impl);

  friend class DiscreteVariable; // to call setParent
  friend class detail::DiscreteVariable_Impl; // to call setParent

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.DiscretePerturbation");
};

/** \relates DiscretePerturbation*/
typedef boost::optional<DiscretePerturbation> OptionalDiscretePerturbation;

/** \relates DiscretePerturbation*/
typedef std::vector<DiscretePerturbation> DiscretePerturbationVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DISCRETEPERTURBATION_HPP

