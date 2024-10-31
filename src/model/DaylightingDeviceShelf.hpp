/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGDEVICESHELF_HPP
#define MODEL_DAYLIGHTINGDEVICESHELF_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class InteriorPartitionSurface;
  class ShadingSurface;

  namespace detail {

    class DaylightingDeviceShelf_Impl;

  }  // namespace detail

  /** DaylightingDeviceShelf is a ModelObject that wraps the OpenStudio IDD object 'OS_DaylightingDevice_Shelf'. */
  class MODEL_API DaylightingDeviceShelf : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DaylightingDeviceShelf(const SubSurface& subSurface);

    virtual ~DaylightingDeviceShelf() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DaylightingDeviceShelf(const DaylightingDeviceShelf& other) = default;
    DaylightingDeviceShelf(DaylightingDeviceShelf&& other) = default;
    DaylightingDeviceShelf& operator=(const DaylightingDeviceShelf&) = default;
    DaylightingDeviceShelf& operator=(DaylightingDeviceShelf&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<InteriorPartitionSurface> insideShelf() const;

    boost::optional<ShadingSurface> outsideShelf() const;

    boost::optional<double> viewFactortoOutsideShelf() const;

    //@}
    /** @name Setters */
    //@{

    bool setInsideShelf(const InteriorPartitionSurface& insideShelf);

    bool setOutsideShelf(const ShadingSurface& outsideShelf);

    bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);

    void resetViewFactortoOutsideShelf();

    //@}

    /// Get the related sub surface.
    SubSurface subSurface() const;

   protected:
    /// @cond
    using ImplType = detail::DaylightingDeviceShelf_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit DaylightingDeviceShelf(std::shared_ptr<detail::DaylightingDeviceShelf_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DaylightingDeviceShelf");
  };

  /** \relates DaylightingDeviceShelf*/
  using OptionalDaylightingDeviceShelf = boost::optional<DaylightingDeviceShelf>;

  /** \relates DaylightingDeviceShelf*/
  using DaylightingDeviceShelfVector = std::vector<DaylightingDeviceShelf>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICESHELF_HPP
