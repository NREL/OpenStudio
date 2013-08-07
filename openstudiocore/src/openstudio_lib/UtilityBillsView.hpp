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

#include "../shared_gui_components/OSDialog.hpp"

#include <model/Model.hpp>
#include <model/UtilityBill.hpp>

#include <QWidget>

class QButtonGroup;
class QDate;
class QDateEdit;
class QDoubleSpinBox;
class QGridLayout;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QScrollArea;

namespace openstudio {

class BillFormatDialog;

class BillingPeriod;
class Date;
class FuelType;

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

  std::vector<std::pair<IddObjectType, std::string> > modelObjectTypesAndNames();

};

class UtilityBillsInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

public:

  UtilityBillsInspectorView(const openstudio::model::Model& model,
    bool addScrollArea,
    QWidget * parent = 0);

  virtual ~UtilityBillsInspectorView() {}

protected:

  //virtual void onSelectItem(OSItem *item);
  //virtual void onClearSelection();
  virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);
  virtual void onUpdate();

protected slots:

  //virtual void toggleUnits(bool displayIP);

private:

  void createWidgets();
  void showBillFormatDialog();
  void getSortedBills();
  void showAddButton();
  void hideAddButton();
  void enableAddButton();
  void disableAddButton();

  bool setStartDate(const Date& startDate);
  Date startDate() const;

  bool setEndDate(const Date& endDate); 
  Date endDate() const;

  bool setNumberOfDays(unsigned numberOfDays);
  unsigned numberOfDays() const;

  bool setConsumption(double consumption);
  boost::optional<double> consumption() const;

  bool setPeakDemand(double peakDemand); 
  boost::optional<double> peakDemand() const;

  bool setTotalCost(double totalCost);
  boost::optional<double> totalCost() const;

  void addBillingPeriod(model::BillingPeriod & billingPeriod);
  void addBillingPeriod(model::BillingPeriod & billingPeriod, unsigned index);
  void addBillingPeriods();

  void deleteBillingPeriod();
  void deleteBillingPeriods();

  boost::optional<model::UtilityBill> m_utilityBill;

  BillFormatDialog * m_billFormatDialog;

  BillFormat m_billFormat;

  bool m_showPeak;
  
  QButtonGroup * m_buttonGroup;

  QLineEdit * m_name;
  QLineEdit * m_consumptionUnits;
  QLineEdit * m_energyDemandUnits;
  QLineEdit * m_weatherFile;

  QDoubleSpinBox * m_windowTimesteps;

  QPushButton * m_addBillingPeriod;

  QGridLayout * m_billGridLayout;
  
private slots:
  
  void addBillingPeriod(bool checked);
  void deleteBillingPeriod(int index);
  void setBillFormat(BillFormat billFormat);

};

class BillingPeriodWidget : public QWidget
{

  Q_OBJECT

public:

  BillingPeriodWidget(QGridLayout * gridLayout,
    model::BillingPeriod billingPeriod,
    FuelType fuelType,
    BillFormat billFormat,
    unsigned index,
    QWidget * parent = 0);

  virtual ~BillingPeriodWidget() {}

private:

  void createWidgets(QGridLayout * gridLayout,
    model::BillingPeriod billingPeriod,
    FuelType fuelType,
    BillFormat billFormat);

  void getLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex, const QString& text);
  void getStartDateLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex);
  void getEndDateLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex);
  void getBillingPeriodLabel(QGridLayout * gridLayout, int rowIndex, int columnIndex);

  void getDateEdit(QGridLayout * gridLayout, int rowIndex, int columnIndex, QDateEdit * & dateEdit);
  void getStartDateCalendar(QGridLayout * gridLayout, int rowIndex, int columnIndex);
  void getEndDateCalendar(QGridLayout * gridLayout, int rowIndex, int columnIndex);

  void getBillingPeriodLineEdit(QGridLayout * gridLayout, int rowIndex, int columnIndex);

  QDateEdit * m_startDateEdit;
  QDateEdit * m_endDateEdit;

  QDoubleSpinBox * m_billingPeriodIntEdit;
  QDoubleSpinBox * m_energyUseDoubleEdit;
  QDoubleSpinBox * m_peaklDoubleEdit;
  QDoubleSpinBox * m_costDoubleEdit;

  QPushButton * m_deleteBillWidget;

  unsigned m_index;

private slots:

  void startDateChanged(const QDate & newdate);
  void endDateChanged(const QDate & newdate);

};

class BillFormatDialog : public OSDialog
{
  Q_OBJECT

public:

  BillFormatDialog(QWidget * parent = 0);

  virtual ~BillFormatDialog() {}

  BillFormat billFormat(){return m_billFormat;}

private:

  virtual void createLayout();

  BillFormat m_billFormat;

signals:

  void billFormatSignal(BillFormat);

private slots:

  void on_okButton(bool checked);
  void setBillFormat(int index);

};

} // openstudio

#endif // OPENSTUDIO_UTILITYBILLSTABVIEW_H
