/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_PERFORMANCEPRECISIONTRADEOFFS_HPP
#define MODEL_PERFORMANCEPRECISIONTRADEOFFS_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PerformancePrecisionTradeoffs_Impl;

  }  // namespace detail

  /** PerformancePrecisionTradeoffs is a ModelObject that wraps the OpenStudio IDD object 'OS:PerformancePrecisionTradeoffs'. */
  class MODEL_API PerformancePrecisionTradeoffs : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~PerformancePrecisionTradeoffs() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PerformancePrecisionTradeoffs(const PerformancePrecisionTradeoffs& other) = default;
    PerformancePrecisionTradeoffs(PerformancePrecisionTradeoffs&& other) = default;
    PerformancePrecisionTradeoffs& operator=(const PerformancePrecisionTradeoffs&) = default;
    PerformancePrecisionTradeoffs& operator=(PerformancePrecisionTradeoffs&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> zoneRadiantExchangeAlgorithmValues();
    static std::vector<std::string> validZoneRadiantExchangeAlgorithmValues();

    static std::vector<std::string> overrideModeValues();
    static std::vector<std::string> validOverrideModeValues();

    /** @name Getters */
    //@{

    bool useCoilDirectSolutions() const;
    bool isUseCoilDirectSolutionsDefaulted() const;

    std::string zoneRadiantExchangeAlgorithm() const;
    bool isZoneRadiantExchangeAlgorithmDefaulted() const;

    std::string overrideMode() const;
    bool isOverrideModeDefaulted() const;

    double maxZoneTempDiff() const;
    bool isMaxZoneTempDiffDefaulted() const;

    double maxAllowedDelTemp() const;
    bool isMaxAllowedDelTempDefaulted() const;

    bool useRepresentativeSurfacesforCalculations() const;
    bool isUseRepresentativeSurfacesforCalculationsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setUseCoilDirectSolutions(bool useCoilDirectSolutions);
    void resetUseCoilDirectSolutions();

    bool setZoneRadiantExchangeAlgorithm(const std::string& zoneRadiantExchangeAlgorithm);
    void resetZoneRadiantExchangeAlgorithm();

    bool setOverrideMode(const std::string& overrideMode);
    void resetOverrideMode();

    bool setMaxZoneTempDiff(double maxZoneTempDiff);
    void resetMaxZoneTempDiff();

    bool setMaxAllowedDelTemp(double maxAllowedDelTemp);
    void resetMaxAllowedDelTemp();

    bool setUseRepresentativeSurfacesforCalculations(bool useRepresentativeSurfacesforCalculations);
    void resetUseRepresentativeSurfacesforCalculations();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    explicit PerformancePrecisionTradeoffs(Model& model);

    /// @cond
    using ImplType = detail::PerformancePrecisionTradeoffs_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::PerformancePrecisionTradeoffs_Impl;

    explicit PerformancePrecisionTradeoffs(std::shared_ptr<detail::PerformancePrecisionTradeoffs_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PerformancePrecisionTradeoffs");
  };

  /** \relates PerformancePrecisionTradeoffs*/
  using OptionalPerformancePrecisionTradeoffs = boost::optional<PerformancePrecisionTradeoffs>;

  /** \relates PerformancePrecisionTradeoffs*/
  using PerformancePrecisionTradeoffsVector = std::vector<PerformancePrecisionTradeoffs>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PERFORMANCEPRECISIONTRADEOFFS_HPP
