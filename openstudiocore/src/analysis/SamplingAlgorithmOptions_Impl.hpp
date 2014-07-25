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

#ifndef ANALYSIS_SAMPLINGALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_SAMPLINGALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions_Impl.hpp"

#include "SamplingAlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** SamplingAlgorithmOptions_Impl is a DakotaAlgorithmOptions_Impl that is the implementation class for SamplingAlgorithmOptions. */

  class ANALYSIS_API SamplingAlgorithmOptions_Impl : public DakotaAlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SamplingAlgorithmOptions_Impl();

    /** Constructor provided for deserialization; not for general use. */
    SamplingAlgorithmOptions_Impl(const boost::optional<SamplingAlgorithmSampleType>& sampleType,
                                  const boost::optional<SamplingAlgorithmRNGType>& rngType,
                                  const std::vector<Attribute>& options);

    virtual ~SamplingAlgorithmOptions_Impl() {}

    virtual AlgorithmOptions clone() const;

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
  
    void clearSeed();  

    /** Places the string "fixed_seed" in the .in file if true, otherwise nothing. */
    void setFixedSeed(bool value);

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static SamplingAlgorithmOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    boost::optional<SamplingAlgorithmSampleType> m_sampleType;
    boost::optional<SamplingAlgorithmRNGType> m_rngType;

   private:
    REGISTER_LOGGER("openstudio.analysis.SamplingAlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_SAMPLINGALGORITHMOPTIONS_IMPL_HPP

