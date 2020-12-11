import openstudio
# import openstudio_dynamic as openstudio

# workspace test
workspace = openstudio.Workspace()
zone = workspace.addObject(
    openstudio.IdfObject(openstudio.IddObjectType("Zone"))
)
zone = zone.get()
zone.setName("New Zone")

for obj in workspace.objects():
    print(obj)

# model test
model = openstudio.model.Model()
space = openstudio.model.Space(model)
space.setName("New Space")

for s in openstudio.model.getSpaces(model):
    print(s)
