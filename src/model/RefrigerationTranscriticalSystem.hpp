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

#ifndef MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_HPP
#define MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class RefrigerationCase;
  class RefrigerationCompressor;
  class RefrigerationWalkIn;
  class RefrigerationGasCoolerAirCooled;
  class ThermalZone;

  namespace detail {

    class RefrigerationTranscriticalSystem_Impl;

  }  // namespace detail

  /** RefrigerationTranscriticalSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:TranscriticalSystem'. */
  class MODEL_API RefrigerationTranscriticalSystem : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationTranscriticalSystem(const Model& model);

    virtual ~RefrigerationTranscriticalSystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationTranscriticalSystem(const RefrigerationTranscriticalSystem& other) = default;
    RefrigerationTranscriticalSystem(RefrigerationTranscriticalSystem&& other) = default;
    RefrigerationTranscriticalSystem& operator=(const RefrigerationTranscriticalSystem&) = default;
    RefrigerationTranscriticalSystem& operator=(RefrigerationTranscriticalSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> refrigerationSystemWorkingFluidTypeValues();

    std::vector<IdfObject> remove();

    ModelObject clone(Model model) const;

    bool addMediumTemperatureCase(const RefrigerationCase& refrigerationCase);

    void removeMediumTemperatureCase(const RefrigerationCase& refrigerationCase);

    void removeAllMediumTemperatureCases();

    std::vector<RefrigerationCase> mediumTemperatureCases() const;

    bool addLowTemperatureCase(const RefrigerationCase& refrigerationCase);

    void removeLowTemperatureCase(const RefrigerationCase& refrigerationCase);

    void removeAllLowTemperatureCases();

    std::vector<RefrigerationCase> lowTemperatureCases() const;

    bool addMediumTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    void removeMediumTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    void removeAllMediumTemperatureWalkins();

    std::vector<RefrigerationWalkIn> mediumTemperatureWalkins() const;

    bool addLowTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    void removeLowTemperatureWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    void removeAllLowTemperatureWalkins();

    std::vector<RefrigerationWalkIn> lowTemperatureWalkins() const;

    bool addHighPressureCompressor(const RefrigerationCompressor& compressor);

    void removeHighPressureCompressor(const RefrigerationCompressor& compressor);

    void removeAllHighPressureCompressors();

    std::vector<RefrigerationCompressor> highPressureCompressors() const;

    bool addLowPressureCompressor(const RefrigerationCompressor& compressor);

    void removeLowPressureCompressor(const RefrigerationCompressor& compressor);

    void removeAllLowPressureCompressors();

    std::vector<RefrigerationCompressor> lowPressureCompressors() const;

    /** @name Getters */
    //@{

    boost::optional<RefrigerationGasCoolerAirCooled> refrigerationGasCooler() const;

    double receiverPressure() const;

    bool isReceiverPressureDefaulted() const;

    double subcoolerEffectiveness() const;

    bool isSubcoolerEffectivenessDefaulted() const;

    std::string refrigerationSystemWorkingFluidType() const;

    double sumUASuctionPipingforMediumTemperatureLoads() const;

    bool isSumUASuctionPipingforMediumTemperatureLoadsDefaulted() const;

    boost::optional<ThermalZone> mediumTemperatureSuctionPipingZone() const;

    double sumUASuctionPipingforLowTemperatureLoads() const;

    bool isSumUASuctionPipingforLowTemperatureLoadsDefaulted() const;

    boost::optional<ThermalZone> lowTemperatureSuctionPipingZone() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRefrigerationGasCooler(const RefrigerationGasCoolerAirCooled& refrigerationGasCoolerAirCooled);

    bool setReceiverPressure(double receiverPressure);

    void resetReceiverPressure();

    bool setSubcoolerEffectiveness(double subcoolerEffectiveness);

    void resetSubcoolerEffectiveness();

    bool setRefrigerationSystemWorkingFluidType(const std::string& refrigerationSystemWorkingFluidType);

    bool setSumUASuctionPipingforMediumTemperatureLoads(double sumUASuctionPipingforMediumTemperatureLoads);

    void resetSumUASuctionPipingforMediumTemperatureLoads();

    bool setMediumTemperatureSuctionPipingZone(const ThermalZone& thermalZone);

    void resetMediumTemperatureSuctionPipingZone();

    bool setSumUASuctionPipingforLowTemperatureLoads(double sumUASuctionPipingforLowTemperatureLoads);

    void resetSumUASuctionPipingforLowTemperatureLoads();

    bool setLowTemperatureSuctionPipingZone(const ThermalZone& thermalZone);

    void resetLowTemperatureSuctionPipingZone();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationTranscriticalSystem_Impl;

    explicit RefrigerationTranscriticalSystem(std::shared_ptr<detail::RefrigerationTranscriticalSystem_Impl> impl);

    friend class detail::RefrigerationTranscriticalSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationTranscriticalSystem");
  };

  /** \relates RefrigerationTranscriticalSystem*/
  using OptionalRefrigerationTranscriticalSystem = boost::optional<RefrigerationTranscriticalSystem>;

  /** \relates RefrigerationTranscriticalSystem*/
  using RefrigerationTranscriticalSystemVector = std::vector<RefrigerationTranscriticalSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_HPP
