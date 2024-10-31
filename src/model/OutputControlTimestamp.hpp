/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLTIMESTAMP_HPP
#define MODEL_OUTPUTCONTROLTIMESTAMP_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputControlTimestamp_Impl;

  }  // namespace detail

  /** OutputControlTimestamp is a ModelObject that wraps the OpenStudio IDD object 'OS:OutputControl:Timestamp'. */
  class MODEL_API OutputControlTimestamp : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputControlTimestamp() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputControlTimestamp(const OutputControlTimestamp& other) = default;
    OutputControlTimestamp(OutputControlTimestamp&& other) = default;
    OutputControlTimestamp& operator=(const OutputControlTimestamp&) = default;
    OutputControlTimestamp& operator=(OutputControlTimestamp&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    bool iso8601Format() const;

    bool timestampAtBeginningOfInterval() const;

    //@}
    /** @name Setters */
    //@{

    bool setISO8601Format(bool iso8601Format);

    bool setTimestampAtBeginningOfInterval(bool timestampAtBeginningOfInterval);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputControlTimestamp_Impl;

    explicit OutputControlTimestamp(std::shared_ptr<detail::OutputControlTimestamp_Impl> impl);

    friend class detail::OutputControlTimestamp_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputControlTimestamp(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputControlTimestamp");
  };

  /** \relates OutputControlTimestamp*/
  using OptionalOutputControlTimestamp = boost::optional<OutputControlTimestamp>;

  /** \relates OutputControlTimestamp*/
  using OutputControlTimestampVector = std::vector<OutputControlTimestamp>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLTIMESTAMP_HPP
