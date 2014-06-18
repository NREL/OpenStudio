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
  
  virtual ~LifeCycleCostsView() {}

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
                        QWidget * parent = 0);

  virtual ~LifeCycleCostsTabView() {}

private:

  LifeCycleCostsView * m_lifeCycleCostsView;

};

} // openstudio

#endif // OPENSTUDIO_LIFECYCLECOSTSTABVIEW_HPP
