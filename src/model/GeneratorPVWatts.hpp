/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORPVWATTS_HPP
#define MODEL_GENERATORPVWATTS_HPP

#include "ModelAPI.hpp"
#include "Generator.hpp"

#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

  class PlanarSurface;

  namespace detail {

    class GeneratorPVWatts_Impl;

  }  // namespace detail

  /** GeneratorPVWatts is a Generator that wraps the OpenStudio IDD object 'OS:Generator:PVWatts'. */
  class MODEL_API GeneratorPVWatts : public Generator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorPVWatts(const Model& model, double dcSystemCapacity);

    explicit GeneratorPVWatts(const Model& model, const PlanarSurface& surface, double dcSystemCapacity);

    virtual ~GeneratorPVWatts() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorPVWatts(const GeneratorPVWatts& other) = default;
    GeneratorPVWatts(GeneratorPVWatts&& other) = default;
    GeneratorPVWatts& operator=(const GeneratorPVWatts&) = default;
    GeneratorPVWatts& operator=(GeneratorPVWatts&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string pvWattsVersion() const;

    double dcSystemCapacity() const;

    std::string moduleType() const;

    bool isModuleTypeDefaulted() const;

    std::string arrayType() const;

    bool isArrayTypeDefaulted() const;

    double systemLosses() const;

    bool isSystemLossesDefaulted() const;

    double tiltAngle() const;

    bool isTiltAngleDefaulted() const;

    double azimuthAngle() const;

    bool isAzimuthAngleDefaulted() const;

    boost::optional<PlanarSurface> surface() const;

    double groundCoverageRatio() const;

    bool isGroundCoverageRatioDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setDCSystemCapacity(double dcSystemCapacity);

    bool setModuleType(const std::string& moduleType);

    void resetModuleType();

    bool setArrayType(const std::string& arrayType);

    void resetArrayType();

    bool setSystemLosses(double systemLosses);

    void resetSystemLosses();

    bool setTiltAngle(double tiltAngle);

    void resetTiltAngle();

    bool setAzimuthAngle(double azimuthAngle);

    void resetAzimuthAngle();

    bool setSurface(const PlanarSurface& surface);

    void resetSurface();

    bool setGroundCoverageRatio(double groundCoverageRatio);

    void resetGroundCoverageRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond

    using ImplType = detail::GeneratorPVWatts_Impl;

    explicit GeneratorPVWatts(std::shared_ptr<detail::GeneratorPVWatts_Impl> impl);

    friend class detail::GeneratorPVWatts_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorPVWatts");
  };

  /** \relates GeneratorPVWatts*/
  using OptionalGeneratorPVWatts = boost::optional<GeneratorPVWatts>;

  /** \relates GeneratorPVWatts*/
  using GeneratorPVWattsVector = std::vector<GeneratorPVWatts>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORPVWATTS_HPP
