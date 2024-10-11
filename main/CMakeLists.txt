set(SOURCES main.cpp cppgpio.cpp)
idf_component_register(SRCS ${SOURCES}
INCLUDE_DIRS . ../include)