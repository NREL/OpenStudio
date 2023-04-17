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

#ifndef MODEL_REFRIGERATIONSYSTEM_HPP
#define MODEL_REFRIGERATIONSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class RefrigerationAirChiller;
  class RefrigerationCase;
  class RefrigerationCompressor;
  class RefrigerationWalkIn;
  class RefrigerationSecondarySystem;
  class RefrigerationCondenserCascade;
  class RefrigerationSubcoolerLiquidSuction;
  class RefrigerationSubcoolerMechanical;
  class ThermalZone;

  namespace detail {

    class RefrigerationSystem_Impl;

  }  // namespace detail

  /** RefrigerationSystem is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:System'. */
  class MODEL_API RefrigerationSystem : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationSystem(const Model& model);

    virtual ~RefrigerationSystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationSystem(const RefrigerationSystem& other) = default;
    RefrigerationSystem(RefrigerationSystem&& other) = default;
    RefrigerationSystem& operator=(const RefrigerationSystem&) = default;
    RefrigerationSystem& operator=(RefrigerationSystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> refrigerationSystemWorkingFluidTypeValues();

    static std::vector<std::string> suctionTemperatureControlTypeValues();

    static std::vector<std::string> intercoolerTypeValues();

    std::vector<IdfObject> remove();

    ModelObject clone(Model model) const;

    // RefrigerationCase will be removed from any ModelObjectList it is already on. Clone the case if you want it on several.
    bool addCase(const RefrigerationCase& refrigerationCase);

    // Removes child from the list, but does not remove the child object itself
    void removeCase(const RefrigerationCase& refrigerationCase);

    // Removes all child object from the list, but does not remove the children themselves
    void removeAllCases();

    std::vector<RefrigerationCase> cases() const;

    // RefrigerationWalkIn will be removed from any ModelObjectList it is already on. Clone the walkin if you want it on several.
    bool addWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    // Removes child from the list, but does not remove the child object itself
    void removeWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    // Removes all child object from the list, but does not remove the children themselves
    void removeAllWalkins();

    std::vector<RefrigerationWalkIn> walkins() const;

    // Compressor will be removed from any ModelObjectList (either as a compressor or high stage compressor) it is already on.
    // Clone the walkin if you want it on several.
    bool addCompressor(const RefrigerationCompressor& compressor);

    // Removes child from the list, but does not remove the child object itself
    void removeCompressor(const RefrigerationCompressor& compressor);

    // Removes all child object from the list, but does not remove the children themselves
    void removeAllCompressors();

    std::vector<RefrigerationCompressor> compressors() const;

    // Compressor will be removed from any ModelObjectList (either as a compressor or high stage compressor) it is already on.
    // Clone the walkin if you want it on several.
    bool addHighStageCompressor(const RefrigerationCompressor& highStageCompressor);

    // Removes child from the list, but does not remove the child object itself
    void removeHighStageCompressor(const RefrigerationCompressor& highStageCompressor);

    // Removes all child object from the list, but does not remove the children themselves
    void removeAllHighStageCompressors();

    std::vector<RefrigerationCompressor> highStageCompressors() const;

    // RefrigerationSecondarySystem will be removed from any ModelObjectList it is already on. Clone it if you want it on several.
    bool addSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystem);

    // Removes child from the list, but does not remove the child object itself
    void removeSecondarySystemLoad(const RefrigerationSecondarySystem& refrigerationSecondarySystem);

    // Removes all child object from the list, but does not remove the children themselves
    void removeAllSecondarySystemLoads();

    std::vector<RefrigerationSecondarySystem> secondarySystemLoads() const;

    // RefrigerationCondenserCascade will be removed from any ModelObjectList it is already on. Clone it if you want it on several.
    bool addCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade);

    // Removes child from the list, but does not remove the child object itself
    void removeCascadeCondenserLoad(const RefrigerationCondenserCascade& refrigerationCondenserCascade);

    // Removes all child object from the list, but does not remove the children themselves
    void removeAllCascadeCondenserLoads();

    std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads() const;

    // RefrigerationAirChiller will be removed from any ModelObjectList it is already on. Clone it if you want it on several.
    bool addAirChiller(const RefrigerationAirChiller& airChiller);

    // Removes child from the list, but does not remove the child object itself
    void removeAirChiller(const RefrigerationAirChiller& airChiller);

    // Removes all child object from the list, but does not remove the children themselves
    void removeAllAirChillers();

    std::vector<RefrigerationAirChiller> airChillers() const;

    /** @name Getters */
    //@{

    boost::optional<ModelObject> refrigerationCondenser() const;

    double minimumCondensingTemperature() const;

    std::string refrigerationSystemWorkingFluidType() const;

    std::string suctionTemperatureControlType() const;

    bool isSuctionTemperatureControlTypeDefaulted() const;

    boost::optional<RefrigerationSubcoolerMechanical> mechanicalSubcooler() const;

    boost::optional<RefrigerationSubcoolerLiquidSuction> liquidSuctionHeatExchangerSubcooler() const;

    double sumUASuctionPiping() const;

    bool isSumUASuctionPipingDefaulted() const;

    boost::optional<ThermalZone> suctionPipingZone() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    std::string numberofCompressorStages() const;

    std::string intercoolerType() const;

    bool isIntercoolerTypeDefaulted() const;

    double shellandCoilIntercoolerEffectiveness() const;

    bool isShellandCoilIntercoolerEffectivenessDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /** \fn bool setRefrigerationCondenser(const ModelObject& refrigerationCondenser)
      \brief Sets the refrigeration condenser for this refrigeration system. Will remove it from any existing Refrigeration System (unicity enforced)
      \param[in] refrigerationCondenser The refrigeration condenser.

      Valid inputs: \n
      \ref RefrigerationCondenserAirCooled \n
      \ref RefrigerationCondenserCascade \n
      \ref RefrigerationCondenserEvaporativeCooled \n
      \ref RefrigerationCondenserWaterCooled \n
  */
    bool setRefrigerationCondenser(const ModelObject& refrigerationCondenser);

    void resetRefrigerationCondenser();

    bool setMinimumCondensingTemperature(double minimumCondensingTemperature);

    bool setRefrigerationSystemWorkingFluidType(const std::string& refrigerationSystemWorkingFluidType);

    bool setSuctionTemperatureControlType(const std::string& suctionTemperatureControlType);

    void resetSuctionTemperatureControlType();

    // Will remove it from any existing Refrigeration System (unicity enforced)
    bool setMechanicalSubcooler(const RefrigerationSubcoolerMechanical& refrigerationSubcoolerMechanical);

    void resetMechanicalSubcooler();

    // Will remove it from any existing Refrigeration System (unicity enforced)
    bool setLiquidSuctionHeatExchangerSubcooler(const RefrigerationSubcoolerLiquidSuction& refrigerationSubcoolerLiquidSuction);

    void resetLiquidSuctionHeatExchangerSubcooler();

    bool setSumUASuctionPiping(double sumUASuctionPiping);

    void resetSumUASuctionPiping();

    bool setSuctionPipingZone(const ThermalZone& thermalZone);

    void resetSuctionPipingZone();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    bool setIntercoolerType(const std::string& intercoolerType);

    void resetIntercoolerType();

    bool setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness);

    void resetShellandCoilIntercoolerEffectiveness();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationSystem_Impl;

    explicit RefrigerationSystem(std::shared_ptr<detail::RefrigerationSystem_Impl> impl);

    friend class detail::RefrigerationSystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationSystem");
  };

  /** \relates RefrigerationSystem*/
  using OptionalRefrigerationSystem = boost::optional<RefrigerationSystem>;

  /** \relates RefrigerationSystem*/
  using RefrigerationSystemVector = std::vector<RefrigerationSystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONSYSTEM_HPP
