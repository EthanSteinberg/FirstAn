set(HDRS 
      )
 
set(SRCS
      main.cpp
)
 
add_executable(FirstAn ${HDRS} ${SRCS})
target_link_libraries(FirstAn ${SFML_LIBRARY} util)

add_subdirectory(util)
