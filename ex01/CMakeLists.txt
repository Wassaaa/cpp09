set(MAIN_SOURCES
    main.cpp
    RPN.cpp
)

file(GLOB SOURCES ${MAIN_SOURCES})

add_executable(RPN ${SOURCES})

add_executable(test_RPN test_RPN.cpp RPN.cpp)
enable_testing()
add_test(NAME RPNTests COMMAND test_RPN)
