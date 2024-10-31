/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGPLANT_HPP
#define MODEL_SIZINGPLANT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class PlantLoop;

  namespace detail {

    class SizingPlant_Impl;

  }  // namespace detail

  /** SizingPlant is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:Plant'. */
  class MODEL_API SizingPlant : public ModelObject
  {
   public:
    explicit SizingPlant(const Model& model, const PlantLoop& plantLoop);

    virtual ~SizingPlant() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SizingPlant(const SizingPlant& other) = default;
    SizingPlant(SizingPlant&& other) = default;
    SizingPlant& operator=(const SizingPlant&) = default;
    SizingPlant& operator=(SizingPlant&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> loopTypeValues();

    static std::vector<std::string> sizingOptionValues();

    static std::vector<std::string> coincidentSizingFactorModeValues();

    PlantLoop plantLoop() const;

    bool setPlantLoop(const PlantLoop& plantLoop);

    std::string loopType() const;

    bool setLoopType(const std::string& loopType);

    double designLoopExitTemperature() const;

    bool setDesignLoopExitTemperature(double designLoopExitTemperature);

    double loopDesignTemperatureDifference() const;

    bool setLoopDesignTemperatureDifference(double loopDesignTemperatureDifference);

    std::string sizingOption() const;

    bool setSizingOption(const std::string& sizingOption);

    int zoneTimestepsinAveragingWindow() const;

    bool setZoneTimestepsinAveragingWindow(int zoneTimestepsinAveragingWindow);

    std::string coincidentSizingFactorMode() const;

    bool setCoincidentSizingFactorMode(const std::string& coincidentSizingFactorMode);

   protected:
    /// @cond
    using ImplType = detail::SizingPlant_Impl;

    explicit SizingPlant(std::shared_ptr<detail::SizingPlant_Impl> impl);

    friend class detail::SizingPlant_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.SizingPlant");
  };

  /** \relates SizingPlant*/
  using OptionalSizingPlant = boost::optional<SizingPlant>;

  /** \relates SizingPlant*/
  using SizingPlantVector = std::vector<SizingPlant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGPLANT_HPP
