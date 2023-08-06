# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
set(jni_lib_dir ${JNI_LIB_PATH})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

  file(GLOB ACCUM "src/main/cpp/Accum/*.c")
  file(GLOB APTRANS "src/main/cpp/Aptrans/*.c")
  file(GLOB BATCH "src/main/cpp/Batch/*.c")
  file(GLOB CJSON "src/main/cpp/cJSON/*.c")
  file(GLOB COMM "src/main/cpp/Comm/*.c")
  file(GLOB CTLS "src/main/cpp/Ctls/*.c")
  file(GLOB CTOSAPI "src/main/cpp/CtosApi/*.c")
  file(GLOB DATABASE "src/main/cpp/Database/*.c")
  file(GLOB DEBUG "src/main/cpp/Debug/*.c")
  file(GLOB DES "src/main/cpp/Des/*.c")
  file(GLOB ERM "src/main/cpp/Erm/*.c")
  file(GLOB EXTERNAL "src/main/cpp/External/*.c")
  file(GLOB FELICA "src/main/cpp/FeliCa/*.c")
  file(GLOB FILEMODULE "src/main/cpp/FileModule/*.c")
  file(GLOB FTPS "src/main/cpp/Ftps/*.c")
  file(GLOB FUNCTIONSLIST "src/main/cpp/Functionslist/*.c")
  file(GLOB HTLE "src/main/cpp/Htle/*.c")
  file(GLOB ISO8583 "src/main/cpp/Iso8583/*.c")
  file(GLOB MAIN "src/main/cpp/Main/*.c")
  file(GLOB MD5 "src/main/cpp/Md5/*.cpp")
  file(GLOB MIFARE "src/main/cpp/mifare/*.c")
  file(GLOB PCI100 "src/main/cpp/PCI100/*.c")
  file(GLOB PINPAD "src/main/cpp/PinPad/*.c")
  file(GLOB POWERFAIL "src/main/cpp/Powrfail/*.c")
  file(GLOB PRINT "src/main/cpp/Print/*.c")
  file(GLOB SAM "src/main/cpp/Sam/*.c")
  file(GLOB SETTING "src/main/cpp/Setting/*.c")
  file(GLOB TCPSERVER "src/main/cpp/TcpServer/*.c")
  file(GLOB TMS "src/main/cpp/TMS/*.c")
  file(GLOB TMSDYNMENU "src/main/cpp/TMSDYNMenu/*.c")
  file(GLOB TOTAL "src/main/cpp/Total/*.c")
  file(GLOB TRANS "src/main/cpp/Trans/*.c")
  file(GLOB UI "src/main/cpp/Ui/*.c")
  file(GLOB UTILS "src/main/cpp/Utils/*.c")
  file(GLOB WALLET "src/main/cpp/DigitalWallet/*.c")
  file(GLOB MD5 "src/main/cpp/Md5/*.cpp")


  file(GLOB JNIUI "src/main/cpp/*.c")

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
             ${ACCUM}
             ${APTRANS}
             ${BATCH}
             ${CJSON}
             ${COMM}
             ${CTLS}
             ${CTOSAPI}
             ${DATABASE}
             ${DEBUG}
             ${DES}
             ${ERM}
             ${EXTERNAL}
             ${FELICA}
             ${FILEMODULE}
             ${FTPS}
             ${FUNCTIONSLIST}
             ${HTLE}
             ${ISO8583}
             ${MAIN}
             ${MD5}
             ${MIFARE}
             ${PCI100}
             ${PINPAD}
             ${POWERFAIL}
             ${PRINT}
             ${SAM}
             ${SETTING}
             ${TCPSERVER}
             ${TMS}
             ${TMSDYNMENU}
             ${TOTAL}
             ${TRANS}
             ${UI}
             ${UTILS}
             ${WALLET}
             ${MD5}


             ${JNIUI}
	     )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

include_directories(${PROJECT_SOURCE_DIR}/src/main/cpp/CtosApi/)

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

#��̬��ʽ����
add_library(openssl SHARED IMPORTED )
add_library(ssl SHARED IMPORTED )
add_library(curl SHARED IMPORTED )
#���������.so��
set_target_properties(openssl PROPERTIES IMPORTED_LOCATION ${jni_lib_dir}/libcrypto.so)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${jni_lib_dir}/libssl.so)
set_target_properties(curl PROPERTIES IMPORTED_LOCATION ${jni_lib_dir}/libcurl.so)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       ${jni_lib_dir}/libctosapi.so
                       ${jni_lib_dir}/libcfgexpress.so
                       ${jni_lib_dir}/libsqlite3.so
                       ${jni_lib_dir}/libISOEnginee.so
                        ${jni_lib_dir}/libPBM.so
                       ${jni_lib_dir}/libEPADSO.so

                       ${jni_lib_dir}/libcabarcode.so
                       ${jni_lib_dir}/libcacl.so
                       ${jni_lib_dir}/libcaclmdl.so
                       ${jni_lib_dir}/libcaemvl2ap.so
                       ${jni_lib_dir}/libcafont.so
                       ${jni_lib_dir}/libcafs.so
                       ${jni_lib_dir}/libcainterface.so
                       ${jni_lib_dir}/libcamsr.so
                       ${jni_lib_dir}/libcaprt.so
                       ${jni_lib_dir}/libcartc.so
                       ${jni_lib_dir}/libcasc.so
                       ${jni_lib_dir}/libcasystem.so
                       ${jni_lib_dir}/libcrypto.so
                       ${jni_lib_dir}/libsbkms.so
                       ${jni_lib_dir}/libsbcrypto.so
                       ${jni_lib_dir}/libcustkms.so
                       ${jni_lib_dir}/libcaqrcode.so
                       ${jni_lib_dir}/libcars232.so

                       openssl
                       ssl
                       curl
                       ${ZLIB_LIBRARIES}
                       )
