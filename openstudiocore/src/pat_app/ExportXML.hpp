/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

namespace pat {

  class ExportXML {
  public:
    
    ExportXML();

    virtual ~ExportXML();

    bool exportXML(const analysisdriver::SimpleProject project, QString xmlFilePath);

  private:

    boost::optional<QDomElement> exportMeasure(QDomDocument& doc,
                                               const analysis::WorkflowStepJob& wfJob);

    std::vector<QDomElement> exportSwapModelAlternatives(QDomDocument& doc,
                                                         const QJsonArray& measures);

    boost::optional<QDomElement> exportAlternative(QDomDocument& doc,
                                                   Attribute& alternativeAttr,
                                                   analysis::DataPoint dataPt,
                                                   std::vector<analysis::WorkflowStepJob>& jobs,
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
                                              bool& isBaseline,
                                              const Attribute& checksAttr);

  };

} // pat
} // openstudio

#endif // PATAPP_EXPORTXML_HPP
