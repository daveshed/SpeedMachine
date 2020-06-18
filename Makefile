.PHONY: test
test:
	g++ \
		SpeedMachine.cpp \
		SonarBarrier.cpp \
		UltrasonicSensor.cpp \
		Debounce.cpp \
		test/Osal.cpp \
		test/MockTime.cpp \
		test/TestDebounce.cpp \
		test/TestBarrier.cpp \
		test/TestSensor.cpp \
		test/TestSpeedMachine.cpp \
		test/main.cpp \
		-g -L /usr/lib/ -lCppUTest -lCppUTestExt -I /usr/include -I test/ -o Test.o
	./Test.o -c