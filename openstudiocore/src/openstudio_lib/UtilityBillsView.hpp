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

#ifndef OPENSTUDIO_UTILITYBILLSTABVIEW_H
#define OPENSTUDIO_UTILITYBILLSTABVIEW_H

#include <openstudio_lib/ModelObjectInspectorView.hpp>
#include <openstudio_lib/ModelSubTabView.hpp>

#include <model/Model.hpp>
#include <model/UtilityBill.hpp>

#include <utilities/data/DataEnums.hpp>

#include <QWidget>

class QButtonGroup;
class QDate;
class QDateEdit;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;
class QVBoxLayout;

namespace openstudio {

class BillFormatDialog;
class BillingPeriod;
class Date;
class FuelType;
class OSComboBox2;
class OSDoubleEdit2;
class OSIntegerEdit2;
class OSLineEdit2;
class OSUnsignedEdit2;
class UtilityBillListView;

class BillingPeriodWidget;

enum BillFormat{
  STARTDATE_ENDDATE,
  STARTDATE_NUMDAYS,
  ENDDATE_NUMDAYS
};

class UtilityBillsView : public ModelSubTabView
{
  Q_OBJECT

public:

  UtilityBillsView(const model::Model & model,
    QWidget * parent = 0);
  
  virtual ~UtilityBillsView() {}

private:

  void resetUtilityBills();

  static std::vector<std::pair<FuelType, std::string> > utilityBillFuelTypesAndNames();

};

class UtilityBillsInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

public:

  UtilityBillsInspectorView(const openstudio::model::Model& model,
    bool addScrollArea,
    QWidget * parent = 0);

  virtual ~UtilityBillsInspectorView() {}

  boost::optional<QString> runPeriodDates();

protected:

  virtual void onSelectItem(OSItem *item);
  virtual void onClearSelection();
  virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);
  virtual void onUpdate();

protected slots:

  //virtual void toggleUnits(bool displayIP);

private:

  void setCorrectInspectorView();
  void createWidgets();
  void getSortedBills();
  void showAddButton();
  void hideAddButton();
  void enableAddButton();
  void disableAddButton();
  void attach(openstudio::model::UtilityBill & utilityBill);
  void detach();
  void refresh();
  void updateRunPeriodDatesLabel();

  void createBillingPeriodHeaderWidget();
  void addBillingPeriodWidget(model::BillingPeriod & billingPeriod);
  void addBillingPeriodWidgets();

  void deleteBillingPeriodWidgets();

  QString getEnergyUseLabelText();
  QString getPeakLabelText();

  void updateEnergyUseLabelText();
  void updatePeakLabelText();

  boost::optional<model::UtilityBill> m_utilityBill;

  std::vector<BillingPeriodWidget *> m_billingPeriodWidgets;

  BillFormat m_billFormat;

  bool m_showPeak;

  QWidget * m_billingPeriodHeaderWidget;
  
  QButtonGroup * m_buttonGroup;

  OSLineEdit2 * m_name;

  QLabel * m_consumptionUnitsLabel;
  OSComboBox2 * m_consumptionUnits;
  QLabel * m_peakDemandUnitsLabel;
  OSComboBox2 * m_peakDemandUnits;
  QLabel * m_windowTimestepsLabel;
  OSUnsignedEdit2 * m_windowTimesteps;

  QPushButton * m_addBillingPeriod;

  QVBoxLayout * m_billPeriodLayout;

  QWidget * m_billPeriodLayoutWidget;

  QLabel * m_runPeriodDatesLabel;

  QLabel * m_energyUseLabel;
  QLabel * m_peakLabel;

  QString m_energyUseUnits;
  QString m_peakUnits;

  int m_hiddenWidgetIndex;
  int m_visibleWidgetIndex;
  
signals:
  void showSubTabView(bool showSubTabView);

private slots:
  void addBillingPeriod(bool checked);
  void deleteBillingPeriod(int index);
  void setBillFormat(int index);
  void setBillFormat(BillFormat billFormat);
  void updateEnergyUseLabelText(const QString& text);
  void updatePeakLabelText(const QString& text);
  void updateRunPeriodDates();
  void deleteAllWidgetsAndLayoutItems(QLayout * layout, bool deleteWidgets = true);

};

class BillingPeriodWidget : public QWidget
{

  Q_OBJECT

public:

  BillingPeriodWidget(model::BillingPeriod billingPeriod,
    FuelType fuelType,
    BillFormat billFormat,
    QWidget * parent = 0);

  virtual ~BillingPeriodWidget() {}

  void attach(openstudio::model::BillingPeriod & billingPeriod);
  void detach();

  QDateEdit * m_startDateEdit;
  QDateEdit * m_endDateEdit;

  OSIntegerEdit2 * m_billingPeriodIntEdit;
  OSDoubleEdit2 * m_energyUseDoubleEdit;
  OSDoubleEdit2 * m_peakDoubleEdit;
  OSDoubleEdit2 * m_costDoubleEdit;

  QPushButton * m_deleteBillWidget;

private:

  void createWidgets(FuelType fuelType,
    BillFormat billFormat);

  boost::optional<model::BillingPeriod> m_billingPeriod;

private slots:

  void modelObjectChanged();
  void startDateChanged(const QDate & newdate);
  void endDateChanged(const QDate & newdate);

};

} // openstudio

#endif // OPENSTUDIO_UTILITYBILLSTABVIEW_H
