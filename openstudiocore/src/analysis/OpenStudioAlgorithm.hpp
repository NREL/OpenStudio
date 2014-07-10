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

#ifndef ANALYSIS_OPENSTUDIOALGORITHM_HPP
#define ANALYSIS_OPENSTUDIOALGORITHM_HPP

#include "AnalysisAPI.hpp"
#include "Algorithm.hpp"

namespace openstudio {
namespace analysis {

class Analysis;

namespace detail {

  class OpenStudioAlgorithm_Impl;

} // detail

/** OpenStudioAlgorithm is an Algorithm whose logic is directly encoded in the implementation 
 *  of createNextIteration. */
class ANALYSIS_API OpenStudioAlgorithm : public Algorithm {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~OpenStudioAlgorithm() {}

  //@}
  /** @name Actions */
  //@{
  
  /** Create the next iteration of work for analysis in the form of constructed but incomplete 
   *  \link DataPoint DataPoints \endlink. Throws openstudio::Exception if analysis.algorithm() 
   *  != *this. Returns the number of DataPoints added. */
  int createNextIteration(Analysis& analysis);

  //@}
 protected:
  /// @cond
  typedef detail::OpenStudioAlgorithm_Impl ImplType;

  explicit OpenStudioAlgorithm(std::shared_ptr<detail::OpenStudioAlgorithm_Impl> impl);

  friend class detail::OpenStudioAlgorithm_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.OpenStudioAlgorithm");
};

/** \relates OpenStudioAlgorithm*/
typedef boost::optional<OpenStudioAlgorithm> OptionalOpenStudioAlgorithm;

/** \relates OpenStudioAlgorithm*/
typedef std::vector<OpenStudioAlgorithm> OpenStudioAlgorithmVector;

} // analysis
} // openstudio

#endif // ANALYSIS_OPENSTUDIOALGORITHM_HPP

