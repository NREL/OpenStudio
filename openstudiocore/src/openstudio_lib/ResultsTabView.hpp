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

#ifndef OPENSTUDIO_RESULTSTABVIEW_H
#define OPENSTUDIO_RESULTSTABVIEW_H

#include <openstudio_lib/MainTabView.hpp>

#include <model/Model.hpp>
#include <model/UtilityBill.hpp>

#include <utilities/idf/WorkspaceObject_Impl.hpp>
#include <utilities/sql/SqlFile.hpp>
#include <utilities/units/Unit.hpp>

#include <boost/smart_ptr.hpp>

#include <vtkCharts/BarChart.h>

#include <QComboBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

class QGridLayout;
class QPushButton;
class QStackedWidget;
class QTableWidget;
class QVBoxLayout;

namespace vtkCharts {
  class BarChart;
}

namespace openstudio {

  // standard results
  class ConsumptionData
  {
    public:
      ConsumptionData();
      ConsumptionData(const std::vector<openstudio::EndUseFuelType> &t_fuelTypes, const SqlFile &t_sqlFile);
      
      boost::optional<double> getValue(const openstudio::EndUseFuelType &t_fuelType,
          const openstudio::EndUseCategoryType &t_categoryType,
          const openstudio::MonthOfYear &t_monthOfYear) const;

      void setValue(const openstudio::EndUseFuelType &t_fuelType,
          const openstudio::EndUseCategoryType &t_categoryType,
          const openstudio::MonthOfYear &t_monthOfYear,
          const boost::optional<double> &t_value);

      static ConsumptionData random();
    private:
      REGISTER_LOGGER("openstudio::ConsumptionData");
      std::map<openstudio::EndUseFuelType, std::map<openstudio::EndUseCategoryType, std::map<openstudio::MonthOfYear, boost::optional<double> > > > m_data;
  };

  class ResultsConsumptionChart : public QWidget
  {
    Q_OBJECT;

    public:
      ResultsConsumptionChart(const openstudio::EndUseFuelType &t_fuelType, 
        const openstudio::Unit &t_unit, QWidget *t_parent=0);
      virtual ~ResultsConsumptionChart() {}
      void setData(const ConsumptionData &t_data, const openstudio::Unit &t_unit);
      openstudio::EndUseFuelType getFuelType() const;

    private:
      REGISTER_LOGGER("openstudio::ResultsConsumptionChart");
      boost::shared_ptr<vtkCharts::BarChart> m_chart;
      openstudio::EndUseFuelType m_fuelType;
      openstudio::Unit m_unit;
      QLabel* m_label;
  };

  class ResultsConsumptionLegend : public QWidget
  {
    Q_OBJECT;

    public:
      ResultsConsumptionLegend(QWidget *t_parent = 0);
      virtual ~ResultsConsumptionLegend() {}
      static std::vector<vtkCharts::Color3ub> getColors();

    private:
      REGISTER_LOGGER("openstudio::ResultsConsumptionLegend");

   };

  class ResultsConsumptionTable : public QWidget
  {
    Q_OBJECT;

    public:
      ResultsConsumptionTable(const openstudio::EndUseFuelType &t_fuelType, 
          const openstudio::Unit &t_unit, QWidget *t_parent = 0);
      virtual ~ResultsConsumptionTable() {}
      void setData(const ConsumptionData &t_data, const openstudio::Unit &t_unit);
      openstudio::EndUseFuelType getFuelType() const;

    private:
      REGISTER_LOGGER("openstudio::ResultsConsumptionTable");
      void buildDataGrid();
      void setDataMonthTotals(const ConsumptionData &t_data);
      void setDataCategoryTotals(const ConsumptionData &t_data);
      void setDataValues(const ConsumptionData &t_data);
      QLabel *createDataLabel(bool t_bold);
      void setDataValue(QLabel *t_label, const boost::optional<double> &t_data);
      void setRowHighlights();
      void updateUnitsLabel();

      openstudio::EndUseFuelType m_fuelType;
      openstudio::Unit m_unit;
      QGridLayout *m_grid;
      std::map<openstudio::EndUseCategoryType, std::map<openstudio::MonthOfYear, QLabel*> > m_labels;
      std::map<openstudio::EndUseCategoryType, QLabel*> m_categoryTotals;
      std::map<openstudio::MonthOfYear, QLabel*> m_monthTotals;
      QLabel* m_total;
      QLabel* m_label;
  };

  // utility bill results

  class UtilityBillComparisonView : public QWidget
  {
    Q_OBJECT;

    public:
      UtilityBillComparisonView(const openstudio::model::Model& model, QWidget *t_parent=0);
      virtual ~UtilityBillComparisonView() {}

    public slots:
      void buildGridLayout();

    private slots:
    
      void selectCalibrationMethod(const QString &);
      void onObjectAdded(const WorkspaceObject& workspaceObject);
      void onObjectRemoved(const WorkspaceObject& workspaceObject);
          
    private:
      REGISTER_LOGGER("openstudio::UtilityBillComparisonChart");

      openstudio::model::Model m_model;
      QLabel* m_calibrationMethodLabel;
      double m_calibrationMaxCVRMSE;
      double m_calibrationMaxNMBE;
      QGridLayout* m_gridLayout;
  };

  class UtilityBillComparisonChart : public QWidget
  {
    Q_OBJECT;

    public:
      UtilityBillComparisonChart(const openstudio::model::UtilityBill& utilityBill, bool isDemandChart, 
                                 double calibrationMaxCVRMSE, double calibrationMaxNMBE, QWidget *t_parent=0);
      virtual ~UtilityBillComparisonChart() {}
      openstudio::model::UtilityBill utilityBill() const;

    private slots:
    
      void onUtilityBillChanged();
      void plotConsumption();
      void plotDemand();

    private:
      REGISTER_LOGGER("openstudio::UtilityBillComparisonChart");
      openstudio::model::UtilityBill m_utilityBill;
      bool m_isDemandChart;
      double m_calibrationMaxCVRMSE;
      double m_calibrationMaxNMBE;

      boost::shared_ptr<vtkCharts::BarChart> m_chart;
      QLabel* m_label;
  };

  class UtilityBillComparisonLegend : public QWidget
  {
    Q_OBJECT;

    public:
      UtilityBillComparisonLegend(const openstudio::FuelType& fuelType, QWidget *t_parent = 0);
      virtual ~UtilityBillComparisonLegend() {}
      static std::vector<vtkCharts::Color3ub> getColors(const openstudio::FuelType& fuelType);

    private:
      REGISTER_LOGGER("openstudio::UtilityBillComparisonLegend");

      openstudio::FuelType m_fuelType;
   };

  class UtilityBillComparisonTable : public QWidget
  {
    Q_OBJECT;

    public:
      UtilityBillComparisonTable(const openstudio::model::UtilityBill& utilityBill, bool isDemandChart, 
                                 double calibrationMaxCVRMSE, double calibrationMaxNMBE, QWidget *t_parent = 0);
      virtual ~UtilityBillComparisonTable() {}
      openstudio::model::UtilityBill utilityBill() const;

    private slots:
    
      void onUtilityBillChanged();
      void setRowHighlights();

    private:
      REGISTER_LOGGER("openstudio::UtilityBillComparisonTable");
      openstudio::model::UtilityBill m_utilityBill;
      bool m_isDemandChart;
      double m_calibrationMaxCVRMSE;
      double m_calibrationMaxNMBE;

      QGridLayout *m_grid;
      QLabel* m_total;
      QLabel* m_label;
  };

  // main widget

  class ResultsView : public QWidget
  {
    Q_OBJECT;

    public:
      ResultsView(const model::Model & model, QWidget *t_parent = 0);
      virtual ~ResultsView() {}
      void searchForExistingResults(const openstudio::path &t_runDir);

    public slots:
      void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceResultsPath);
      void onUnitSystemChange(bool t_isIP);

    private slots:
      void openResultsViewerClicked();
      void selectView(int index);
      void onObjectAdded(const WorkspaceObject& workspaceObject);
      void onObjectRemoved(const WorkspaceObject& workspaceObject);
      void updateReportButtons();

    private:
      REGISTER_LOGGER("openstudio::ResultsView");
      static openstudio::Unit getUnit(openstudio::EndUseFuelType t_type, bool t_isIP);

      openstudio::model::Model m_model;
      bool m_isIP;

      // standard results
      ResultsConsumptionChart *m_electricConsumptionChart;
      ResultsConsumptionChart *m_gasConsumptionChart;
      ResultsConsumptionLegend *m_consumptionLegend;
      ResultsConsumptionTable *m_electricConsumptionTable;
      ResultsConsumptionTable *m_gasConsumptionTable;
      ResultsConsumptionTable *m_districtHeatingConsumptionTable;
      ResultsConsumptionTable *m_districtCoolingConsumptionTable;

      // utility bill results
      UtilityBillComparisonView* m_utilityBillComparisonView;

      QStackedWidget * m_stackedWidget;
      QLabel * m_reportLabel;
      QPushButton * m_standardResultsBtn;
      QPushButton * m_calibrationResultsBtn;
      QPushButton * m_openResultsViewerBtn;
      
      openstudio::path m_sqlFilePath;
      openstudio::path m_radianceResultsPath;
  };

  class ResultsTabView : public MainTabView
  {
    Q_OBJECT;

    public:

      ResultsTabView(const model::Model & model,
          const QString & tabLabel,
          bool hasSubTabs,
          QWidget * parent = 0);
      virtual ~ResultsTabView() {}
      void searchForExistingResults(const openstudio::path &t_runDir);

    public slots:
      void resultsGenerated(const openstudio::path &t_sqlFile, const openstudio::path &t_radianceResultsPath);
      void onUnitSystemChange(bool t_isIP);

    signals:

      //void removeResultClicked(model::myModelClass &);
      void removeResultClicked();

      void importResultClicked();

    private:
      ResultsView * m_resultsView;
      REGISTER_LOGGER("openstudio::ResultsTabView");
  };

} // openstudio

#endif // OPENSTUDIO_RESULTSTABVIEW_H
