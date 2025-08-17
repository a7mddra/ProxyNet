CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iinclude -MMD -MP
LDLIBS   := -lpam -lpam_misc -lutil

OBJ_DIR  := build
BIN      := main
DEP_FILE := dependencies.txt

SRCS := $(shell find . -type f -name '*.cpp' -not -path "./$(OBJ_DIR)/*")
OBJS := $(patsubst ./%, $(OBJ_DIR)/%, $(patsubst %.cpp, %.o, $(SRCS)))
DEPS := $(OBJS:.o=.d)

.PHONY: all run install-deps clean check-owner

all: $(BIN)
	@echo "Built $(BIN). Run it with ./$(BIN) or 'make run' to run with sudo"

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN) $(LDLIBS)
	@chmod +x $(BIN)

run: $(BIN)
	@echo "Running $(BIN) with sudo - be careful, this runs as root!"
	@sudo ./$(BIN)

check-owner:
	@echo "assets ownership:"
	@ls -l assets || true

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

install-deps:
	@if [ -z "$$(command -v apt-get 2>/dev/null)" ]; then \
		echo "apt-get not found - install dependencies manually"; exit 1; \
	fi
	@if [ ! -f "$(DEP_FILE)" ]; then \
		echo "$(DEP_FILE) not found in project root"; exit 1; \
	fi
	@echo "Installing packages from $(DEP_FILE)..."
	@sudo xargs -a $(DEP_FILE) -r apt-get install -y

-include $(DEPS)

clean:
	@rm -rf $(OBJ_DIR) $(BIN)
	@echo "Cleaned."
