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

#ifndef MODEL_REFRIGERATIONCOMPRESSOR_HPP
#define MODEL_REFRIGERATIONCOMPRESSOR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class RefrigerationSystem;
  class CurveBicubic;

  namespace detail {

    class RefrigerationCompressor_Impl;

  }  // namespace detail

  /** RefrigerationCompressor is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Compressor'. */
  class MODEL_API RefrigerationCompressor : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationCompressor(const Model& model);

    virtual ~RefrigerationCompressor() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationCompressor(const RefrigerationCompressor& other) = default;
    RefrigerationCompressor(RefrigerationCompressor&& other) = default;
    RefrigerationCompressor& operator=(const RefrigerationCompressor&) = default;
    RefrigerationCompressor& operator=(RefrigerationCompressor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> modeofOperationValues();

    /** @name Getters */
    //@{

    CurveBicubic refrigerationCompressorPowerCurve() const;

    CurveBicubic refrigerationCompressorCapacityCurve() const;

    boost::optional<double> ratedSuperheat() const;

    boost::optional<double> ratedReturnGasTemperature() const;

    boost::optional<double> ratedLiquidTemperature() const;

    boost::optional<double> ratedSubcooling() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    std::string modeofOperation() const;

    // bool isModeofOperationDefaulted() const;

    boost::optional<CurveBicubic> transcriticalCompressorPowerCurve() const;

    boost::optional<CurveBicubic> transcriticalCompressorCapacityCurve() const;

    // Returns the parent RefrigerationSystem if any
    boost::optional<RefrigerationSystem> system() const;

    //@}
    /** @name Setters */
    //@{

    bool setRefrigerationCompressorPowerCurve(const CurveBicubic& curveBicubic);

    bool setRefrigerationCompressorCapacityCurve(const CurveBicubic& curveBicubic);

    bool setRatedSuperheat(double ratedSuperheat);

    void resetRatedSuperheat();

    bool setRatedReturnGasTemperature(double ratedReturnGasTemperature);

    void resetRatedReturnGasTemperature();

    bool setRatedLiquidTemperature(double ratedLiquidTemperature);

    void resetRatedLiquidTemperature();

    bool setRatedSubcooling(double ratedSubcooling);

    void resetRatedSubcooling();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    // bool setModeofOperation(const std::string& modeofOperation);

    // void resetModeofOperation();

    bool setTranscriticalCompressorPowerCurve(const CurveBicubic& curveBicubic);

    void resetTranscriticalCompressorPowerCurve();

    bool setTranscriticalCompressorCapacityCurve(const CurveBicubic& curveBicubic);

    void resetTranscriticalCompressorCapacityCurve();

    // Remove from parentt system if any
    void removeFromSystem();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationCompressor_Impl;

    explicit RefrigerationCompressor(std::shared_ptr<detail::RefrigerationCompressor_Impl> impl);

    friend class detail::RefrigerationCompressor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCompressor");
  };

  /** \relates RefrigerationCompressor*/
  using OptionalRefrigerationCompressor = boost::optional<RefrigerationCompressor>;

  /** \relates RefrigerationCompressor*/
  using RefrigerationCompressorVector = std::vector<RefrigerationCompressor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCOMPRESSOR_HPP
