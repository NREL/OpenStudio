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

#ifndef OPENSTUDIO_LIFECYCLECOSTSTABVIEW_H
#define OPENSTUDIO_LIFECYCLECOSTSTABVIEW_H

#include <openstudio_lib/MainTabView.hpp>

#include <model/LifeCycleCostParameters.hpp>
#include <model/Model.hpp>

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

  void LifeCycleCostsView::attach(openstudio::model::LifeCycleCostParameters & lifeCycleCostParameters);

  void LifeCycleCostsView::detach();

  model::Model m_model;

  boost::optional<model::LifeCycleCostParameters> m_lifeCycleCostParameters;

  OSIntegerEdit2 * m_AnalysisLengthIntegerEdit;
  
  OSDoubleEdit2 * m_RealDiscountRateDoubleEdit;

  OSDoubleEdit2 * m_ElectricityDoubleEdit;
  OSDoubleEdit2 * m_NaturalGasDoubleEdit;
  OSDoubleEdit2 * m_SteamDoubleEdit;
  OSDoubleEdit2 * m_GasolineDoubleEdit;

  OSDoubleEdit2 * m_DieselDoubleEdit;
  OSDoubleEdit2 * m_PropaneDoubleEdit;
  OSDoubleEdit2 * m_CoalDoubleEdit;
  OSDoubleEdit2 * m_FuelOil_1DoubleEdit;

  OSDoubleEdit2 * m_FuelOil_2DoubleEdit;
  OSDoubleEdit2 * m_WaterDoubleEdit;

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

#endif // OPENSTUDIO_LIFECYCLECOSTSTABVIEW_H
