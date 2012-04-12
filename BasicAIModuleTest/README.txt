Google mock libraries are required to be able to run the BasicAIModuleTest project.

The libraries are different depending on whether the system is windows xp 32-bit or windows 7 64-bit.

Both sets of libaries are included under folders in ./lib

Please copy the appropriate folders (MD and MDd) from either the ./lib/winxp-32bit/ or ./lib/win7-64bit into the ./lib folder.

The directory structure when this process is completed should look like this (excluding the win*-*bit folders):

BasicAIModuleTest/lib/MD/gmock.lib
BasicAIModuleTest/lib/MD/gmock_main.lib
BasicAIModuleTest/lib/MDd/gmock.lib
BasicAIModuleTest/lib/MDd/gmock_main.lib

Once the libs for the correct platform are in place, the BasicAIModuleTest project should be able to be built and run in either release or debug modes.
