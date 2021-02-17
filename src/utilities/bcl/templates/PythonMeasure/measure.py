import openstudio
import os

#path = openstudio.path( os.path.join(os.getcwd(), "in.osm") )
translator = openstudio.osversion.VersionTranslator()
#model = translator.loadModel(path)
#print(model.get())
openstudio.openStudioLongVersion()
print(openstudio.openStudioLongVersion())