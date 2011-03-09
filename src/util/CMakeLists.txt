set(HDRS 
   util.h
      )
 
set(SRCS
   util.cpp
)
 
add_library(util ${HDRS} ${SRCS})
target_link_libraries(util ${GLEW_LIBRARY})
