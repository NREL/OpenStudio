/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~RefrigerationTranscriticalSystem() override = default;
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
