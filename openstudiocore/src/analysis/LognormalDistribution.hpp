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
