
CFLAGS += -DUSE_GTK2
USE_GTK2=1
CFLAGS += -DUSE_PIXBUF
USE_PIXBUF=1
CFLAGS += -DUSE_PANGO
USE_PANGO=1
USE_EXIF=1
EXIFCXXFLAGS=-I/usr/include/libexif
EXIFCXXFLAGS += -DUSE_EXIF
EXIFLIBS=-lexif