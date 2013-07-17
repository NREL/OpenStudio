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

#ifndef OPENSTUDIO_LIFECYCLECOSTSTABVIEW_H
#define OPENSTUDIO_LIFECYCLECOSTSTABVIEW_H

#include <openstudio_lib/MainTabView.hpp>

#include <model/Model.hpp>

#include <QWidget>

class QButtonGroup;
class QComboBox;
class QStackedWidget;

namespace openstudio {

class OSDoubleEdit;
class OSIntegerEdit;

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

  model::Model m_model;

  OSIntegerEdit * m_AnalysisLengthIntegerEdit;
  
  OSDoubleEdit * m_RealDiscountRateDoubleEdit;

  OSDoubleEdit * m_ElectricityDoubleEdit;
  OSDoubleEdit * m_NaturalGasDoubleEdit;
  OSDoubleEdit * m_SteamDoubleEdit;
  OSDoubleEdit * m_GasolineDoubleEdit;

  OSDoubleEdit * m_DieselDoubleEdit;
  OSDoubleEdit * m_PropaneDoubleEdit;
  OSDoubleEdit * m_CoalDoubleEdit;
  OSDoubleEdit * m_FuelOil_1DoubleEdit;

  OSDoubleEdit * m_FuelOil_2DoubleEdit;
  OSDoubleEdit * m_WaterDoubleEdit;

  QButtonGroup * m_fempGroup;
  QButtonGroup * m_nistGroup;

  QComboBox * m_nistRegionComboBox;
  QComboBox * m_nistSectorComboBox;

  QStackedWidget * m_stackedWidget;

private slots:

  void fempGroupClicked(int index);

  void nistGroupClicked(int index);

  void nistSectorChanged(const QString& string);

  void nistRegionChanged(const QString& string);

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
