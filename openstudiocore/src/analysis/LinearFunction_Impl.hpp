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

#ifndef ANALYSIS_LINEARFUNCTION_IMPL_HPP
#define ANALYSIS_LINEARFUNCTION_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Function_Impl.hpp"

namespace openstudio {
namespace analysis {

class LinearFunction;
class DataPoint;

namespace detail {

  /** LinearFunction_Impl is a Function_Impl that is the implementation class for LinearFunction.*/
  class ANALYSIS_API LinearFunction_Impl : public Function_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Basic linear function of variables. Throws openstudio::Exception if variables.size() !=
     *  coefficients.size(). */
    LinearFunction_Impl(const std::string& name,
                        const std::vector<Variable>& variables,
                        const std::vector<double>& coefficients);

    /** Constructor provided for deserialization; not for general use. */
    LinearFunction_Impl(const UUID& uuid,
                        const UUID& versionUUID,
                        const std::string& name,
                        const std::string& displayName,
                        const std::string& description,
                        const std::vector<Variable>& variables,
                        const std::vector<double>& coefficients);

    LinearFunction_Impl(const LinearFunction_Impl& other);

    virtual ~LinearFunction_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Getters */
    //@{

    std::vector<double> coefficients() const;

    virtual double getValue(const DataPoint& dataPoint) const;

    //@}
    /** @name Setters */
    //@{

    bool setCoefficients(const std::vector<double>& coefficients);

    //@}
    /** @name Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static LinearFunction fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    std::vector<double> m_coefficients;
   private:
    REGISTER_LOGGER("openstudio.analysis.LinearFunction");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_LINEARFUNCTION_IMPL_HPP

