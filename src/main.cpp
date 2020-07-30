#include <functional>
#include <Arduino.h>

const uint32_t STEPS = 100000;

class VirtTest {
public:
  uint32_t calculate();

protected:
  virtual uint32_t calculateStep(uint32_t value);
};

class VirtTest2 : public VirtTest {
protected:
  uint32_t calculateStep(uint32_t value) override;
};

class VirtTest3 : public VirtTest {
protected:
  inline uint32_t calculateStep(uint32_t value) override {
    return value + 3;
  }
};

class FuncTest {
public:
  typedef uint32_t (*func_t)(uint32_t);

  uint32_t calculate(func_t func);

protected:
  func_t _func;
};

class FuncTest2 {
public:
  typedef std::function<uint32_t(uint32_t)> func_t;

  uint32_t calculate(func_t func);

protected:
  func_t _func;
};

uint32_t VirtTest::calculate() {
  uint32_t result = 0;

  for (uint32_t i = 0; i < STEPS; ++i) {
    result += calculateStep(i);
  }
  return result;
}

uint32_t VirtTest::calculateStep(uint32_t value) {
  return value + 1;
}

uint32_t VirtTest2::calculateStep(uint32_t value) {
  return value + 2;
}

uint32_t FuncTest::calculate(func_t func) {
  uint32_t result = 0;

  _func = func;
  for (uint32_t i = 0; i < STEPS; ++i) {
    result += _func(i);
  }
  return result;
}

uint32_t FuncTest2::calculate(func_t func) {
  uint32_t result = 0;

  _func = func;
  for (uint32_t i = 0; i < STEPS; ++i) {
    result += _func(i);
  }
  return result;
}

static uint32_t calculateStep(uint32_t value) {
  return value + 4;
}

VirtTest t1;
VirtTest2 t2;
VirtTest3 t3;
FuncTest t4;
FuncTest2 t5;
FuncTest2 t6;

void setup() {
  Serial.begin(115200);
  Serial.println();

  uint32_t result, time;

  Serial.print(F("VirtTest::calculate()="));
  Serial.flush();
  time = micros();
  result = t1.calculate();
  time = micros() - time;
  Serial.print(result);
  Serial.print(F(", time is "));
  Serial.print(time);
  Serial.println(F(" us"));

  Serial.print(F("VirtTest2::calculate()="));
  Serial.flush();
  time = micros();
  result = t2.calculate();
  time = micros() - time;
  Serial.print(result);
  Serial.print(F(", time is "));
  Serial.print(time);
  Serial.println(F(" us"));

  Serial.print(F("VirtTest3::calculate()="));
  Serial.flush();
  time = micros();
  result = t3.calculate();
  time = micros() - time;
  Serial.print(result);
  Serial.print(F(", time is "));
  Serial.print(time);
  Serial.println(F(" us"));

  Serial.print(F("FuncTest::calculate(&)="));
  Serial.flush();
  time = micros();
  result = t4.calculate(calculateStep);
  time = micros() - time;
  Serial.print(result);
  Serial.print(F(", time is "));
  Serial.print(time);
  Serial.println(F(" us"));

  Serial.print(F("FuncTest2::calculate(&)="));
  Serial.flush();
  time = micros();
  result = t5.calculate(calculateStep);
  time = micros() - time;
  Serial.print(result);
  Serial.print(F(", time is "));
  Serial.print(time);
  Serial.println(F(" us"));

  Serial.print(F("FuncTest2::calculate([])="));
  Serial.flush();
  time = micros();
  result = t6.calculate([](uint32_t value) {
    return value + 5;
  });
  time = micros() - time;
  Serial.print(result);
  Serial.print(F(", time is "));
  Serial.print(time);
  Serial.println(F(" us"));

  Serial.flush();
  ESP.deepSleep(0);
}

void loop() {}
