/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COMFORTVIEWFACTORANGLES_HPP
#define MODEL_COMFORTVIEWFACTORANGLES_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "Surface.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ComfortViewFactorAngles_Impl;
  }

  class MODEL_API AngleFactor
  {
   public:
    AngleFactor(const Surface& surface, double angleFactor);

    Surface surface() const;
    double angleFactor() const;

    bool operator==(const AngleFactor& other) const = default;
    bool operator!=(const AngleFactor& other) const;

   private:
    Surface m_surface;
    double m_angleFactor;
    REGISTER_LOGGER("openstudio.model.AngleFactor");
  };

  /** \relates AngleFactor */
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::AngleFactor& angleFactor);

  class MODEL_API ComfortViewFactorAngles : public ModelObject
  {
   public:
    explicit ComfortViewFactorAngles(const Model& model);

    virtual ~ComfortViewFactorAngles() override = default;

    static IddObjectType iddObjectType();

    std::vector<AngleFactor> comfortViewFactorAngles() const;
    unsigned int numberofComfortViewFactorAngles() const;
    boost::optional<AngleFactor> getComfortViewFactorAngle(unsigned groupIndex) const;

    bool addAngleFactor(const AngleFactor& angleFactor);
    bool addAngleFactor(const Surface& surface, double angleFactor);
    void removeComfortViewFactorAngle(int groupIndex);
    void removeAllComfortViewFactorAngles();

   protected:
    using ImplType = detail::ComfortViewFactorAngles_Impl;

    explicit ComfortViewFactorAngles(std::shared_ptr<detail::ComfortViewFactorAngles_Impl> impl);

    friend class detail::ComfortViewFactorAngles_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

   private:
    REGISTER_LOGGER("openstudio.model.ComfortViewFactorAngles");
  };

  using OptionalComfortViewFactorAngles = boost::optional<ComfortViewFactorAngles>;
  using ComfortViewFactorAnglesVector = std::vector<ComfortViewFactorAngles>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMFORTVIEWFACTORANGLES_HPP