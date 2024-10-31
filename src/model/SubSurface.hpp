/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SUBSURFACE_HPP
#define MODEL_SUBSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  class Surface;
  class ShadingSurface;
  class ShadingControl;
  class ShadingSurfaceGroup;
  class DaylightingDeviceShelf;
  class DaylightingDeviceTubular;
  class DaylightingDeviceLightWell;
  class WindowPropertyFrameAndDivider;
  class SurfacePropertyOtherSideCoefficients;
  class SurfacePropertyOtherSideConditionsModel;
  class SurfacePropertyConvectionCoefficients;
  class SurfacePropertyLocalEnvironment;
  class SurfacePropertyIncidentSolarMultiplier;
  class AirflowNetworkSurface;
  class AirflowNetworkDetailedOpening;
  class AirflowNetworkSimpleOpening;
  class AirflowNetworkCrack;
  class AirflowNetworkEffectiveLeakageArea;
  class AirflowNetworkHorizontalOpening;
  class AirflowNetworkSpecifiedFlowRate;

  namespace detail {

    class SubSurface_Impl;

  }  // namespace detail

  /** SubSurface is a PlanarSurface that wraps the OpenStudio IDD object 'OS_SubSurface'. */
  class MODEL_API SubSurface : public PlanarSurface
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SubSurface(const std::vector<Point3d>& vertices, const Model& model);

    virtual ~SubSurface() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SubSurface(const SubSurface& other) = default;
    SubSurface(SubSurface&& other) = default;
    SubSurface& operator=(const SubSurface&) = default;
    SubSurface& operator=(SubSurface&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validSubSurfaceTypeValues();

    //@}
    /** @name Getters */
    //@{

    std::string subSurfaceType() const;

    bool isSubSurfaceTypeDefaulted() const;

    boost::optional<double> viewFactortoGround() const;

    bool isViewFactortoGroundDefaulted() const;

    bool isViewFactortoGroundAutocalculated() const;

    bool allowShadingControl() const;

    OS_DEPRECATED(3, 1, 0) boost::optional<ShadingControl> shadingControl() const;

    std::vector<ShadingControl> shadingControls() const;

    unsigned int numberofShadingControls() const;

    bool allowWindowPropertyFrameAndDivider() const;

    boost::optional<WindowPropertyFrameAndDivider> windowPropertyFrameAndDivider() const;

    double multiplier() const;

    bool isMultiplierDefaulted() const;

    boost::optional<double> numberofVertices() const;

    bool isNumberofVerticesDefaulted() const;

    bool isNumberofVerticesAutocalculated() const;

    boost::optional<double> assemblyUFactor() const;

    boost::optional<double> assemblySHGC() const;

    boost::optional<double> assemblyVisibleTransmittance() const;

    //@}
    /** @name Setters */
    //@{

    bool setSubSurfaceType(const std::string& subSurfaceType);

    void resetSubSurfaceType();

    bool setViewFactortoGround(boost::optional<double> viewFactortoGround);

    bool setViewFactortoGround(double viewFactortoGround);

    void resetViewFactortoGround();

    void autocalculateViewFactortoGround();

    // This method is deprecated, please use addShadingControl or addShadingControls.
    // This will remove this SubSurface from any shading control(s) it is on (`removeAllShadingControls()`) then will call `addShadingControl(shadingControl)`
    // NOTE: for backward compatibility with C++ interfaces, the argument is kept as `const ShadingControl&`,
    // but internally this will do a const_cast since the ShadingControl will be mutated
    OS_DEPRECATED(3, 1, 0) bool setShadingControl(const ShadingControl& shadingControl);

    // Replaced with removeAllShadingControls
    OS_DEPRECATED(3, 1, 0) void resetShadingControl();

    bool addShadingControl(ShadingControl& shadingControl);

    bool addShadingControls(std::vector<ShadingControl>& shadingControls);

    void removeShadingControl(ShadingControl& shadingControl);

    void removeAllShadingControls();

    bool setWindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& windowPropertyFrameAndDivider);

    void resetWindowPropertyFrameAndDivider();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    bool setNumberofVertices(boost::optional<double> numberofVertices);

    bool setNumberofVertices(double numberofVertices);

    void resetNumberofVertices();

    void autocalculateNumberofVertices();

    //@}

    // TODO: test that area is correct with multiplier

    /// get the surface
    boost::optional<Surface> surface() const;

    /// set the surface
    bool setSurface(const Surface& surface);

    /// get the adjacent subsurface
    boost::optional<SubSurface> adjacentSubSurface() const;

    /// set the adjacent subsurface, will fail unless both sub surfaces are parented by surfaces
    /// which are adjacent
    bool setAdjacentSubSurface(SubSurface& subSurface);

    /// reset the adjacent subsurface, will clear references on both this and adjacent sub surface
    void resetAdjacentSubSurface();

    /** Returns the SurfacePropertyConvectionCoefficients, if it exists. */
    // cppcheck-suppress [duplInheritedMember] because PlanarSurface is dumb and returns a vector
    boost::optional<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

    /** Returns the SurfacePropertyLocalEnvironment, if it exists. */
    boost::optional<SurfacePropertyLocalEnvironment> surfacePropertyLocalEnvironment() const;

    /** Return the SurfacePropertyIncidentSolarMultiplier that points to this surface if any */
    boost::optional<SurfacePropertyIncidentSolarMultiplier> surfacePropertyIncidentSolarMultiplier() const;

    /** Returns the adjacent SurfaceSurfacePropertyOtherSideCoefficients, if it exists. */
    boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients() const;

    /** Sets the SurfacePropertyOtherSideCoefficients. */
    bool setSurfacePropertyOtherSideCoefficients(const SurfacePropertyOtherSideCoefficients& otherSideCoefficients);

    /** Resets the SurfacePropertyOtherSideCoefficients. */
    void resetSurfacePropertyOtherSideCoefficients();

    /** Returns the adjacent SurfacePropertyOtherSideConditionsModel, if it exists. */
    boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel() const;

    /** Sets the SurfacePropertyOtherSideConditionsModel. */
    bool setSurfacePropertyOtherSideConditionsModel(const SurfacePropertyOtherSideConditionsModel& otherSideModel);

    /** Resets the SurfacePropertyOtherSideConditionsModel. */
    void resetSurfacePropertyOtherSideConditionsModel();

    /** Assign default sub surface type based on vertices. */
    void assignDefaultSubSurfaceType();

    /** Return the surface()'s outsideBoundaryCondition, or an empty string. */
    std::string outsideBoundaryCondition() const;

    /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors. */
    // DLM: todo add argument for horizontal offset
    boost::optional<ShadingSurface> addOverhang(double depth, double offset);

    /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors.
   *  Offset is a fraction of the total window height, projection factor is based on height and offset. */
    // DLM: todo add argument for horizontal offset
    boost::optional<ShadingSurface> addOverhangByProjectionFactor(double projectionFactor, double offsetFraction);

    // DLM: todo add methods to create fins

    /** Returns any shading surface groups associated with this sub surface. */
    std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

    /** Returns true if this sub surface allows the addition of a daylighting light shelf. */
    bool allowDaylightingDeviceShelf() const;

    /** Get the daylighting light shelf associated with this sub surface if there is one. */
    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

    /** Add a new daylighting light shelf associated with this sub surface. Only succeeds if this is a fixed window,
     *  operable window, or glass door. Will return existing daylighting light shelf if there already is one. */
    boost::optional<DaylightingDeviceShelf> addDaylightingDeviceShelf() const;

    /** Returns true if this sub surface allows the addition of a daylighting light tubular dome. */
    bool allowDaylightingDeviceTubularDome() const;

    /** Returns true if this sub surface allows the addition of a daylighting light tubular diffuser. */
    bool allowDaylightingDeviceTubularDiffuser() const;

    /** Get the daylighting light tubular associated with this sub surface if there is one. */
    boost::optional<DaylightingDeviceTubular> daylightingDeviceTubular() const;

    /** Returns true if this sub surface allows the addition of a daylighting light light well. */
    bool allowDaylightingDeviceLightWell() const;

    /** Get the daylighting light light well associated with this sub surface if there is one. */
    boost::optional<DaylightingDeviceLightWell> daylightingDeviceLightWell() const;

    /** Add a new daylighting light well associated with this sub surface. Only succeeds if this is a fixed window,
    *   operable window, or glass door. Will return existing daylighting light well if there already is one. */
    boost::optional<DaylightingDeviceLightWell> addDaylightingDeviceLightWell() const;

    AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkDetailedOpening& surfaceAirflowLeakage);
    AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkSimpleOpening& surfaceAirflowLeakage);
    AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkCrack& surfaceAirflowLeakage);
    AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkEffectiveLeakageArea& surfaceAirflowLeakage);
    AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkHorizontalOpening& surfaceAirflowLeakage);
    AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkSpecifiedFlowRate& surfaceAirflowLeakage);

    boost::optional<AirflowNetworkSurface> airflowNetworkSurface() const;

    /* Get the total area of the sub surface rough area which includes the frame */
    double roughOpeningArea() const;

    /* Get the rough opening vertices for the sub surface including the frame */
    std::vector<Point3d> roughOpeningVertices() const;

    // Gets the total area of the frame
    double frameArea() const;

    // Gets the total area of the divider
    double dividerArea() const;

    // DLM: todo add methods to create light shelves by projection factor

    //@}
    /** @name Queries */
    //@{

   protected:
    /// @cond
    using ImplType = detail::SubSurface_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit SubSurface(std::shared_ptr<detail::SubSurface_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SubSurface");
  };

  /// Applys a skylight pattern to exterior roofs in selected spaces.
  /// Pattern should be in Building coordinates, on the z = 0 plane, with normal in positive z direction.
  /// Returns new sub surfaces created.
  MODEL_API std::vector<SubSurface> applySkylightPattern(const std::vector<std::vector<Point3d>>& pattern, const std::vector<Space>& spaces,
                                                         const boost::optional<ConstructionBase>& construction);

  /** \relates SubSurface*/
  using OptionalSubSurface = boost::optional<SubSurface>;

  /** \relates SubSurface*/
  using SubSurfaceVector = std::vector<SubSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SUBSURFACE_HPP
