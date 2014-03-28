TEMPLATE = subdirs
SUBDIRS = purchasing

src_imports.subdir = imports
src_imports.depends = purchasing
SUBDIRS += src_imports

android {
    SUBDIRS += android
}
