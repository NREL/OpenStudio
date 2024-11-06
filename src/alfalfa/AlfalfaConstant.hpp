#ifndef ALFALFA_COMPONENT_CONSTANT_HPP
#define ALFALFA_COMPONENT_CONSTANT_HPP

#include "AlfalfaAPI.hpp"

#include "AlfalfaComponentBase.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaConstant : public AlfalfaComponentBase
  {
   public:
    /**
     * Create an AlfalfaConstant Component with a set value.
     */
    AlfalfaConstant(double value);

    std::string deriveName() const override;

    Json::Value toJSON() const override;

    AlfalfaComponentCapability capability() const override {
      return AlfalfaComponentCapability::Output;
    }

    AlfalfaComponentType type() const override {
      return AlfalfaComponentType::Constant;
    }

    std::unique_ptr<AlfalfaComponentBase> clone() const override {
      return std::make_unique<AlfalfaConstant>(*this);
    }

    double value() const;

   private:
    double m_value;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
