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

#ifndef MODEL_HEATBALANCEALGORITHM_HPP
#define MODEL_HEATBALANCEALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class HeatBalanceAlgorithm_Impl;

  }  // namespace detail

  /** HeatBalanceAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "HeatBalanceAlgorithm".
 *
 *  HeatBalanceAlgorithm is a unique object that specifies the global heat and moisture algorithms
 *  used across building construction calculations. HeatBalanceAlgorithm does not have a public constructor because it is a unique ModelObject.
 *  To get the HeatBalanceAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<HeatBalanceAlgorithm>().
 *  To get the HeatBalanceAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<HeatBalanceAlgorithm>().
 */
  class MODEL_API HeatBalanceAlgorithm : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~HeatBalanceAlgorithm() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatBalanceAlgorithm(const HeatBalanceAlgorithm& other) = default;
    HeatBalanceAlgorithm(HeatBalanceAlgorithm&& other) = default;
    HeatBalanceAlgorithm& operator=(const HeatBalanceAlgorithm&) = default;
    HeatBalanceAlgorithm& operator=(HeatBalanceAlgorithm&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //static std::vector<std::string> validAlgorithmValues(); TODO delete
    static std::vector<std::string> algorithmValues();

    //@}
    /** @name Getters */
    //@{

    std::string algorithm() const;

    bool isAlgorithmDefaulted() const;

    double surfaceTemperatureUpperLimit() const;

    bool isSurfaceTemperatureUpperLimitDefaulted() const;

    double minimumSurfaceConvectionHeatTransferCoefficientValue() const;

    bool isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

    double maximumSurfaceConvectionHeatTransferCoefficientValue() const;

    bool isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAlgorithm(const std::string& algorithm);

    void resetAlgorithm();

    bool setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit);

    void resetSurfaceTemperatureUpperLimit();

    bool setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue);

    void resetMinimumSurfaceConvectionHeatTransferCoefficientValue();

    bool setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue);

    void resetMaximumSurfaceConvectionHeatTransferCoefficientValue();

    //@}

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new HeatBalanceAlgorithm object in the model.
    explicit HeatBalanceAlgorithm(const Model& model);

    //@}

    /// @cond

    using ImplType = detail::HeatBalanceAlgorithm_Impl;

    friend class detail::HeatBalanceAlgorithm_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit HeatBalanceAlgorithm(Model& model);
    // constructor
    explicit HeatBalanceAlgorithm(std::shared_ptr<detail::HeatBalanceAlgorithm_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.HeatBalanceAlgorithm");

    /// @endcond
  };

  /** \relates HeatBalanceAlgorithm*/
  using OptionalHeatBalanceAlgorithm = boost::optional<HeatBalanceAlgorithm>;

  /** \relates HeatBalanceAlgorithm*/
  using HeatBalanceAlgorithmVector = std::vector<HeatBalanceAlgorithm>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATBALANCEALGORITHM_HPP
