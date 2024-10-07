/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WATERHEATERSIZING_HPP
#define MODEL_WATERHEATERSIZING_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class WaterToWaterComponent;
  class WaterHeaterMixed;
  class WaterHeaterStratified;

  namespace detail {

    class WaterHeaterSizing_Impl;

  }  // namespace detail

  /** WaterHeaterSizing is a ModelObject that wraps the OpenStudio IDD object 'OS:WaterHeater:Sizing'. */
  class MODEL_API WaterHeaterSizing : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~WaterHeaterSizing() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WaterHeaterSizing(const WaterHeaterSizing& other) = default;
    WaterHeaterSizing(WaterHeaterSizing&& other) = default;
    WaterHeaterSizing& operator=(const WaterHeaterSizing&) = default;
    WaterHeaterSizing& operator=(WaterHeaterSizing&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> designModeValues();

    /** @name Getters */
    //@{

    WaterToWaterComponent waterHeater() const;

    boost::optional<std::string> designMode() const;

    boost::optional<double> timeStorageCanMeetPeakDraw() const;

    boost::optional<double> timeforTankRecovery() const;

    boost::optional<double> nominalTankVolumeforAutosizingPlantConnections() const;

    boost::optional<int> numberofBedrooms() const;

    boost::optional<int> numberofBathrooms() const;

    boost::optional<double> storageCapacityperPerson() const;

    boost::optional<double> recoveryCapacityperPerson() const;

    boost::optional<double> storageCapacityperFloorArea() const;

    boost::optional<double> recoveryCapacityperFloorArea() const;

    boost::optional<double> numberofUnits() const;

    boost::optional<double> storageCapacityperUnit() const;

    boost::optional<double> recoveryCapacityPerUnit() const;

    boost::optional<double> storageCapacityperCollectorArea() const;

    boost::optional<double> heightAspectRatio() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignMode(const std::string& designMode);

    void resetDesignMode();

    bool setTimeStorageCanMeetPeakDraw(double timeStorageCanMeetPeakDraw);

    void resetTimeStorageCanMeetPeakDraw();

    bool setTimeforTankRecovery(double timeforTankRecovery);

    void resetTimeforTankRecovery();

    bool setNominalTankVolumeforAutosizingPlantConnections(double nominalTankVolumeforAutosizingPlantConnections);

    void resetNominalTankVolumeforAutosizingPlantConnections();

    bool setNumberofBedrooms(int numberofBedrooms);

    void resetNumberofBedrooms();

    bool setNumberofBathrooms(int numberofBathrooms);

    void resetNumberofBathrooms();

    bool setStorageCapacityperPerson(double storageCapacityperPerson);

    void resetStorageCapacityperPerson();

    bool setRecoveryCapacityperPerson(double recoveryCapacityperPerson);

    void resetRecoveryCapacityperPerson();

    bool setStorageCapacityperFloorArea(double storageCapacityperFloorArea);

    void resetStorageCapacityperFloorArea();

    bool setRecoveryCapacityperFloorArea(double recoveryCapacityperFloorArea);

    void resetRecoveryCapacityperFloorArea();

    bool setNumberofUnits(double numberofUnits);

    void resetNumberofUnits();

    bool setStorageCapacityperUnit(double storageCapacityperUnit);

    void resetStorageCapacityperUnit();

    bool setRecoveryCapacityPerUnit(double recoveryCapacityPerUnit);

    void resetRecoveryCapacityPerUnit();

    bool setStorageCapacityperCollectorArea(double storageCapacityperCollectorArea);

    void resetStorageCapacityperCollectorArea();

    bool setHeightAspectRatio(double heightAspectRatio);

    void resetHeightAspectRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond

    friend class WaterHeaterMixed;
    friend class WaterHeaterStratified;

    explicit WaterHeaterSizing(const WaterToWaterComponent& waterHeater);
    bool setWaterHeater(const WaterToWaterComponent& waterHeater);

    using ImplType = detail::WaterHeaterSizing_Impl;

    explicit WaterHeaterSizing(std::shared_ptr<detail::WaterHeaterSizing_Impl> impl);

    friend class detail::WaterHeaterSizing_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.WaterHeaterSizing");
  };

  /** \relates WaterHeaterSizing*/
  using OptionalWaterHeaterSizing = boost::optional<WaterHeaterSizing>;

  /** \relates WaterHeaterSizing*/
  using WaterHeaterSizingVector = std::vector<WaterHeaterSizing>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERHEATERSIZING_HPP
