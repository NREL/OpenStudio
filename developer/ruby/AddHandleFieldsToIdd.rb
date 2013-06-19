require 'openstudio'

os_idd = OpenStudio::IddFileAndFactoryWrapper.new("OpenStudio".to_IddFileType).iddFile

os_idd.objects.each { |object|
  object.insertHandleField
}

os_idd.save(OpenStudio::Path.new("/home/ehale/Desktop/OpenStudio.idd"),true)

