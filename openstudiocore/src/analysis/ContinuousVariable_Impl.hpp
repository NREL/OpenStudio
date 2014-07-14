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

#ifndef ANALYSIS_CONTINUOUSVARIABLE_IMPL_HPP
#define ANALYSIS_CONTINUOUSVARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "InputVariable_Impl.hpp"

namespace openstudio {
namespace analysis {

class ContinuousVariable;

namespace detail {

  /** ContinuousVariable_Impl is an InputVariable_Impl that is the implementation class
   *  for ContinuousVariable.*/
  class ANALYSIS_API ContinuousVariable_Impl : public InputVariable_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Construct a variable with a range of (-infinity,+infinity), and no step size. */
    ContinuousVariable_Impl(const std::string& name);

    /** Constructor provided for deserialization; not for general use. */
    ContinuousVariable_Impl(const UUID& uuid,
                            const UUID& versionUUID,
                            const std::string& name,
                            const std::string& displayName,
                            const std::string& description,
                            const boost::optional<UncertaintyDescription>& udesc,
                            boost::optional<double> minimum,
                            boost::optional<double> maximum,
                            boost::optional<double> increment,
                            boost::optional<int> nSteps);

    ContinuousVariable_Impl(const ContinuousVariable_Impl& other);

    virtual ~ContinuousVariable_Impl() {}

    //@}
    /** @name Getters and Queries */
    //@{

    boost::optional<double> minimum() const;

    boost::optional<double> maximum() const;

    boost::optional<double> increment() const;

    boost::optional<int> nSteps() const;

    /** Returns true if value in [minimum,maximum]. */
    bool isFeasible(double value) const;

    /** Returns a vector of incremental values, if minimum() && maximum() &&
     *  (increment() || nSteps()). There are two exceptional cases. If minimum() > maximum(),
     *  then an empty vector is returned. If equal(minimum(),maximum()), then a single value
     *  is returned. */
    std::vector<double> incrementalValues() const;

    //@}
    /** @name Setters */
    //@{

    void setMinimum(double minimum);

    void setMaximum(double maximum);

    /** Sets increment and nullifies nSteps. */
    void setIncrement(double increment);

    /** Sets nSteps and nullifies increment. */
    void setNSteps(int nSteps);

    void clearMinimum();

    void clearMaximum();

    void clearIncrement();

    void clearNSteps();

    //@}
    /** @name Actions */
    //@{

    /** Truncates value to [minimum,maximum]. If minimum > maximum, returns boost::none. */
    boost::optional<double> truncate(double value) const;

    //@}
    /** @name Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    virtual QVariant toServerFormulationVariant() const;

    //@}
   protected:
    boost::optional<double> m_minimum;
    boost::optional<double> m_maximum;
    boost::optional<double> m_increment;
    boost::optional<int> m_nSteps;
   private:
    REGISTER_LOGGER("openstudio.analysis.ContinuousVariable");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_CONTINUOUSVARIABLE_IMPL_HPP

