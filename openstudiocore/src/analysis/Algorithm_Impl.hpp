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

#ifndef ANALYSIS_ALGORITHM_IMPL_HPP
#define ANALYSIS_ALGORITHM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject_Impl.hpp"

#include "AlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** Algorithm_Impl is a AnalysisObject_Impl that is the implementation class for Algorithm.*/
  class ANALYSIS_API Algorithm_Impl : public AnalysisObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Algorithm name is set by class, not user. */
    Algorithm_Impl(const std::string& name,const AlgorithmOptions& options);

    /** Constructor provided for deserialization; not for general use. */
    Algorithm_Impl(const UUID& uuid,
                   const UUID& versionUUID,
                   const std::string& name,
                   const std::string& displayName,
                   const std::string& description,
                   bool complete,
                   bool failed,
                   int iter,
                   const AlgorithmOptions& options);

    Algorithm_Impl(const Algorithm_Impl& other);

    virtual ~Algorithm_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Disallow overwriting Algorithm names. */
    virtual void setName(const std::string& newName);

    //@}
    /** @name Getters and Queries */
    //@{

    /** Returns true if the Algorithm ran to completion. */
    bool isComplete() const;

    /** Returns true if the Algorithm ran to completion, but failed in some way. Perhaps not all
     *  simulations completed successfully, or output results could not be retrieved for some other
     *  reason. */
    bool failed() const;

    /** Returns the iteration number, which is incremented during the process of running an Analysis.
     *  After construction, iter() == -1. If an algorithm runs the baseline point before doing
     *  anything else, that iteration is iter 0. Otherwise, iter 1, 2, etc. are used. */
    int iter() const;

    AlgorithmOptions options() const;

    /** Returns true if Algorithm can operate on problem. For example, DesignOfExperiments can work
     *  on all \link Problem Problems \endlink, but SequentialSearch requires an OptimizationProblem
     *  with two objective functions and \link DiscreteVariable DiscreteVariables\endlink. */
    virtual bool isCompatibleProblemType(const Problem& problem) const = 0;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    void markComplete();

    /** Reset the complete flag. Used when clearing maximum iterations or simulations, for
     *  instance. */
    void resetComplete();

    void markFailed();

    void resetFailed();

    virtual void reset();

    virtual QVariant toVariant() const;

    static Algorithm factoryFromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    bool m_complete;
    bool m_failed;
    int m_iter;
    AlgorithmOptions m_options;
   private:
    REGISTER_LOGGER("openstudio.analysis.Algorithm");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_ALGORITHM_IMPL_HPP

