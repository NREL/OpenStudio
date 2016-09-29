/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
