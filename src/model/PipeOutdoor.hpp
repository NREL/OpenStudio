/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PIPEOUTDOOR_HPP
#define MODEL_PIPEOUTDOOR_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Construction;
  class Node;

  namespace detail {

    class PipeOutdoor_Impl;

  }  // namespace detail

  /** PipeOutdoor is a StraightComponent that wraps the OpenStudio IDD object 'OS:Pipe:Outdoor'. */
  class MODEL_API PipeOutdoor : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PipeOutdoor(const Model& model);

    virtual ~PipeOutdoor() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PipeOutdoor(const PipeOutdoor& other) = default;
    PipeOutdoor(PipeOutdoor&& other) = default;
    PipeOutdoor& operator=(const PipeOutdoor&) = default;
    PipeOutdoor& operator=(PipeOutdoor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Construction> construction() const;

    boost::optional<Node> ambientTemperatureOutdoorAirNode() const;

    double pipeInsideDiameter() const;

    double pipeLength() const;

    //@}
    /** @name Setters */
    //@{

    bool setConstruction(const Construction& construction);

    void resetConstruction();

    bool setAmbientTemperatureOutdoorAirNode(const Node& node);

    void resetAmbientTemperatureOutdoorAirNode();

    bool setPipeInsideDiameter(double pipeInsideDiameter);

    bool setPipeLength(double pipeLength);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PipeOutdoor_Impl;

    explicit PipeOutdoor(std::shared_ptr<detail::PipeOutdoor_Impl> impl);

    friend class detail::PipeOutdoor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PipeOutdoor");
  };

  /** \relates PipeOutdoor*/
  using OptionalPipeOutdoor = boost::optional<PipeOutdoor>;

  /** \relates PipeOutdoor*/
  using PipeOutdoorVector = std::vector<PipeOutdoor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PIPEOUTDOOR_HPP
