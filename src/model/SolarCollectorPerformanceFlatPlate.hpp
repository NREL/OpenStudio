/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SolarCollectorPerformanceFlatPlate;

  namespace detail {

    class SolarCollectorFlatPlateWater_Impl;
    class SolarCollectorPerformanceFlatPlate_Impl;

  }  // namespace detail

  /** SolarCollectorPerformanceFlatPlate is a ModelObject that wraps the OpenStudio IDD object 'OS:SolarCollectorPerformance:FlatPlate'. */
  class MODEL_API SolarCollectorPerformanceFlatPlate : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SolarCollectorPerformanceFlatPlate() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SolarCollectorPerformanceFlatPlate(const SolarCollectorPerformanceFlatPlate& other) = default;
    SolarCollectorPerformanceFlatPlate(SolarCollectorPerformanceFlatPlate&& other) = default;
    SolarCollectorPerformanceFlatPlate& operator=(const SolarCollectorPerformanceFlatPlate&) = default;
    SolarCollectorPerformanceFlatPlate& operator=(SolarCollectorPerformanceFlatPlate&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> testFluidValues();

    static std::vector<std::string> testCorrelationTypeValues();

    /** @name Getters */
    //@{

    double grossArea() const;

    std::string testFluid() const;

    bool isTestFluidDefaulted() const;

    double testFlowRate() const;

    std::string testCorrelationType() const;

    double coefficient1ofEfficiencyEquation() const;

    double coefficient2ofEfficiencyEquation() const;

    boost::optional<double> coefficient3ofEfficiencyEquation() const;

    boost::optional<double> coefficient2ofIncidentAngleModifier() const;

    boost::optional<double> coefficient3ofIncidentAngleModifier() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossArea(double grossArea);

    bool setTestFluid(const std::string& testFluid);

    void resetTestFluid();

    bool setTestFlowRate(double testFlowRate);

    bool setTestCorrelationType(const std::string& testCorrelationType);

    bool setCoefficient1ofEfficiencyEquation(double coefficient1ofEfficiencyEquation);

    bool setCoefficient2ofEfficiencyEquation(double coefficient2ofEfficiencyEquation);

    bool setCoefficient3ofEfficiencyEquation(double coefficient3ofEfficiencyEquation);

    void resetCoefficient3ofEfficiencyEquation();

    bool setCoefficient2ofIncidentAngleModifier(double coefficient2ofIncidentAngleModifier);

    void resetCoefficient2ofIncidentAngleModifier();

    bool setCoefficient3ofIncidentAngleModifier(double coefficient3ofIncidentAngleModifier);

    void resetCoefficient3ofIncidentAngleModifier();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    explicit SolarCollectorPerformanceFlatPlate(const Model& model);

    /// @cond
    using ImplType = detail::SolarCollectorPerformanceFlatPlate_Impl;

    explicit SolarCollectorPerformanceFlatPlate(std::shared_ptr<detail::SolarCollectorPerformanceFlatPlate_Impl> impl);

    friend class detail::SolarCollectorPerformanceFlatPlate_Impl;
    friend class detail::SolarCollectorFlatPlateWater_Impl;
    friend class SolarCollectorFlatPlateWater;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformanceFlatPlate");
  };

  /** \relates SolarCollectorPerformanceFlatPlate*/
  using OptionalSolarCollectorPerformanceFlatPlate = boost::optional<SolarCollectorPerformanceFlatPlate>;

  /** \relates SolarCollectorPerformanceFlatPlate*/
  using SolarCollectorPerformanceFlatPlateVector = std::vector<SolarCollectorPerformanceFlatPlate>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_HPP
