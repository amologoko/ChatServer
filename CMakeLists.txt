CMAKE_MINIMUM_REQUIRED(VERSION 3.11)
PROJECT(ChatServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_MACOSX_RPATH ON) 

INCLUDE(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

if(MSVC)
  if(MSVC_VERSION AND MSVC_VERSION GREATER_EQUAL 1910)
    add_definitions(-DGTEST_LANG_CXX11=1 -DGTEST_HAS_TR1_TUPLE=0)
  endif(MSVC_VERSION AND MSVC_VERSION GREATER_EQUAL 1910)
endif(MSVC)

# Create library #####################################################
ADD_LIBRARY(${CMAKE_PROJECT_NAME} ProcessOptions.cpp PersistentStore.cpp ChatServer.cpp ChatSession.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CONAN_LIBS})

# Install library ####################################################
INSTALL(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION lib)

# Create application tests ######################################################
ADD_EXECUTABLE(test_${PROJECT_NAME} main.cpp)
TARGET_LINK_LIBRARIES(test_${PROJECT_NAME} ${CONAN_LIBS} ${CMAKE_PROJECT_NAME})

# Create application tests ######################################################
ADD_EXECUTABLE(test_persistent_storage test_persistent_storage.cpp)
TARGET_LINK_LIBRARIES(test_persistent_storage ${CONAN_LIBS} ${CMAKE_PROJECT_NAME})

# Include tests #######################################################
ENABLE_TESTING()
ADD_TEST(NAME PersistentStore
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
         COMMAND test_persistent_storage)
