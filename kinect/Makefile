#------------------------------------------------------#
#			Configuration
#------------------------------------------------------#


#Output filename
NAME= kinect


#files
SOURCES := $(wildcard *.cpp) 
OBJS := $(patsubst %.cpp, %.o, $(SOURCES)) 
DEPS := $(wildcard include/*.h) 

#compiler-type things
CXXFLAGS=-Iinclude -O3 -I/usr/include/nite -I/usr/include/ni -DUSE_GLUT
LDFLAGS=-lGL -lGLU -lOpenNI -lXnVNite_1_3_1 -lglut

#------------------------------------------------------#
#					Compile
#------------------------------------------------------#
all: $(OBJS) $(NAME)
  
$(NAME): $(OBJS) 
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)
	
#------------------------------------------------------#
#					Clean
#------------------------------------------------------#
clean:
	rm -f *.o
	rm -f $(NAME)

