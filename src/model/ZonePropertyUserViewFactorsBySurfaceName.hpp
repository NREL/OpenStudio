/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_HPP
#define MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  /* class Surface;
class SubSurface;
class InternalMass; */

  namespace detail {

    class ZonePropertyUserViewFactorsBySurfaceName_Impl;

  }  // namespace detail

  /** This class implements a view factor */
  class MODEL_API ViewFactor
  {
   public:
    /* Only accepts ModelObjects that are of type Surface, Subsurface or InternalMass, will throw otherwise */
    ViewFactor(const ModelObject& fromSurface, const ModelObject& toSurface, double viewFactor);

    ModelObject fromSurface() const;
    ModelObject toSurface() const;
    double viewFactor() const;

   private:
    // From
    ModelObject m_from_surface;
    ModelObject m_to_surface;
    double m_view_factor;
    REGISTER_LOGGER("openstudio.model.ViewFactor");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::ViewFactor& viewFactor);

  /** ZonePropertyUserViewFactorsBySurfaceName is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneProperty:UserViewFactors:BySurfaceName'. */
  class MODEL_API ZonePropertyUserViewFactorsBySurfaceName : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /*
   * This constructor shouldn't be used, ThermalZone::getZonePropertyUserViewFactorsBySurfaceName should be prefered
   * as it will either instantiate a new object or return the existing one.
   * A ThermalZone should not have more than one ZonePropertyUserViewFactorsBySurfaceName. If you try to instantiate a second one for a given
   * ThermalZone this constructor will throw
   */
    explicit ZonePropertyUserViewFactorsBySurfaceName(const ThermalZone& thermalZone);

    virtual ~ZonePropertyUserViewFactorsBySurfaceName() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZonePropertyUserViewFactorsBySurfaceName(const ZonePropertyUserViewFactorsBySurfaceName& other) = default;
    ZonePropertyUserViewFactorsBySurfaceName(ZonePropertyUserViewFactorsBySurfaceName&& other) = default;
    ZonePropertyUserViewFactorsBySurfaceName& operator=(const ZonePropertyUserViewFactorsBySurfaceName&) = default;
    ZonePropertyUserViewFactorsBySurfaceName& operator=(ZonePropertyUserViewFactorsBySurfaceName&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ThermalZone thermalZone() const;

    std::vector<ViewFactor> viewFactors() const;

    unsigned int numberofViewFactors() const;

    /** If a viewFactor group is already present (cf `viewFactorIndex()`), it will Warn and override the viewFactor value */
    boost::optional<unsigned> viewFactorIndex(const ViewFactor& viewFactor) const;

    boost::optional<ViewFactor> getViewFactor(unsigned groupIndex) const;

    //@}
    /** @name Setters */
    //@{

    /** If a viewFactor group is already present (cf `viewFactorIndex()`), it will Warn and override the viewFactor value */
    bool addViewFactor(const ViewFactor& viewFactor);

    // Overloads, they create a ViewFactor wrapper, then call `addViewFactor(const ViewFactor& viewFactor)`
    bool addViewFactor(const Surface& fromSurface, const Surface& toSurface, double viewFactor);

    bool addViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double viewFactor);

    bool addViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double viewFactor);

    bool addViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface, double viewFactor);

    bool addViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double viewFactor);

    bool addViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass, double viewFactor);

    bool addViewFactor(const InternalMass& fromInternalMass, const InternalMass& toInernalMass, double viewFactor);

    bool addViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double viewFactor);

    bool addViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface, double viewFactor);

    bool addViewFactors(const std::vector<ViewFactor>& viewFactors);

    void removeViewFactor(int groupIndex);

    void removeAllViewFactors();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZonePropertyUserViewFactorsBySurfaceName_Impl;

    explicit ZonePropertyUserViewFactorsBySurfaceName(std::shared_ptr<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl> impl);

    friend class detail::ZonePropertyUserViewFactorsBySurfaceName_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZonePropertyUserViewFactorsBySurfaceName");
  };

  /** \relates ZonePropertyUserViewFactorsBySurfaceName*/
  using OptionalZonePropertyUserViewFactorsBySurfaceName = boost::optional<ZonePropertyUserViewFactorsBySurfaceName>;

  /** \relates ZonePropertyUserViewFactorsBySurfaceName*/
  using ZonePropertyUserViewFactorsBySurfaceNameVector = std::vector<ZonePropertyUserViewFactorsBySurfaceName>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEPROPERTYUSERVIEWFACTORSBYSURFACENAME_HPP
