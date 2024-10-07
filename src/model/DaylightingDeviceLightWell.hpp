/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGDEVICELIGHTWELL_HPP
#define MODEL_DAYLIGHTINGDEVICELIGHTWELL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class InteriorPartitionSurface;
  class ShadingSurface;

  namespace detail {

    class DaylightingDeviceLightWell_Impl;

  }  // namespace detail

  /** DaylightingDeviceLightWell is a ModelObject that wraps the OpenStudio IDD object 'OS_DaylightingDevice_LightWell'. */
  class MODEL_API DaylightingDeviceLightWell : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DaylightingDeviceLightWell(const SubSurface& subSurface, double heightofWell, double perimeterofBottomofWell, double areaofBottomofWell,
                                        double visibleReflectanceofWellWalls);

    explicit DaylightingDeviceLightWell(const SubSurface& subSurface);

    virtual ~DaylightingDeviceLightWell() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DaylightingDeviceLightWell(const DaylightingDeviceLightWell& other) = default;
    DaylightingDeviceLightWell(DaylightingDeviceLightWell&& other) = default;
    DaylightingDeviceLightWell& operator=(const DaylightingDeviceLightWell&) = default;
    DaylightingDeviceLightWell& operator=(DaylightingDeviceLightWell&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    SubSurface subSurface() const;

    double heightofWell() const;

    double perimeterofBottomofWell() const;

    double areaofBottomofWell() const;

    double visibleReflectanceofWellWalls() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeightofWell(double heightofWell);

    bool setPerimeterofBottomofWell(double perimeterofBottomofWell);

    bool setAreaofBottomofWell(double areaofBottomofWell);

    bool setVisibleReflectanceofWellWalls(double visibleReflectanceofWellWalls);

    //@}

   protected:
    /// @cond
    using ImplType = detail::DaylightingDeviceLightWell_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit DaylightingDeviceLightWell(std::shared_ptr<detail::DaylightingDeviceLightWell_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DaylightingDeviceLightWell");
  };

  /** \relates DaylightingDeviceLightWell*/
  using OptionalDaylightingDeviceLightWell = boost::optional<DaylightingDeviceLightWell>;

  /** \relates DaylightingDeviceLightWell*/
  using DaylightingDeviceLightWellVector = std::vector<DaylightingDeviceLightWell>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICELIGHTWELL_HPP
