/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API RefrigerationTranscriticalSystem_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationTranscriticalSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationTranscriticalSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationTranscriticalSystem_Impl(const RefrigerationTranscriticalSystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationTranscriticalSystem_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      std::vector<IdfObject> remove() override;

      ModelObject clone(Model model) const override;

      template <class T>
      std::vector<T> listTemplate(const boost::optional<ModelObjectList>& modelObjectList) const;

      template <class T>
      void removeAllTemplate(boost::optional<ModelObjectList>& modelObjectList);

      template <class T>
      void removeTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList);

      template <class T>
      bool addTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList);

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

      bool setReceiverPressure(double receiverPressure);

      void resetReceiverPressure();

      bool setSubcoolerEffectiveness(double subcoolerEffectiveness);

      void resetSubcoolerEffectiveness();

      bool setRefrigerationSystemWorkingFluidType(const std::string& refrigerationSystemWorkingFluidType);

      bool setSumUASuctionPipingforMediumTemperatureLoads(double sumUASuctionPipingforMediumTemperatureLoads);

      void resetSumUASuctionPipingforMediumTemperatureLoads();

      bool setMediumTemperatureSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone);

      void resetMediumTemperatureSuctionPipingZone();

      bool setSumUASuctionPipingforLowTemperatureLoads(double sumUASuctionPipingforLowTemperatureLoads);

      void resetSumUASuctionPipingforLowTemperatureLoads();

      bool setLowTemperatureSuctionPipingZone(const boost::optional<ThermalZone>& thermalZone);

      void resetLowTemperatureSuctionPipingZone();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONTRANSCRITICALSYSTEM_IMPL_HPP
