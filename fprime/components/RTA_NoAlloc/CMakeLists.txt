# FILE: fprime/components/RTA_NoAlloc/CMakeLists.txt
add_library(RTA_NoAlloc STATIC RTA_NoAllocComponent.cpp)
target_include_directories(RTA_NoAlloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ../../reuse/runtime/c_a653)
