m = OpenStudio::Model::Model.new
m.getSite
m.getSiteWaterMainsTemperature
m.getWeatherFile

# puts m

remote = OpenStudio::RemoteBCL.new
# remote.downloadComponent('f38beff0-edc6-0131-6605-48e0eb16a403')
# remote.downloadComponent('318c49f0-edc7-0131-6dac-48e0eb16a403')
# remote.downloadComponent('d3ac66a0-acec-012f-0c6a-00ff10b04504')
# component = remote.waitForComponentDownload()
# component = component.get
# osc_path = component.files("osc")[0]
osc_path = 'C:\Users\jrobert1\BCL\d3ac66a0-acec-012f-0c6a-00ff10b04504\328940ab-bb98-4fab-a28a-c39284962d3b\files\watermains.osc'
vt = OpenStudio::OSVersion::VersionTranslator.new
component_object = vt.loadComponent(OpenStudio::Path.new(osc_path))
componentData = m.insertComponent(component_object.get)

puts m