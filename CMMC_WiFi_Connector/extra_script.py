from SCons.Script import DefaultEnvironment
import glob, shutil, os

env = DefaultEnvironment()
# uncomment line below to see environment variables
# print env.Dump()
# print ARGUMENTS

# copyfile("src/WiFiConnector.h", "tmp/WiFiConnector/WiFiConnector.h")
# copyfile("src/WiFiConnector.cpp", "tmp/WiFiConnector/WiFiConnector.cpp")

if not os.path.exists("pio_compile_here/WiFiConnector"):
    os.makedirs("pio_compile_here/WiFiConnector")

for file in glob.iglob('src/*.*'):
    print 'Copied file %s' % (file)
    shutil.copy2(file, "pio_compile_here/WiFiConnector")
