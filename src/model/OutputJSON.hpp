/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTJSON_HPP
#define MODEL_OUTPUTJSON_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputJSON_Impl;

  }  // namespace detail

  /** OutputJSON is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:JSON'. */
  class MODEL_API OutputJSON : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputJSON() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputJSON(const OutputJSON& other) = default;
    OutputJSON(OutputJSON&& other) = default;
    OutputJSON& operator=(const OutputJSON&) = default;
    OutputJSON& operator=(OutputJSON&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> optionTypeValues();

    /** @name Getters */
    //@{

    std::string optionType() const;

    bool outputJSON() const;

    bool outputCBOR() const;

    bool outputMessagePack() const;

    //@}
    /** @name Setters */
    //@{

    bool setOptionType(const std::string& optionType);

    bool setOutputJSON(bool outputJSON);

    bool setOutputCBOR(bool outputCBOR);

    bool setOutputMessagePack(bool outputMessagePack);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputJSON_Impl;

    explicit OutputJSON(std::shared_ptr<detail::OutputJSON_Impl> impl);

    friend class detail::OutputJSON_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputJSON(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputJSON");
  };

  /** \relates OutputJSON*/
  using OptionalOutputJSON = boost::optional<OutputJSON>;

  /** \relates OutputJSON*/
  using OutputJSONVector = std::vector<OutputJSON>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTJSON_HPP
