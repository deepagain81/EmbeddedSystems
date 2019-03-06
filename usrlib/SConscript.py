# This file contains all that a SConstruct file normally contains.
# In order to use Doxygen, however, all Python files must end with
# a .py extension. So, SConstuct simply executes this file.

# Documentation for this file. If the \file tag isn't present,
# this file won't be documented.
## \file
#  This file supports ESOS builds.

## Common sources used for the PIC24 support library
ESOSSupportLibSources = [ 
  'src/esos_f14ui.c',
  'src/esos_sensor.c',
  'src/esos_pic24_sensor.c',
  'src/esos_lcd44780.c',
  'src/esos_pic24_lcd44780.c']

env = env.Clone(CPPDEFINES=['BUILT_ON_ESOS' '_NOASSERT'])
