/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP
#define MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ZoneCapacitanceMultiplierResearchSpecial_Impl;

  }  // namespace detail

  /** ZoneCapacitanceMultiplierResearchSpecial is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneCapacitanceMultiplier:ResearchSpecial'. */
  class MODEL_API ZoneCapacitanceMultiplierResearchSpecial : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ZoneCapacitanceMultiplierResearchSpecial() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneCapacitanceMultiplierResearchSpecial(const ZoneCapacitanceMultiplierResearchSpecial& other) = default;
    ZoneCapacitanceMultiplierResearchSpecial(ZoneCapacitanceMultiplierResearchSpecial&& other) = default;
    ZoneCapacitanceMultiplierResearchSpecial& operator=(const ZoneCapacitanceMultiplierResearchSpecial&) = default;
    ZoneCapacitanceMultiplierResearchSpecial& operator=(ZoneCapacitanceMultiplierResearchSpecial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double temperatureCapacityMultiplier() const;

    bool isTemperatureCapacityMultiplierDefaulted() const;

    double humidityCapacityMultiplier() const;

    bool isHumidityCapacityMultiplierDefaulted() const;

    double carbonDioxideCapacityMultiplier() const;

    bool isCarbonDioxideCapacityMultiplierDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setTemperatureCapacityMultiplier(double temperatureCapacityMultiplier);

    void resetTemperatureCapacityMultiplier();

    bool setHumidityCapacityMultiplier(double humidityCapacityMultiplier);

    void resetHumidityCapacityMultiplier();

    bool setCarbonDioxideCapacityMultiplier(double carbonDioxideCapacityMultiplier);

    void resetCarbonDioxideCapacityMultiplier();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneCapacitanceMultiplierResearchSpecial_Impl;

    explicit ZoneCapacitanceMultiplierResearchSpecial(std::shared_ptr<detail::ZoneCapacitanceMultiplierResearchSpecial_Impl> impl);

    friend class detail::ZoneCapacitanceMultiplierResearchSpecial_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit ZoneCapacitanceMultiplierResearchSpecial(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneCapacitanceMultiplierResearchSpecial");
  };

  /** \relates ZoneCapacitanceMultiplierResearchSpecial*/
  using OptionalZoneCapacitanceMultiplierResearchSpecial = boost::optional<ZoneCapacitanceMultiplierResearchSpecial>;

  /** \relates ZoneCapacitanceMultiplierResearchSpecial*/
  using ZoneCapacitanceMultiplierResearchSpecialVector = std::vector<ZoneCapacitanceMultiplierResearchSpecial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONECAPACITANCEMULTIPLIERRESEARCHSPECIAL_HPP
