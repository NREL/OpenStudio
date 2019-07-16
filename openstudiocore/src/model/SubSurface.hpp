/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_SUBSURFACE_HPP
#define MODEL_SUBSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

namespace openstudio {
namespace model {

class Surface;
class ShadingSurface;
class ShadingControl;
class ShadingSurfaceGroup;
class DaylightingDeviceShelf;
class WindowPropertyFrameAndDivider;
class SurfacePropertyOtherSideCoefficients;
class SurfacePropertyOtherSideConditionsModel;
class SurfacePropertyConvectionCoefficients;
class AirflowNetworkSurface;
class AirflowNetworkDetailedOpening;
class AirflowNetworkSimpleOpening;
class AirflowNetworkCrack;
class AirflowNetworkEffectiveLeakageArea;
class AirflowNetworkHorizontalOpening;

namespace detail {

  class SubSurface_Impl;

} // detail

/** SubSurface is a PlanarSurface that wraps the OpenStudio IDD object 'OS_SubSurface'. */
class MODEL_API SubSurface : public PlanarSurface {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SubSurface(const std::vector<Point3d>& vertices, const Model& model);

  virtual ~SubSurface() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validSubSurfaceTypeValues();

  //@}
  /** @name Getters */
  //@{

  std::string subSurfaceType() const;

  boost::optional<double> viewFactortoGround() const;

  bool isViewFactortoGroundDefaulted() const;

  bool isViewFactortoGroundAutocalculated() const;

  bool allowShadingControl() const;

  boost::optional<ShadingControl> shadingControl() const;

  bool allowWindowPropertyFrameAndDivider() const;

  boost::optional<WindowPropertyFrameAndDivider> windowPropertyFrameAndDivider() const;

  double multiplier() const;

  bool isMultiplierDefaulted() const;

  boost::optional<double> numberofVertices() const;

  bool isNumberofVerticesDefaulted() const;

  bool isNumberofVerticesAutocalculated() const;

  //@}
  /** @name Setters */
  //@{

  bool setSubSurfaceType(std::string subSurfaceType);

  bool setViewFactortoGround(boost::optional<double> viewFactortoGround);

  bool setViewFactortoGround(double viewFactortoGround);

  void resetViewFactortoGround();

  void autocalculateViewFactortoGround();

  bool setShadingControl(const ShadingControl& shadingControl);

  void resetShadingControl();

  bool setWindowPropertyFrameAndDivider(const WindowPropertyFrameAndDivider& windowPropertyFrameAndDivider);

  void resetWindowPropertyFrameAndDivider();

  bool setMultiplier(double multiplShadingControlier);

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
  boost::optional<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

  /** Returns the adjacent SurfaceSurfacePropertyOtherSideCoefficients, if it exists. */
  boost::optional<SurfacePropertyOtherSideCoefficients> surfacePropertyOtherSideCoefficients() const;

  /** Sets the SurfacePropertyOtherSideCoefficients. */
  bool setSurfacePropertyOtherSideCoefficients(SurfacePropertyOtherSideCoefficients& otherSideCoefficients);

  /** Resets the SurfacePropertyOtherSideCoefficients. */
  void resetSurfacePropertyOtherSideCoefficients();

  /** Returns the adjacent SurfacePropertyOtherSideConditionsModel, if it exists. */
  boost::optional<SurfacePropertyOtherSideConditionsModel> surfacePropertyOtherSideConditionsModel() const;

  /** Sets the SurfacePropertyOtherSideConditionsModel. */
  bool setSurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel& otherSideModel);

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

  /** Add a daylighting light shelf associated with this sub surface.  Only succeeds if this is a fixed window,
   * operable window, or glass door. Will return existing daylighting light shelf if there already is one. */
  boost::optional<DaylightingDeviceShelf> addDaylightingDeviceShelf() const;

  AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkDetailedOpening& surfaceAirflowLeakage);
  AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkSimpleOpening& surfaceAirflowLeakage);
  AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkCrack& surfaceAirflowLeakage);
  AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkEffectiveLeakageArea& surfaceAirflowLeakage);
  AirflowNetworkSurface getAirflowNetworkSurface(const AirflowNetworkHorizontalOpening& surfaceAirflowLeakage);

  boost::optional<AirflowNetworkSurface> airflowNetworkSurface() const;

  // DLM: todo add methods to create light shelves by projection factor

 protected:
  /// @cond
  typedef detail::SubSurface_Impl ImplType;

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
MODEL_API std::vector<SubSurface> applySkylightPattern(const std::vector<std::vector<Point3d> >& pattern, const std::vector<Space>& spaces, const boost::optional<ConstructionBase>& construction);

/** \relates SubSurface*/
typedef boost::optional<SubSurface> OptionalSubSurface;

/** \relates SubSurface*/
typedef std::vector<SubSurface> SubSurfaceVector;

} // model
} // openstudio

#endif // MODEL_SUBSURFACE_HPP

