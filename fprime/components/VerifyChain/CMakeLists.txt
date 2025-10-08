# FILE: fprime/components/VerifyChain/CMakeLists.txt
add_library(VerifyChain STATIC VerifyChainComponent.cpp)
target_include_directories(VerifyChain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ../../reuse/libverify)
