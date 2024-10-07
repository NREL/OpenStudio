/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_HPP
#define MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace energyplus {
  class ForwardTranslator;
}

namespace model {

  namespace detail {
    class UnitarySystemPerformanceMultispeed_Impl;
  }  // namespace detail

  class MODEL_API SupplyAirflowRatioField
  {
   public:
    /// Default Constructor. Autosizes both heating and cooling.
    SupplyAirflowRatioField();

    /// Constructor that sets values for both heating and cooling.
    SupplyAirflowRatioField(double heatingRatio, double coolingRatio);

    /// "Constructor" that autosizes cooling ratio and sets heating ratio.
    static SupplyAirflowRatioField fromHeatingRatio(double heatingRatio);

    /// "Constructor" that autosizes heating ration and sets cooling ratio.
    static SupplyAirflowRatioField fromCoolingRatio(double coolingRatio);

    /** @name Getters */
    //@{
    boost::optional<double> heatingRatio() const;
    boost::optional<double> coolingRatio() const;
    bool isHeatingRatioAutosized() const;
    bool isCoolingRatioAutosized() const;
    //@}

   protected:
    std::vector<std::string> getHeatingCoolingRatiosAsStrings() const;

    friend class detail::UnitarySystemPerformanceMultispeed_Impl;
    friend class openstudio::energyplus::ForwardTranslator;

   private:
    SupplyAirflowRatioField(bool isHeating, double value);
    boost::optional<double> m_heatingRatio;  // implementation detail, boost::none if autosized, otherwise the value
    boost::optional<double> m_coolingRatio;
  };

  /** UnitarySystemPerformanceMultispeed is a WaterToAirComponent that wraps the OpenStudio IDD object 'OS:UnitarySystemPerformance:Multispeed'. */
  class MODEL_API UnitarySystemPerformanceMultispeed : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    explicit UnitarySystemPerformanceMultispeed(const Model& model);

    virtual ~UnitarySystemPerformanceMultispeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    UnitarySystemPerformanceMultispeed(const UnitarySystemPerformanceMultispeed& other) = default;
    UnitarySystemPerformanceMultispeed(UnitarySystemPerformanceMultispeed&& other) = default;
    UnitarySystemPerformanceMultispeed& operator=(const UnitarySystemPerformanceMultispeed&) = default;
    UnitarySystemPerformanceMultispeed& operator=(UnitarySystemPerformanceMultispeed&&) = default;
    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool singleModeOperation() const;

    double noLoadSupplyAirflowRateRatio() const;

    std::vector<SupplyAirflowRatioField> supplyAirflowRatioFields() const;

    //@}

    //** @name Setters */

    bool setSingleModeOperation(bool singleMode);

    void resetSingleModeOperation();

    bool setNoLoadSupplyAirflowRateRatio(double noLoadSupplyAirflowRateRatio);

    bool setSupplyAirflowRatioFields(const std::vector<SupplyAirflowRatioField>& airflowRatioFields);

    bool addSupplyAirflowRatioField(const SupplyAirflowRatioField& airflowRatio);

    bool addSupplyAirflowRatioField(double heatingRatio, double coolingRatio);

    void resetSupplyAirflowRatioFields();

    //@{

    //@}

   protected:
    /// @cond
    using ImplType = detail::UnitarySystemPerformanceMultispeed_Impl;

    explicit UnitarySystemPerformanceMultispeed(std::shared_ptr<detail::UnitarySystemPerformanceMultispeed_Impl> impl);

    friend class detail::UnitarySystemPerformanceMultispeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.UnitarySystemPerformanceMultispeed");
  };

  /** \relates UnitarySystemPerformanceMultispeed */
  using OptionalUnitarySystemPerformanceMultispeed = boost::optional<UnitarySystemPerformanceMultispeed>;

  /** \relates UnitarySystemPerformanceMultispeed */
  using UnitarySystemPerformanceMultispeedVector = std::vector<UnitarySystemPerformanceMultispeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_UNITARYSYSTEMPERFORMANCEMULTISPEED_HPP
