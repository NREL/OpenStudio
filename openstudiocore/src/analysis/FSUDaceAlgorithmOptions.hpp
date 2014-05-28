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

#ifndef ANALYSIS_FSUDACEALGORITHMOPTIONS_HPP
#define ANALYSIS_FSUDACEALGORITHMOPTIONS_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions.hpp"

#include "../utilities/core/Enum.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class FSUDaceAlgorithmOptions_Impl;
  class AnalysisObject_Impl;

} // detail

/** \class FSUDaceAlgorithmType
 *  \brief Lists the types of sampling methods offered by the FSU Dace libary.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( FSUDaceAlgorithmType,
  ((halton)(Halton))
  ((hammersley)(Hammersley))
  ((cvt)(Centroidal Voronoi Tesselation))
);
 *  \endcode
 *  The valueName is the DAKOTA key for this algorithm type. The valueDescription is for 
 *  reading and display. */
OPENSTUDIO_ENUM( FSUDaceAlgorithmType,
  ((halton)(Halton))
  ((hammersley)(Hammersley))
  ((cvt)(Centroidal Voronoi Tesselation))
);

/** \class FSUDaceCvtTrialType
 *  \brief Lists the types of trials that may be used with the FSUDaceAlgorithmType::cvt. 
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual 
 *  macro call is: 
 *  \code
OPENSTUDIO_ENUM( FSUDaceCvtTrialType,
  ((random)(Random))
  ((grid)(Grid))
  ((halton)(Halton))
);
 *  \endcode
 *  The valueName is the DAKOTA key for this CVT trial type. The valueDescription is for 
 *  reading and display. */
OPENSTUDIO_ENUM( FSUDaceCvtTrialType,
  ((random)(Random))
  ((grid)(Grid))
  ((halton)(Halton))
);

/** FSUDaceAlgorithmOptions is a DakotaAlgorithmOptions class that defines options specific to 
 *  FSUDaceAlgorithm. */
class ANALYSIS_API FSUDaceAlgorithmOptions : public DakotaAlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Sets the algorithmType, and also generates and sets a random seed. The default is to have 
   *  the seed fixed so that the analysis can be restarted in case of a failure. */
  explicit FSUDaceAlgorithmOptions(const FSUDaceAlgorithmType& algorithmType);

  /** Constructor provided for deserialization; not for general use. */
  FSUDaceAlgorithmOptions(const FSUDaceAlgorithmType& algorithmType,
                          const boost::optional<FSUDaceCvtTrialType>& trialType,
                          const std::vector<Attribute>& options);

  virtual ~FSUDaceAlgorithmOptions() {}

  //@}
  /** @name Getters */
  //@{

  FSUDaceAlgorithmType algorithmType() const;

  /** Returns the trial type if it exists, evaluates to false otherwise. Only applies to
   *  FSUDaceAlgorithmType::cvt. */
  boost::optional<FSUDaceCvtTrialType> trialType() const;

  /** Returns the user-specified number of samples to be used. Applies to all
   *  \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink. */
  int samples() const;

  /** Returns the user-specified starting sequence number. Only applicable to 
   *  FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. */
  std::vector<int> sequenceStart() const;

  /** Returns the user-specified sequence leap number. Only applicable to 
   *  FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. */
  std::vector<int> sequenceLeap() const;

  /** Returns whether or not the user-specified fixed_sequence option is active, if set;
   *  the default value is false. Only applicable to FSUDaceAlgorithmType::halton and 
   *  FSUDaceAlgorithmType::hammersley. */
  bool fixedSequence() const;

  /** Returns whether or not the user-specified latinize option is active, if set;
   *  the default value is false. Applies to all \link FSUDaceAlgorithmType 
   *  FSUDaceAlgorithmTypes \endlink. */
  bool latinize() const;

  /** Returns whether or not the user-specified variance_based_decomp option is active, if set;
   *  the default value is false. Applies to all \link FSUDaceAlgorithmType 
   *  FSUDaceAlgorithmTypes \endlink. */
  bool varianceBasedDecomp() const;

  /** Returns whether or not the user-specified quality_metrics option is active, if set;
   *  the default value is false. Applies to all \link FSUDaceAlgorithmType 
   *  FSUDaceAlgorithmTypes \endlink. */
  bool qualityMetrics() const;

  /** Returns the explict pseudo-random number generator seed if it exists, evaluates to false 
   *  otherwise. Only applies to FSUDaceAlgorithmType::cvt. */
  boost::optional<int> seed() const;

  /** Returns the number of trials specified if it exists, evaluates to false 
   *  otherwise. Only applies to FSUDaceAlgorithmType::cvt. */
  boost::optional<int> numTrials() const;

  /** Returns whether or not the user-specified fixed_seed option is active, if set;
   *  the default value is false. Only applies to FSUDaceAlgorithmType::cvt. */
  bool fixedSeed() const;

  //@}
  /** @name Setters */
  //@{

  /** The number of samples must be greater than zero. Applies to all \link FSUDaceAlgorithmType 
   *  FSUDaceAlgorithmTypes \endlink. */
  bool setSamples(int value);

  /** To set, there must be one value for every variable, and all values must be nonnegative. 
   *  Only applicable to FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. 
   *  DAKOTA provides defaults if left unspecified. */
  bool setSequenceStart(const std::vector<int>& value);
  
  void clearSequenceStart();  

  /** To set, There must be one value for every variable, and all values must be greater than zero. 
   *  Only applicable to FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. 
   *  DAKOTA provides defaults if left unspecified. */
  bool setSequenceLeap(const std::vector<int>& value);
  
  void clearSequenceLeap();  

  /** Places the string "fixed_sequence" in the .in file if true, otherwise prints nothing.
   *  Only applicable to FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. */
  void setFixedSequence(bool value);

  /** Places the string "latinize" in the .in file if true, otherwise prints nothing.
   *  Applies to all \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink.*/
  void setLatinize(bool value);
  
  /** Places the string "variance_based_decomp" in the .in file if true, otherwise prints nothing.
   *  Applies to all \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink. This is a 
   *  computationally intestisive option; it requires the evaluation of n*(m+2) samples, where n 
   *  is the number of samples specified and m is the number of variables. */
  void setVarianceBasedDecomp(bool value);

  /** Places the string "quality_metrics" in the .in file if true, otherwise prints nothing.
   *  Applies to all \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink. */
  void setQualityMetrics(bool value);

  /** Seed value must be greater than zero. Only applies to FSUDaceAlgorithmType::cvt. */
  bool setSeed(int value);
  
  /** Clears the seed. Use with caution--clearing the seed value will essentially negate the 
   *  Dakota restart functionality, as the samples generated after restart will not match the 
   *  original samples. */
  void clearSeed();  

  /** Number of Trials must be greater than zero and is typically very large (>10000). Only 
   *  applies to FSUDaceAlgorithmType::cvt. */
  bool setNumTrials(int value);
  
  void clearNumTrials();  

  /** Places the string "fixed_seed" in the .in file if true, otherwise prints nothing.
   *  Only applies to FSUDaceAlgorithmType::cvt. */
  void setFixedSeed(bool value);

  /** Specifies the CVT trial type. Only applies to FSUDaceAlgorithmType::cvt. Defaults to random
   *  if left unspecified. */
  void setTrialType(FSUDaceCvtTrialType value);
  
  void clearTrialType();  

  //@}
 protected:
  /// @cond
  typedef detail::FSUDaceAlgorithmOptions_Impl ImplType;

  explicit FSUDaceAlgorithmOptions(std::shared_ptr<detail::FSUDaceAlgorithmOptions_Impl> impl);

  friend class detail::FSUDaceAlgorithmOptions_Impl;
  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class AlgorithmOptions;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.FSUDaceAlgorithmOptions");
};

/** \relates FSUDaceAlgorithmOptions*/

typedef boost::optional<FSUDaceAlgorithmOptions> OptionalFSUDaceAlgorithmOptions;

/** \relates FSUDaceAlgorithmOptions*/

typedef std::vector<FSUDaceAlgorithmOptions> FSUDaceAlgorithmOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_FSUDACEALGORITHMOPTIONS_HPP

