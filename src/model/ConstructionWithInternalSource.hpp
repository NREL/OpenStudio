/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONSTRUCTIONWITHINTERNALSOURCE_HPP
#define MODEL_CONSTRUCTIONWITHINTERNALSOURCE_HPP

#include "ModelAPI.hpp"
#include "LayeredConstruction.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ConstructionWithInternalSource_Impl;
  }

  /** ConstructionWithInternalSource is a LayeredConstruction that wraps EnergyPlus IDD object
 *  'Construction:InternalSource'. ConstructionWithInternalSource is identical to Construction,
 *  except that it contains an embedded heat source. */
  class MODEL_API ConstructionWithInternalSource : public LayeredConstruction
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /// Default constructor, provided for backward compatibility.  The model object will
    /// be invalid after using this constructor, setLayers should be called immediately after
    /// this constructor.
    explicit ConstructionWithInternalSource(const Model& model);

    /// Constructor with vector of material layers, throws if opaqueMaterials size < 2.
    explicit ConstructionWithInternalSource(const std::vector<OpaqueMaterial>& opaqueMaterials);

    virtual ~ConstructionWithInternalSource() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConstructionWithInternalSource(const ConstructionWithInternalSource& other) = default;
    ConstructionWithInternalSource(ConstructionWithInternalSource&& other) = default;
    ConstructionWithInternalSource& operator=(const ConstructionWithInternalSource&) = default;
    ConstructionWithInternalSource& operator=(ConstructionWithInternalSource&&) = default;

    /// The source is located after layer number.  Must be >= 1 and <= numLayers-1.
    /// Defaults to 1.
    int sourcePresentAfterLayerNumber() const;
    bool setSourcePresentAfterLayerNumber(int sourcePresentAfterLayerNumber);

    /// A separate temperature calculation occurs after layer number.
    /// Must be >= 1 and <= numLayers-1. Defaults to 1.
    int temperatureCalculationRequestedAfterLayerNumber() const;
    bool setTemperatureCalculationRequestedAfterLayerNumber(int temperatureCalculationRequestedAfterLayerNumber);

    /// A value of 1 states that the user is only interested in a one-dimensional calculation.
    /// This is appropriate for electric resistance heating and for hydronic heating
    /// (when boiler/hot water heater performance is not affected by return and supply water
    /// temperatures). A value of 1 will trigger a two-dimensional solution for this surface only.
    /// This may be necessary for hydronic radiant cooling situations since chiller performance is
    /// affected by the water temperatures provided.
    /// Defaults to 1.
    int dimensionsForTheCTFCalculation() const;
    bool setDimensionsForTheCTFCalculation(int dimensionsForTheCTFCalculation);

    /// How far apart in meters the hydronic tubing or electrical resistance wires are spaced
    /// in the direction perpendicular to the main direction of heat transfer.
    /// Note that this parameter is only used for two-dimensional solutions.
    /// Defaults to 0.154 meters.
    double tubeSpacing() const;
    bool setTubeSpacing(double tubeSpacing);

    /// Used in conjunction with field Temperature Calculation Requested After Layer Number
    /// this field is the location perpendicular to the main direction of heat transfer
    /// 0.0 means in line with the tubing, 1.0 means at the midpoint between two adjacent pipes
    /// this field is ignored for 1-D calculations
    /// Defaults to 0.0.
    double twoDimensionalTemperatureCalculationPosition() const;
    bool setTwoDimensionalTemperatureCalculationPosition(double twoDimensionalTemperatureCalculationPosition);

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();
    ConstructionWithInternalSource reverseConstructionWithInternalSource() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ConstructionWithInternalSource_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ConstructionWithInternalSource_Impl;

    explicit ConstructionWithInternalSource(std::shared_ptr<detail::ConstructionWithInternalSource_Impl> impl);
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ConstructionWithInternalSource");
  };

  /** \relates ConstructionWithInternalSource */
  using OptionalConstructionWithInternalSource = boost::optional<ConstructionWithInternalSource>;

  /** \relates ConstructionWithInternalSource */
  using ConstructionWithInternalSourceVector = std::vector<ConstructionWithInternalSource>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONWITHINTERNALSOURCE_HPP
