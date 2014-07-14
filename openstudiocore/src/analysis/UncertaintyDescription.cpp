/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "UncertaintyDescription.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "GenericUncertaintyDescription.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Finder.hpp"
#include "../utilities/core/Json.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  UncertaintyDescription_Impl::UncertaintyDescription_Impl(const UncertaintyDescriptionType& type)
    : m_type(type)
  {
    OS_ASSERT(this->type() != UncertaintyDescriptionType::Generic);
    populateAttributeDescriptions();
    restoreDefaults();
  }

  UncertaintyDescription_Impl::UncertaintyDescription_Impl(const UncertaintyDescriptionType& type,
                                                           const std::vector<Attribute>& attributes)
    : m_type(type),
      m_attributes(attributes)
  {
    OS_ASSERT(this->type() != UncertaintyDescriptionType::Generic);
    populateAttributeDescriptions();
  }

  UncertaintyDescription_Impl::UncertaintyDescription_Impl(const UncertaintyDescription_Impl& other)
    : m_type(other.type()),
      m_attributes(other.attributes(true)),
      m_attributeDescriptions(other.attributeDescriptions())
  {
    OS_ASSERT(type() != UncertaintyDescriptionType::Generic);
  }

  UncertaintyDescriptionType UncertaintyDescription_Impl::type() const {
    return m_type;
  }

  std::vector<Attribute> UncertaintyDescription_Impl::attributes(bool clones) const {
    if (clones) {
      AttributeVector result;
      for (const Attribute& attribute : m_attributes) {
        result.push_back(attribute.clone());
      }            
      return result;
    }
    return m_attributes;
  }

  Attribute UncertaintyDescription_Impl::getAttribute(const std::string& attributeName, bool clone) const {
    OptionalAttribute result = findByName<Attribute>(m_attributes,attributeName,true);
    OS_ASSERT(result);
    if (clone) {
      return result->clone();
    }
    return result.get();
  }

  bool UncertaintyDescription_Impl::isSet(const std::string& attributeName) const {
    return findByName<Attribute>(m_attributes,attributeName,true);
  }

  std::vector<AttributeDescription> UncertaintyDescription_Impl::attributeDescriptions() const {
    return m_attributeDescriptions;
  }

  AttributeDescription UncertaintyDescription_Impl::getAttributeDescription(
    const std::string& attributeName) const
  {
    OptionalAttributeDescription result = findStructByName<AttributeDescription>(m_attributeDescriptions,
                                                                                 attributeName,
                                                                                 true);
    OS_ASSERT(result);
    return result.get();
  }

  bool UncertaintyDescription_Impl::hasDescription(const std::string& attributeName) const {
    return findStructByName<AttributeDescription>(m_attributeDescriptions,attributeName,true);
  }

  bool UncertaintyDescription_Impl::isComplete() const {
    for (const AttributeDescription& desc : attributeDescriptions()) {
      if (desc.required && !isSet(desc.name)) {
        return false;
      }
    }
    return true;
  }

  bool UncertaintyDescription_Impl::setAttribute(const Attribute& candidate, bool check) {
    if (check) {
      if (!hasDescription(candidate.name()) || 
          !isConsistent(candidate,getAttributeDescription(candidate.name()))) 
      {
        return false;
      }
    }
    if (isSet(candidate.name())) {
      Attribute myCopy = getAttribute(candidate.name(),false);
      if (myCopy.valueType() == AttributeValueType::AttributeVector) {
        myCopy.setValue(candidate.valueAsAttributeVector());
      }
      else {
        myCopy.setValue(candidate.valueAsQVariant());
      }
    }
    else {
      m_attributes.push_back(candidate.clone());
      bool ok = prepareForDisplay(m_attributes.back(),getAttributeDescription(candidate.name()));
      OS_ASSERT(ok);
    }
    return true;
  }

  bool UncertaintyDescription_Impl::clearAttribute(const std::string& attributeName, bool check) {
    if (check) {
      if (!hasDescription(attributeName)) {
        return false;
      }
      AttributeDescription desc = getAttributeDescription(attributeName);
      if (desc.required) {
        return false;
      }
    }   
    auto it = findIteratorByName<Attribute>(m_attributes,attributeName,true);
    if (it == m_attributes.end()) {
      return false;
    }
    m_attributes.erase(it);
    return true;
  }

  void UncertaintyDescription_Impl::restoreDefaults() {
    m_attributes.clear();
    std::vector<double> vectorValue(2);
    switch (type().value()) {
      case UncertaintyDescriptionType::normal_uncertain :
        setAttribute(Attribute("means",0.0),false);
        setAttribute(Attribute("std_deviations",1.0),false);
       break;
      case UncertaintyDescriptionType::lognormal_uncertain :
        setAttribute(Attribute("means",1.0),false);
        setAttribute(Attribute("std_deviations",1.0),false);
       break;
      case UncertaintyDescriptionType::uniform_uncertain :
        setAttribute(Attribute("lower_bounds",0.0),false);
        setAttribute(Attribute("upper_bounds",1.0),false);
       break;
      case UncertaintyDescriptionType::loguniform_uncertain :
        setAttribute(Attribute("lower_bounds",0.1),false);
        setAttribute(Attribute("upper_bounds",1.0),false);
       break;
      case UncertaintyDescriptionType::triangular_uncertain :
        setAttribute(Attribute("modes",0.5),false);
        setAttribute(Attribute("lower_bounds",0.0),false);
        setAttribute(Attribute("upper_bounds",1.0),false);
       break;
      case UncertaintyDescriptionType::exponential_uncertain :
        setAttribute(Attribute("betas",1.0),false);
       break;
      case UncertaintyDescriptionType::beta_uncertain :
        setAttribute(Attribute("alphas",2.0),false);
        setAttribute(Attribute("betas",2.0),false);
        setAttribute(Attribute("lower_bounds",0.0),false);
        setAttribute(Attribute("upper_bounds",1.0),false);
       break;
      case UncertaintyDescriptionType::gamma_uncertain :
        setAttribute(Attribute("alphas",2.0),false);
        setAttribute(Attribute("betas",0.5),false);
       break;
      case UncertaintyDescriptionType::gumbel_uncertain :
        setAttribute(Attribute("alphas",1.0),false);
        setAttribute(Attribute("betas",2.0),false);
       break;
      case UncertaintyDescriptionType::frechet_uncertain :
        setAttribute(Attribute("alphas",2.0),false);
        setAttribute(Attribute("betas",1.0),false);
       break;
      case UncertaintyDescriptionType::weibull_uncertain :
        setAttribute(Attribute("alphas",1.0),false);
        setAttribute(Attribute("betas",1.0),false);
       break;
      case UncertaintyDescriptionType::histogram_bin_uncertain :
        vectorValue[0] = 0.0;
        vectorValue[1] = 1.0;
        setAttribute(createAttributeFromVector("abscissas",vectorValue),false);
        vectorValue[0] = 1.0;
        vectorValue[1] = 0.0;
        setAttribute(createAttributeFromVector("counts",vectorValue),false);
       break;
      case UncertaintyDescriptionType::poisson_uncertain :
        setAttribute(Attribute("lambdas",1.0),false);
       break;
      case UncertaintyDescriptionType::binomial_uncertain :
        setAttribute(Attribute("prob_per_trial",0.5),false);
        setAttribute(Attribute("num_trials",20),false);
       break;
      case UncertaintyDescriptionType::negative_binomial_uncertain :
        setAttribute(Attribute("prob_per_trial",0.5),false);
        setAttribute(Attribute("num_trials",20),false);
       break;
      case UncertaintyDescriptionType::geometric_uncertain :
        setAttribute(Attribute("prob_per_trial",0.5),false);
       break;
      case UncertaintyDescriptionType::hypergeometric_uncertain :
        setAttribute(Attribute("total_population",20),false);
        setAttribute(Attribute("selected_population",10),false);
        setAttribute(Attribute("num_drawn",10),false);
       break;
      case UncertaintyDescriptionType::histogram_point_uncertain :
        vectorValue[0] = 0.0;
        vectorValue[1] = 1.0;
        setAttribute(createAttributeFromVector("abscissas",vectorValue),false);
        vectorValue[0] = 1.0;
        vectorValue[1] = 1.0;
        setAttribute(createAttributeFromVector("counts",vectorValue),false);
       break;
      default :
        OS_ASSERT(false);
    }
  }

  void UncertaintyDescription_Impl::populateAttributeDescriptions() {
    switch (type().value()) {
      case UncertaintyDescriptionType::normal_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("means",
                                                               "Mean",
                                                               "The mean value of this (optionally bounded) normal distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("std_deviations",
                                                               "Standard Deviation",
                                                               "The standard deviation of this (optionally bounded) normal distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("lower_bounds",
                                                               "Lower Bound",
                                                               "The lower bound of the region from which samples are drawn",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false,
                                                               QVariant(QString("-DBL_MAX"))));
        m_attributeDescriptions.push_back(AttributeDescription("upper_bounds",
                                                               "Upper Bound",
                                                               "The upper bound of the region from which samples are drawn",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false,
                                                               QVariant(QString("+DBL_MAX"))));
       break;
       case UncertaintyDescriptionType::lognormal_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("means",
                                                               "Mean",
                                                               "The mean value of this (optionally bounded) lognormal distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false));
        m_attributeDescriptions.push_back(AttributeDescription("std_deviations",
                                                               "Standard Deviation",
                                                               "The standard deviation of this (optionally bounded) lognormal distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false));
        m_attributeDescriptions.push_back(AttributeDescription("error_factors",
                                                               "Error Factor",
                                                               "The error factor of this (optionally bounded) lognormal distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false));
        m_attributeDescriptions.push_back(AttributeDescription("lambdas",
                                                               "Lambda",
                                                               "The mean value (lambda) of the underlying normal distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false));
        m_attributeDescriptions.push_back(AttributeDescription("zetas",
                                                               "Zeta",
                                                               "The standard deviation (zeta) of the underlying normal distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false));
        m_attributeDescriptions.push_back(AttributeDescription("lower_bounds",
                                                               "Lower Bound",
                                                               "The lower bound of the region from which samples are drawn",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false,
                                                               QVariant(QString("0"))));
        m_attributeDescriptions.push_back(AttributeDescription("upper_bounds",
                                                               "Upper Bound",
                                                               "The upper bound of the region from which samples are drawn",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               false,
                                                               QVariant(QString("+DBL_MAX"))));
       break;
       case UncertaintyDescriptionType::uniform_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("lower_bounds",
                                                               "Lower Bound",
                                                               "The lower bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("upper_bounds",
                                                               "Upper Bound",
                                                               "The upper bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::loguniform_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("lower_bounds",
                                                               "Lower Bound",
                                                               "The lower bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("upper_bounds",
                                                               "Upper Bound",
                                                               "The upper bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::triangular_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("modes",
                                                               "Mode",
                                                               "The mode of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("lower_bounds",
                                                               "Lower Bound",
                                                               "The lower bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("upper_bounds",
                                                               "Upper Bound",
                                                               "The upper bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::exponential_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("betas",
                                                               "Beta",
                                                               "The beta parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::beta_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("alphas",
                                                               "Alpha",
                                                               "The alpha parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("betas",
                                                               "Beta",
                                                               "The beta parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("lower_bounds",
                                                               "Lower Bound",
                                                               "The lower bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("upper_bounds",
                                                               "Upper Bound",
                                                               "The upper bound of the sampling region",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::gamma_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("alphas",
                                                               "Alpha",
                                                               "The alpha parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("betas",
                                                               "Beta",
                                                               "The beta parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::gumbel_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("alphas",
                                                               "Alpha",
                                                               "The alpha parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("betas",
                                                               "Beta",
                                                               "The beta parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::frechet_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("alphas",
                                                               "Alpha",
                                                               "The alpha parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("betas",
                                                               "Beta",
                                                               "The beta parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::weibull_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("alphas",
                                                               "Alpha",
                                                               "The alpha parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("betas",
                                                               "Beta",
                                                               "The beta parameter of the distribution",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::histogram_bin_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("abscissas",
                                                               "Abscissas",
                                                               "The abscissa values ('x' coordinates) that define the histogram bins",
                                                               AttributeValueType(AttributeValueType::AttributeVector), // of type double
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("counts",
                                                               "Counts",
                                                               "The count or frequency associated with a bin",
                                                               AttributeValueType(AttributeValueType::AttributeVector), // of type double
                                                               false));
        m_attributeDescriptions.push_back(AttributeDescription("ordinates",
                                                               "Ordinates",
                                                               "The 'y' coordinates associated with the abscissa",
                                                               AttributeValueType(AttributeValueType::AttributeVector), // of type double
                                                               false));
        m_attributeDescriptions.push_back(AttributeDescription("num_pairs",
                                                               "Number of Pairs",
                                                               "The number of pairs of (x,counts) or (x,ordinates) that have been defined",
                                                               AttributeValueType(AttributeValueType::Integer),
                                                               false));
       break;
       case UncertaintyDescriptionType::poisson_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("lambdas",
                                                               "Lambda",
                                                               "The number of expected occurances in the time interval",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::binomial_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("prob_per_trial",
                                                               "Probability Per Trial",
                                                               "The probability of failure per trial",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("num_trials",
                                                               "Number of Trials",
                                                               "The number of trials",
                                                               AttributeValueType(AttributeValueType::Integer),
                                                               true));
       break;
       case UncertaintyDescriptionType::negative_binomial_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("prob_per_trial",
                                                               "Probability Per Trial",
                                                               "The probability of success per trial",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("num_trials",
                                                               "Number of Trials",
                                                               "The number of trials",
                                                               AttributeValueType(AttributeValueType::Integer),
                                                               true));
       break;
       case UncertaintyDescriptionType::geometric_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("prob_per_trial",
                                                               "Probability Per Trial",
                                                               "The probability of failure per trial",
                                                               AttributeValueType(AttributeValueType::Double),
                                                               true));
       break;
       case UncertaintyDescriptionType::hypergeometric_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("total_population",
                                                               "Total Population",
                                                               "The total population from which samples are drawn",
                                                               AttributeValueType(AttributeValueType::Integer),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("selected_population",
                                                               "Selected Population",
                                                               "The number of items in the selected population (e.g., the number of white balls)",
                                                               AttributeValueType(AttributeValueType::Integer),
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("num_drawn",
                                                               "Number Drawn",
                                                               "The number of items (e.g., balls) drawn",
                                                               AttributeValueType(AttributeValueType::Integer),
                                                               true));
       break;
       case UncertaintyDescriptionType::histogram_point_uncertain :
        m_attributeDescriptions.push_back(AttributeDescription("abscissas",
                                                               "Abscissas",
                                                               "The abscissa values ('x' coordinates) that define the histogram bins",
                                                               AttributeValueType(AttributeValueType::AttributeVector), // of type double
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("counts",
                                                               "Counts",
                                                               "The count or frequency associated with a bin",
                                                               AttributeValueType(AttributeValueType::AttributeVector), // of type double
                                                               true));
        m_attributeDescriptions.push_back(AttributeDescription("num_pairs",
                                                               "Number of Pairs",
                                                               "The number of pairs of (x,counts) or (x,ordinates) that have been defined",
                                                               AttributeValueType(AttributeValueType::Integer),
                                                               false));
       break;
      default :
        OS_ASSERT(false);
    }
  }

} // detail

UncertaintyDescription UncertaintyDescription::clone() const {
  std::shared_ptr<detail::UncertaintyDescription_Impl> cloneImpl(
      new detail::UncertaintyDescription_Impl(*impl()));
  return UncertaintyDescription(cloneImpl);
}
  
std::vector<UncertaintyDescriptionType> UncertaintyDescription::validTypes(
  const VariableValueType& variableValueType)
{
  UncertaintyDescriptionTypeVector result;
  if (variableValueType == VariableValueType::Continuous) {
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::normal_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::lognormal_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::uniform_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::loguniform_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::triangular_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::exponential_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::beta_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::gamma_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::gumbel_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::frechet_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::weibull_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::histogram_bin_uncertain));
  }
  else if (variableValueType == VariableValueType::Discrete) {
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::poisson_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::binomial_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::negative_binomial_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::geometric_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::hypergeometric_uncertain));
    result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::histogram_point_uncertain));
  }
  return result;
}

std::vector<UncertaintyDescriptionType> UncertaintyDescription::validTypes(
  const VariableValueType& variableValueType,
  const UncertaintyType& uncertaintyType)
{
  UncertaintyDescriptionTypeVector result;
  if (uncertaintyType == UncertaintyType::Aleatory) {
    if (variableValueType == VariableValueType::Continuous) {
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::normal_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::lognormal_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::uniform_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::loguniform_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::triangular_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::exponential_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::beta_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::gamma_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::gumbel_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::frechet_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::weibull_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::histogram_bin_uncertain));
    }
    else if (variableValueType == VariableValueType::Discrete) {
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::poisson_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::binomial_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::negative_binomial_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::geometric_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::hypergeometric_uncertain));
      result.push_back(UncertaintyDescriptionType(UncertaintyDescriptionType::histogram_point_uncertain));
    }
  }
  return result;
}

UncertaintyDescriptionType UncertaintyDescription::type() const {
  return impl()->type();
}

void UncertaintyDescription::restoreDefaults() {
  return impl()->restoreDefaults();
}

UncertaintyDescription::UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl> impl) 
  : m_impl(impl)
{}

std::shared_ptr<detail::UncertaintyDescription_Impl> UncertaintyDescription::impl() const {
  return m_impl;
}

namespace detail {

  QVariant toVariant(const UncertaintyDescription& udesc) {
    GenericUncertaintyDescription generic = udesc.cast<GenericUncertaintyDescription>();

    QVariantMap udescMap;
    udescMap["type"] = toQString(generic.actualType().valueName());
    if (!generic.attributes().empty()) {
      QVariantList attributesList;
      for (const Attribute& attribute : generic.attributes()) {
        attributesList.push_back(openstudio::detail::toVariant(attribute));
      }
      udescMap["attributes"] = QVariant(attributesList);
    }

    return QVariant(udescMap);
  }

  UncertaintyDescription toUncertaintyDescription(const QVariant& variant,
                                                  const VersionString& version)
  {
    QVariantMap map = variant.toMap();

    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));

    GenericUncertaintyDescription result(UncertaintyDescriptionType(map["type"].toString().toStdString()),
                                         attributes);

    return result.cast<UncertaintyDescription>();
  }

} // detail

} // analysis
} // openstudio
