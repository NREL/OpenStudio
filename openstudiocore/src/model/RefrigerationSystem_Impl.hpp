/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_REFRIGERATIONSYSTEM_IMPL_HPP
#define MODEL_REFRIGERATIONSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class ModelObjectList;
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

  /** RefrigerationSystem_Impl is a ModelObject_Impl that is the implementation class for RefrigerationSystem.*/
  class MODEL_API RefrigerationSystem_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationSystem_Impl(const IdfObject& idfObject,
                             Model_Impl* model,
                             bool keepHandle);

    RefrigerationSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    RefrigerationSystem_Impl(const RefrigerationSystem_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~RefrigerationSystem_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    std::vector<IdfObject> remove() override;

    ModelObject clone(Model model) const override;

    std::vector<RefrigerationCase> cases() const;

    std::vector<RefrigerationWalkIn> walkins() const;

    std::vector<RefrigerationCompressor> compressors() const;

    std::vector<RefrigerationCompressor> highStageCompressors() const;

    std::vector<RefrigerationSecondarySystem> secondarySystemLoads() const;

    std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads() const;

    std::vector<RefrigerationAirChiller> airChillers() const;

    template <class T>
    std::vector<T> listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const;

    boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList() const;

    boost::optional<ModelObjectList> refrigerationTransferLoadList() const;

    boost::optional<ModelObject> refrigerationCondenser() const;

    ModelObjectList compressorList() const;

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

    boost::optional<ModelObjectList> highStageCompressorList() const;

    virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

    virtual std::vector<std::string> emsInternalVariableNames() const override;

    //@}
    /** @name Setters */
    //@{

    template <class T>
    void removeAllTemplate( boost::optional<ModelObjectList>& modelObjectList );

    template <class T>
    void removeTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList );

    template <class T>
    bool addTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList );

    bool addCase( const RefrigerationCase & refrigerationCase);

    void removeCase( const RefrigerationCase & refrigerationCase);

    void removeAllCases();

    bool addWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeAllWalkins();

    bool addCompressor( const RefrigerationCompressor & compressor);

    void removeCompressor( const RefrigerationCompressor & compressor);

    void removeAllCompressors();

    bool addHighStageCompressor( const RefrigerationCompressor & highStageCompressor);

    void removeHighStageCompressor( const RefrigerationCompressor & highStageCompressor);

    void removeAllHighStageCompressors();

    bool addSecondarySystemLoad( const RefrigerationSecondarySystem & refrigerationSecondarySystem);

    void removeSecondarySystemLoad( const RefrigerationSecondarySystem & refrigerationSecondarySystem);

    void removeAllSecondarySystemLoads();

    bool addCascadeCondenserLoad( const RefrigerationCondenserCascade & refrigerationCondenserCascade);

    void removeCascadeCondenserLoad( const RefrigerationCondenserCascade & refrigerationCondenserCascade);

    void removeAllCascadeCondenserLoads();

    bool addAirChiller( const RefrigerationAirChiller & airChiller);

    void removeAirChiller( const RefrigerationAirChiller & airChiller);

    void removeAllAirChillers();

    bool setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetRefrigeratedCaseAndWalkInList();

    bool setRefrigerationTransferLoadList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetRefrigerationTransferLoadList();

    bool setRefrigerationCondenser(const ModelObject& refrigerationCondenser);

    bool setCompressorList(const ModelObjectList& modelObjectList);

    bool setMinimumCondensingTemperature(double minimumCondensingTemperature);

    bool setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType);

    bool setSuctionTemperatureControlType(std::string suctionTemperatureControlType);

    void resetSuctionTemperatureControlType();

    bool setMechanicalSubcooler(const boost::optional<RefrigerationSubcoolerMechanical>& refrigerationSubcoolerMechanical);

    void resetMechanicalSubcooler();

    bool setLiquidSuctionHeatExchangerSubcooler(const boost::optional<RefrigerationSubcoolerLiquidSuction>& refrigerationSubcoolerLiquidSuction);

    void resetLiquidSuctionHeatExchangerSubcooler();

    bool setSumUASuctionPiping(double sumUASuctionPiping);

    void resetSumUASuctionPiping();

    bool setSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone);

    void resetSuctionPipingZone();

    bool setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    bool setIntercoolerType(std::string intercoolerType);

    void resetIntercoolerType();

    bool setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness);

    void resetShellandCoilIntercoolerEffectiveness();

    bool setHighStageCompressorList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetHighStageCompressorList();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationSystem");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<ModelObject> optionalRefrigerationCondenser() const;
    boost::optional<ModelObjectList> optionalCompressorList() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONSYSTEM_IMPL_HPP
