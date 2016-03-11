require 'openstudio'

number_of_models = 0

Dir.glob('./*.osm').each do |p|
  model_path = OpenStudio::Path.new("in.osm")
  idf_file = OpenStudio::IdfFile::load(model_path)
  model = OpenStudio::Model::Model.new(idf_file.get)
  number_of_models += 1
end

report_vector = OpenStudio::AttributeVector.new
report_vector << OpenStudio::Attribute.new("numberOfModels", number_of_models)

report = OpenStudio::Attribute.new("Report", report_vector)

File.open("report.xml", 'w') do |f| 
  f.puts report.to_s
end
