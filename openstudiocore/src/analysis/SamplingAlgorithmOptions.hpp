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

#ifndef ANALYSIS_SAMPLINGALGORITHMOPTIONS_HPP
#define ANALYSIS_SAMPLINGALGORITHMOPTIONS_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions.hpp"

#include "../utilities/core/Enum.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class SamplingAlgorithmOptions_Impl;
  class AnalysisObject_Impl;

} // detail

// ETH@20120920 TODO: Rename this enum to SamplingAlgorithmType.
/** \class SamplingAlgorithmSampleType
 *  \brief Lists the types of sampling methods offered by the Sampling libary.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( SamplingAlgorithmSampleType,
  ((random)(Random))
  ((lhs)(Latin Hypercube Sampling))
); 
 *  \endcode
 *  The valueName is the DAKOTA key for this algorithm type. The valueDescription is for 
 *  reading and display. */
OPENSTUDIO_ENUM( SamplingAlgorithmSampleType,
  ((random)(Random))
  ((lhs)(Latin Hypercube Sampling))
);

/** \class SamplingAlgorithmRNGType
 *  \brief Lists the types of pseudo-random number generators that may be used with the 
 *  Sampling library.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( SamplingAlgorithmRNGType,
  ((mt19937)(Mersenne Twister))
  ((rnum2)(Random Number Generator 2))
); 
 *  \endcode
 *  The valueName is the DAKOTA key for this pseudo-random number generator. The 
 *  valueDescription is for reading and display. */
OPENSTUDIO_ENUM( SamplingAlgorithmRNGType,
  ((mt19937)(Mersenne Twister))
  ((rnum2)(Random Number Generator 2))
);

/** SamplingAlgorithmOptions is a DakotaAlgorithmOptions class that defines options specific to 
 *  SamplingAlgorithm. */
class ANALYSIS_API SamplingAlgorithmOptions : public DakotaAlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Generates and sets a random seed. The default is to have the seed fixed so that the 
   *  analysis can be restarted in case of a failure. */
  SamplingAlgorithmOptions();

  /** Constructor provided for deserialization; not for general use. */
  SamplingAlgorithmOptions(const boost::optional<SamplingAlgorithmSampleType>& sampleType,
                           const boost::optional<SamplingAlgorithmRNGType>& rngType,
                           const std::vector<Attribute>& options);

  virtual ~SamplingAlgorithmOptions() {}

  //@}
  /** @name Getters */
  //@{

  /** Returns the sampling type if it exists, evaluates to false otherwise. DAKOTA will 
   *  automatically use latin hypercube sampling (LHS) if unspecified. */
  boost::optional<SamplingAlgorithmSampleType> sampleType() const;

  /** Returns the random number generator (RNG) type if it exists, evaluates to false 
   *  otherwise. DAKOTA defaults to Mersenne twister (mt19937). */
  boost::optional<SamplingAlgorithmRNGType> rngType() const;

  /** Returns the user-specified number of samples. */
  int samples() const;

  /** Returns whether or not the user-specified all_variables option is active, if set;
   *  the default value is false. */
  bool allVariables() const;

  /** Returns whether or not the user-specified variance_based_decomp option is active, if set;
   *  the default value is false. */
  bool varianceBasedDecomp() const;

  /** Returns whether or not the user-specified drop_tolerance option is active, if set;
   *  the default value is false. */
  bool dropTolerance() const;

  /** Returns the explict pseudo-random number generator seed if it exists, evaluates to false 
   *  otherwise. */
  boost::optional<int> seed() const;

  /** Returns whether or not the user-specified fixed_seed option is active, if set;
   *  the default value is false. */
  bool fixedSeed() const;

  //@}
  /** @name Setters */
  //@{
  
  void setSampleType(SamplingAlgorithmSampleType value);
  
  void clearSampleType();   
  
  void setRNGType(SamplingAlgorithmRNGType value);
  
  void clearRNGType();   

  /** The number of samples must be greater than zero. */
  bool setSamples(int value);

  /** Places the string "all_variables" in the .in file if true, otherwise nothing. */
  void setAllVariables(bool value);
  
  /** Places the string "variance_based_decomp" in the .in file if true, otherwise nothing.
   *  This is a computationally intensive option; it requires the evaluation of n*(m+2) samples,
   *  where n is the number of samples specified and m is the number of variables. */
  void setVarianceBasedDecomp(bool value);

  /** Places the string "drop_tolerance" in the .in file if true, otherwise nothing. */
  void setDropTolerance(bool value);

  /** Seed value must be greater than zero. */
  bool setSeed(int value);
  
  /** Clears the seed. Use with caution--clearing the seed value will essentially negate the 
   *  Dakota restart functionality, as the samples generated after restart will not match the 
   *  original samples. */
  void clearSeed();  

  /** Places the string "fixed_seed" in the .in file if true, otherwise nothing. */
  void setFixedSeed(bool value);

  //@}
 protected:
  /// @cond
  typedef detail::SamplingAlgorithmOptions_Impl ImplType;

  explicit SamplingAlgorithmOptions(std::shared_ptr<detail::SamplingAlgorithmOptions_Impl> impl);

  friend class detail::SamplingAlgorithmOptions_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class AlgorithmOptions;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.SamplingAlgorithmOptions");
};

/** \relates SamplingAlgorithmOptions*/

typedef boost::optional<SamplingAlgorithmOptions> OptionalSamplingAlgorithmOptions;

/** \relates SamplingAlgorithmOptions*/

typedef std::vector<SamplingAlgorithmOptions> SamplingAlgorithmOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_SAMPLINGALGORITHMOPTIONS_HPP

