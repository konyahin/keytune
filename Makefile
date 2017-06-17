SOURCES = \
    main.c \
    keyboard.c

TARGET = keytune.exe

$(TARGET): $(SOURCES)
    cl $(SOURCES) /link user32.lib Gdi32.lib /out:$(TARGET)

clear:
    del *.obj $(TARGET)