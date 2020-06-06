.PHONY: test
test:
	g++ UltrasonicSensor.cpp test/Osal.cpp test/MockTime.cpp test/test.cpp test/main.cpp -L /usr/lib/ -lCppUTest -lCppUTestExt -I /usr/include -o foo
	./foo