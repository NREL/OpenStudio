/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Curve.hpp"
#include "Curve_Impl.hpp"

#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    Curve_Impl::Curve_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ResourceObject_Impl(idfObject, model, keepHandle) {}

    Curve_Impl::Curve_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    Curve_Impl::Curve_Impl(const Curve_Impl& other, Model_Impl* model, bool keepHandle) : ResourceObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> Curve_Impl::parent() const {
      ParentObjectVector parents = getObject<Curve>().getModelObjectSources<ParentObject>();
      if (parents.size() > 1u) {
        LOG(Error, "This Curve, " << briefDescription() << " has multiple parents. Returning the first.");
      }
      if (!parents.empty()) {
        return parents[0];
      }
      return boost::none;
    }

    double Curve_Impl::evaluate(double x) const {
      return evaluate(DoubleVector(1u, x));
    }

    double Curve_Impl::evaluate(double x, double y) const {
      DoubleVector values(2u);
      values[0] = x;
      values[1] = y;
      return evaluate(values);
    }

    double Curve_Impl::evaluate(double x, double y, double z) const {
      DoubleVector values(3u);
      values[0] = x;
      values[1] = y;
      values[2] = z;
      return evaluate(values);
    }

    double Curve_Impl::evaluate(double w, double x, double y, double z) const {
      DoubleVector values(4u);
      values[0] = w;
      values[1] = x;
      values[2] = y;
      values[3] = z;
      return evaluate(values);
    }

    double Curve_Impl::evaluate(double v, double w, double x, double y, double z) const {
      DoubleVector values(5u);
      values[0] = v;
      values[1] = w;
      values[2] = x;
      values[3] = y;
      values[4] = z;
      return evaluate(values);
    }

    std::vector<EMSActuatorNames> Curve_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Curve", "Curve Result"}};
      return actuators;
    }

    std::vector<std::string> Curve_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  int Curve::numVariables() const {
    return getImpl<detail::Curve_Impl>()->numVariables();
  }

  double Curve::evaluate(const std::vector<double>& x) const {
    return getImpl<detail::Curve_Impl>()->evaluate(x);
  }

  double Curve::evaluate(double x) const {
    return getImpl<detail::Curve_Impl>()->evaluate(x);
  }

  double Curve::evaluate(double x, double y) const {
    return getImpl<detail::Curve_Impl>()->evaluate(x, y);
  }

  double Curve::evaluate(double x, double y, double z) const {
    return getImpl<detail::Curve_Impl>()->evaluate(x, y, z);
  }

  double Curve::evaluate(double w, double x, double y, double z) const {
    return getImpl<detail::Curve_Impl>()->evaluate(w, x, y, z);
  }

  double Curve::evaluate(double v, double w, double x, double y, double z) const {
    return getImpl<detail::Curve_Impl>()->evaluate(v, w, x, y, z);
  }

  /// @cond
  Curve::Curve(IddObjectType type, const Model& model) : ResourceObject(type, model) {
    OS_ASSERT(getImpl<detail::Curve_Impl>());
  }

  Curve::Curve(std::shared_ptr<detail::Curve_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
