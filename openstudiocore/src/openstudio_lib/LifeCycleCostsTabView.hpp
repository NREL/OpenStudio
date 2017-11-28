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

#ifndef OPENSTUDIO_LIFECYCLECOSTSTABVIEW_HPP
#define OPENSTUDIO_LIFECYCLECOSTSTABVIEW_HPP

#include "MainTabView.hpp"

#include "../model/LifeCycleCostParameters.hpp"
#include "../model/Model.hpp"

#include <QWidget>

class QButtonGroup;
class QLabel;
class QStackedWidget;

namespace openstudio {

class OSComboBox2;
class OSDoubleEdit2;
class OSIntegerEdit2;

namespace model {
  class Model;
}

class LifeCycleCostsView : public QWidget
{
  Q_OBJECT

public:

  LifeCycleCostsView(const model::Model & model);

  virtual ~LifeCycleCostsView();

private:

  void createWidgets();

  QWidget * createInflationRatesWidget();

  QWidget * createNistWidget();

  void attach(openstudio::model::LifeCycleCostParameters & lifeCycleCostParameters);

  void detach();

  model::Model m_model;

  boost::optional<model::LifeCycleCostParameters> m_lifeCycleCostParameters;

  OSIntegerEdit2 * m_analysisLengthIntegerEdit;
  
  OSDoubleEdit2 * m_realDiscountRateDoubleEdit;

  OSDoubleEdit2 * m_electricityDoubleEdit;
  OSDoubleEdit2 * m_naturalGasDoubleEdit;
  OSDoubleEdit2 * m_steamDoubleEdit;
  OSDoubleEdit2 * m_gasolineDoubleEdit;

  OSDoubleEdit2 * m_dieselDoubleEdit;
  OSDoubleEdit2 * m_propaneDoubleEdit;
  OSDoubleEdit2 * m_coalDoubleEdit;
  OSDoubleEdit2 * m_fuelOil_1DoubleEdit;

  OSDoubleEdit2 * m_fuelOil_2DoubleEdit;
  OSDoubleEdit2 * m_waterDoubleEdit;

  QButtonGroup * m_fempGroup;
  QButtonGroup * m_nistGroup;

  OSComboBox2 * m_nistRegionComboBox;
  OSComboBox2 * m_nistSectorComboBox;

  QStackedWidget * m_stackedWidget;

  QLabel * m_analysisLengthLabel;
  QLabel * m_realDiscountRateLabel;

private slots:

  void fempGroupClicked(int index);

  void nistGroupClicked(int index);

};

class LifeCycleCostsTabView : public MainTabView
{

  Q_OBJECT

public:

  LifeCycleCostsTabView(const model::Model & model,
                        const QString& modelTempDir,
                        QWidget * parent = nullptr);

  virtual ~LifeCycleCostsTabView();

private:

  LifeCycleCostsView * m_lifeCycleCostsView;

};

} // openstudio

#endif // OPENSTUDIO_LIFECYCLECOSTSTABVIEW_HPP
