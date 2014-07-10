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

#ifndef ANALYSIS_ANALYSISENUMS_HPP
#define ANALYSIS_ANALYSISENUMS_HPP

#include "../utilities/core/Enum.hpp"

#include <vector>

namespace openstudio {
namespace analysis {

namespace detail {
  class UncertaintyDescription_Impl;
}

/** \class DakotaFunctionType 
 *  \brief List of function types DAKOTA expects us to evaluate.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(DakotaFunctionType,
    ((response_fn)(response))
    ((obj_fn)(objective function))
); 
 *  \endcode
 *  The valueName is the DAKOTA key for the function type in its params.in and results.out 
 *  files. The valueDescription is for reading and display. */
OPENSTUDIO_ENUM(DakotaFunctionType,
    ((response_fn)(response))
    ((obj_fn)(objective function))
);

/** \class VariableValueType
 *  \brief List of variable numeric types. Some features are only applicable to one or the 
 *  other.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(VariableValueType,
                ((Continuous))
                ((Discrete))
); 
 *  \endcode */ 
OPENSTUDIO_ENUM(VariableValueType,
                ((Continuous))
                ((Discrete))
);

/** \class UncertaintyType
 *  \brief List of uncertainty types.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(UncertaintyType,
                ((Aleatory))
                ((Epistemic))
);
 *  \endcode
 *  Aleatory uncertainty refers to the case where the uncertainty is inherent to the variable, 
 *  that is, truly stochastic. Epistemic uncertainty arises from a lack of knowledge--that is, 
 *  with more research this type of uncertainty should be removable. Sometimes the uncertainties 
 *  are layered. For example, the mean and variance of an aleatory uncertainty may be modeled as
 *  epistemic uncertainties. */
OPENSTUDIO_ENUM(UncertaintyType,
                ((Aleatory))
                ((Epistemic))
);

/** \class UncertaintyDescriptionType
 *  \brief List of all the uncertainty types supported by DAKOTA.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM(UncertaintyDescriptionType,
                ((Generic)(Generic))
                ((normal_uncertain)(Normal Distribution))
                ((lognormal_uncertain)(Lognormal Distribution))
                ((uniform_uncertain)(Uniform Distribution))
                ((loguniform_uncertain)(Loguniform Distribution))
                ((triangular_uncertain)(Triangular Distribution))
                ((exponential_uncertain)(Exponential Distribution))
                ((beta_uncertain)(Beta Distribution))
                ((gamma_uncertain)(Gamma Distribution))
                ((gumbel_uncertain)(Gumbel Distribution))
                ((frechet_uncertain)(Frechet Distribution))
                ((weibull_uncertain)(Weibull Distribution))
                ((histogram_bin_uncertain)(Histogram Bin Distribution))
                ((poisson_uncertain)(Poisson Distribution))
                ((binomial_uncertain)(Binomial Distribution))
                ((negative_binomial_uncertain)(Negative Binomial Distribution))
                ((geometric_uncertain)(Geometric Distribution))
                ((hypergeometric_uncertain)(Hypergeometric Distribution))
                ((histogram_point_uncertain)(Histogram Point Distribution))
); 
 *  \endcode
 *  Variables decorated with UncertaintyDescriptions of these types can be sampled according
 *  to these distributions using SamplingAlgorithm. The distributions generally apply to either
 *  continuous or discrete variables. The static methods in UncertaintyDescription can be used
 *  to narrow down the applicable uncertainty types for a given situation. To create an
 *  uncertainty description, use one of the concrete classes deriving from
 *  UncertaintyDescription. */
OPENSTUDIO_ENUM(UncertaintyDescriptionType,
                ((Generic)(Generic))
                ((normal_uncertain)(Normal Distribution))
                ((lognormal_uncertain)(Lognormal Distribution))
                ((uniform_uncertain)(Uniform Distribution))
                ((loguniform_uncertain)(Loguniform Distribution))
                ((triangular_uncertain)(Triangular Distribution))
                ((exponential_uncertain)(Exponential Distribution))
                ((beta_uncertain)(Beta Distribution))
                ((gamma_uncertain)(Gamma Distribution))
                ((gumbel_uncertain)(Gumbel Distribution))
                ((frechet_uncertain)(Frechet Distribution))
                ((weibull_uncertain)(Weibull Distribution))
                ((histogram_bin_uncertain)(Histogram Bin Distribution))
                ((poisson_uncertain)(Poisson Distribution))
                ((binomial_uncertain)(Binomial Distribution))
                ((negative_binomial_uncertain)(Negative Binomial Distribution))
                ((geometric_uncertain)(Geometric Distribution))
                ((hypergeometric_uncertain)(Hypergeometric Distribution))
                ((histogram_point_uncertain)(Histogram Point Distribution))
);

typedef std::vector<UncertaintyDescriptionType> UncertaintyDescriptionTypeVector;

} // analysis
} // openstudio

#endif // ANALYSISENUMS
