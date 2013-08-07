/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelRule_Impl.hpp>
#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>
#include <ruleset/ModelObjectActionClause.hpp>
#include <ruleset/ModelObjectActionClause_Impl.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelRule_Impl::ModelRule_Impl(const std::string& name)
    : Rule_Impl(name)
  {}

  ModelRule_Impl::ModelRule_Impl(const std::string& name, bool active, const UUID& uuid, const UUID& versionUUID,
                                 const std::vector<FilterClause>& filters, const std::vector<ActionClause>& actions)
    : Rule_Impl(name, active, uuid, versionUUID, filters, actions)
  {}

  ModelRule_Impl::ModelRule_Impl(const QDomElement& element)
    : Rule_Impl(element)
  {
  }

  std::string ModelRule_Impl::xmlElementName() const
  {
    return ModelRule::xmlElementName();
  }

  void ModelRule_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    Rule_Impl::writeValues(doc, element);
  }

  bool ModelRule_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && Rule_Impl::equivalentData(other);
    return result;
  }

  bool ModelRule_Impl::apply(openstudio::model::Model& model) const
  {
    bool anyPassedAllFilters = false;
    bool allActionsSuccessful = true;

    // todo: check ModelFilterClauses and apply ModelActionClauses here

    std::vector<ModelObjectFilterClause> modelObjectFilters = this->getFilters<ModelObjectFilterClause>();
    std::vector<ModelObjectActionClause> modelObjectActions = this->getActions<ModelObjectActionClause>();

    // loop over each object
    BOOST_FOREACH(openstudio::model::ModelObject modelObject, model.modelObjects()){

      bool passedAllFilters = true;

      BOOST_FOREACH(ModelObjectFilterClause modelObjectFilter, modelObjectFilters){
        if (!modelObjectFilter.check(modelObject)){
          passedAllFilters = false;
          break;
        }
      }

      if (!passedAllFilters){
        continue;
      }

      anyPassedAllFilters = true;

      BOOST_FOREACH(ModelObjectActionClause modelObjectAction, modelObjectActions){
        allActionsSuccessful = allActionsSuccessful && modelObjectAction.apply(modelObject);
      }

    }

    return (anyPassedAllFilters && allActionsSuccessful);
  }


} // detail

std::string ModelRule::xmlElementName()
{
  return "ModelRule";
}

ModelRule::ModelRule(const std::string& name)
  : Rule(boost::shared_ptr<detail::ModelRule_Impl>(new detail::ModelRule_Impl(name)))
{
  OS_ASSERT(getImpl<detail::ModelRule_Impl>());
}

ModelRule::ModelRule(const std::string& name, bool active, const UUID& uuid, const UUID& versionUUID,
                     const std::vector<FilterClause>& filters, const std::vector<ActionClause>& actions)
  : Rule(boost::shared_ptr<detail::ModelRule_Impl>(new detail::ModelRule_Impl(name,active,uuid,versionUUID,filters,actions)))
{
  OS_ASSERT(getImpl<detail::ModelRule_Impl>());
}

ModelRule::ModelRule(const QDomElement& element)
  : Rule(boost::shared_ptr<detail::ModelRule_Impl>(new detail::ModelRule_Impl(element)))
{
  OS_ASSERT(getImpl<detail::ModelRule_Impl>());
}

/// @cond
ModelRule::ModelRule(boost::shared_ptr<detail::ModelRule_Impl> impl)
  : Rule(impl)
{
  OS_ASSERT(getImpl<detail::ModelRule_Impl>());
}
/// @endcond

bool ModelRule::apply(openstudio::model::Model& model) const
{
  return getImpl<detail::ModelRule_Impl>()->apply(model);
}

} // ruleset
} // openstudio
