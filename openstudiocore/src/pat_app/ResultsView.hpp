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

#ifndef OPENSTUDIO_RESULTSVIEW_H
#define OPENSTUDIO_RESULTSVIEW_H

#include <pat_app/PatMainTabView.hpp>

#include <analysis/DataPoint.hpp>

#include <QAbstractButton>

class QPushButton;
class QStringList;
class QTableWidget;
class QTimer;
class QPaintEvent;
class QStackedWidget;

namespace openstudio{

class Attribute;
class OSListView;
class OpenDirectoryButton;

namespace pat {

class ResultsView : public PatMainTabView
{
  Q_OBJECT

  public:

    ResultsView();

    virtual ~ResultsView() {}

    OSListView* baselineDataPointResultListView;

    OSListView* dataPointResultsListView;

    OSListView* dataPointCalibrationListView;

    double calibrationMaxNMBE() const;

    double calibrationMaxCVRMSE() const;

  signals: 

    void openButtonClicked(bool clicked);

    void openDirButtonClicked(bool clicked);

    void calibrationThresholdsChanged(double maxNMBE, double maxCVRMSE);

  public slots:

    void updateReportButtons();

    void selectView(int index);

    void enableViewFileButton(bool enable);

    void enableOpenDirectoryButton(bool enable);

    void selectCalibrationMethod(const QString &);

  private:
    QStackedWidget * m_stackedWidget;

    QPushButton * m_standardResultsBtn;
    QPushButton * m_calibrationResultsBtn;
    QPushButton* m_viewFileButton;

    QLabel* m_calibrationMethodLabel;
    double m_calibrationMaxNMBE;
    double m_calibrationMaxCVRMSE;

    OpenDirectoryButton* m_openDirButton;
};

class ResultsHeader : public QWidget
{
  Q_OBJECT

public:
  
  ResultsHeader(bool isBaseline);

};

class DataPointResultsView : public QAbstractButton
{
  Q_OBJECT

public:

  DataPointResultsView(const openstudio::analysis::DataPoint& dataPoint,
                       const openstudio::analysis::DataPoint& baselineDataPoint,
                       bool alternateRow);

  virtual ~DataPointResultsView() {}

public slots:
  
  void update();

  void setHasEmphasis(bool hasEmphasis);

protected:

  void paintEvent(QPaintEvent * e);

private:

  QLabel* m_nameLabel;
  //QLabel* m_measuresLabel;
  QLabel* m_netSiteEnergyIntensityLabel;
  QLabel* m_netSiteEnergyIntensityDivLabel;
  QLabel* m_netSiteEnergyIntensityPctLabel;
  QLabel* m_peakElectricDemandLabel;
  QLabel* m_peakElectricDemandDivLabel;
  QLabel* m_peakElectricDemandPctLabel;
  QLabel* m_electricityLabel;
  QLabel* m_electricityDivLabel;
  QLabel* m_electricityPctLabel;
  QLabel* m_naturalGasLabel;
  QLabel* m_naturalGasDivLabel;
  QLabel* m_naturalGasPctLabel;
  QLabel* m_districtCoolingLabel;
  QLabel* m_districtCoolingDivLabel;
  QLabel* m_districtCoolingPctLabel;
  QLabel* m_districtHeatingLabel;
  QLabel* m_districtHeatingDivLabel;
  QLabel* m_districtHeatingPctLabel;

  QLabel* m_capitalCostLabel;
  QLabel* m_capitalCostDivLabel;
  QLabel* m_capitalCostPctLabel;

  QLabel* m_energyCostLabel;
  QLabel* m_energyCostDivLabel;
  QLabel* m_energyCostPctLabel;

  QLabel* m_simplePayBackLabel;

  QLabel* m_lifeCycleCostLabel; 
  QLabel* m_lifeCycleCostDivLabel; 
  QLabel* m_lifeCycleCostPctLabel;

  QLabel* m_scrollSpacer;

  boost::optional<double> getValue(const std::string& attribute);
  boost::optional<double> getBaselineValue(const std::string& attribute);
  boost::optional<double> getDifference(const std::string& attribute);
  boost::optional<double> getPctDifference(const std::string& attribute);
  boost::optional<double> simplePayback();

  boost::optional<double> getValue(const std::string& attribute, const Attribute& attr);

  void setBaselineLabels(QLabel* resultLabel, const boost::optional<double> & value);
  void setBaselineLabels(QLabel* resultLabel, QLabel* resultDivLabel, QLabel* resultPctLabel, const boost::optional<double> & value);
  void setResultLabels(QLabel* resultLabel, const boost::optional<double> & value);
  void setResultLabels(QLabel* resultLabel, QLabel* resultDivLabel, QLabel* resultPctLabel, const boost::optional<double> & value, const boost::optional<double> & pct);
  void setResultLabelText(QLabel* label, const boost::optional<double> & value);
  void setResultPctLabelText(QLabel* label, const boost::optional<double> & value);

  openstudio::analysis::DataPoint m_dataPoint;
  openstudio::analysis::DataPoint m_baselineDataPoint;
  bool m_alternateRow;
};

class DataPointCalibrationHeaderView : public QWidget
{
  Q_OBJECT

public:
  
  DataPointCalibrationHeaderView();

private:

};

class DataPointCalibrationView : public QAbstractButton
{
  Q_OBJECT

public:

  DataPointCalibrationView(const openstudio::analysis::DataPoint& dataPoint,
                           const openstudio::analysis::DataPoint& baselineDataPoint,
                           bool alternateRow, double maxNMBE, double maxCVRMSE);

  virtual ~DataPointCalibrationView() {}

public slots:
  
  void update();

  void setHasEmphasis(bool hasEmphasis);

protected:

  void paintEvent(QPaintEvent * e);

private:

  QLabel* m_nameLabel;

  openstudio::analysis::DataPoint m_dataPoint;
  openstudio::analysis::DataPoint m_baselineDataPoint;
  bool m_alternateRow;
  double m_calibrationMaxNMBE;
  double m_calibrationMaxCVRMSE;
};

}

} // openstudio

#endif // OPENSTUDIO_RESULTSVIEW_H
