import glob, shutil, os

# uncomment line below to see environment variables
# print env.Dump()
# print ARGUMENTS

if not os.path.exists("pio_compile_here/WiFiConnector"):
    os.makedirs("pio_compile_here/WiFiConnector")

for file in glob.iglob('src/*.*'):
    print 'Copied %s' % (file)
    shutil.copy2(file, "pio_compile_here/WiFiConnector")
