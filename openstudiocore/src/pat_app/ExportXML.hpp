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

#ifndef OPENSTUDIO_EXPORTXML_H
#define OPENSTUDIO_EXPORTXML_H

#include <utilities/core/Path.hpp>
#include <QDomDocument>
#include <QDomElement>
#include <analysis/Problem.hpp>
#include <analysisdriver/SimpleProject.hpp>
#include <analysis/Measure.hpp>
#include <analysis/WorkflowStep.hpp>

#include <model/ModelObject.hpp>

#include <map>

class QDomDocument;
class QDomElement;
class QDomNodeList;

namespace openstudio {

  class ProgressBar;
  class Transformation;

namespace analysis {


namespace exportxml {

  class ExportXML {
  public:
    
    ExportXML();

    virtual ~ExportXML();

    bool exportXML(const analysisdriver::SimpleProject project, QString xmlFilePath);

  private:

    QString escapeName(const std::string& name);

    boost::optional<QDomElement> exportMeasure(QDomDocument& doc, BCLMeasure& measure );
    boost::optional<QDomElement> exportDesignAlternative(QDomDocument& doc, 
                                                          //model::Model& model,
                                                          SqlFile& sql,
                                                          analysis::DataPoint dataPt,
                                                          std::vector<WorkflowStepJob>& jobs,
                                                          bool isBaseline);
    boost::optional<QDomElement> exportAnnual(QDomDocument& doc,
                                              //model::Model& model,
                                              SqlFile& sql );
    boost::optional<QDomElement> exportMonthly(QDomDocument& doc,
                                                //model::Model& model,
                                                SqlFile& sql);
    boost::optional<QDomElement> exportAlternativeMeasure(QDomDocument& doc, WorkflowStepJob& wfJob);

    boost::optional<QDomElement> exportCashFlow(QDomDocument& doc, 
                                                std::map<std::string, double>& cashFlowMap, 
                                                std::string cashFlowTypeStr);    


  };

} // exportxml
} // analysis
} // openstudio

#endif // OPENSTUDIO_EXPORTXML_H
