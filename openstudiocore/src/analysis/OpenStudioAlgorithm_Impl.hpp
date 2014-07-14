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

#ifndef ANALYSIS_OPENSTUDIOALGORITHM_IMPL_HPP
#define ANALYSIS_OPENSTUDIOALGORITHM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Algorithm_Impl.hpp"

namespace openstudio {
namespace analysis {

class Analysis;

namespace detail {

  /** OpenStudioAlgorithm_Impl is a Algorithm_Impl that is the implementation class for OpenStudioAlgorithm.*/
  class ANALYSIS_API OpenStudioAlgorithm_Impl : public Algorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    OpenStudioAlgorithm_Impl(const std::string& name,const AlgorithmOptions& options);

    OpenStudioAlgorithm_Impl(const UUID& uuid,
                             const UUID& versionUUID,
                             const std::string& name,
                             const std::string& displayName,
                             const std::string& description,
                             bool complete,
                             bool failed,
                             int iter,
                             const AlgorithmOptions& options);

    OpenStudioAlgorithm_Impl(const OpenStudioAlgorithm_Impl& other);

    virtual ~OpenStudioAlgorithm_Impl() {}

    //@}
    /** @name Actions */
    //@{

    /** Create the next iteration of work for analysis in the form of constructed but incomplete
     *  \link DataPoint DataPoints \endlink. Throws openstudio::Exception if analysis.algorithm()
     *  != *this. */
    virtual int createNextIteration(Analysis& analysis) = 0;

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.OpenStudioAlgorithm");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_OPENSTUDIOALGORITHM_IMPL_HPP

