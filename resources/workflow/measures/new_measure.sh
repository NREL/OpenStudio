$os_build/Products/openstudio measure new \
  --class-name OutdatedReportingMeasure \
  --type ReportingMeasure \
  --language Ruby \
  --name "Outdated Reporting Measure" \
  --description "ReportingMeasure is an old ReportingUserScript" \
  --modeler-description "The ReportingMeasure Measure derives the deprecated ReportingUserScript class, and arguments doesn't take model" \
  --taxonomy-tag "Envelope.Form" ./OutdatedReportingMeasure
