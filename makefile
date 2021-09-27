#######################
# Makefile
#######################

# link parameter
CC := g++
AR := ar

#PATH
ifeq (,$(PRO_DIR))
  PRO_DIR =$(shell pwd)
endif
SRC_PATH = $(PRO_DIR)/src
PROTOC_PATH = $(PRO_DIR)/protobuf
HYPERSCAN_PATH = $(PRO_DIR)/hyperscan

#FLAGS
LIBS = -pthread -lpthread -lprotobuf
LIBS += -static -lhs -lhs_runtime -lcorpusomatic -lcrosscompileutil -ldatabaseutil -lexpressionutil
LIBS += -L$(PROTOC_PATH)/lib
LIBS += -L$(HYPERSCAN_PATH)/lib
CFLAGS := -g -Wall
CFLAGS += -I $(PROTOC_PATH)/include
CFLAGS += -I $(HYPERSCAN_PATH)/include
CFLAGS += -I $(SRC_PATH)
TARGET = tclient tserver

# all object files.
.PHONY:all
all: $(TARGET)

tclient:$(SRC_PATH)/client.o $(SRC_PATH)/message.pb.o $(SRC_PATH)/SocketUDP.o
	$(CC) $(CFLAGS) -o tclient $(SRC_PATH)/client.o $(SRC_PATH)/message.pb.o $(SRC_PATH)/SocketUDP.o $(LIBS)

tserver:$(SRC_PATH)/server.o $(SRC_PATH)/message.pb.o $(SRC_PATH)/regmatch.o $(SRC_PATH)/SocketUDP.o
	$(CC) $(CFLAGS) -o tserver $(SRC_PATH)/server.o $(SRC_PATH)/message.pb.o $(SRC_PATH)/SocketUDP.o $(SRC_PATH)/regmatch.o $(LIBS)
	
$(SRC_PATH)/client.o:$(SRC_PATH)/client.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_PATH)/message.pb.o:$(SRC_PATH)/message.pb.cc
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_PATH)/server.o:$(SRC_PATH)/server.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_PATH)/regmatch.o:$(SRC_PATH)/regmatch.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_PATH)/SocketUDP.o:$(SRC_PATH)/SocketUDP.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:clean
clean:
	rm -f $(SRC_PATH)/*.o
	rm -f tclient tserver