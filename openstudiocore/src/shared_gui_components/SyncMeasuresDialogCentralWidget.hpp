/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef SHAREDGUICOMPONENTS_SYNCMEASURESDIALOGCENTRALWIDGET_HPP
#define SHAREDGUICOMPONENTS_SYNCMEASURESDIALOGCENTRALWIDGET_HPP

#include <QWidget>

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"

class QPushButton;
class QProgressBar;

namespace openstudio {

class BCLComponent;
class BCLMeasure;
class Component;
class ComponentList;
class CollapsibleComponentList;
class MeasureManager;

class SyncMeasuresDialogCentralWidget : public QWidget
{
  Q_OBJECT

public:
  SyncMeasuresDialogCentralWidget(const WorkflowJSON& workflow,
    MeasureManager * measureManager,
    QWidget * parent = nullptr);
  virtual ~SyncMeasuresDialogCentralWidget() {}

  Component * checkedComponent() const;
  void setMeasures(const std::vector<BCLMeasure> & measures);
  QProgressBar * progressBar;
  QPushButton * lowerPushButton;

protected:

private:
  void createLayout();
  void init();
  void displayMeasures(int pageIdx);

  CollapsibleComponentList * m_collapsibleComponentList;
  ComponentList * m_componentList;
  int m_pageIdx;
  std::vector<BCLMeasure> m_measures;
  WorkflowJSON m_workflow;
  MeasureManager * m_measureManager;

signals:
  void componentClicked(bool checked);
  void getComponentsByPage(int pageNum);
  void noComponents();
  void closeDlg();

private slots:
  void upperPushButtonClicked();
  void lowerPushButtonClicked();
  void on_getComponentsByPage(int pageIdx);

};

} // namespace openstudio

#endif // SHAREDGUICOMPONENTS_SYNCMEASURESDIALOGCENTRALWIDGET_HPP
