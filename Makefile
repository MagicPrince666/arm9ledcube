CXX = arm-f1c100s-linux-uclibcgnueabi-g++
TARGET1	= ledcube

DIR		= .
INC		= -I. 
CFLAGS	= -Wall
LDFLAGS = 

OBJPATH	= .

FILES	= $(foreach dir,$(DIR),$(wildcard $(dir)/*.cpp))

OBJS	= $(patsubst %.cpp,%.o,$(FILES))

all:$(OBJS) $(TARGET1)

$(OBJS):%.o:%.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $(OBJPATH)/$(notdir $@) $< 

$(TARGET1):$(OBJPATH)
	$(CXX) -o $@ $(OBJPATH)/*.o $(LDFLAGS)
	
#./xag_tcpscs 192.168.1.240 5555 ledcube

clean:
	-rm $(OBJPATH)/*.o
	-rm $(TARGET1)
	