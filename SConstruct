target = 'libGETDecoder'
incdir = 'include'
srcdir = 'src'

# Don't edit under this line
import os
dictName = target + 'Dict.cc'
headers = Glob(incdir + '/*.hh')
dictHeaders = ''
for header in headers:
  if str(header) != incdir + '/GETConfig.hh':
    dictHeaders += str(header) + ' '

linkdefName = target + 'LinkDef.hh'
linkdefFile = open(linkdefName, 'w')
linkdefFile.write('#ifdef __CINT__\n\n')
linkdefFile.write('#pragma link off all globals;\n')
linkdefFile.write('#pragma link off all classes;\n')
linkdefFile.write('#pragma link off all functions;\n\n')
for header in headers:
  if str(header) != incdir + '/GETConfig.hh':
    className = str(header).replace(incdir + '/', '').replace('.hh', '')
    linkdefFile.write('#pragma link C++ class ' + className + '+;\n')
linkdefFile.write('\n#endif')
linkdefFile.close()

rootcint = Builder(action = 'rootcint -f $TARGET -c ' + dictHeaders + '$SOURCE');
dictEnv = Environment(ENV = {'PATH' : os.environ['PATH']})
dictEnv.Append(BUILDERS = {'Dict' : rootcint})
linkdef = dictEnv.Dict(dictName, linkdefName)
Clean(linkdef, linkdefName)
Clean(linkdef, dictName.replace('.cc', '.h'))

env = Environment(ENV = {'PATH' : os.environ['PATH']})
env['CPPPATH'] = './include'
env.ParseConfig('root-config --cflags')
env.ParseConfig('root-config --libs')
env.SharedLibrary(target, Glob(srcdir + '/*.cc') + [dictName])
