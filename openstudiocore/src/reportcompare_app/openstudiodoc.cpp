#include <QDebug>
#include <QCoreApplication>
#include <QMessageBox>
#include <QProgressDialog>

#include "openstudiodoc.h"
#include "opetableelement.h"
#include "opsgraphelement.h"
#include "htmltutil.h"
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>

OpenStudioDoc::OpenStudioDoc(const QString &projectName, QWebView *webView)
    :IDoc(projectName, webView)
{
    QWebElement body = getBody(webView);
    QWebElement elmit = body.firstChild();

    //QWebElementCollection graphs = elmit.findAll("SVG");
    //For electricity consumption.
    //find class "x axis" and child class "tick"
    //append 2 line with
    //<svg:tspan>line1
    QString js = "var table = document.getElementsByTagName(\"table\");";
    js += "var i;";
    js += "for (i = 0; i < table.length; i++) {";
    js += "    table[i].style.maxWidth = \"50000px\";";
    js += "}";

    webView->page()->mainFrame()->evaluateJavaScript(js);

    QWebElement graph1 = elmit.findFirst(".span5");
    graph1.removeClass("span5");

    QWebElementCollection graphs = elmit.findAll("SVG");
    if(graphs.count()==2){
        QSharedPointer<OpsGraphElement> gelm = QSharedPointer<OpsGraphElement>
                (new OpsGraphElement(this, "Electricity Consumption Graph"
                                     , graphs.at(0)));

        elements.append(gelm);

        //float svgw = gelm->svgWidth();

        gelm = QSharedPointer<OpsGraphElement>
                (new OpsGraphElement(this, "Natural Gas Consumption Graph"
                                     , graphs[1]));

        gelm->setPosXY(40.5f-39.776f, 20.5f);

        elements.append(gelm);
    }
    else{
        qWarning("CAN'T FIND SVG TAG ELEMENT.");
    }

    QString title;
    bool isnext1 = nextTableWithTitle(title, elmit);
    while(isnext1){
        qDebug()  << "Create At title : " << title;
        QSharedPointer<OPETableElement> telm = QSharedPointer<OPETableElement>
                (new OPETableElement(this, title, elmit));
        elements.append(telm);
        elmit = elmit.nextSibling();
        isnext1 = nextTableWithTitle(title, elmit);
    }
}

OpenStudioDoc::~OpenStudioDoc()
{

}

void OpenStudioDoc::doCmp(const QString &projectName, QWebView *webView2)
{
    QString error;
    resetTables();

    QSharedPointer<OpenStudioDoc> other = QSharedPointer<OpenStudioDoc>(new OpenStudioDoc(projectName, webView2));
    QProgressDialog progress("Doing compare Open Studio.", "Abort", 0, (int)other->elementCount(), NULL);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    QCoreApplication::processEvents();

    int i=0;
    QList<QSharedPointer<ICompareElement> >::iterator eit = other->getElements().begin();
    while(eit!=other->getElements().end()){
        ICompareElement* ce = (*eit).data();
        qDebug()  << "Create At title : " << ce->Id();
        ICompareElement* cmpelm = find(ce->Id());
        if(cmpelm){
            if(cmpelm->Compare(ce)){
                cmpelm->Update();
                cmpelm->setUse(true);
            }
            else{
                error += cmpelm->getError()+"\n";
                cmpelm->clearError();
            }
        }
        progress.setValue(i++);
        eit++;
        QCoreApplication::processEvents();
    }
    progress.setValue((int)other->elementCount());
    QCoreApplication::processEvents();
    if(!error.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText(error);
        msgBox.exec();
    }
}

void OpenStudioDoc::drawCmpGraph()
{
    QString js;
    js+=

"var margin = {\
        top: 20.5,\
        right: 20,\
        bottom: 30,\
        left: 40.5\
    },\
    width = 560 - margin.left - margin.right,\
        height = 300 - margin.top - margin.bottom;\
\
    var x = d3.scale.ordinal()\
        .rangeRoundBands([0, width], 0.1);\
\
    var y = d3.scale.linear()\
        .rangeRound([height, 0]);\
\
    var color = d3.scale.ordinal()\
        .range([\"#EF1C21\", \"#0071BD\", \"#F7DF10\", \"#DEC310\", \"#4A4D4A\", \"#B5B2B5\", \"#FF79AD\", \"#632C94\", \"#F75921\", \"#293094\", \"#CE5921\", \"#FFB239\", \"#29AAE7\", \"#8CC739\"]);\
\
    var xAxis = d3.svg.axis()\
        .scale(x)\
        .orient(\"bottom\");\
\
    var yAxis = d3.svg.axis()\
        .scale(y)\
        .orient(\"left\")\
        .tickFormat(d3.format(\".2s\"));\
\
    var tip = d3.tip()\
        .attr('class', 'd3-tip')\
        .offset([-10, 0])\
        .html(function (d) {\
        return \"<strong>\" + d.name + \":</strong> <span style='color:red'>\" + (d.y1 - d.y0).toFixed(2) + \"</span>\";\
    });\
\
    var charts = [\"Electricity Consumption\", \"Natural Gas Consumption\"];\
\
    for (var c in charts) {\
\
        if (c == charts.length - 1) {\
            margin.right = 125;\
        }\
\
        $('#title-' + c).text(charts[c]);\
\
        var svg = d3.select(\"#chart-\" + c).append(\"svg\")\
            .attr(\"width\", width + margin.left + margin.right)\
            .attr(\"height\", height + margin.top + margin.bottom)\
            .append(\"g\")\
            .attr(\"transform\", \"translate(\" + margin.left + \",\" + margin.top + \")\");\
        svg.call(tip);\
\
        data = [];\
        for (var enduse in consumption[charts[c]].data) {\
            var enduse_data = consumption[charts[c]].data[enduse];\
            for (var month = 0; month < months.length; month++) {\
                if (data.length < months.length) {\
                    var temp = {\
                        \"Month\": months[month]\
                    };\
                    temp[enduse] = enduse_data[month];\
                    data.push(temp);\
                } else {\
                    data[month][enduse] = enduse_data[month];\
                }\
            }\
        }\
        color.domain(d3.keys(data[0]).filter(function (key) {\
            return key !== \"Month\";\
        }));\
\
        data.forEach(function (d) {\
            var y0 = 0;\
            d.consumption = color.domain().map(function (name) {\
                return {\
                    name: name,\
                    y0: y0,\
                    y1: y0 += +d[name]\
                };\
            });\
            d.total = d.consumption[d.consumption.length - 1].y1;\
        });\
\
        //data.sort(function(a, b) { return b.total - a.total; });\
\
        x.domain(data.map(function (d) {\
            return d.Month;\
        }));\
        y.domain([0, d3.max(data, function (d) {\
            return d.total;\
        })]);\
\
        svg.append(\"g\")\
            .attr(\"class\", \"x axis\")\
            .attr(\"transform\", \"translate(0,\" + height + \")\")\
            .call(xAxis);\
\
        svg.append(\"g\")\
            .attr(\"class\", \"y axis\")\
            .call(yAxis)\
            .append(\"text\")\
            .attr(\"transform\", \"rotate(-90)\")\
            .attr(\"y\", -36)\
            .attr(\"dy\", \".71em\")\
            .style(\"text-anchor\", \"end\")\
            .text(consumption[charts[c]].units);\
\
        var month = svg.selectAll(\".month\")\
            .data(data)\
            .enter().append(\"g\")\
            .attr(\"class\", \"g\")\
            .attr(\"transform\", function (d) {\
            return \"translate(\" + x(d.Month) + \",0)\";\
        });\
\
        month.selectAll(\"rect\")\
            .data(function (d) {\
            return d.consumption;\
        })\
            .enter().append(\"rect\")\
            .attr(\"width\", x.rangeBand())\
            .attr(\"y\", function (d) {\
            return y(d.y1);\
        })\
            .attr(\"height\", function (d) {\
            return y(d.y0) - y(d.y1);\
        })\
            .style(\"fill\", function (d) {\
            return color(d.name);\
        })\
            .on('mouseover', tip.show)\
            .on('mouseout', tip.hide);\
\
        if (c == charts.length - 1) {\
            var legend = svg.selectAll(\".legend\")\
                .data(color.domain().slice())\
                .enter().append(\"g\")\
                .attr(\"class\", \"legend\")\
                .attr(\"transform\", function (d, i) {\
                return \"translate(0,\" + i * 20 + \")\";\
            });\
\
            legend.append(\"rect\")\
                .attr(\"x\", width + 105)\
                .attr(\"width\", 18)\
                .attr(\"height\", 18)\
                .style(\"fill\", color);\
\
            legend.append(\"text\")\
                .attr(\"x\", width + 105 - 3)\
                .attr(\"y\", 9)\
                .attr(\"dy\", \".35em\")\
                .style(\"text-anchor\", \"end\")\
                .text(function (d) {\
                return d;\
            });\
        }\
\
        svg.selectAll(\".g\").attr(\"stroke\", \"black\")\
            .attr(\"stroke-width\", \"1\");\
                   }";

}


QString OpenStudioDoc::getMonthsVariable()
{
    //QString str
    return "";
}

QString OpenStudioDoc::genDataGraph()
{

//var months = [
//    "Jan",
//    "Feb",
//    "Mar",
//    "Apr",
//    "May",
//    "Jun",
//    "Jul",
//    "Aug",
//    "Sep",
//    "Oct",
//    "Nov",
//    "Dec"];

//var consumption = {
//	"Electricity Consumption":{
//		"units":"kWh",
//		"data":{
//			"Heating":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Cooling":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Lighting":[639.756,583.736,663.528,596.681,663.528,636.931,623.278,663.528,620.453,639.756,636.931,623.278],
//			"Exterior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Equipment":[383.186,347.639,390.639,362.878,390.639,376.306,377.214,390.639,370.333,383.186,376.306,377.214],
//			"Exterior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Fans":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Pumps":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Rejection":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Humidification":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Recovery":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Water Systems":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Refrigeration":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Generators":[0,0,0,0,0,0,0,0,0,0,0,0]
//		}
//	},
//	"Natural Gas Consumption":{
//		"units":"Million Btu",
//		"data":{
//			"Heating":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Cooling":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Fans":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Pumps":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Rejection":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Humidification":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Recovery":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Water Systems":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Refrigeration":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Generators":[0,0,0,0,0,0,0,0,0,0,0,0]
//		}
//	},
//	"Additional Fuel Consumption":{
//		"units":"Million Btu",
//		"data":{
//			"Heating":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Cooling":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Fans":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Pumps":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Rejection":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Humidification":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Recovery":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Water Systems":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Refrigeration":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Generators":[0,0,0,0,0,0,0,0,0,0,0,0]
//		}
//	},
//	"District Cooling Consumption":{
//		"units":"Million Btu",
//		"data":{
//			"Heating":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Cooling":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Fans":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Pumps":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Rejection":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Humidification":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Recovery":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Water Systems":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Refrigeration":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Generators":[0,0,0,0,0,0,0,0,0,0,0,0]
//		}
//	},
//	"District Heating Consumption":{
//		"units":"Million Btu",
//		"data":{
//			"Heating":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Cooling":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Fans":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Pumps":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Rejection":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Humidification":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Recovery":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Water Systems":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Refrigeration":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Generators":[0,0,0,0,0,0,0,0,0,0,0,0]
//		}
//	},
//	"Water Consumption":{
//		"units":"Million Btu",
//		"data":{
//			"Heating":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Cooling":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Lighting":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Interior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Exterior Equipment":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Fans":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Pumps":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Rejection":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Humidification":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Heat Recovery":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Water Systems":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Refrigeration":[0,0,0,0,0,0,0,0,0,0,0,0],
//			"Generators":[0,0,0,0,0,0,0,0,0,0,0,0]
//		}
//	}
//};
    return QString();
}

