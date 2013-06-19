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


#include "ModelObjectPerturbationJob.hpp"
#include <ruleset/ModelRuleset.hpp>
#include <model/Model.hpp>

#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

namespace openstudio {
namespace runmanager {
namespace detail {

  ModelObjectPerturbationJob::ModelObjectPerturbationJob(const UUID &t_uuid,
      const Tools &tools,
      const JobParams &params,
      const Files &files,
      const JobState &t_restoreData)
    : ModelInModelOutJob(t_uuid, JobType::ModelObjectPerturbation, tools, params, files, t_restoreData)
  {
  }


  ModelObjectPerturbationJob::~ModelObjectPerturbationJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  // DLM: could this be passed in as non-const reference?
  model::Model ModelObjectPerturbationJob::modelToModelRun(const model::Model &t_model)
  {
    openstudio::ruleset::ModelRuleset modelRuleset = this->getModelRuleset();

    // DLM: I think this does a deep copy, unneccesary
    // JMT: Model is a pimpl and uses the default copy constructory - very cheap copy not a clone
    // ETH: Should the method signature be void modelToModelRun(model::Model& t_model)?
    model::Model result(t_model);
   
    /* bool test =*/  modelRuleset.apply(result);

    return result;
  }

  openstudio::ruleset::ModelRuleset ModelObjectPerturbationJob::getModelRuleset() const
  {
    std::string ruleData = allParams().get("rule_data").children.at(0).value;
 
    QDomDocument document;
    document.setContent(toQString(ruleData));

    QDomElement element = document.documentElement();

    openstudio::ruleset::ModelRuleset result(element);

    return result;
  }

  JobParams ModelObjectPerturbationJob::formatRuleData(const openstudio::ruleset::ModelRuleset& modelRuleset)
  {
    JobParams p;

    QString str;
    QTextStream qts(&str);
    modelRuleset.toXml().save(qts, 2);
    std::string ruleData = str.toStdString();

    p.append("rule_data", ruleData);
    return p;
  }

}
}
}


