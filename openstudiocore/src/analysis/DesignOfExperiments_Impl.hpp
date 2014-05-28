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

#ifndef ANALYSIS_DESIGNOFEXPERIMENTS_IMPL_HPP
#define ANALYSIS_DESIGNOFEXPERIMENTS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "OpenStudioAlgorithm_Impl.hpp"

#include "DesignOfExperiments.hpp"

namespace openstudio {
namespace analysis {

class DesignOfExperimentsOptions;

namespace detail {

  /** DesignOfExperiments_Impl is a Algorithm_Impl that is the implementation class for DesignOfExperiments.*/
  class ANALYSIS_API DesignOfExperiments_Impl : public OpenStudioAlgorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DesignOfExperiments_Impl(const DesignOfExperimentsOptions& options);

    /** Constructor provided for deserialization; not for general use. */
    DesignOfExperiments_Impl(const UUID& uuid,
                             const UUID& versionUUID,
                             const std::string& displayName,
                             const std::string& description,
                             bool complete,
                             bool failed,
                             int iter,
                             const DesignOfExperimentsOptions& options);

    DesignOfExperiments_Impl(const DesignOfExperiments_Impl& other);

    virtual ~DesignOfExperiments_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns true if Algorithm can operate on problem. For example, DesignOfExperiments can work
     *  on all \link Problem Problems \endlink, but SequentialSearch requires an OptimizationProblem
     *  with two objective functions and \link DiscreteVariable DiscreteVariables\endlink. */
    virtual bool isCompatibleProblemType(const Problem& problem) const;

    /** Create the next iteration of work for analysis in the form of constructed but incomplete
     *  \link DataPoint DataPoints \endlink. Throws openstudio::Exception if analysis.algorithm()
     *  != *this. */
    virtual int createNextIteration(Analysis& analysis);

    //@}
    /** @name Getters and Queries */
    //@{

    DesignOfExperimentsOptions designOfExperimentsOptions() const;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static DesignOfExperiments fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.DesignOfExperiments");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_DESIGNOFEXPERIMENTS_IMPL_HPP

