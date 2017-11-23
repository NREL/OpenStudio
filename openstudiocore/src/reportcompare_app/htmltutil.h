#ifndef HTMLTUTIL_H
#define HTMLTUTIL_H

#include <QString>
#include <QColor>
#include <QtWebKit/QWebElement>

bool peakFloat(const QString& str, float& val);
QString doHtmlTextColor(const QString& str, const QColor& color);
bool nextTableWithTitle(QString &title, QWebElement &table);
QString doCompareNumberText(const QString& src, const QString& compare);

#endif // HTMLTUTIL_H
