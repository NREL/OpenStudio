/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SUBSURFACE_IMPL_HPP
#define MODEL_SUBSURFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

  class AirflowNetworkComponent;
  class AirflowNetworkSurface;
  class Surface;
  class SubSurface;
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

  namespace detail {

    /** SubSurface_Impl is a PlanarSurface_Impl that is the implementation class for SubSurface.*/
    class MODEL_API SubSurface_Impl : public PlanarSurface_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SubSurface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SubSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SubSurface_Impl(const SubSurface_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SubSurface_Impl() override = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      /// set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// remove self and all children objects recursively
      virtual std::vector<IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /// should subtract this surface from parent's gross area for net area
      virtual bool subtractFromGrossArea() const override;

      /// get the construction object
      /// if the planar surface is paired with an adjacent planar surface, attempts to resolve any surface matching conflict
      virtual boost::optional<ConstructionBase> construction() const override;

      /// get the construction object and the search distance that was needed to find the construction
      /// does not consider adjacent planar surfaces
      virtual boost::optional<std::pair<ConstructionBase, int>> constructionWithSearchDistance() const override;

      /// Returns true if the construction is not directly referenced by this surface .
      virtual bool isConstructionDefaulted() const override;

      virtual bool setVertices(const std::vector<Point3d>& vertices) override;

      /// set the construction object
      virtual bool setConstruction(const ConstructionBase& construction) override;

      /// Resets the construction object.
      virtual void resetConstruction() override;

      /// Returns the containing PlanarSurfaceGroup if available.
      virtual boost::optional<PlanarSurfaceGroup> planarSurfaceGroup() const override;

      /// Returns the containing Space if available.
      virtual boost::optional<Space> space() const override;

      /** Get the u-factor of this surface. Includes film coefficients. */
      virtual boost::optional<double> uFactor() const override;

      /** Get the conductance of this surface. Does not include film coefficients. */
      virtual boost::optional<double> thermalConductance() const override;

      /** Set the u-factor of this surface in W/m^2*K, if possible. value should already include appropriate
     *  film coefficients. By default, assumes still air indoors and 15 mph outdoor air speed. */
      virtual bool setUFactor(double value) override;

      /** Set the conductance of this surface in W/m^2*K, if possible. value should not include any film
     *  coefficients. */
      virtual bool setThermalConductance(double value) override;

      /** @name Getters */
      //@{

      std::string subSurfaceType() const;

      bool isSubSurfaceTypeDefaulted() const;

      std::vector<std::string> subSurfaceTypeValues() const;

      boost::optional<double> viewFactortoGround() const;

      bool isViewFactortoGroundDefaulted() const;

      bool isViewFactortoGroundAutocalculated() const;

      bool allowShadingControl() const;

      std::vector<ShadingControl> shadingControls() const;

      unsigned int numberofShadingControls() const;

      bool allowWindowPropertyFrameAndDivider() const;

      boost::optional<WindowPropertyFrameAndDivider> windowPropertyFrameAndDivider() const;

      double multiplier() const;

      bool isMultiplierDefaulted() const;

      boost::optional<double> numberofVertices() const;

      bool isNumberofVerticesDefaulted() const;

      bool isNumberofVerticesAutocalculated() const;

      // Assembly methods

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

      /** Returns the surface property convection coefficients */
      // cppcheck-suppress [duplInheritedMember] because PlanarSurface is dumb and returns a vector
      boost::optional<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;
      boost::optional<SurfacePropertyLocalEnvironment> surfacePropertyLocalEnvironment() const;

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

      /** Default sub surface type based on vertices. */
      std::string defaultSubSurfaceType() const;

      /** Assign default sub surface type based on vertices. */
      void assignDefaultSubSurfaceType();

      /** Return the surface()'s outsideBoundaryCondition, or an empty string. */
      std::string outsideBoundaryCondition() const;

      /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors. */
      boost::optional<ShadingSurface> addOverhang(double depth, double offset);

      /** Add an overhang to the sub surface, only valid for fixed windows, operable windows, and glass doors.
     *  Offset is a fraction of the total window height, projection factor is based on height and offset. */
      boost::optional<ShadingSurface> addOverhangByProjectionFactor(double projectionFactor, double offsetFraction);

      /** Returns any shading surface groups associated with this sub surface. */
      std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

      /** Returns true if this sub surface allows the addition of a daylighting light shelf. */
      bool allowDaylightingDeviceShelf() const;

      /** Get the daylighting light shelf associated with this sub surface if there is one. */
      boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

      /** Add a daylighting light shelf associated with this sub surface.  Only succeeds if this is a fixed window,
     * operable window, or glass door. Will return existing daylighting light shelf if there already is one. */
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

      /** Add a daylighting light light well associated with this sub surface.  Only succeeds if this is a fixed window,
     * operable window, or glass door. Will return existing daylighting light light well if there already is one. */
      boost::optional<DaylightingDeviceLightWell> addDaylightingDeviceLightWell() const;

      AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkComponent& surfaceAirflowLeakage);

      boost::optional<AirflowNetworkSurface> airflowNetworkSurface() const;

      //@}
      /** @name Queries */
      //@{

      /** Gets the fenestration value from the sql file **/
      boost::optional<double> getExteriorFenestrationValue(std::string columnName) const;

     protected:
     private:
      friend class openstudio::model::SubSurface;

      REGISTER_LOGGER("openstudio.model.SubSurface");

      boost::optional<ModelObject> surfaceAsModelObject() const;
      boost::optional<ModelObject> adjacentSubSurfaceAsModelObject() const;

      bool setSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setAdjacentSubSurfaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SUBSURFACE_IMPL_HPP
