set(HDRS 
   util.h
   matrix.h
   glStuff.h
   text.h
      )
 
set(SRCS
   util.cpp
   matrix.cpp
   glStuff.cpp
   text.cpp
   myVector2.cpp
   algoMaxRects.cpp
   myBox.cpp
)
 
add_library(util ${HDRS} ${SRCS})
target_link_libraries(util ${GLEW_LIBRARY} ${FREETYPE_LIBRARIES})
