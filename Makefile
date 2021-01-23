CXX      := g++
CXXFLAGS := -pipe `wx-config --static=yes --cxxflags`
	
LDFLAGS := `wx-config --static=yes --libs all`

SRCDIR   := src
OBJDIR   := obj

SOURCES  := $(notdir $(wildcard $(SRCDIR)/*.cpp))
OBJECTS  := $(SOURCES:.cpp=.o)


TARGET   := sapling-gui

.PHONY: clean install debug install

$(TARGET): $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

$(OBJDIR):
	mkdir $@

clean:
	rm -rf $(TARGET) $(OBJDIR)

install:
	cp $(TARGET) /usr/bin
	cp res/$(TARGET).png /usr/share/icons/hicolor/48x48/apps
	mkdir -p /usr/share/$(TARGET)
	cp res/$(TARGET).png /usr/share/$(TARGET)
	cp $(TARGET).desktop /usr/share/applications

debug:
	echo $(SOURCES)
