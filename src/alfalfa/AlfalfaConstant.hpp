#ifndef ALFALFA_COMPONENT_CONSTANT_HPP
#define ALFALFA_COMPONENT_CONSTANT_HPP

#include "ComponentBase.hpp"

#include "AlfalfaAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaConstant : public ComponentBase
  {
   public:
    /**
     * Create an AlfalfaConstant Component with a set value.
     */
    AlfalfaConstant(double value);

    std::string deriveName() const override;

    Json::Value toJSON() const override;

    ComponentCapability capability() const override {
      return ComponentCapability::Output;
    }

    ComponentType type() const override {
      return ComponentType::Constant;
    }

    std::unique_ptr<ComponentBase> clone() const override {
      return std::make_unique<AlfalfaConstant>(*this);
    }

    double value() const;

   private:
    double m_value;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
