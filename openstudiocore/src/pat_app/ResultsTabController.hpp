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

#ifndef PATAPP_RESULTSTABCONTROLLER_HPP
#define PATAPP_RESULTSTABCONTROLLER_HPP

#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/DataPoint.hpp"

#include <QObject>
#include <QPointer>
#include <QSharedPointer>

namespace openstudio {
  
namespace pat { 

class ResultsView;
class BaselineDataPointResultListController;
class DataPointResultsListController;
class DataPointResultItemDelegate;
class DataPointCalibrationListController;
class DataPointCalibrationItemDelegate;

class ResultsTabController : public QObject
{
  Q_OBJECT

  public:

    QPointer<ResultsView> resultsView;

    ResultsTabController();

    virtual ~ResultsTabController();

  private slots:

    void selectView(int);

    void onOpenButtonClicked();

    void openDirectory();

    void downloadResults();

    void dataPointDetailsComplete(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    void enableDownloadResultsButton();

    void enableViewFileButton();

    void enableOpenDirectoryButton();

  private:

    QSharedPointer<BaselineDataPointResultListController> m_baselineDataPointResultListController;
    QSharedPointer<DataPointResultsListController> m_dataPointResultsListController;
    QSharedPointer<DataPointResultItemDelegate> m_dataPointResultItemDelegate;
    QSharedPointer<DataPointCalibrationListController> m_dataPointCalibrationListController;
    QSharedPointer<DataPointCalibrationItemDelegate> m_dataPointCalibrationItemDelegate;

    QSharedPointer<OSItemSelectionController> m_currentSelectionController;
};

/// Item representing a data point on the results tab
/// Used for both the standard and calibration results
class DataPointResultListItem : public OSListItem
{
  Q_OBJECT

 public:

  DataPointResultListItem(const openstudio::analysis::DataPoint& dataPoint,
                          const openstudio::analysis::DataPoint& baselineDataPoint,
                          bool alternateRow);

  virtual ~DataPointResultListItem() {}

  openstudio::analysis::DataPoint dataPoint() const;
  openstudio::analysis::DataPoint baselineDataPoint() const;
  bool alternateRow() const;

 private:

  openstudio::analysis::DataPoint m_dataPoint;
  openstudio::analysis::DataPoint m_baselineDataPoint;
  bool m_alternateRow;
};

/// Delegate which creates a DataPointResultListItem
class DataPointResultItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  virtual ~DataPointResultItemDelegate() {}

  /// Widget returned will be a DataPointResultListItem
  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

/// Delegate which creates a DataPointCalibrationListItem
class DataPointCalibrationItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  DataPointCalibrationItemDelegate(double maxNMBE, double maxCVRMSE);

  virtual ~DataPointCalibrationItemDelegate() {}

  /// Widget returned will be a DataPointCalibrationListItem
  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  public slots:
    
  void setCalibrationThresholds(double maxNMBE, double maxCVRMSE);

  private:

  double m_calibrationMaxNMBE;
  double m_calibrationMaxCVRMSE;
};

/// Controller class for the list of baseline data points on the results tab
class BaselineDataPointResultListController : public OSListController
{
  Q_OBJECT

 public:

  BaselineDataPointResultListController(const openstudio::analysis::Analysis& analysis);

  virtual ~BaselineDataPointResultListController() {}

  /// The OSListItem returned will be a DataPointResultListItem
  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

 private:

   openstudio::analysis::Analysis m_analysis;
};

/// Controller class for the list of non data points on the results tab
class DataPointResultsListController : public OSListController
{
  Q_OBJECT

 public:

  DataPointResultsListController(const openstudio::analysis::Analysis& analysis);

  virtual ~DataPointResultsListController() {}

  /// The OSListItem returned will be a DataPointResultListItem
  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

 private:

   std::vector<openstudio::analysis::DataPoint> dataPoints();

   openstudio::analysis::Analysis m_analysis;
};

/// Controller class for the list of calibration results on the results tab
class DataPointCalibrationListController : public OSListController
{
  Q_OBJECT

 public:

  DataPointCalibrationListController(const openstudio::analysis::Analysis& analysis);

  virtual ~DataPointCalibrationListController() {}

  /// The OSListItem returned will be a DataPointCalibrationListItem
  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

 private:

   std::vector<openstudio::analysis::DataPoint> dataPoints();

   openstudio::analysis::Analysis m_analysis;
};

}

} // openstudio

#endif // PATAPP_RESULTSTABCONTROLLER_HPP
