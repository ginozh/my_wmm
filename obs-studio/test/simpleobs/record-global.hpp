/***********************************************************************
 * File : record-global.hpp
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2020-04-20     storm   1.0    created
 * 
 ***********************************************************************
 */

#ifndef  RECORD_GLOBAL_INC
#define  RECORD_GLOBAL_INC

#include <QApplication>
#include <QMutex>
#include <vector>
#include <QObject>
#include <QWidget>
#include <QDialog>
#include <deque>
#include <obs-frontend-api.h>
#include <util/util.hpp>
#include <util/dstr.h>
#include <util/platform.h>
#include "util/profiler.h"
#include "window-main.hpp"

#if 1
extern bool portable_mode;
extern bool opt_start_recording;
extern bool opt_start_replaybuffer;
extern bool opt_minimize_tray;
extern bool opt_studio_mode;
extern bool opt_always_on_top;
extern std::string opt_starting_scene;
extern bool remuxAfterRecord;
extern std::string remuxFilename;
#endif

std::string CurrentDateTimeString();
std::string GenerateSpecifiedFilename(const char *extension, bool noSpace,
				      const char *format);

typedef std::function<void()> VoidFunc;
class RecordGlobal : public QObject
{
    Q_OBJECT
private:
    RecordGlobal();
    static RecordGlobal *m_pInstance;
	void AppInit();
	bool OBSInit();
	//bool InitTheme();

	bool InitGlobalConfig();
	bool InitGlobalConfigDefaults();
	bool InitLocale();
public:
    static RecordGlobal* instance();

	inline QDialog *GetRecordDialog() const { return m_pRecordDialog; }
	inline void SetRecordDialog(QDialog* dialog)  { m_pRecordDialog=dialog; }

	inline config_t *GlobalConfig() const { return globalConfig; }

	inline const char *GetLocale() const { return locale.c_str(); }
	inline const char *GetTheme() const { return theme.c_str(); }
	bool SetTheme(std::string name, std::string path = "");

	inline lookup_t *GetTextLookup() const { return textLookup; }

	inline const char *GetString(const char *lookupVal) const
	{
		return textLookup.GetString(lookupVal);
	}

	inline void PushUITranslation(obs_frontend_translate_ui_cb cb)
	{
		translatorHooks.emplace_front(cb);
	}

	inline void PopUITranslation() { translatorHooks.pop_front(); }

	profiler_name_store_t *GetProfilerNameStore() const
	{
		return profilerNameStore;
	}

	const char *GetLastLog() const;
	const char *GetCurrentLog() const;
	const char *GetLastCrashLog() const;

	std::string GetVersionString() const;

	const char *GetRenderModule() const;

	inline void IncrementSleepInhibition()
	{
		if (!sleepInhibitor)
			return;
		if (sleepInhibitRefs++ == 0)
			os_inhibit_sleep_set_active(sleepInhibitor, true);
	}

	inline void DecrementSleepInhibition()
	{
		if (!sleepInhibitor)
			return;
		if (sleepInhibitRefs == 0)
			return;
		if (--sleepInhibitRefs == 0)
			os_inhibit_sleep_set_active(sleepInhibitor, false);
	}
private:
    static QMutex mutexInstance;
	std::string locale;
	std::string theme;
	ConfigFile globalConfig;
	TextLookup textLookup;
	profiler_name_store_t *profilerNameStore = nullptr;
	os_inhibit_t *sleepInhibitor = nullptr;
	int sleepInhibitRefs = 0;
	std::deque<obs_frontend_translate_ui_cb> translatorHooks;
    QDialog* m_pRecordDialog=nullptr;
};

int GetConfigPath(char *path, size_t size, const char *name);
char *GetConfigPathPtr(const char *name);

std::vector<std::pair<std::string, std::string>> GetLocaleNames();
inline const char *Str(const char *lookup)
{
	return RecordGlobal::instance()->GetString(lookup);
}
#define QTStr(lookupVal) QString::fromUtf8(Str(lookupVal))
bool GetFileSafeName(const char *name, std::string &file);
bool GetClosestUnusedFileName(std::string &path, const char *extension);
static inline int GetProfilePath(char *path, size_t size, const char *file)
{
	OBSMainWindow *window =
		reinterpret_cast<OBSMainWindow *>(RecordGlobal::instance()->GetRecordDialog());
	return window->GetProfilePath(path, size, file);
}
int GetProgramDataPath(char *path, size_t size, const char *name);
char *GetProgramDataPathPtr(const char *name);
bool WindowPositionValid(QRect rect);

inline config_t *GetGlobalConfig()
{
	return RecordGlobal::instance()->GlobalConfig();
}

#endif   /* ----- #ifndef RECORD_GLOBAL_INC  ----- */

