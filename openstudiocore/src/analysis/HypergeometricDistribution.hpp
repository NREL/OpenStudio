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

#ifndef ANALYSIS_HYPERGEOMETRICDISTRIBUTION_HPP
#define ANALYSIS_HYPERGEOMETRICDISTRIBUTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {
namespace analysis {

/** HypergeometricDistribution is an UncertaintyDescription that can apply aleatory uncertainty to a
 *  discrete variable. */
class ANALYSIS_API HypergeometricDistribution : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Returns a hypergeometric distribution with total population of 20, selected population of 10,
   *  and number of draws of 10. */
  HypergeometricDistribution();

  HypergeometricDistribution(int totalPopulation, int selectedPopulation, int numDrawn);

  virtual ~HypergeometricDistribution() {}

  //@}

  static UncertaintyDescriptionType type();

  /** @name Getters */
  //@{

  int totalPopulation() const;

  int selectedPopulation() const;

  int numDrawn() const;

  //@}
  /** @name Setters */
  //@{

  /* Total population must be nonnegative. */
  bool setTotalPopulation(int value);

  /* Selected population must be nonnegative. */
  bool setSelectedPopulation(int value);

  /* Number Drawn must be nonnegative. */
  bool setNumDrawn(int value);

  //@}
 protected:
  explicit HypergeometricDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class AnalysisObject;
  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.HypergeometricDistribution");
};

} // analysis
} // openstudio

#endif // ANALYSIS_HYPERGEOMETRICDISTRIBUTION_HPP
