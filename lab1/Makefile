APP=$(shell ls | grep cpp | sed s/.cpp//g)
all: $(APP)

$(APP): $(APP).o
	g++ $(APP).o -o $(APP) `pkg-config --libs opencv4`
$(APP).o: $(APP).cpp
	g++ `pkg-config --cflags opencv4` $(APP).cpp -c

clean:
	rm -f $(APP)