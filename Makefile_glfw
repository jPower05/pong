CPPFLAGS = -c -pthread -g -O2 -Wall -Wextra

LDLIBS   = -lXrandr -lrt -lX11 -lGLU -lGL -lm -lglfw -lglut
LFLAGS   = -pthread 

TARGET 	= pong
SRC 	= pong.cpp pong_psp.cpp pong_glfw.cpp
OBJS	= $(subst .cpp,.o,$(SRC))

default: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o *~ *.bak deps.mak

$(TARGET): $(OBJS)
	g++ $(LFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET) 

-include deps.mak

deps deps.mak:
	g++ -MM $(SRC) > deps.mak

zip: 
	zip $(TARGET).zip $(SRC) *.h	
