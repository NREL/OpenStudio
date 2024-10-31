/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SKYTEMPERATURE_HPP
#define MODEL_SKYTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class SkyTemperature_Impl;
  }  // namespace detail

  class MODEL_API SkyTemperature : public ModelObject
  {
   public:
    // constructor
    explicit SkyTemperature(const Model& model);

    virtual ~SkyTemperature() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SkyTemperature(const SkyTemperature& other) = default;
    SkyTemperature(SkyTemperature&& other) = default;
    SkyTemperature& operator=(const SkyTemperature&) = default;
    SkyTemperature& operator=(SkyTemperature&&) = default;

    static IddObjectType iddObjectType();

   protected:
    using ImplType = detail::SkyTemperature_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit SkyTemperature(std::shared_ptr<detail::SkyTemperature_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SkyTemperature");
  };

  /** \relates SkyTemperature */
  using OptionalSkyTemperature = boost::optional<SkyTemperature>;

  /** \relates SkyTemperature */
  using SkyTemperatureVector = std::vector<SkyTemperature>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SKYTEMPERATURE_HPP
