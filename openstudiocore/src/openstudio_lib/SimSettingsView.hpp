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

#ifndef OPENSTUDIO_SIMSETTINGSVIEW_HPP
#define OPENSTUDIO_SIMSETTINGSVIEW_HPP

#include <QWidget>

#include "../model/Model.hpp"
#include "../model/ShadowCalculation.hpp"
#include "../model/SimulationControl.hpp"

class QButtonGroup;
class QCheckBox;
class QDate;
class QDateEdit;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

namespace openstudio {

  class OSComboBox;
  class OSComboBox2;
  class OSIntegerEdit;
  class OSIntegerEdit2;
  class OSLineEdit;
  class OSQuantityEdit;
  class OSSwitch;
  class OSSwitch2;
  class ScheduleCalendarWidget;

class SimSettingsView : public QWidget
{
  Q_OBJECT

public:

  SimSettingsView(bool isIP,
                  const model::Model & model,
                  QWidget * parent = 0);

  virtual ~SimSettingsView() {}

private:

  void createWidgets();
  void setCoarseRadianceSettings();
  void setFineRadianceSettings();
  void enableRadianceParametersWidget(bool isEnabled);

  QWidget * createRunPeriodWidget();
  QWidget * createRunControlWidget();
  QWidget * createSimulationControlWidget();
  QWidget * createSizingParametersWidget();
  QWidget * createProgramControlWidget();
  QWidget * createTimestepWidget();
  QWidget * createOutputControlReportingTolerancesWidget();
  QWidget * createConvergenceLimitsWidget();
  QWidget * createShadowCalculationWidget();
  QWidget * createSurfaceConvectionAlgorithmInsideWidget();
  QWidget * createSurfaceConvectionAlgorithmOutsideWidget();
  QWidget * createHeatBalanceAlgorithmWidget();
  QWidget * createZoneAirHeatBalanceAlgorithmWidget();
  QWidget * createZoneAirContaminantBalanceWidget();
  QWidget * createZoneCapacitanceMultipleResearchSpecialWidget();
  QWidget * createRadianceParametersWidget();

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSComboBox * & comboBox);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSComboBox2 * & comboBox);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QLabel * & label,
                QString text,
                OSComboBox * & comboBox);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QLabel * & label,
                QString text,
                OSComboBox2 * & comboBox);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSIntegerEdit * & integerEdit);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSIntegerEdit2 * & integerEdit);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QLabel * & label,
                QString text,
                OSIntegerEdit * & integerEdit);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QLabel * & label,
                QString text,
                OSIntegerEdit2 * & integerEdit);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSLineEdit * & lineEdit);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSQuantityEdit * & quantityEdit);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QLabel * & label,
                QString text,
                OSQuantityEdit * & quantityEdit);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSSwitch * & osSwitch);

  void addField(QGridLayout * gridLayout,
                int row,
                int column,
                QString text,
                OSSwitch2 * & osSwitch);

  void attachAll();
  void attachRunPeriod();
  void attachRunPeriodControlDaylightSavingTime();
  void attachSimulationControl();
  void attachSizingParameters();
  void attachProgramControl();
  void attachTimestep();
  void attachOutputControlReportingTolerances();
  void attachConvergenceLimits();
  void attachShadowCalculation();
  void attachSurfaceConvectionAlgorithmInside();
  void attachSurfaceConvectionAlgorithmOutside();
  void attachHeatBalanceAlgorithm();
  void attachZoneAirHeatBalanceAlgorithm();
  void attachZoneAirContaminantBalance();
  void attachZoneCapacitanceMultipleResearchSpecial();
  void attachRadianceParameters();

  void detachAll();
  void detachRunPeriod();
  void detachRunPeriodControlDaylightSavingTime();
  void detachSimulationControl();
  void detachSizingParameters();
  void detachProgramControl();
  void detachTimestep();
  void detachOutputControlReportingTolerances();
  void detachConvergenceLimits();
  void detachShadowCalculation();
  void detachSurfaceConvectionAlgorithmInside();
  void detachSurfaceConvectionAlgorithmOutside();
  void detachHeatBalanceAlgorithm();
  void detachZoneAirHeatBalanceAlgorithm();
  void detachZoneAirContaminantBalance();
  void detachZoneCapacitanceMultipleResearchSpecial();
  void detachRadianceParameters();

  model::Model m_model;
  boost::optional<model::ShadowCalculation> m_shadowCalculation;
  boost::optional<model::SimulationControl> m_simulationControl;

  QCheckBox * m_runSimWeatherFiles;
  QCheckBox * m_runSimDesignDays;
  QCheckBox * m_performZoneSizing;
  QCheckBox * m_performSystemSizing;
  QCheckBox * m_performPlantSizing;

  QLabel * m_dateRangelabel;

  QDateEdit * m_startDateEdit;
  QDateEdit * m_endDateEdit;

  QButtonGroup * m_runPeriodGroup;
  QButtonGroup * m_radianceGroup;
  
  QLineEdit * m_runPeriodName;

  bool m_isIP;

  // SimulationControl
  OSSwitch2 * m_doZoneSizingCalculation;
  OSSwitch2 * m_doSystemSizingCalculation;
  OSSwitch2 * m_doPlantSizingCalculation;
  OSSwitch2 * m_runSimulationforSizingPeriods;
  OSSwitch2 * m_runSimulationforWeatherFileRunPeriods;
  OSIntegerEdit2 * m_maximumNumberofWarmupDays;
  OSIntegerEdit2 * m_minimumNumberofWarmupDays;
  OSQuantityEdit * m_loadsConvergenceToleranceValue;
  OSQuantityEdit * m_temperatureConvergenceToleranceValue;
  OSComboBox * m_solarDistribution;

  // Radiance
  OSIntegerEdit *  m_accumulatedRaysperRecord;
  OSQuantityEdit * m_directThreshold;
  OSQuantityEdit * m_directCertainty;
  OSQuantityEdit * m_directJitter;
  OSQuantityEdit * m_directPretest;
  OSIntegerEdit * m_ambientBouncesVMX;
  OSIntegerEdit * m_ambientBouncesDMX;
  OSIntegerEdit * m_ambientDivisionsVMX;
  OSIntegerEdit * m_ambientDivisionsDMX;
  OSIntegerEdit * m_ambientSupersamples;
  OSQuantityEdit * m_limitWeightVMX;
  OSQuantityEdit * m_limitWeightDMX;
  OSIntegerEdit * m_klemsSamplingDensity;
  OSComboBox * m_skyDiscretizationResolution;
  QLabel * m_accumulatedRaysperRecordLbl;
  QLabel * m_directThresholdLbl;
  QLabel * m_directCertaintyLbl;
  QLabel * m_directJitterLbl;
  QLabel * m_directPretestLbl;
  QLabel * m_ambientBouncesVMXLbl;
  QLabel * m_ambientBouncesDMXLbl;
  QLabel * m_ambientDivisionsVMXLbl;
  QLabel * m_ambientDivisionsDMXLbl;
  QLabel * m_ambientSupersamplesLbl;
  QLabel * m_limitWeightVMXLbl;
  QLabel * m_limitWeightDMXLbl;
  QLabel * m_klemsSamplingDensityLbl;
  QLabel * m_skyDiscretizationResolutionLbl;

  // SizingParameters
  OSQuantityEdit * m_heatingSizingFactor;
  OSQuantityEdit * m_coolingSizingFactor;
  OSIntegerEdit * m_timestepsinAveragingWindow;

  // ProgramControl
  OSIntegerEdit * m_numberOfThreadsAllowed;

  // Timestep
  OSIntegerEdit * m_numberOfTimestepsPerHour;

  // OutputControlReportingTolerances
  OSQuantityEdit * m_toleranceForTimeHeatingSetpointNotMet;
  OSQuantityEdit * m_toleranceForTimeCoolingSetpointNotMet;

  // ConvergenceLimits
  OSIntegerEdit * m_maximumHVACIterations;
  OSIntegerEdit * m_minimumPlantIterations;
  OSIntegerEdit * m_maximumPlantIterations;
  OSIntegerEdit * m_minimumSystemTimestep;

  // ShadowCalculation
  OSIntegerEdit * m_calculationFrequency;
  OSIntegerEdit * m_maximumFiguresInShadowOverlapCalculations;
  OSComboBox2 * m_polygonClippingAlgorithm;
  OSComboBox2 * m_skyDiffuseModelingAlgorithm;

  // SurfaceConvectionAlgorithmInside
  OSLineEdit * m_algorithmSurfaceConvectionInside;

  // SurfaceConvectionAlgorithmOutside
  OSLineEdit * m_algorithmSurfaceConvectionOutside;

  // HeatBalance
  OSLineEdit * m_algorithmHeatBalance;
  OSQuantityEdit * m_surfaceTemperatureUpperLimit;
  OSQuantityEdit * m_minimumSurfaceConvectionHeatTransferCoefficientValue;
  OSQuantityEdit * m_maximumSurfaceConvectionHeatTransferCoefficientValue;

  // ZoneAirHeatBalanceAlgorithm
  OSLineEdit * m_algorithmZoneAirHeatBalance;

  // ZoneAirContaminantBalance
  OSSwitch * m_carbonDioxideConcentration;
  OSLineEdit * m_outdoorCarbonDioxideScheduleName;

  // ZoneCapacitanceMultiplierResearchSpecial
  OSQuantityEdit * m_temperatureCapacityMultiplier;
  OSQuantityEdit * m_humidityCapacityMultiplier;
  OSQuantityEdit * m_carbonDioxideCapacityMultiplier;

signals:

  void toggleUnitsClicked(bool displayIP);

private slots:

  void on_startDateChanged(const QDate & date);

  void on_endDateChanged(const QDate & date);

  void on_runPeriodGroupClicked(int idx);

  void on_radianceGroupClicked(int idx);

  void on_runSimWeatherFiles(int state);

  void on_runSimDesignDays(int state);

  void on_performZoneSizing(int state);

  void on_performSystemSizing(int state);

  void on_performPlantSizing(int state);

  void toggleUnits(bool displayIP);

  void updateYearDescription();

  void initStartDateEdit();

};

} // openstudio

#endif // OPENSTUDIO_SIMSETTINGSVIEW_HPP
