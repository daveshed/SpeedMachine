.PHONY: test
test:
	g++ \
		UltrasonicSensor.cpp \
		SpeedMachine.cpp \
		test/Osal.cpp \
		test/MockTime.cpp \
		test/TestSensor.cpp \
		test/TestSpeedMachine.cpp \
		test/main.cpp \
		-g -L /usr/lib/ -lCppUTest -lCppUTestExt -I /usr/include -o Test.o
	./Test.o -c