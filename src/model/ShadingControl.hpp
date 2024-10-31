/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADINGCONTROL_HPP
#define MODEL_SHADINGCONTROL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  class Construction;
  class ShadingMaterial;
  class Schedule;
  class SubSurface;

  namespace detail {

    class ShadingControl_Impl;

  }  // namespace detail

  /** ShadingControl is a ResourceObject that wraps the OpenStudio IDD object 'OS:ShadingControl'. */
  class MODEL_API ShadingControl : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /// Create a ShadingControl object which references the passed in construction for the shaded state.
    /// This constructor should be used to model switchable glazing only at this time, other configurations
    /// will not be supported by the Radiance ForwardTranslator.  Note that changes to the construction made
    /// after this constructor will not be reflected in this ShadingControl object.  If changes to the construction
    /// are made a new ShadingControl object should be constructed.
    explicit ShadingControl(const Construction& construction);

    /// Create a ShadingControl object which references the passed in material for the shading layer.
    /// This constructor is preferred for all configurations other than switchable glazing.
    explicit ShadingControl(const ShadingMaterial& shadingMaterial);

    virtual ~ShadingControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ShadingControl(const ShadingControl& other) = default;
    ShadingControl(ShadingControl&& other) = default;
    ShadingControl& operator=(const ShadingControl&) = default;
    ShadingControl& operator=(ShadingControl&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> shadingTypeValues();

    static std::vector<std::string> shadingControlTypeValues();

    static std::vector<std::string> typeofSlatAngleControlforBlindsValues();

    static std::vector<std::string> multipleSurfaceControlTypeValues();

    /** @name Getters */
    //@{

    std::string shadingType() const;

    boost::optional<Construction> construction() const;

    boost::optional<ShadingMaterial> shadingMaterial() const;

    std::string shadingControlType() const;

    bool isShadingControlTypeDefaulted() const;

    boost::optional<Schedule> schedule() const;

    boost::optional<double> setpoint() const;

    bool isSetpointDefaulted()
      const;  // TODO: This makes little sense. Based on the shadingControlType, it's basically required. There's a default harcoded only for OnIfHighSolarOnWindow

    bool glareControlIsActive() const;

    std::string typeofSlatAngleControlforBlinds() const;

    bool isTypeofSlatAngleControlforBlindsDefaulted() const;

    boost::optional<Schedule> slatAngleSchedule() const;

    boost::optional<double> setpoint2() const;

    std::string multipleSurfaceControlType() const;

    //@}
    /** @name Setters */
    //@{

    // DLM: should this be removed?
    /// Sets the shading type field.  There is no error checking to ensure that shadingType is correct
    /// given the shaded construction or shading material.
    bool setShadingType(const std::string& shadingType);

    bool setShadingControlType(const std::string& shadingControlType);

    void resetShadingControlType();

    bool setSchedule(const Schedule& schedule);

    void resetSchedule();

    bool setSetpoint(double setpoint);

    void resetSetpoint();  // TODO: makes little sense like isSetpointDefaulted

    bool setGlareControlIsActive(bool glareControlIsActive);

    void resetGlareControlIsActive();

    bool setTypeofSlatAngleControlforBlinds(const std::string& typeofSlatAngleControlforBlinds);

    void resetTypeofSlatAngleControlforBlinds();

    bool setSlatAngleSchedule(const Schedule& slatAngleSchedule);

    void resetSlatAngleSchedule();

    bool setSetpoint2(double setpoint2);

    bool setMultipleSurfaceControlType(const std::string& multipleSurfaceControlType);

    //@}
    /** @name Other */
    //@{

    // Check if the current ShadingControlType requires Setpoint(1)
    bool isControlTypeValueNeedingSetpoint1();
    // Check if the current ShadingControlType requires Setpoint(2)
    bool isControlTypeValueNeedingSetpoint2();

    // Check if the current ShadingControlType allows a Schedule
    bool isControlTypeValueAllowingSchedule();
    // Check if the current ShadingControlType requires a Schedule
    bool isControlTypeValueRequiringSchedule();

    // Check if the current ShadingType allows a Slat Angle Control
    bool isTypeValueAllowingSlatAngleControl();

    // Extensible: Surfaces
    std::vector<SubSurface> subSurfaces() const;
    unsigned int numberofSubSurfaces() const;

    /*
   * Get the index of a given SubSurface (1-indexed)
   */
    boost::optional<unsigned> subSurfaceIndex(const SubSurface& subSurface) const;

    /*
   * Add a new SubSurface at the end of all of the existing SubSurfaces
   */
    bool addSubSurface(const SubSurface& subSurface);

    /*
   * Add a new SubSurface to the list which a given index (1 to x).
   * Internally calls addSubSurface then setSubSurfaceIndex, see remarks there
   */
    bool addSubSurface(const SubSurface& subSurface, unsigned index);

    /*
   * You can shuffle the priority of a given SubSurface after having added it
   * If index is below 1, it's reset to 1.
   * If index is greater than the number of SubSurfaces, will reset to last
   */
    bool setSubSurfaceIndex(const SubSurface& subSurface, unsigned index);

    /*
   * Remove the given SubSurface from this object's subsurfaces
   */
    bool removeSubSurface(const SubSurface& subSurface);

    /*
   * Remove the SubSurface at the given index (1-indexed)
   */
    bool removeSubSurface(unsigned index);

    // Bulk operations

    /**
   * Does not clear any subSurfaces already added, just calls calls addSubSurface for each
   * It will return the global status, but will continue trying if there are problems
   * (eg: if you make or a vector that has a subSurface from another model, the valid subSurfaces will be
   * added indeed, but it'll eventually return false)
   */

    bool addSubSurfaces(const std::vector<SubSurface>& subSurfaces);

    /*
   * Set all SubSurfaces using a vector of SubSurface
   * Internally calls removeAllSubSurfaces(), then addSubSurfaces
   * It will return the global status, but will continue trying if there are problems
   * (eg: if you make or a vector that has a subSurface from another model, the valid subSurface will be
   * added indeed, but it'll eventually return false)
   */
    bool setSubSurfaces(const std::vector<SubSurface>& subSurfaces);

    /*
   * Removes all SubSurfaces in this object
   */
    void removeAllSubSurfaces();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ShadingControl_Impl;

    explicit ShadingControl(std::shared_ptr<detail::ShadingControl_Impl> impl);

    friend class detail::ShadingControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ShadingControl");
  };

  /** \relates ShadingControl*/
  using OptionalShadingControl = boost::optional<ShadingControl>;

  /** \relates ShadingControl*/
  using ShadingControlVector = std::vector<ShadingControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGCONTROL_HPP
