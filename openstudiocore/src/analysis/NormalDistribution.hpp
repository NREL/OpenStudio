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

#ifndef ANALYSIS_NORMALDISTRIBUTION_HPP
#define ANALYSIS_NORMALDISTRIBUTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {
namespace analysis {

/** NormalDistribution is an UncertaintyDescription that can apply aleatory uncertainty to a
 *  continuous variable. */
class ANALYSIS_API NormalDistribution : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Returns a standard normal distribution (mean 0, standardDeviation 1). */
  NormalDistribution();

  NormalDistribution(double mean, double standardDeviation);

  virtual ~NormalDistribution() {}

  //@}

  static UncertaintyDescriptionType type();

  /** @name Getters */
  //@{

  double mean() const;

  double standardDeviation() const;

  boost::optional<double> lowerBound() const;

  boost::optional<double> upperBound() const;

  //@}
  /** @name Setters */
  //@{

  void setMean(double value);

  /* Standard Deviation must be positive. */
  bool setStandardDeviation(double value);

  void setLowerBound(double value);

  void resetLowerBound();

  void setUpperBound(double value);

  void resetUpperBound();

  //@}
 protected:
  explicit NormalDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);
  
  friend class AnalysisObject;
  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.NormalDistribution");
};

} // analysis
} // openstudio

#endif // ANALYSIS_NORMALDISTRIBUTION_HPP
