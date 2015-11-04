require 'openstudio'

report_vector = OpenStudio::AttributeVector.new
report_vector << OpenStudio::Attribute.new("complete", true)
report_vector << OpenStudio::Attribute.new("path", "#{File.expand_path(__FILE__)}")

report = OpenStudio::Attribute.new("Report", report_vector)
report.saveToXml(OpenStudio::Path.new("report.xml"))
