include(ECMAddTests)

find_package(Qt5Test REQUIRED NO_MODULE)

macro(add_unit_test testname testsrc)
    set(${testname}_SRCS ${testsrc})
    add_executable(${testname} ${${testname}_SRCS})
    add_test(${testname} ${testname})
    ecm_mark_as_test(${testname})
    target_link_libraries(
        ${testname}
        Qt5::Test
    )
endmacro(add_unit_test)

add_unit_test(test_timeformat testtimeformat.cpp)
