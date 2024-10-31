/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP
#define MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;

  }  // namespace detail

  /** SurfacePropertyConvectionCoefficientsMultipleSurface is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ConvectionCoefficients:MultipleSurface'. */
  class MODEL_API SurfacePropertyConvectionCoefficientsMultipleSurface : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyConvectionCoefficientsMultipleSurface(const Model& model);

    virtual ~SurfacePropertyConvectionCoefficientsMultipleSurface() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyConvectionCoefficientsMultipleSurface(const SurfacePropertyConvectionCoefficientsMultipleSurface& other) = default;
    SurfacePropertyConvectionCoefficientsMultipleSurface(SurfacePropertyConvectionCoefficientsMultipleSurface&& other) = default;
    SurfacePropertyConvectionCoefficientsMultipleSurface& operator=(const SurfacePropertyConvectionCoefficientsMultipleSurface&) = default;
    SurfacePropertyConvectionCoefficientsMultipleSurface& operator=(SurfacePropertyConvectionCoefficientsMultipleSurface&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> surfaceTypeValues();

    static std::vector<std::string> convectionCoefficient1LocationValues();

    static std::vector<std::string> convectionCoefficient1TypeValues();

    static std::vector<std::string> convectionCoefficient2LocationValues();

    static std::vector<std::string> convectionCoefficient2TypeValues();

    /** @name Getters */
    //@{

    boost::optional<std::string> surfaceType() const;

    boost::optional<std::string> convectionCoefficient1Location() const;

    boost::optional<std::string> convectionCoefficient1Type() const;

    boost::optional<double> convectionCoefficient1() const;

    boost::optional<Schedule> convectionCoefficient1Schedule() const;

    // DLM: not supporting user curves yet
    //boost::optional<UserConvectionModels> convectionCoefficient1UserCurve() const;

    boost::optional<std::string> convectionCoefficient2Location() const;

    boost::optional<std::string> convectionCoefficient2Type() const;

    boost::optional<double> convectionCoefficient2() const;

    boost::optional<Schedule> convectionCoefficient2Schedule() const;

    // DLM: not supporting user curves yet
    //boost::optional<UserConvectionModels> convectionCoefficient2UserCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setSurfaceType(const std::string& surfaceType);

    void resetSurfaceType();

    bool setConvectionCoefficient1Location(const std::string& convectionCoefficient1Location);

    void resetConvectionCoefficient1Location();

    bool setConvectionCoefficient1Type(const std::string& convectionCoefficient1Type);

    void resetConvectionCoefficient1Type();

    bool setConvectionCoefficient1(double convectionCoefficient1);

    void resetConvectionCoefficient1();

    bool setConvectionCoefficient1Schedule(Schedule& schedule);

    void resetConvectionCoefficient1Schedule();

    // DLM: not supporting user curves yet
    //bool setConvectionCoefficient1UserCurve(const UserConvectionModels& userConvectionModels);

    //void resetConvectionCoefficient1UserCurve();

    bool setConvectionCoefficient2Location(const std::string& convectionCoefficient2Location);

    void resetConvectionCoefficient2Location();

    bool setConvectionCoefficient2Type(const std::string& convectionCoefficient2Type);

    void resetConvectionCoefficient2Type();

    bool setConvectionCoefficient2(double convectionCoefficient2);

    void resetConvectionCoefficient2();

    bool setConvectionCoefficient2Schedule(Schedule& schedule);

    void resetConvectionCoefficient2Schedule();

    // DLM: not supporting user curves yet
    //bool setConvectionCoefficient2UserCurve(const UserConvectionModels& userConvectionModels);

    //void resetConvectionCoefficient2UserCurve();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;

    explicit SurfacePropertyConvectionCoefficientsMultipleSurface(
      std::shared_ptr<detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl> impl);

    friend class detail::SurfacePropertyConvectionCoefficientsMultipleSurface_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyConvectionCoefficientsMultipleSurface");
  };

  /** \relates SurfacePropertyConvectionCoefficientsMultipleSurface*/
  using OptionalSurfacePropertyConvectionCoefficientsMultipleSurface = boost::optional<SurfacePropertyConvectionCoefficientsMultipleSurface>;

  /** \relates SurfacePropertyConvectionCoefficientsMultipleSurface*/
  using SurfacePropertyConvectionCoefficientsMultipleSurfaceVector = std::vector<SurfacePropertyConvectionCoefficientsMultipleSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYCONVECTIONCOEFFICIENTSMULTIPLESURFACE_HPP
