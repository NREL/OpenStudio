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

#ifndef ANALYSIS_POISSONDISTRIBUTION_HPP
#define ANALYSIS_POISSONDISTRIBUTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {
namespace analysis {

/** PoissonDistribution is an UncertaintyDescription that can apply aleatory uncertainty to a
 *  discrete variable. */
class ANALYSIS_API PoissonDistribution : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Returns a Poisson distribution with lambda of 1. */
  PoissonDistribution();

  PoissonDistribution(double lambda);

  virtual ~PoissonDistribution() {}

  //@}

  static UncertaintyDescriptionType type();

  /** @name Getters */
  //@{

  double lambda() const;

  //@}
  /** @name Setters */
  //@{

  /** Lambda must be a positive value. */
  bool setLambda(double value);

  //@}
 protected:
  explicit PoissonDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class AnalysisObject;
  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.PoissonDistribution");
};

} // analysis
} // openstudio

#endif // ANALYSIS_POISSONDISTRIBUTION_HPP
