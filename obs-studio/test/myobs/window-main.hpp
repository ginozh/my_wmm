#pragma once

// #include <QMainWindow>
#include <QDialog>

#include <util/config-file.h>

// class OBSMainWindow : public QMainWindow 
class OBSMainWindow : public QDialog
{
	Q_OBJECT

public:
	//inline OBSMainWindow(QWidget *parent) : QMainWindow(parent) {}
	inline OBSMainWindow(QWidget *parent) : QDialog(parent) {}

	virtual config_t *Config() const = 0;
	virtual void OBSInit() = 0;

	virtual int GetProfilePath(char *path, size_t size,
				   const char *file) const = 0;
};
