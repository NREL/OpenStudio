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

#ifndef ANALYSIS_UNIFORMDISTRIBUTION_HPP
#define ANALYSIS_UNIFORMDISTRIBUTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {
namespace analysis {

/** UniformDistribution is an UncertaintyDescription that can apply aleatory uncertainty to a
 *  continuous variable. */
class ANALYSIS_API UniformDistribution : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Returns a uniform distribution with lower bound of 0 and upper bound of 1. */
  UniformDistribution();

  UniformDistribution(double lowerBound, double upperBound);

  virtual ~UniformDistribution() {}

  //@}

  static UncertaintyDescriptionType type();

  /** @name Getters */
  //@{

  double lowerBound() const;

  double upperBound() const;

  //@}
  /** @name Setters */
  //@{

  void setLowerBound(double value);

  void setUpperBound(double value);

  //@}
 protected:
  explicit UniformDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class AnalysisObject;
  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.UniformDistribution");
};

} // analysis
} // openstudio

#endif // ANALYSIS_UNIFORMDISTRIBUTION_HPP
