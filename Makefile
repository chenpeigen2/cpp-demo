# C/C++ 特性演示 —— 一键编译运行
CC      := clang
CXX     := clang++
CFLAGS  := -std=c2x -Wall -Wextra      # C23
CXXFLAGS:= -std=c++23 -Wall -Wextra    # C++23
LDFLAGS := -pthread                    # 线程演示需要；对其他文件无害

C_SRCS  := $(wildcard c/*.c)
CXX_SRCS:= $(wildcard cpp/*.cpp)
C_BINS  := $(patsubst c/%.c,build/c-%,$(C_SRCS))
CXX_BINS:= $(patsubst cpp/%.cpp,build/cpp-%,$(CXX_SRCS))

.PHONY: all run clean
all: $(C_BINS) $(CXX_BINS)

build/c-%: c/%.c | build
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

build/cpp-%: cpp/%.cpp | build
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

build:
	mkdir -p build

# 依次运行每个演示，任何一个非 0 退出即失败（这就是本项目的"测试"）
run: all
	@fail=0; for b in $(C_BINS) $(CXX_BINS); do \
	   echo "=== $$b ==="; $$b || { echo "FAILED: $$b"; fail=1; }; \
	 done; exit $$fail

clean:
	rm -rf build
