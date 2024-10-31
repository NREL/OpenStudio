/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONSTRUCTIONS_HPP
#define MODEL_OUTPUTCONSTRUCTIONS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputConstructions_Impl;

  }  // namespace detail

  /** OutputConstructions is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:Constructions'. */
  class MODEL_API OutputConstructions : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputConstructions() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputConstructions(const OutputConstructions& other) = default;
    OutputConstructions(OutputConstructions&& other) = default;
    OutputConstructions& operator=(const OutputConstructions&) = default;
    OutputConstructions& operator=(OutputConstructions&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool reportConstructions() const;

    bool reportMaterials() const;

    //@}
    /** @name Setters */
    //@{

    bool setReportConstructions(bool reportConstructions);

    bool setReportMaterials(bool reportMaterials);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputConstructions_Impl;

    explicit OutputConstructions(std::shared_ptr<detail::OutputConstructions_Impl> impl);

    friend class detail::OutputConstructions_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputConstructions(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputConstructions");
  };

  /** \relates OutputConstructions*/
  using OptionalOutputConstructions = boost::optional<OutputConstructions>;

  /** \relates OutputConstructions*/
  using OutputConstructionsVector = std::vector<OutputConstructions>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONSTRUCTIONS_HPP
