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
class OSSwitch2;

class HVACSystemsView : public QWidget
{
  Q_OBJECT

  public:

  HVACSystemsView();

  virtual ~HVACSystemsView() {}

  OSViewSwitcher * mainViewSwitcher;

  HVACToolbarView * hvacToolbarView;

  QSize sizeHint() const override;

  protected:

  void paintEvent(QPaintEvent * event) override;
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

  OSComboBox2 * systemComboBox;

  // Hide or show add, delete, zoom in, and zoom out buttons.
  // If control buttons are hidden only the systemComboBox and label are shown.
  void showControls(bool show);

  protected:

  void paintEvent(QPaintEvent * event) override;

  private:

  QStackedWidget * m_viewStack;
};

class HVACGraphicsView : public QGraphicsView
{
  Q_OBJECT

  public:

  HVACGraphicsView( QWidget * parent = nullptr );

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

  OSComboBox2 * nightCycleComboBox;
};

class MechanicalVentilationView : public QWidget
{
  Q_OBJECT;

  public:

  MechanicalVentilationView();

  virtual ~MechanicalVentilationView();

  QComboBox * economizerComboBox;

  QComboBox * ventilationCalcMethodComboBox;

  OSSwitch2 * dcvButton;
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

  OSComboBox2 * controlZoneComboBox;
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

  OSComboBox2 * controlZoneComboBox;
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

