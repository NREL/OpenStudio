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

#ifndef ANALYSIS_LOGNORMALDISTRIBUTION_HPP
#define ANALYSIS_LOGNORMALDISTRIBUTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {
namespace analysis {

/** LognormalDistribution is an UncertaintyDescription that can apply aleatory uncertainty to a
 *  continuous variable. */
class ANALYSIS_API LognormalDistribution : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** This distribution requires one of the three following pairs: mean and standardDeviation,
   *  mean and errorFactor, or lambda and zeta. The default constructor creates a distribution with
   *  mean 1 and standard deviation of 1.*/
  LognormalDistribution();

  virtual ~LognormalDistribution() {}

  //@}

  static UncertaintyDescriptionType type();

  /** @name Getters */
  //@{

  boost::optional<double> mean() const;

  boost::optional<double> standardDeviation() const;
  
  boost::optional<double> errorFactor() const;

  boost::optional<double> lambda() const;

  boost::optional<double> zeta() const;

  boost::optional<double> lowerBound() const;

  boost::optional<double> upperBound() const;

  //@}
  /** @name Setters */
  //@{

  /* Mean must be positive. */
  bool setMean(double value);

  void resetMean();

  /* Standard Deviation must be positive. */
  bool setStandardDeviation(double value);

  void resetStandardDeviation();

  /* Error factor must be greater than 1. */
  bool setErrorFactor(double value);

  void resetErrorFactor();

  void setLambda(double value);

  void resetLambda();

  /* Zeta must be positive. */
  bool setZeta(double value);

  void resetZeta();

  /* Bounds must be positive. */
  bool setLowerBound(double value);

  void resetLowerBound();

  void setUpperBound(double value);

  void resetUpperBound();

  //@}
 protected:
  explicit LognormalDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class AnalysisObject;
  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.LognormalDistribution");
};

} // analysis
} // openstudio

#endif // ANALYSIS_LOGNORMALDISTRIBUTION_HPP
