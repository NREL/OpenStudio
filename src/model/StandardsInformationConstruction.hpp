/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_STANDARDSINFORMATIONCONSTRUCTION_HPP
#define MODEL_STANDARDSINFORMATIONCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;
  class Material;

  namespace detail {
    class ConstructionBase_Impl;
    class StandardsInformationConstruction_Impl;
  }  // namespace detail

  /** StandardsInformationConstruction is the ModelObject that wraps OpenStudio IDD object
 *  'OS:StandardsInformation:ConstructionFields'. StandardsInformationConstruction provides a
 *  place to register information about a construction that building energy standards often need
 *  to know, but is irrelevant to simulation. It contains user-specified values that are not
 *  generally checked against input or output data. */
  class MODEL_API StandardsInformationConstruction : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~StandardsInformationConstruction() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    StandardsInformationConstruction(const StandardsInformationConstruction& other) = default;
    StandardsInformationConstruction(StandardsInformationConstruction&& other) = default;
    StandardsInformationConstruction& operator=(const StandardsInformationConstruction&) = default;
    StandardsInformationConstruction& operator=(StandardsInformationConstruction&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static std::vector<std::string> standardPerturbableLayerTypeValues();

    static std::vector<std::string> fenestrationTypeValues();

    static std::vector<std::string> fenestrationAssemblyContextValues();

    static std::vector<std::string> fenestrationNumberOfPanesValues();

    static std::vector<std::string> fenestrationFrameTypeValues();

    static std::vector<std::string> fenestrationDividerTypeValues();

    static std::vector<std::string> fenestrationTintValues();

    static std::vector<std::string> fenestrationGasFillValues();

    static std::vector<std::string> intendedSurfaceTypeValues();

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    /** Return the ConstructionBase object that this object describes. */
    ConstructionBase construction() const;

    /** Return the surface type to which this construction should be applied. */
    boost::optional<std::string> intendedSurfaceType() const;

    /** This is a freeform field used to identify the construction type for standards.
       \note Standards applied to this model will use this field to determine correct constructions.
       \note More information can be found at https://github.com/NREL/openstudio-standards. */
    boost::optional<std::string> standardsConstructionType() const;

    /** Returns a list of suggestions for standards construction type based on intendedSurfaceType. */
    std::vector<std::string> suggestedStandardsConstructionTypes() const;

    /** Return the layer of construction() whose thickness can be perturbed (usually to reach a
   *  certain property value for the overall construction), if the construction() exists and is a
   *  LayeredConstruction. */
    boost::optional<Material> perturbableLayer() const;

    /** Return a description of the perturbableLayer(), for instance, 'Insulation'.*/
    boost::optional<std::string> perturbableLayerType() const;

    bool isPerturbableLayerTypeDefaulted() const;

    boost::optional<std::string> otherPerturbableLayerType() const;

    /** Returns a list of suggestions for construction standard. */
    std::vector<std::string> suggestedConstructionStandards() const;

    /** Identifies the standard which specifies this construction.*/
    boost::optional<std::string> constructionStandard() const;

    /** Returns a list of suggestions for construction standard source based on construction standard. */
    std::vector<std::string> suggestedConstructionStandardSources() const;

    /** Identifies the table or section in standard which specifies this construction. */
    boost::optional<std::string> constructionStandardSource() const;

    /** Specific type of fenestration that this construction represents. */
    boost::optional<std::string> fenestrationType() const;

    /** Where the fenestration assembled. */
    boost::optional<std::string> fenestrationAssemblyContext() const;

    /** Number of panes for this fenestration construction. */
    boost::optional<std::string> fenestrationNumberOfPanes() const;

    /** Type of framing for this fenestration construction. */
    boost::optional<std::string> fenestrationFrameType() const;

    /** Type of divider for this fenestration construction. */
    boost::optional<std::string> fenestrationDividerType() const;

    /** Tint of this fenestration construction. */
    boost::optional<std::string> fenestrationTint() const;

    /** Type of gas used between panes of this fenestration construction. */
    boost::optional<std::string> fenestrationGasFill() const;

    /** Does this fenestration construction include a low-e coating. */
    bool fenestrationLowEmissivityCoating() const;

    //@}
    /** @name Setters */
    //@

    /** Sets the intendedSurfaceType to type. \sa intendedSurfaceTypeValues */
    bool setIntendedSurfaceType(const std::string& type);
    void resetIntendedSurfaceType();

    /** Set the constructionType to type. To be used with standardsinterface, this string should
   *  match an appropriate enumeration value in the standardsinterface::DataDictionary. The
   *  enumeration for which this should be a value generally varies by intendedSurfaceType(). */
    bool setStandardsConstructionType(const std::string& type);
    void resetStandardsConstructionType();

    /** Set the perturbableLayer to the one at layerIndex. Returns false if construction() is not a
   *  LayeredConstruction or
   *
   *  \code
   *  layerIndex >= construction().get().cast<LayeredConstruction>().numLayers().
   *  \endcode */
    bool setPerturbableLayer(unsigned layerIndex);

    /** Set the perturbableLayer to material. Returns false if construction() is not a
   *  LayeredConstruction or
   *
   *  \code
   *  MaterialVector layers = construction().get().cast<LayeredConstruction>().layers();
   *  std::find(layers.begin(),layers.end(),material) == layers.end();
   *  \endcode */
    bool setPerturbableLayer(const Material& material);

    /** Clears all PerturbableLayer information, including type. */
    void resetPerturbableLayer();

    /** Set the perturbableLayerType to type. Can be one of the provided types, or a free-form
   *  string. \sa standardPerturbableLayerTypeValues */
    bool setPerturbableLayerType(const std::string& type);
    void resetPerturbableLayerType();

    bool setOtherPerturbableLayerType(const std::string& otherPerturbableLayerType);
    void resetOtherPerturbableLayerType();

    bool setConstructionStandard(const std::string& constructionStandard);

    void resetConstructionStandard();

    bool setConstructionStandardSource(const std::string& constructionStandardSource);

    void resetConstructionStandardSource();

    bool setFenestrationType(const std::string& fenestrationType);

    void resetFenestrationType();

    bool setFenestrationAssemblyContext(const std::string& fenestrationAssemblyContext);

    void resetFenestrationAssemblyContext();

    bool setFenestrationNumberOfPanes(const std::string& fenestrationNumberofPanes);

    void resetFenestrationNumberOfPanes();

    bool setFenestrationFrameType(const std::string& fenestrationFrameType);

    void resetFenestrationFrameType();

    bool setFenestrationDividerType(const std::string& fenestrationDividerType);

    void resetFenestrationDividerType();

    bool setFenestrationTint(const std::string& fenestrationTint);

    void resetFenestrationTint();

    bool setFenestrationGasFill(const std::string& fenestrationGasFill);

    void resetFenestrationGasFill();

    bool setFenestrationLowEmissivityCoating(bool fenestrationLowEmissivityCoating);

    void setFenestrationLowEmissivityCoatingNoFail(bool fenestrationLowEmissivityCoating);

    void resetFenestrationLowEmissivityCoating();

    //@}
   protected:
    /// @cond
    using ImplType = detail::StandardsInformationConstruction_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::StandardsInformationConstruction_Impl;
    friend class detail::ConstructionBase_Impl;

    /** Create new object as child of construction. */
    explicit StandardsInformationConstruction(const ConstructionBase& construction);

    explicit StandardsInformationConstruction(std::shared_ptr<detail::StandardsInformationConstruction_Impl> impl);
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.StandardsInformationConstruction");
  };

  /** \relates StandardsInformationConstruction */
  using OptionalStandardsInformationConstruction = boost::optional<StandardsInformationConstruction>;

  /** \relates StandardsInformationConstruction */
  using StandardsInformationConstructionVector = std::vector<StandardsInformationConstruction>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_STANDARDSINFORMATIONCONSTRUCTION_HPP
