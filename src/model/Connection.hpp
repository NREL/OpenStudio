/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONNECTION_HPP
#define MODEL_CONNECTION_HPP

#include "ModelAPI.hpp"
#include "../utilities/core/Logger.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class Connection_Impl;
  }  // namespace detail

  class MODEL_API Connection : public ModelObject
  {
   public:
    // constructor
    explicit Connection(const Model& model);

    virtual ~Connection() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Connection(const Connection& other) = default;
    Connection(Connection&& other) = default;
    Connection& operator=(const Connection&) = default;
    Connection& operator=(Connection&&) = default;

    boost::optional<ModelObject> sourceObject() const;

    boost::optional<unsigned> sourceObjectPort() const;

    boost::optional<ModelObject> targetObject() const;

    boost::optional<unsigned> targetObjectPort() const;

    bool setSourceObject(const ModelObject& object);

    bool setSourceObjectPort(unsigned port);

    bool setTargetObject(const ModelObject& object);

    bool setTargetObjectPort(unsigned port);

    static IddObjectType iddObjectType();

   protected:
    using ImplType = detail::Connection_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ModelObject_Impl;

    // constructor
    explicit Connection(std::shared_ptr<detail::Connection_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Connection");
  };

  /** \relates Connection */
  using OptionalConnection = boost::optional<Connection>;

  /** \relates Connection */
  using ConnectionVector = std::vector<Connection>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONNECTION_HPP
