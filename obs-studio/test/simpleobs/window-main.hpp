#pragma once

#if 0
#include <QMainWindow>
#else
#include <QDialog>
#endif

#include <util/config-file.h>

#if 0
class OBSMainWindow : public QMainWindow 
#else
class OBSMainWindow : public QDialog
#endif
{
	Q_OBJECT

public:
#if 0
	inline OBSMainWindow(QWidget *parent) : QMainWindow(parent) {}
#else
	inline OBSMainWindow(QWidget *parent) : QDialog(parent) {}
#endif

	virtual config_t *Config() const = 0;
	virtual void OBSInit() = 0;

	virtual int GetProfilePath(char *path, size_t size,
				   const char *file) const = 0;
};
