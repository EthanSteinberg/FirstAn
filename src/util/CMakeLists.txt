set(HDRS 
   util.h
   matrix.h
   glStuff.h
      )
 
set(SRCS
   util.cpp
   matrix.cpp
   glStuff.cpp
)
 
add_library(util ${HDRS} ${SRCS})
target_link_libraries(util ${GLEW_LIBRARY})
