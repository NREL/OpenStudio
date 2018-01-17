/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef PATAPP_RESULTSVIEW_HPP
#define PATAPP_RESULTSVIEW_HPP

#include "PatMainTabView.hpp"

#include "../analysis/DataPoint.hpp"

#include <QAbstractButton>

class QPushButton;
class QStringList;
class QTableWidget;
class QTimer;
class QPaintEvent;
class QStackedWidget;
class QPoint;
class QMenu;

namespace openstudio{

class Attribute;
class OSListView;
class OpenDirectoryButton;

namespace pat {
  
enum DownloadResultsStatus { LOCAL_MODE, RESULTS_DISABLED, RESULTS_DOWNLOADED, RESULTS_UNAVAILABLE, RESULTS_AVAILABLE, RUNNING_DETAILED, RUNNING_SLIM };

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

    void populateMenu(QMenu& menu, const openstudio::path& directory);

  signals: 

    void viewSelected(int);

    void openButtonClicked(bool clicked);

    void openDirButtonClicked(bool clicked);

    void downloadResultsButtonClicked(bool clicked);

    void calibrationThresholdsChanged(double maxNMBE, double maxCVRMSE);

  public slots:

    void updateReportButtons();

    void selectView(int index);

    void enableViewFileButton(bool enable);

    void enableOpenDirectoryButton(bool enable);

    void enableDownloadResultsButton(const DownloadResultsStatus& status);

    void selectCalibrationMethod(const QString &);

  protected slots:

    void openReport();

  private:

    void downloadResultsButtonEnabled(bool enabled);

    void openButtonEnabled(bool enabled);
  
    QStackedWidget * m_stackedWidget;

    QPushButton * m_standardResultsBtn;
    QPushButton * m_calibrationResultsBtn;
    QPushButton* m_viewFileButton;
    QPushButton* m_downloadResultsButton;

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

  void showContextMenu(const QPoint& pos);

protected:

  void paintEvent(QPaintEvent * e) override;

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
  bool m_hasEmphasis;
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
  
  void showContextMenu(const QPoint& pos);

protected:

  void paintEvent(QPaintEvent * e) override;

private:

  QLabel* m_nameLabel;

  openstudio::analysis::DataPoint m_dataPoint;
  openstudio::analysis::DataPoint m_baselineDataPoint;
  bool m_alternateRow;
  bool m_hasEmphasis;
  double m_calibrationMaxNMBE;
  double m_calibrationMaxCVRMSE;
};

QString getListOfMeasures(const openstudio::analysis::DataPoint & dataPoint);

}

} // openstudio

#endif // PATAPP_RESULTSVIEW_HPP
