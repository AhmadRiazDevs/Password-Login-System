# ============================================================
#  Makefile  —  Password Login System
#  Usage:
#    make          → build the executable
#    make run      → build & run
#    make clean    → remove build artifacts
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET   = PasswordLoginSystem

SRCS     = main.cpp \
           src/User.cpp \
           src/Admin.cpp \
           src/RegularUser.cpp \
           src/FileManager.cpp \
           src/AuthenticationManager.cpp \
           src/LoginHistoryManager.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo ""
	@echo "  Build successful! Run with: ./$(TARGET)"
	@echo ""

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
	@echo "  Cleaned build artifacts."
