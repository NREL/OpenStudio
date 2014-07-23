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

#ifndef ANALYSIS_HISTOGRAMBINDISTRIBUTION_HPP
#define ANALYSIS_HISTOGRAMBINDISTRIBUTION_HPP

#include "AnalysisAPI.hpp"
#include "UncertaintyDescription.hpp"

namespace openstudio {
namespace analysis {

/** HistogramBinDistribution is an UncertaintyDescription that can apply aleatory uncertainty to a
 *  continuous variable. */
class ANALYSIS_API HistogramBinDistribution : public UncertaintyDescription {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Returns a histogram bin distribution with abscissas of [0,1] and counts of [1,0]. */
  HistogramBinDistribution();

  /** This distribution requires either counts or ordinates; the defaut here is counts. */
  HistogramBinDistribution(std::vector<double> abscissas, std::vector<double> counts);

  virtual ~HistogramBinDistribution() {}

  //@}

  static UncertaintyDescriptionType type();

  /** @name Getters */
  //@{

  std::vector<double> abscissas() const;

  std::vector<double> counts() const;

  std::vector<double> ordinates() const;

  boost::optional<int> numPairs() const;

  //@}
  /** @name Setters */
  //@{

  /** Abscissas values must be strictly increasing. */
  bool setAbscissas(const std::vector<double>& value);

  /** The last value of counts must be 0; all other values must be positive. */
  bool setCounts(const std::vector<double>& value);

  void resetCounts();

  /** The last value of ordinates must be 0; all other values must be positive. */
  bool setOrdinates(const std::vector<double>& value);

  void resetOrdinates();

  //@}
 protected:
  explicit HistogramBinDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl);

  friend class AnalysisObject;
  friend class UncertaintyDescription;
 private:
  REGISTER_LOGGER("openstudio.analysis.HistogramBinDistribution");
};

} // analysis
} // openstudio

#endif // ANALYSIS_HISTOGRAMBINDISTRIBUTION_HPP
