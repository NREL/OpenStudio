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

#ifndef OPENSTUDIO_INSPECTORVIEW_HPP
#define OPENSTUDIO_INSPECTORVIEW_HPP

#include <QDialog>
#include <QWidget>

#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/StraightComponent.hpp"
#include "../model/StraightComponent_Impl.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement


class InspectorGadget;

class QComboBox;
class QPushButton;
class QStackedWidget;
class QTimer;
class QToolButton;
class QVBoxLayout;

namespace openstudio {

namespace model {

class Loop;
class ThermalZone;

}

class BaseInspectorView;
class LibraryTabWidget;
class LoopChooserView;
class ZoneChooserView;
class OSItem;

class InspectorView : public QWidget
{
  Q_OBJECT;

  public:

  InspectorView(QWidget* parent = nullptr);
  virtual ~InspectorView() {}

  void update();

  BaseInspectorView * currentView() { return m_currentView; }

  bool mouseOverInspectorView() { return m_mouseOverInspectorView; };

  signals:

  void addZoneClicked(model::ThermalZone &);
  void removeZoneClicked(model::ThermalZone &);
  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);
  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);
  void toggleUnitsClicked(bool displayIP);
  void itemRemoveClicked(OSItem *);
  void removeButtonClicked(bool);
  void workspaceObjectRemoved();

  void moveBranchForZoneSupplySelected(model::ThermalZone & zone, const Handle & newPlenumHandle);
  void moveBranchForZoneReturnSelected(model::ThermalZone & zone, const Handle & newPlenumHandle);
  void moveBranchForZoneToNewSupplyPlenumSelected(model::ThermalZone & zone, const Handle & newPlenumZoneHandle);
  void moveBranchForZoneToNewReturnPlenumSelected(model::ThermalZone & zone, const Handle & newPlenumZoneHandle);

  public slots:

  void layoutModelObject( model::OptionalModelObject &, bool readOnly, bool displayIP );
  virtual void toggleUnits(bool displayIP);

  protected slots:

  virtual void enterEvent(QEvent * event) override;
  virtual void leaveEvent(QEvent * event) override;

  private:

  QVBoxLayout * m_vLayout;
  BaseInspectorView * m_currentView;
  QStackedWidget * m_stackedWidget;
  boost::optional<model::ModelObject> m_modelObject;
  bool m_mouseOverInspectorView = false;
};

class BaseInspectorView : public QWidget, public Nano::Observer
{
  Q_OBJECT;

  public:

  BaseInspectorView(QWidget * parent = nullptr);

  virtual ~BaseInspectorView() {}

  void addMainView( QWidget * widget );

  virtual void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) = 0;

  virtual void update() {}

  LibraryTabWidget * m_libraryTabWidget;

  signals:

  void toggleUnitsClicked(bool displayIP); 

  void removeButtonClicked(bool);

  void workspaceObjectRemoved();

  private:

  QVBoxLayout * m_vLayout;

  public slots:

  virtual void toggleUnits(bool displayIP);
};

class GenericInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:
  
  GenericInspectorView(QWidget * parent = nullptr);

  virtual ~GenericInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  private:

  InspectorGadget * m_inspectorGadget;

  signals:

  void removeButtonClicked(bool);
};

class SplitterMixerInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  SplitterMixerInspectorView(QWidget * parent = nullptr);

  virtual ~SplitterMixerInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  signals:

  void addZoneClicked(model::ThermalZone &);

  void removeZoneClicked(model::ThermalZone &);

  private:

  ZoneChooserView * m_zoneChooserView;
};

class NewPlenumDialog : public QDialog
{
  Q_OBJECT;

  public:

  NewPlenumDialog(QWidget * parent = nullptr);

  virtual ~NewPlenumDialog() {}

  QComboBox * zoneChooser;

  private slots:

  void onCancelClicked(); 
  void onApplyClicked(); 
};

class PlenumChooserView : public QWidget
{
  Q_OBJECT;

  public:
  
  PlenumChooserView(QWidget * parent = nullptr);

  virtual ~PlenumChooserView() {}

  QComboBox * supplyPlenumChooser;
  QComboBox * returnPlenumChooser;
  QToolButton * newSupplyPlenumButton;
  QToolButton * newReturnPlenumButton;
};

class RefrigerationWalkinInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  RefrigerationWalkinInspectorView(QWidget * parent = nullptr);

  virtual ~RefrigerationWalkinInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  private:

  InspectorGadget * m_walkinInspectorGadget;
  QWidget * m_zoneBoundaryWidget;
  bool m_displayIP;
  boost::optional<model::ModelObject> m_modelObject;
};

class ThermalZoneInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ThermalZoneInspectorView(QWidget * parent = nullptr);

  virtual ~ThermalZoneInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  signals:

  void moveBranchForZoneSupplySelected(model::ThermalZone & zone, const Handle & newPlenumHandle);
  void moveBranchForZoneReturnSelected(model::ThermalZone & zone, const Handle & newPlenumHandle);

  private slots:

  void onSupplyPlenumChooserChanged(int newIndex);
  void onReturnPlenumChooserChanged(int newIndex);

  void onNewSupplyPlenumClicked();
  void onNewReturnPlenumClicked();

  private:

  void update() override;
  InspectorGadget * m_inspectorGadget;
  PlenumChooserView * m_plenumChooser;
  bool m_displayIP;
  boost::optional<model::ModelObject> m_modelObject;
};

class WaterToAirInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  WaterToAirInspectorView(QWidget * parent = nullptr);

  virtual ~WaterToAirInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private slots:

  void onWorkspaceObjectChanged();

  void layoutControllerObject();

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  InspectorGadget * m_coilControllerInspectorGadget;

  LoopChooserView * m_loopChooserView, * m_coolingLoopChooserView;

  bool m_displayIP;
};

class AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_coolingLoopChooserView;
};

class AirTerminalInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalInspectorView() {}

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  protected:

  template <class T>
  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP);

  //private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_loopChooserView;
};

class AirTerminalSingleDuctConstantVolumeReheatInspectorView : public AirTerminalInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctConstantVolumeReheatInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalSingleDuctConstantVolumeReheatInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;
};

class AirTerminalSingleDuctVAVReheatInspectorView : public AirTerminalInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctVAVReheatInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalSingleDuctVAVReheatInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;
};

class AirTerminalSingleDuctParallelPIUReheatInspectorView : public AirTerminalInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctParallelPIUReheatInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalSingleDuctParallelPIUReheatInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;
};

class AirTerminalSingleDuctSeriesPIUReheatInspectorView : public AirTerminalInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctSeriesPIUReheatInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalSingleDuctSeriesPIUReheatInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;
};

class AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView : public AirTerminalInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;
};

class ZoneHVACPackagedTerminalAirConditionerInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACPackagedTerminalAirConditionerInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACPackagedTerminalAirConditionerInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_loopChooserView;
};

class ZoneHVACPackagedTerminalHeatPumpInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACPackagedTerminalHeatPumpInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACPackagedTerminalHeatPumpInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_loopChooserView;
};

class WaterHeaterHeatPumpInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  WaterHeaterHeatPumpInspectorView(QWidget * parent = nullptr);

  virtual ~WaterHeaterHeatPumpInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_loopChooserView;
};

class ZoneHVACFourPipeFanCoilInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACFourPipeFanCoilInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACFourPipeFanCoilInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

  LoopChooserView * m_coolingLoopChooserView;
};

class ZoneHVACLowTempRadiantConstFlowInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACLowTempRadiantConstFlowInspectorView(QWidget * parent = nullptr);

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

  LoopChooserView * m_coolingLoopChooserView;
};

class ZoneHVACLowTempRadiantVarFlowInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACLowTempRadiantVarFlowInspectorView(QWidget * parent = nullptr);

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

  LoopChooserView * m_coolingLoopChooserView;
};

class ZoneHVACWaterToAirHeatPumpInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACWaterToAirHeatPumpInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACWaterToAirHeatPumpInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

  LoopChooserView * m_coolingLoopChooserView;

  LoopChooserView * m_supHeatingLoopChooserView;
};

class ZoneHVACBaseboardConvectiveWaterInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACBaseboardConvectiveWaterInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACBaseboardConvectiveWaterInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

};

class ZoneHVACBaseboardRadiantConvectiveWaterInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACBaseboardRadiantConvectiveWaterInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACBaseboardRadiantConvectiveWaterInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

};

class ZoneHVACUnitHeaterInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACUnitHeaterInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACUnitHeaterInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

};

class ZoneHVACUnitVentilatorInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACUnitVentilatorInspectorView(QWidget * parent = nullptr);

  virtual ~ZoneHVACUnitVentilatorInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

  LoopChooserView * m_coolingLoopChooserView;

};

class AirLoopHVACUnitarySystemInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  AirLoopHVACUnitarySystemInspectorView(QWidget * parent = nullptr);

  virtual ~AirLoopHVACUnitarySystemInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;
  LoopChooserView * m_coolingLoopChooserView;
  LoopChooserView * m_secondaryLoopChooserView;

};

class AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView(QWidget * parent = nullptr);

  virtual ~AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP ) override;

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;
  LoopChooserView * m_coolingLoopChooserView;

};

class ScheduleRulesetInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ScheduleRulesetInspectorView(QWidget * parent = nullptr);

  virtual ~ScheduleRulesetInspectorView() {}

  virtual void layoutModelObject(model::ModelObject & modelObject, bool readOnly, bool displayIP) override;

};

} // openstudio

#endif // OPENSTUDIO_INSPECTORVIEW_HPP

