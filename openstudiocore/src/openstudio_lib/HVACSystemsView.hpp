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

#ifndef OPENSTUDIO_HVACSYSTEMSVIEW_HPP
#define OPENSTUDIO_HVACSYSTEMSVIEW_HPP

#include <QWidget>
#include <QScrollArea>
#include <QGraphicsView>
#include "../shared_gui_components/OSComboBox.hpp"

class QStackedWidget;
class QPushButton;
class QLabel;
class QVBoxLayout;

namespace openstudio {

class HVACGraphicsView;
class HVACControlsView;
class HVACToolbarView;
class VentilationControlsView;
class SingleZoneResetSPView;
class OSViewSwitcher;
class OSSwitch;

class HVACSystemsView : public QWidget
{
  Q_OBJECT

  public:

  HVACSystemsView();

  virtual ~HVACSystemsView() {}

  OSViewSwitcher * mainViewSwitcher;

  HVACToolbarView * hvacToolbarView;

  QSize sizeHint() const;

  protected:

  void paintEvent(QPaintEvent * event);
};

class HVACToolbarView : public QWidget
{
  Q_OBJECT

  public:

  HVACToolbarView();

  virtual ~HVACToolbarView();

  QLabel * label;

  QPushButton * addButton;

  QPushButton * deleteButton;

  QPushButton * topologyViewButton;

  QPushButton * controlsViewButton;

  QPushButton * gridViewButton;

  QPushButton * zoomInButton;

  QPushButton * zoomOutButton;

  OSComboBox * systemComboBox;

  // Hide or show add, delete, zoom in, and zoom out buttons.
  // If control buttons are hidden only the systemComboBox and label are shown.
  void showControls(bool show);

  protected:

  void paintEvent(QPaintEvent * event);

  private:

  QStackedWidget * m_viewStack;
};

class HVACGraphicsView : public QGraphicsView
{
  Q_OBJECT

  public:

  HVACGraphicsView( QWidget * parent = 0 );

  virtual ~HVACGraphicsView() {}

  public slots:

  void zoomIn();

  void zoomOut();

  void resetZoom();

  private:

  float m_zoomX;

  float m_zoomY;
};

class HVACControlsView : public QScrollArea
{
  Q_OBJECT

  public:

  HVACControlsView();

  virtual ~HVACControlsView() {}

  QLabel * coolingTypeLabel;

  QLabel * heatingTypeLabel;

  QLabel * systemNameLabel;

  OSViewSwitcher * supplyAirTemperatureViewSwitcher;

  OSViewSwitcher * ventilationViewSwitcher;

  OSViewSwitcher * hvacOperationViewSwitcher;

  OSComboBox * nightCycleComboBox;
};

class MechanicalVentilationView : public QWidget
{
  Q_OBJECT;

  public:

  MechanicalVentilationView();

  virtual ~MechanicalVentilationView();

  QComboBox * economizerComboBox;

  QComboBox * ventilationCalcMethodComboBox;

  OSSwitch * dcvButton;
};

class NoMechanicalVentilationView : public QWidget
{
  Q_OBJECT;

  public:

  NoMechanicalVentilationView();

  virtual ~NoMechanicalVentilationView();
};

class SingleZoneReheatSPMView : public QWidget
{
  public:

  SingleZoneReheatSPMView();

  virtual ~SingleZoneReheatSPMView();

  OSComboBox * controlZoneComboBox;
};

class ScheduledSPMView : public QWidget
{
  public:

  ScheduledSPMView();

  virtual ~ScheduledSPMView();

  OSViewSwitcher * supplyAirTemperatureViewSwitcher;
};

class FollowOATempSPMView : public QWidget
{
  public:

  FollowOATempSPMView();

  virtual ~FollowOATempSPMView();
};

class OAResetSPMView : public QWidget
{
  public:

  OAResetSPMView();

  virtual ~OAResetSPMView();
};

class AirLoopHVACUnitaryHeatPumpAirToAirControlView : public QWidget
{
  public:

  AirLoopHVACUnitaryHeatPumpAirToAirControlView();

  virtual ~AirLoopHVACUnitaryHeatPumpAirToAirControlView();

  OSComboBox * controlZoneComboBox;
};

class NoSupplyAirTempControlView : public QWidget
{
  public:

  NoSupplyAirTempControlView();

  virtual ~NoSupplyAirTempControlView();
};

class NoControlsView : public QWidget
{
  public:

  NoControlsView();

  virtual ~NoControlsView();
};

} // openstudio

#endif // OPENSTUDIO_HVACSYSTEMSVIEW_HPP

