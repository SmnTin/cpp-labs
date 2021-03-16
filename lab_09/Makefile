CXX = g++

STRICT_WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
	-Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
	-Winit-self  -Wmissing-declarations -Wmissing-include-dirs \
	-Wold-style-cast -Woverloaded-virtual -Wredundant-decls \
	-Wsign-conversion -Wsign-promo -Wstrict-overflow=5 \
	-Wswitch-default -Wundef -Werror -Wno-unused

CXXFLAGS = -O2 -Wall -std=c++14 -Iinclude $(STRICT_WARNINGS)
LDFLAGS = -O2

EXE = lab_09
SRCDIR = src
OBJDIR = obj

OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.cpp))

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXE) $(LDFLAGS)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

include $(wildcard $(OBJDIR)/*.d)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXE)

.PHONY: clean all
