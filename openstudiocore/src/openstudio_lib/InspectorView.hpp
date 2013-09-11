/**********************************************************************
 * Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_INSPECTORVIEW_HPP
#define OPENSTUDIO_INSPECTORVIEW_HPP

#include <QWidget>
#include <model/ModelObject.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>

class InspectorGadget;

class QStackedWidget;

class QVBoxLayout;

class QTimer;

namespace openstudio {

namespace model {

class ThermalZone;

class Loop;

}

class ZoneChooserView;

class BaseInspectorView;

class LibraryTabWidget;

class LoopChooserView;

class InspectorView : public QWidget
{
  Q_OBJECT;

  public:

  InspectorView(QWidget* parent = 0);

  virtual ~InspectorView() {}

  signals:

  void addZoneClicked(model::ThermalZone &);

  void removeZoneClicked(model::ThermalZone &);

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void toggleUnitsClicked(bool displayIP);

  public slots:

  void layoutModelObject( model::OptionalModelObject &, bool readOnly, bool displayIP );

  virtual void toggleUnits(bool displayIP);

  protected:

  private:

  QVBoxLayout * m_vLayout;

  BaseInspectorView * m_currentView;

  QStackedWidget * m_stackedWidget;
};

class BaseInspectorView : public QWidget
{
  Q_OBJECT;

  public:

  BaseInspectorView(QWidget * parent = 0);

  virtual ~BaseInspectorView() {}

  void addMainView( QWidget * widget );

  virtual void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP) = 0;

  signals:

  void toggleUnitsClicked(bool displayIP); 

  protected:

  LibraryTabWidget * m_libraryTabWidget;

  private:

  QVBoxLayout * m_vLayout;

  public slots:

  virtual void toggleUnits(bool displayIP);
};

class GenericInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:
  
  GenericInspectorView(QWidget * parent = 0);

  virtual ~GenericInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP);

  private:

  InspectorGadget * m_inspectorGadget;
};

class SplitterMixerInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  SplitterMixerInspectorView(QWidget * parent = 0);

  virtual ~SplitterMixerInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP);

  signals:

  void addZoneClicked(model::ThermalZone &);

  void removeZoneClicked(model::ThermalZone &);

  private:

  ZoneChooserView * m_zoneChooserView;
};

class WaterToAirInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  WaterToAirInspectorView(QWidget * parent = 0);

  virtual ~WaterToAirInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP);

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

  AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView(QWidget * parent = 0);

  virtual ~AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP);

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_coolingLoopChooserView;
};

class AirTerminalSingleDuctVAVReheatInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  AirTerminalSingleDuctVAVReheatInspectorView(QWidget * parent = 0);

  virtual ~AirTerminalSingleDuctVAVReheatInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP);

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_loopChooserView;
};

class ZoneHVACPackagedTerminalAirConditionerInspectorView : public BaseInspectorView
{
  Q_OBJECT;

  public:

  ZoneHVACPackagedTerminalAirConditionerInspectorView(QWidget * parent = 0);

  virtual ~ZoneHVACPackagedTerminalAirConditionerInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP);

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

  ZoneHVACFourPipeFanCoilInspectorView(QWidget * parent = 0);

  virtual ~ZoneHVACFourPipeFanCoilInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP );

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

  ZoneHVACLowTempRadiantConstFlowInspectorView(QWidget * parent = 0);

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP );

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

  ZoneHVACLowTempRadiantVarFlowInspectorView(QWidget * parent = 0);

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP );

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

  ZoneHVACWaterToAirHeatPumpInspectorView(QWidget * parent = 0);

  virtual ~ZoneHVACWaterToAirHeatPumpInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP );

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

  ZoneHVACBaseboardConvectiveWaterInspectorView(QWidget * parent = 0);

  virtual ~ZoneHVACBaseboardConvectiveWaterInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP );

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

  ZoneHVACUnitHeaterInspectorView(QWidget * parent = 0);

  virtual ~ZoneHVACUnitHeaterInspectorView() {}

  void layoutModelObject( model::ModelObject &, bool readOnly, bool displayIP );

  signals:

  void addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  void removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &);

  private:

  boost::optional<model::ModelObject> m_modelObject;

  InspectorGadget * m_inspectorGadget;

  LoopChooserView * m_heatingLoopChooserView;

};

} // openstudio

#endif // OPENSTUDIO_INSPECTORVIEW_HPP

