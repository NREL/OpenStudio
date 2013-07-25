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

#include <openstudio_lib/ResultsTabView.hpp>

#include <model/Model_Impl.hpp>

#include <QDir>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTableWidget>
#include <QVBoxLayout>

#include <iostream>

#include <vtkCharts/Color.h>

#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/Scale.hpp>
#include <utilities/units/UnitFactory.hpp>

#include <boost/filesystem.hpp>
#include <boost/random.hpp>

namespace openstudio {

  ComparisonData::ComparisonData()
  {
  }

  ComparisonData::ComparisonData(
      const std::vector<openstudio::EndUseFuelType> &t_fuelTypes,
      const SqlFile &t_sql)
  {
    for (std::vector<openstudio::EndUseFuelType>::const_iterator fuelType = t_fuelTypes.begin();
        fuelType != t_fuelTypes.end();
        ++fuelType)
    {
      std::set<int> enduses = openstudio::EndUseCategoryType::getValues();
      for (std::set<int>::const_iterator itr = enduses.begin();
          itr != enduses.end();
          ++itr)
      {
        std::vector<float> uses;
        std::set<int> months = openstudio::MonthOfYear::getValues();
        int monthcount = 0;
        for (std::set<int>::const_iterator monthitr = months.begin();
            monthitr != months.end() && monthcount < 12;
            ++monthitr, ++monthcount)
        {
          setValue(*fuelType, *itr, *monthitr, t_sql.energyConsumptionByMonth(*fuelType, *itr, *monthitr));
        }
      }
    }
  }

  ComparisonData ComparisonData::random()
  {
    ComparisonData data;
    boost::mt19937 rng;
    boost::uniform_int<> boolean(0,1); 
    boost::uniform_real<> value(0,6); 
    boost::variate_generator<boost::mt19937&, boost::uniform_int<> > isempty(rng, boolean);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > makevalue(rng, value);

    std::set<int> endusefueltypes = openstudio::EndUseFuelType::getValues();
    for (std::set<int>::const_iterator fuelType = endusefueltypes.begin();
        fuelType != endusefueltypes.end();
        ++fuelType)
    {
      std::set<int> enduses = openstudio::EndUseCategoryType::getValues();
      for (std::set<int>::const_iterator itr = enduses.begin();
          itr != enduses.end();
          ++itr)
      {
        std::vector<float> uses;
        std::set<int> months = openstudio::MonthOfYear::getValues();
        int monthcount = 0;
        for (std::set<int>::const_iterator monthitr = months.begin();
            monthitr != months.end() && monthcount < 12;
            ++monthitr, ++monthcount)
        {
          boost::optional<double> val;

          if (!isempty())
          {
            val = makevalue();
          } 

          data.setValue(*fuelType, *itr, *monthitr, val);
        }
      }
    }

    return data;
  }

  template <typename T, typename Arg>
    typename T::const_iterator find_map_value(const T &t, const Arg &arg)
    {
      typename T::const_iterator itr = t.find(arg);
      if (itr != t.end())
      {
        return itr;
      } else {
        throw std::out_of_range("");
      }
    }

  boost::optional<double> ComparisonData::getValue(const openstudio::EndUseFuelType &t_fuelType,
      const openstudio::EndUseCategoryType &t_categoryType,
      const openstudio::MonthOfYear &t_monthOfYear) const
  {
    std::map<openstudio::EndUseFuelType, std::map<openstudio::EndUseCategoryType, std::map<openstudio::MonthOfYear, boost::optional<double> > > >::const_iterator itr1 = m_data.find(t_fuelType);

    if (itr1 != m_data.end())
    {
      std::map<openstudio::EndUseCategoryType, std::map<openstudio::MonthOfYear, boost::optional<double> > >::const_iterator itr2 = itr1->second.find(t_categoryType);

      if (itr2 != itr1->second.end())
      {
        std::map<openstudio::MonthOfYear, boost::optional<double> >::const_iterator itr3 = itr2->second.find(t_monthOfYear);
        if (itr3 != itr2->second.end())
        {
          return itr3->second;
        }
      }
    }

    return boost::optional<double>();
  }

  void ComparisonData::setValue(const openstudio::EndUseFuelType &t_fuelType,
      const openstudio::EndUseCategoryType &t_categoryType,
      const openstudio::MonthOfYear &t_monthOfYear,
      const boost::optional<double> &t_value)
  {
    m_data[t_fuelType][t_categoryType][t_monthOfYear] = t_value;
  }



  ResultsComparisonData::ResultsComparisonData(const openstudio::EndUseFuelType &t_fuelType, 
      const openstudio::Unit &t_unit, QWidget *t_parent)
    : QWidget(t_parent), m_fuelType(t_fuelType), m_unit(t_unit)
  {
    QVBoxLayout *vboxlayout = new QVBoxLayout();
    vboxlayout->setContentsMargins(0,0,0,0);
    vboxlayout->setSpacing(10);
    m_label = new QLabel();
    m_label->setObjectName("H2");
    vboxlayout->addWidget(m_label);


    setLayout(vboxlayout);

    setData(ComparisonData(), m_unit);
  }

  openstudio::EndUseFuelType ResultsComparisonData::getFuelType() const
  {
    return m_fuelType;
  }

  void ResultsComparisonData::setData(const ComparisonData &t_data, const openstudio::Unit &t_unit)
  {
    m_unit = t_unit;
    std::string unitstring = t_unit.prettyString().empty()?t_unit.standardString():t_unit.prettyString();
    if(unitstring == "MBtu"){
      unitstring = "Million Btu";
    }
    m_label->setText(openstudio::toQString(m_fuelType.valueDescription() + " Consumption (" + unitstring + ")"));
    setUpdatesEnabled(false);
    QLayout *l = layout();

    if (l->count() > 1)
    {
      l->removeItem(l->itemAt(l->count() - 1));
    }

    m_chart.reset();

    boost::shared_ptr<vtkCharts::BarChart> chart(new vtkCharts::BarChart("Consumption"));
    std::set<int> enduses = openstudio::EndUseCategoryType::getValues();


    std::vector<std::string> labels;
    std::map<int, std::string> monthnames = openstudio::MonthOfYear::getNames();

    int monthnum = 0;
    for (std::map<int, std::string>::const_iterator itr = monthnames.begin();
         itr != monthnames.end() && monthnum < 12;
         ++itr, ++monthnum)
    {
      labels.push_back(itr->second);
    }

    chart->setXTickLabels(labels);

    int numzeros = m_unit.scale().exponent;

    std::string scalestring;
    for (int i = 0; i < numzeros; ++i)
    {
      if (i % 3 == 0 && i != 0)
      {
        scalestring.insert(0, ",");
      }

      scalestring.insert(0, "0");
    }

    scalestring.insert(0, "x");

    chart->setColors(ResultsComparisonLegend::getColors());
    //chart->axis(vtkCharts::Axis::LEFT).setTitle("(" + scalestring + ")");
    chart->axis(vtkCharts::Axis::LEFT).setTitle("");
    chart->axis(vtkCharts::Axis::BOTTOM).setTitle("");


    Unit u = *openstudio::createUnit("J");

    for (std::set<int>::const_iterator itr = enduses.begin();
         itr != enduses.end();
         ++itr)
    {
      std::vector<float> uses;
      std::set<int> months = openstudio::MonthOfYear::getValues();
      int monthcount = 0;
      for (std::set<int>::const_iterator monthitr = months.begin();
           monthitr != months.end() && monthcount < 12;
           ++monthitr, ++monthcount)
      {
        boost::optional<double> val = t_data.getValue(m_fuelType, *itr, *monthitr);
        if (val)
        {
          Quantity q(*val, u);
          uses.push_back(openstudio::convert(q, m_unit)->value());
        } else {
          uses.push_back(0);
        }
      }

      chart->stackSeries(uses, "Consumption", openstudio::EndUseCategoryType::valueDescription(*itr));
    }

    layout()->addWidget(chart->widget());
    chart->rescale();
    m_chart = chart;
    setUpdatesEnabled(true);
  }

  ResultsComparisonLegend::ResultsComparisonLegend(QWidget *t_parent)
    : QWidget(t_parent)
  {
    QVBoxLayout *vboxlayout = new QVBoxLayout();

    std::map<int, std::string> s = openstudio::EndUseCategoryType::getDescriptions();
    std::vector<vtkCharts::Color3ub> c = getColors();


    for (size_t i = 0; i < s.size(); ++i)
    {
      QPixmap pm(10, 10);
      const vtkCharts::Color3ub color = c.at(i);
      pm.fill(QColor(color.Red(), color.Green(), color.Blue()));
      QHBoxLayout *hboxlayout = new QHBoxLayout();
      QLabel *l = new QLabel();
      l->setPixmap(pm);
      l->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

      hboxlayout->addWidget(l);
      hboxlayout->addWidget(new QLabel(openstudio::toQString(s[i])));

      vboxlayout->addLayout(hboxlayout);
    }

    setLayout(vboxlayout);
  }

  std::vector<vtkCharts::Color3ub> ResultsComparisonLegend::getColors()
  {
    // Heating: 237, 28, 36
    // Cooling: 0, 113, 188
    // Interior Lighting: 244, 222, 17
    // Exterior Lighting: 216, 192, 18
    // Interior Equipment: 77, 77, 77
    // Exterior Equipment: 179, 179, 179
    // Fans: 255, 123,172
    // Pumps: 102, 45, 145
    // Heat Rejection: 241, 90, 36
    // Humidification: 46,49, 146
    // Water Systems: 251, 176, 59
    // Refrigeration: 41, 171, 226
    // Generators: 140, 198, 63
    std::vector<vtkCharts::Color3ub> colors;

    colors.push_back(vtkCharts::Color3ub(237, 28, 36));
    colors.push_back(vtkCharts::Color3ub(0, 113, 188));
    colors.push_back(vtkCharts::Color3ub(244, 222, 17));
    colors.push_back(vtkCharts::Color3ub(216, 192, 18));
    colors.push_back(vtkCharts::Color3ub(77, 77, 77));
    colors.push_back(vtkCharts::Color3ub(179, 179, 179));
    colors.push_back(vtkCharts::Color3ub(255, 123, 172));
    colors.push_back(vtkCharts::Color3ub(102, 45, 145));
    colors.push_back(vtkCharts::Color3ub(241, 90, 36));
    colors.push_back(vtkCharts::Color3ub(46, 49, 146));
    colors.push_back(vtkCharts::Color3ub(200, 90, 36));
    colors.push_back(vtkCharts::Color3ub(251, 176, 59));
    colors.push_back(vtkCharts::Color3ub(41, 171, 226));
    colors.push_back(vtkCharts::Color3ub(140, 198, 63));

    return colors;
  }

  void ResultsComparisonTable::setRowHighlights()
  {
    const int numrows = openstudio::EndUseCategoryType::getValues().size();
    const int startingrow = 2;
    const int numcolumns = m_grid->columnCount();

    for (int row = 0; row < numrows; row += 2) 
    {

      for (int column = 0; column < numcolumns; ++column)
      {
        QLayoutItem *item = m_grid->itemAtPosition(row + startingrow, column);
        if (item)
        {
          QWidget *widget = item->widget();
          QLabel *label = dynamic_cast<QLabel *>(widget);
          if (label)
          {
            label->setBackgroundRole(QPalette::Dark);
            label->setForegroundRole(QPalette::Text);
            label->setAutoFillBackground(true);
          }
        }
      }
    }
  }

  openstudio::EndUseFuelType ResultsComparisonTable::getFuelType() const
  {
    return m_fuelType;
  }


  void ResultsComparisonTable::buildDataGrid()
  {
    std::map<int, std::string> types = openstudio::EndUseCategoryType::getDescriptions();
    std::map<int, std::string> months = openstudio::MonthOfYear::getNames();

    QFrame *tophline = new QFrame();
    tophline->setFrameStyle(QFrame::HLine | QFrame::Plain);
    m_grid->setHorizontalSpacing(0);
    m_grid->setVerticalSpacing(0);
    m_grid->addWidget(tophline, 0, 0, 1, 27);

    int row = 2;
    for (std::map<int, std::string>::const_iterator itr = types.begin();
        itr != types.end();
        ++itr, ++row)
    {
      m_grid->addWidget(new QLabel(openstudio::toQString(itr->second)), row, 0);
    }
    m_grid->addWidget(new QLabel("<b>Total</b>"), row, 0);


    // Column labels
    int column = 1;
    for (std::map<int, std::string>::const_iterator monthitr = months.begin();
        monthitr->first <= openstudio::MonthOfYear::Dec;
        ++monthitr, column += 2)
    {
      QFrame *vline = new QFrame();
      vline->setFrameStyle(QFrame::VLine | QFrame::Plain);
      m_grid->addWidget(vline, 1, column, types.size() + 1, 1);
      QLabel *monthname = new QLabel(openstudio::toQString("<b>" + monthitr->second + "</b>"));
      monthname->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      m_grid->addWidget(monthname, 1, column+1);
    }
    QFrame *vline = new QFrame();
    vline->setFrameStyle(QFrame::VLine | QFrame::Plain);
    m_grid->addWidget(vline, 1, column, types.size() + 1, 1);
    ++column;
    m_grid->addWidget(new QLabel("<b>Total</b>"), 1, column);


    // Row labels
    row = 2;
    for (std::map<int, std::string>::const_iterator itr = types.begin();
        itr != types.end();
        ++itr, ++row)
    {
      column = 2;
      for (std::map<int, std::string>::const_iterator monthitr = months.begin();
          monthitr->first <= openstudio::MonthOfYear::Dec;
          ++monthitr, column += 2)
      {
        QLabel *datalabel = createDataLabel(false);
        m_grid->addWidget(datalabel, row, column);
        m_labels[itr->first][monthitr->first] = datalabel;
      }
    }

    // Row totals labels
    row = 2;
    column = m_grid->columnCount() - 1;
    for (std::map<int, std::string>::const_iterator itr = types.begin();
        itr != types.end();
        ++itr, ++row)
    {
      QLabel *datalabel = createDataLabel(false);
      m_grid->addWidget(datalabel, row, column);
      m_categoryTotals[itr->first] = datalabel;
    }

    // Column totals labels
    row = m_grid->rowCount() - 1;
    column = 2;
    for (std::map<int, std::string>::const_iterator monthitr = months.begin();
        monthitr->first <= openstudio::MonthOfYear::Dec;
        ++monthitr, column += 2)
    {
      QLabel *datalabel = createDataLabel(true);
      m_grid->addWidget(datalabel, row, column);
      m_monthTotals[monthitr->first] = datalabel;
    }

    // total toal
    QLabel *totallabel = createDataLabel(true);
    m_grid->addWidget(totallabel, row, column);
    m_total = totallabel;

    setRowHighlights();
  }

  QLabel *ResultsComparisonTable::createDataLabel(bool t_bold)
  {
    QLabel *lbl = new QLabel();
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    if (t_bold)
    {
      QFont f;
      f.setWeight(QFont::Bold);
      lbl->setFont(f);
    }

    setDataValue(lbl, boost::optional<double>());
    return lbl;
  }

  ResultsComparisonTable::ResultsComparisonTable(const openstudio::EndUseFuelType &t_fuelType, 
      const openstudio::Unit &t_unit, QWidget *t_parent)
    : QWidget(t_parent), m_fuelType(t_fuelType), m_unit(t_unit), m_grid(new QGridLayout())
  {
    QVBoxLayout *vboxlayout = new QVBoxLayout();
    m_label = new QLabel();
    m_label->setObjectName("H2");
    vboxlayout->addWidget(m_label);

    updateUnitsLabel();

    buildDataGrid();

    vboxlayout->addLayout(m_grid);

    setLayout(vboxlayout);


  }


  void ResultsComparisonTable::updateUnitsLabel()
  {
    int numzeros = m_unit.scale().exponent;

    std::string scalestring;
    for (int i = 0; i < numzeros; ++i)
    {
      if (i % 3 == 0 && i != 0)
      {
        scalestring.insert(0, ",");
      }

      scalestring.insert(0, "0");
    }

    scalestring.insert(0, "x");
    std::string unitstring = m_unit.prettyString().empty()?m_unit.standardString():m_unit.prettyString();
    if(unitstring == "MBtu"){
      unitstring = "Million Btu";
    }
    //m_label->setText(openstudio::toQString(m_fuelType.valueDescription() + " Consumption (" + unitstring + " " + scalestring + ")"));
    m_label->setText(openstudio::toQString(m_fuelType.valueDescription() + " Consumption (" + unitstring + ")"));
  }

  void ResultsComparisonTable::setDataValue(QLabel *t_label, const boost::optional<double> &t_val)
  {
    if (t_val)
    {
      Quantity q(*t_val, *openstudio::UnitFactory::instance().createUnit("J"));
      t_label->setText(QString::number(QuantityConverter::instance().convert(q, m_unit)->value(), 'g', 4));
    } else {
      t_label->setText("<html>&#x2014;</html>");
    }
  }

  void ResultsComparisonTable::setDataMonthTotals(const ComparisonData &t_data)
  {

    boost::optional<double> total;

    int monthcount = 0;
    std::set<int> months = openstudio::MonthOfYear::getValues();
    for (std::set<int>::const_iterator monthitr = months.begin();
        monthitr != months.end() && monthcount < 12;
        ++monthitr, ++monthcount)
    {
      boost::optional<double> monthtotal;

      std::set<int> enduses = openstudio::EndUseCategoryType::getValues();

      for (std::set<int>::const_iterator itr = enduses.begin();
          itr != enduses.end();
          ++itr)
      {
        boost::optional<double> val = t_data.getValue(m_fuelType, *itr, *monthitr);
        if (val)
        {
          if (monthtotal)
          {
            *monthtotal += *val;
          } else {
            monthtotal = val;
          }
        }
      }

      setDataValue(m_monthTotals[*monthitr], monthtotal);

      if (monthtotal)
      {
        if (total)
        {
          *total+= *monthtotal;
        } else {
          total = monthtotal;
        }
      }
    }

    setDataValue(m_total, total);
  }

void ResultsComparisonTable::setDataCategoryTotals(const ComparisonData &t_data)
{
  std::set<int> enduses = openstudio::EndUseCategoryType::getValues();
  for (std::set<int>::const_iterator itr = enduses.begin();
      itr != enduses.end();
      ++itr)
  {
    boost::optional<double> categorytotal;

    int monthcount = 0;
    std::set<int> months = openstudio::MonthOfYear::getValues();
    for (std::set<int>::const_iterator monthitr = months.begin();
        monthitr != months.end() && monthcount < 12;
        ++monthitr, ++monthcount)
    {

      boost::optional<double> val = t_data.getValue(m_fuelType, *itr, *monthitr);
      if (val)
      {
        if (categorytotal)
        {
          *categorytotal+= *val;
        } else {
          categorytotal = val;
        }
      }
    }

    setDataValue(m_categoryTotals[*itr], categorytotal);
  }


}

void ResultsComparisonTable::setDataValues(const ComparisonData &t_data)
{
  std::set<int> enduses = openstudio::EndUseCategoryType::getValues();

  for (std::set<int>::const_iterator itr = enduses.begin();
       itr != enduses.end();
       ++itr)
  {
    std::set<int> months = openstudio::MonthOfYear::getValues();
    int monthcount = 0;
    for (std::set<int>::const_iterator monthitr = months.begin();
         monthitr != months.end() && monthcount < 12;
         ++monthitr, ++monthcount)
    {
      boost::optional<double> val = t_data.getValue(m_fuelType, *itr, *monthitr);
      setDataValue(m_labels[*itr][*monthitr], val);
    }
  }
}

void ResultsComparisonTable::setData(const ComparisonData &t_data, const openstudio::Unit &t_unit)
{
  m_unit = t_unit;
  updateUnitsLabel();
  setDataValues(t_data);
  setDataMonthTotals(t_data);
  setDataCategoryTotals(t_data);
}



ResultsTabView::ResultsTabView(const model::Model & model,
                               const QString & tabLabel,
                               bool hasSubTab,
                               QWidget * parent)
                               : MainTabView(tabLabel,hasSubTab,parent),
                                 m_resultsView(new ResultsView(model))
{
  QScrollArea *scrollarea = new QScrollArea();
  scrollarea->setWidget(m_resultsView);
  scrollarea->setWidgetResizable(true);
  addTabWidget(scrollarea);
  m_resultsView->setAutoFillBackground(false);
//    addTabWidget(m_resultsView);
}

void ResultsTabView::searchForExistingResults(const openstudio::path &t_runDir)
{
  LOG(Debug, "searchForExistingResults " << openstudio::toString(t_runDir));
  m_resultsView->searchForExistingResults(t_runDir);
}


void ResultsTabView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_resultsView->onUnitSystemChange(t_isIP);

}
void ResultsTabView::resultsGenerated(const openstudio::path &t_sqlFilePath, const openstudio::path &t_radianceResultsPath)
{
  LOG(Debug, "resultsGenerated " << openstudio::toString(t_sqlFilePath) << " " << openstudio::toString(t_radianceResultsPath));
  m_resultsView->resultsGenerated(t_sqlFilePath, t_radianceResultsPath);
}

openstudio::Unit ResultsView::getUnit(openstudio::EndUseFuelType t_type, bool t_isIP)
{
  if (t_type == openstudio::EndUseFuelType::Electricity)
  {
    return *openstudio::UnitFactory::instance().createUnit("kWh");
  } else if (t_type == openstudio::EndUseFuelType::Gas) {
    if (t_isIP)
    {
      return *openstudio::createUnit("MBtu", openstudio::UnitSystem::BTU);
    } else {
      return *openstudio::createUnit("GJ");
    }
  } else if (t_type == openstudio::EndUseFuelType::DistrictHeating) {
    if (t_isIP)
    {
      return *openstudio::createUnit("MBtu", openstudio::UnitSystem::BTU);
    } else {
      return *openstudio::createUnit("GJ");
    }
  } else if (t_type == openstudio::EndUseFuelType::DistrictCooling) {
    if (t_isIP)
    {
      return *openstudio::createUnit("MBtu", openstudio::UnitSystem::BTU);
    } else {
      return *openstudio::createUnit("GJ");
    }
  }

  throw std::runtime_error("Unknown unit type");
}



ResultsView::ResultsView(const model::Model & model, QWidget *t_parent)
  : QWidget(t_parent),
    m_model(model),
    m_isIP(true),
    m_electricConsumptionChart(new ResultsComparisonData(openstudio::EndUseFuelType::Electricity, 
          getUnit(openstudio::EndUseFuelType::Electricity, m_isIP))),
    m_gasConsumptionChart(new ResultsComparisonData(openstudio::EndUseFuelType::Gas, 
          getUnit(openstudio::EndUseFuelType::Gas, m_isIP))),
    m_consumptionLegend(new ResultsComparisonLegend()),
    m_electricConsumptionTable(new ResultsComparisonTable(openstudio::EndUseFuelType::Electricity, 
          getUnit(openstudio::EndUseFuelType::Electricity, m_isIP))),
    m_gasConsumptionTable(new ResultsComparisonTable(openstudio::EndUseFuelType::Gas, 
          getUnit(openstudio::EndUseFuelType::Gas, m_isIP))),
    m_districtHeatingConsumptionTable(new ResultsComparisonTable(openstudio::EndUseFuelType::DistrictHeating, 
          getUnit(openstudio::EndUseFuelType::DistrictHeating, m_isIP))),
    m_districtCoolingConsumptionTable(new ResultsComparisonTable(openstudio::EndUseFuelType::DistrictCooling, 
          getUnit(openstudio::EndUseFuelType::DistrictCooling, m_isIP))),
    m_openResultsViewerBtn(new QPushButton("Open ResultsViewer\nfor Detailed Reports"))
{
  QGridLayout *mainLayout = new QGridLayout();
  mainLayout->setContentsMargins(10,10,10,10);
  mainLayout->setSpacing(10);
  setLayout(mainLayout);



  mainLayout->addWidget(m_electricConsumptionChart, 0, 0, 2, 1);
  mainLayout->addWidget(m_gasConsumptionChart, 0, 1, 2, 1);
  mainLayout->addWidget(m_openResultsViewerBtn, 0, 2, 1, 1);
  mainLayout->addWidget(m_consumptionLegend, 1, 2, 1, 1);
  mainLayout->addWidget(m_electricConsumptionTable, 2, 0, 1, 2);
  mainLayout->addWidget(m_gasConsumptionTable, 3, 0, 1, 2);
  mainLayout->addWidget(m_districtHeatingConsumptionTable, 4, 0, 1, 2);
  mainLayout->addWidget(m_districtCoolingConsumptionTable, 5, 0, 1, 2);

  connect(m_openResultsViewerBtn, SIGNAL(clicked()),
      this, SLOT(openResultsViewerClicked()));
}

void ResultsView::openResultsViewerClicked()
{
  LOG(Debug, "openResultsViewerClicked");

#ifdef Q_OS_MAC
  openstudio::path resultsviewer
    = openstudio::getApplicationRunDirectory() / openstudio::toPath("../../../ResultsViewer.app/Contents/MacOS/ResultsViewer");
#else
  openstudio::path resultsviewer
    = openstudio::getApplicationRunDirectory() / openstudio::toPath("ResultsViewer");
#endif

  QStringList args;

  // instruct ResultsViewer to make its own copies of the sql files passed in and to clean them up
  // when done
  args.push_back("--maketempcopies"); 

  if (!m_sqlFilePath.empty())
  {
    args.push_back(openstudio::toQString(m_sqlFilePath));
  }

  if (!m_radianceResultsPath.empty())
  {
    args.push_back(openstudio::toQString(m_radianceResultsPath));
  }


  if (!QProcess::startDetached(openstudio::toQString(resultsviewer), args))
  {
    QMessageBox::critical(this, "Unable to launch ResultsViewer", "ResultsViewer was not found in the expected location:\n" + openstudio::toQString(resultsviewer));
  }
}

void ResultsView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_isIP = t_isIP;
  resultsGenerated(m_sqlFilePath, m_radianceResultsPath);

}

void ResultsView::searchForExistingResults(const openstudio::path &t_runDir)
{
  LOG(Debug, "Looking for existing results in: " << openstudio::toString(t_runDir));

  std::vector<openstudio::path> eplusout;
  std::vector<openstudio::path> radout;

  for ( boost::filesystem::basic_recursive_directory_iterator<openstudio::path> end, dir(t_runDir); 
        dir != end; 
        ++dir ) 
  {
    openstudio::path p = *dir;
    if (openstudio::toString(p.filename()) == "eplusout.sql")
    {
      eplusout.push_back(p);
    } else if (openstudio::toString(p.filename()) == "radout.sql") {
      radout.push_back(p);
    }
  }

  openstudio::path eplus = eplusout.empty()?openstudio::path():eplusout.back();
  openstudio::path rad = radout.empty()?openstudio::path():radout.back();

  resultsGenerated(eplus, rad);
}

void ResultsView::resultsGenerated(const openstudio::path &t_path, const openstudio::path &t_radianceResultsPath)
{
  LOG(Debug, "resultsGenerated " << openstudio::toString(t_path) << " " << openstudio::toString(t_radianceResultsPath));

  std::vector<openstudio::EndUseFuelType> fueltypes;
  fueltypes.push_back(openstudio::EndUseFuelType::Gas);
  fueltypes.push_back(openstudio::EndUseFuelType::Electricity);
  fueltypes.push_back(openstudio::EndUseFuelType::DistrictHeating);
  fueltypes.push_back(openstudio::EndUseFuelType::DistrictCooling);


  m_sqlFilePath = t_path;
  m_radianceResultsPath = t_radianceResultsPath;

  ComparisonData data(fueltypes,
      SqlFile(t_path));


  m_electricConsumptionChart->setData(data, getUnit(m_electricConsumptionChart->getFuelType(), m_isIP));
  m_gasConsumptionChart->setData(data, getUnit(m_gasConsumptionChart->getFuelType(), m_isIP));
  m_electricConsumptionTable->setData(data, getUnit(m_electricConsumptionTable->getFuelType(), m_isIP)); 
  m_gasConsumptionTable->setData(data, getUnit(m_gasConsumptionTable->getFuelType(), m_isIP)); 

  m_districtHeatingConsumptionTable->setData(data, getUnit(m_districtHeatingConsumptionTable->getFuelType(), m_isIP));
  m_districtCoolingConsumptionTable->setData(data, getUnit(m_districtCoolingConsumptionTable->getFuelType(), m_isIP));

}


} // openstudio

