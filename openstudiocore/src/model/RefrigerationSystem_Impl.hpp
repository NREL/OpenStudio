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

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    std::vector<IdfObject> remove();

    ModelObject clone(Model model) const;

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

    void setMinimumCondensingTemperature(double minimumCondensingTemperature);

    bool setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType);

    bool setSuctionTemperatureControlType(std::string suctionTemperatureControlType);

    void resetSuctionTemperatureControlType();

    bool setMechanicalSubcooler(const boost::optional<RefrigerationSubcoolerMechanical>& refrigerationSubcoolerMechanical);

    void resetMechanicalSubcooler();

    bool setLiquidSuctionHeatExchangerSubcooler(const boost::optional<RefrigerationSubcoolerLiquidSuction>& refrigerationSubcoolerLiquidSuction);

    void resetLiquidSuctionHeatExchangerSubcooler();

    void setSumUASuctionPiping(double sumUASuctionPiping);

    void resetSumUASuctionPiping();

    bool setSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone);

    void resetSuctionPipingZone();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    bool setIntercoolerType(std::string intercoolerType);

    void resetIntercoolerType();

    void setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness);

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

