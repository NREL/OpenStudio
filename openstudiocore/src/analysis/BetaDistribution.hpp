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

#ifndef ANALYSIS_BETADISTRIBUTION_HPP
#define ANALYSIS_BETADISTRIBUTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {
namespace analysis {

/** BetaDistribution is an UncertaintyDescription that can apply aleatory uncertainty to a
 *  continuous variable. */
class ANALYSIS_API BetaDistribution : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Returns a beta distribution with alpha of 2, beta of 2, lower bound of 0, and upper bound of
   *  1. */
  BetaDistribution();

  BetaDistribution(double alpha, double beta, double lowerBound, double upperBound);

  virtual ~BetaDistribution() {}

  //@}

  static UncertaintyDescriptionType type();

  /** @name Getters */
  //@{

  double alpha() const;

  double beta() const;

  double lowerBound() const;

  double upperBound() const;

  //@}
  /** @name Setters */
  //@{

  void setAlpha(double value);

  void setBeta(double value);

  void setLowerBound(double value);

  void setUpperBound(double value);

  //@}
 protected:
  explicit BetaDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class AnalysisObject;
  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.BetaDistribution");
};

} // analysis
} // openstudio

#endif // ANALYSIS_BETADISTRIBUTION_HPP
