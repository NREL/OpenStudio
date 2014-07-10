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

#ifndef MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_IMPL_HPP
#define MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class RefrigerationCase;
class RefrigerationCompressor;
class RefrigerationWalkIn;
class ModelObjectList;
class RefrigerationGasCoolerAirCooled;
class ThermalZone;

namespace detail {

  /** RefrigerationTranscriticalSystem_Impl is a ModelObject_Impl that is the implementation class for RefrigerationTranscriticalSystem.*/
  class MODEL_API RefrigerationTranscriticalSystem_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationTranscriticalSystem_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    RefrigerationTranscriticalSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    RefrigerationTranscriticalSystem_Impl(const RefrigerationTranscriticalSystem_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~RefrigerationTranscriticalSystem_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    std::vector<IdfObject> remove();

    ModelObject clone(Model model) const;

    template <class T>
    std::vector<T> listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const;

    template <class T>
    void removeAllTemplate( boost::optional<ModelObjectList>& modelObjectList );

    template <class T>
    void removeTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList );

    template <class T>
    bool addTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList );

    bool addMediumTemperatureCase( const RefrigerationCase & refrigerationCase);

    void removeMediumTemperatureCase( const RefrigerationCase & refrigerationCase);

    void removeAllMediumTemperatureCases();

    std::vector<RefrigerationCase> mediumTemperatureCases() const;

    bool addLowTemperatureCase( const RefrigerationCase & refrigerationCase);

    void removeLowTemperatureCase( const RefrigerationCase & refrigerationCase);

    void removeAllLowTemperatureCases();

    std::vector<RefrigerationCase> lowTemperatureCases() const;

    bool addMediumTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeMediumTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeAllMediumTemperatureWalkins();

    std::vector<RefrigerationWalkIn> mediumTemperatureWalkins() const;

    bool addLowTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeLowTemperatureWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeAllLowTemperatureWalkins();

    std::vector<RefrigerationWalkIn> lowTemperatureWalkins() const;

    bool addHighPressureCompressor( const RefrigerationCompressor & compressor);

    void removeHighPressureCompressor( const RefrigerationCompressor & compressor);

    void removeAllHighPressureCompressors();

    std::vector<RefrigerationCompressor> highPressureCompressors() const;

    bool addLowPressureCompressor( const RefrigerationCompressor & compressor);

    void removeLowPressureCompressor( const RefrigerationCompressor & compressor);

    void removeAllLowPressureCompressors();

    std::vector<RefrigerationCompressor> lowPressureCompressors() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<ModelObjectList> mediumTemperatureRefrigeratedCaseAndWalkInList() const;

    boost::optional<ModelObjectList> lowTemperatureRefrigeratedCaseAndWalkInList() const;

    boost::optional<RefrigerationGasCoolerAirCooled> refrigerationGasCooler() const;

    boost::optional<ModelObjectList> highPressureCompressorList() const;

    boost::optional<ModelObjectList> lowPressureCompressorList() const;

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

    bool setMediumTemperatureRefrigeratedCaseAndWalkInList(const ModelObjectList& modelObjectList);

    bool setLowTemperatureRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetLowTemperatureRefrigeratedCaseAndWalkInList();

    bool setRefrigerationGasCooler(const RefrigerationGasCoolerAirCooled& refrigerationGasCoolerAirCooled);

    bool setHighPressureCompressorList(const ModelObjectList& modelObjectList);

    bool setLowPressureCompressorList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetLowPressureCompressorList();

    void setReceiverPressure(double receiverPressure);

    void resetReceiverPressure();

    void setSubcoolerEffectiveness(double subcoolerEffectiveness);

    void resetSubcoolerEffectiveness();

    bool setRefrigerationSystemWorkingFluidType(std::string refrigerationSystemWorkingFluidType);

    void setSumUASuctionPipingforMediumTemperatureLoads(double sumUASuctionPipingforMediumTemperatureLoads);

    void resetSumUASuctionPipingforMediumTemperatureLoads();

    bool setMediumTemperatureSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone);

    void resetMediumTemperatureSuctionPipingZone();

    void setSumUASuctionPipingforLowTemperatureLoads(double sumUASuctionPipingforLowTemperatureLoads);

    void resetSumUASuctionPipingforLowTemperatureLoads();

    bool setLowTemperatureSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone);

    void resetLowTemperatureSuctionPipingZone();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationTranscriticalSystem");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    // boost::optional<ModelObjectList> optionalMediumTemperatureRefrigeratedCaseAndWalkInList() const;
    // boost::optional<RefrigerationAllTypesGasCooler> optionalRefrigerationGasCooler() const;
    // boost::optional<ModelObjectList> optionalHighPressureCompressorList() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_IMPL_HPP

