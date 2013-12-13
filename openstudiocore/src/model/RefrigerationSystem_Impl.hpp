/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

class ModelObjectList;
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

    std::vector<RefrigerationSecondarySystem> secondarySystemLoads() const;

    std::vector<RefrigerationCondenserCascade> cascadeCondenserLoads() const;

    template <class T>
    std::vector<T> listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const;

    // TODO: Check return type. From object lists, some candidates are: ModelObjectList.
    boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList() const;

    // TODO: Check return type. From object lists, some candidates are: ModelObjectList.
    boost::optional<ModelObjectList> refrigerationTransferLoadList() const;

    // TODO: Check return type. From object lists, some candidates are: RefrigerationAllTypesCondenser.
    boost::optional<ModelObject> refrigerationCondenser() const;

    // TODO: Check return type. From object lists, some candidates are: ModelObjectList.
    ModelObjectList compressorList() const;

    double minimumCondensingTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: Fluid.
    std::string refrigerationSystemWorkingFluidType() const;

    std::string suctionTemperatureControlType() const;

    bool isSuctionTemperatureControlTypeDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: RefrigerationSubcoolerMechanical.
    boost::optional<RefrigerationSubcoolerMechanical> mechanicalSubcooler() const;

    // TODO: Check return type. From object lists, some candidates are: RefrigerationSubcoolerLiquidSuction.
    boost::optional<RefrigerationSubcoolerLiquidSuction> liquidSuctionHeatExchangerSubcooler() const;

    double sumUASuctionPiping() const;

    bool isSumUASuctionPipingDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    boost::optional<ThermalZone> suctionPipingZone() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    // std::string numberofCompressorStages() const;

    // bool isNumberofCompressorStagesDefaulted() const;

    std::string intercoolerType() const;

    bool isIntercoolerTypeDefaulted() const;

    double shellandCoilIntercoolerEffectiveness() const;

    bool isShellandCoilIntercoolerEffectivenessDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: ModelObjectList.
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

    bool addSecondarySystemLoad( const RefrigerationSecondarySystem & refrigerationSecondarySystem);

    void removeSecondarySystemLoad( const RefrigerationSecondarySystem & refrigerationSecondarySystem);

    void removeAllSecondarySystemLoads();

    bool addCascadeCondenserLoad( const RefrigerationCondenserCascade & refrigerationCondenserCascade);

    void removeCascadeCondenserLoad( const RefrigerationCondenserCascade & refrigerationCondenserCascade);

    void removeAllCascadeCondenserLoads();

    // TODO: Check argument type. From object lists, some candidates are: ModelObjectList.
    bool setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetRefrigeratedCaseAndWalkInList();

    // TODO: Check argument type. From object lists, some candidates are: ModelObjectList.
    bool setRefrigerationTransferLoadList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetRefrigerationTransferLoadList();

    // TODO: Check argument type. From object lists, some candidates are: RefrigerationAllTypesCondenser.
    bool setRefrigerationCondenser(const ModelObject& refrigerationCondenser);

    // TODO: Check argument type. From object lists, some candidates are: ModelObjectList.
    bool setCompressorList(const ModelObjectList& modelObjectList);

    void setMinimumCondensingTemperature(double minimumCondensingTemperature);

    // TODO: Check argument type. From object lists, some candidates are: Fluid.
    bool setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType);

    bool setSuctionTemperatureControlType(std::string suctionTemperatureControlType);

    void resetSuctionTemperatureControlType();

    // TODO: Check argument type. From object lists, some candidates are: RefrigerationSubcoolerMechanical.
    bool setMechanicalSubcooler(const boost::optional<RefrigerationSubcoolerMechanical>& refrigerationSubcoolerMechanical);

    void resetMechanicalSubcooler();

    // TODO: Check argument type. From object lists, some candidates are: RefrigerationSubcoolerLiquidSuction.
    bool setLiquidSuctionHeatExchangerSubcooler(const boost::optional<RefrigerationSubcoolerLiquidSuction>& refrigerationSubcoolerLiquidSuction);

    void resetLiquidSuctionHeatExchangerSubcooler();

    void setSumUASuctionPiping(double sumUASuctionPiping);

    void resetSumUASuctionPiping();

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    bool setSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone);

    void resetSuctionPipingZone();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    // bool setNumberofCompressorStages(std::string numberofCompressorStages);

    // void resetNumberofCompressorStages();

    bool setIntercoolerType(std::string intercoolerType);

    void resetIntercoolerType();

    void setShellandCoilIntercoolerEffectiveness(double shellandCoilIntercoolerEffectiveness);

    void resetShellandCoilIntercoolerEffectiveness();

    // TODO: Check argument type. From object lists, some candidates are: ModelObjectList.
    bool setHighStageCompressorList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetHighStageCompressorList();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationSystem");

    // TODO: Check the return types of these methods.
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

