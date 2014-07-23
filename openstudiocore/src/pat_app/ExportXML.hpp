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

#ifndef PATAPP_EXPORTXML_HPP
#define PATAPP_EXPORTXML_HPP

#include "../utilities/core/Path.hpp"
#include <QDomDocument>
#include <QDomElement>
#include "../analysis/Problem.hpp"
#include "../analysisdriver/SimpleProject.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/WorkflowStep.hpp"

#include "../model/ModelObject.hpp"

#include <map>

class QDomDocument;
class QDomElement;
class QDomNodeList;

namespace openstudio {

  class ProgressBar;
  class SqlFile;
  class Transformation;

namespace analysis {


namespace exportxml {

  class ExportXML {
  public:
    
    ExportXML();

    virtual ~ExportXML();

    bool exportXML(const analysisdriver::SimpleProject project, QString xmlFilePath);

  private:

    boost::optional<QDomElement> exportMeasure(QDomDocument& doc,
                                               const WorkflowStepJob& wfJob);


    boost::optional<QDomElement> exportAlternative(QDomDocument& doc, 
                                                   Attribute& alternativeAttr,
                                                   analysis::DataPoint dataPt,
                                                   std::vector<WorkflowStepJob>& jobs,
                                                   std::string edaBaselineName,
                                                   std::string proposedBaselineName,
                                                   std::string certificationBaselineName);

    boost::optional<QDomElement> exportCashFlow(QDomDocument& doc, 
                                                const Attribute& cashFlowAttr);

    boost::optional<QDomElement> exportAnnual(QDomDocument& doc,
                                              const Attribute& annualAttr );

    boost::optional<QDomElement> exportMonthly(QDomDocument& doc,
                                                const Attribute& monthlyAttr);

    boost::optional<QDomElement> exportChecks(QDomDocument& doc,
                                              const Attribute& checksAttr);

  };

} // exportxml
} // analysis
} // openstudio

#endif // PATAPP_EXPORTXML_HPP
