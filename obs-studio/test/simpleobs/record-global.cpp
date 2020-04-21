/***********************************************************************
 * File : record-global.cpp
 * Brief: 
 * 
 * History
 * ---------------------------------------------------------------------
 * 2020-04-20     storm   1.0    created
 * 
 ***********************************************************************
 */

#include <QScreen>
#include <QFile>
#include <sstream>
#include "record-global.hpp"
#include "util/platform.h"
#include "util/dstr.h"
#include "util/profiler.hpp"
#ifdef TARGET_OS_MAC
#include "moc_predefs.h"
#else
#define DL_D3D11 ""
#define DL_D3D9 ""
#define DL_OPENGL "libobs-opengl.so"
#endif
#include "qt-wrappers.hpp"
#include "window-basic-main.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <pthread.h>
#endif

#include <iostream>
#include "platform.hpp"
#include "util/util.hpp"
#include "ui-config.h"

using namespace std;
#define DEFAULT_LANG "en-US"

static string currentLogFile;
static string lastLogFile;
static string lastCrashLogFile;
#if 1
bool portable_mode = false;
bool opt_start_recording = false;
bool opt_start_replaybuffer = false;
bool opt_minimize_tray = false;
bool opt_studio_mode = false;
bool opt_always_on_top = false;
string opt_starting_collection;
string opt_starting_profile;
string opt_starting_scene;

bool remuxAfterRecord = false;
string remuxFilename;
#endif
RecordGlobal* RecordGlobal::m_pInstance = NULL;
QMutex RecordGlobal::mutexInstance;

string CurrentDateTimeString()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d, %X", &tstruct);
	return buf;
}

string GenerateSpecifiedFilename(const char *extension, bool noSpace,
				 const char *format)
{
	OBSBasic *main = reinterpret_cast<OBSBasic *>(RecordGlobal::instance()->GetRecordDialog());
	bool autoRemux = config_get_bool(main->Config(), "Video", "AutoRemux");

	if ((strcmp(extension, "mp4") == 0) && autoRemux)
		extension = "mkv";

	BPtr<char> filename =
		os_generate_formatted_filename(extension, !noSpace, format);

	remuxFilename = string(filename);
	remuxAfterRecord = autoRemux;

	return string(filename);
}

vector<pair<string, string>> GetLocaleNames()
{
	string path;
	if (!GetDataFilePath("locale.ini", path))
		throw "Could not find locale.ini path";

	ConfigFile ini;
	if (ini.Open(path.c_str(), CONFIG_OPEN_EXISTING) != 0)
		throw "Could not open locale.ini";

	size_t sections = config_num_sections(ini);

	vector<pair<string, string>> names;
	names.reserve(sections);
	for (size_t i = 0; i < sections; i++) {
		const char *tag = config_get_section(ini, i);
		const char *name = config_get_string(ini, tag, "Name");
		names.emplace_back(tag, name);
	}

	return names;
}

static const char *run_program_init = "run_program_init";
RecordGlobal::RecordGlobal()
{
	sleepInhibitor = os_inhibit_sleep_create("OBS Video/audio");

	profiler_start(); // ./libobs/util/profiler.c
	profile_register_root(run_program_init, 0); //./libobs/util/profiler.c
	ScopeProfiler prof{run_program_init};
#if 0
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#endif
#if !defined(_WIN32) && !defined(__APPLE__) && BROWSER_AVAILABLE
	setenv("QT_NO_GLIB", "1", true);
#endif

	QCoreApplication::addLibraryPath(".");

#if __APPLE__
	InstallNSApplicationSubclass();
#endif
    AppInit();

#if 0
    OBSTranslator translator;
    program.installTranslator(&translator);
#endif
    OBSInit();
}
RecordGlobal* RecordGlobal::instance()  
{  
    //QMutexLocker locker(&mutexInstance);
    if(m_pInstance == NULL)  //判断是否第一次调用  
        m_pInstance = new RecordGlobal();  
    return m_pInstance;  
}  
#define DEFAULT_LANG "en-US"

bool RecordGlobal::InitGlobalConfigDefaults()
{
	config_set_default_string(globalConfig, "General", "Language",
				  DEFAULT_LANG);
	config_set_default_uint(globalConfig, "General", "MaxLogs", 10);
	config_set_default_int(globalConfig, "General", "InfoIncrement", -1);
	config_set_default_string(globalConfig, "General", "ProcessPriority",
				  "Normal");
	config_set_default_bool(globalConfig, "General", "EnableAutoUpdates",
				true);

#if _WIN32
	config_set_default_string(globalConfig, "Video", "Renderer",
				  "Direct3D 11");
#else
	config_set_default_string(globalConfig, "Video", "Renderer", "OpenGL");
#endif

	config_set_default_bool(globalConfig, "BasicWindow", "PreviewEnabled",
				true);
	config_set_default_bool(globalConfig, "BasicWindow",
				"PreviewProgramMode", false);
	config_set_default_bool(globalConfig, "BasicWindow",
				"SceneDuplicationMode", true);
	config_set_default_bool(globalConfig, "BasicWindow", "SwapScenesMode",
				true);
	config_set_default_bool(globalConfig, "BasicWindow", "SnappingEnabled",
				true);
	config_set_default_bool(globalConfig, "BasicWindow", "ScreenSnapping",
				true);
	config_set_default_bool(globalConfig, "BasicWindow", "SourceSnapping",
				true);
	config_set_default_bool(globalConfig, "BasicWindow", "CenterSnapping",
				false);
	config_set_default_double(globalConfig, "BasicWindow", "SnapDistance",
				  10.0);
	config_set_default_bool(globalConfig, "BasicWindow",
				"RecordWhenStreaming", false);
	config_set_default_bool(globalConfig, "BasicWindow",
				"KeepRecordingWhenStreamStops", false);
	config_set_default_bool(globalConfig, "BasicWindow", "SysTrayEnabled",
				true);
	config_set_default_bool(globalConfig, "BasicWindow",
				"SysTrayWhenStarted", false);
	config_set_default_bool(globalConfig, "BasicWindow", "SaveProjectors",
				false);
	config_set_default_bool(globalConfig, "BasicWindow", "ShowTransitions",
				true);
	config_set_default_bool(globalConfig, "BasicWindow",
				"ShowListboxToolbars", true);
	config_set_default_bool(globalConfig, "BasicWindow", "ShowStatusBar",
				true);
	config_set_default_bool(globalConfig, "BasicWindow", "ShowSourceIcons",
				true);
	config_set_default_bool(globalConfig, "BasicWindow", "StudioModeLabels",
				true);

	if (!config_get_bool(globalConfig, "General", "Pre21Defaults")) {
		config_set_default_string(globalConfig, "General",
					  "CurrentTheme", DEFAULT_THEME);
	}

	config_set_default_string(globalConfig, "General", "HotkeyFocusType",
				  "NeverDisableHotkeys");

	config_set_default_bool(globalConfig, "BasicWindow",
				"VerticalVolControl", false);

	config_set_default_bool(globalConfig, "BasicWindow",
				"MultiviewMouseSwitch", true);

	config_set_default_bool(globalConfig, "BasicWindow",
				"MultiviewDrawNames", true);

	config_set_default_bool(globalConfig, "BasicWindow",
				"MultiviewDrawAreas", true);

#ifdef _WIN32
	uint32_t winver = GetWindowsVersion();

	config_set_default_bool(globalConfig, "Audio", "DisableAudioDucking",
				true);
	config_set_default_bool(globalConfig, "General", "BrowserHWAccel",
				winver > 0x601);
#endif

#ifdef __APPLE__
	config_set_default_bool(globalConfig, "Video", "DisableOSXVSync", true);
	config_set_default_bool(globalConfig, "Video", "ResetOSXVSyncOnExit",
				true);
#endif
	return true;
}
static bool do_mkdir(const char *path)
{
	if (os_mkdirs(path) == MKDIR_ERROR) {
		OBSErrorBox(NULL, "Failed to create directory %s", path);
		return false;
	}

	return true;
}
static bool MakeUserDirs()
{
	char path[512];

	if (GetConfigPath(path, sizeof(path), "obs-studio/basic") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;

	if (GetConfigPath(path, sizeof(path), "obs-studio/logs") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;

	if (GetConfigPath(path, sizeof(path), "obs-studio/profiler_data") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;

#ifdef _WIN32
	if (GetConfigPath(path, sizeof(path), "obs-studio/crashes") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;

	if (GetConfigPath(path, sizeof(path), "obs-studio/updates") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;
#endif

	if (GetConfigPath(path, sizeof(path), "obs-studio/plugin_config") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;

	return true;
}
static bool MakeUserProfileDirs()
{
	char path[512];

	if (GetConfigPath(path, sizeof(path), "obs-studio/basic/profiles") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;

	if (GetConfigPath(path, sizeof(path), "obs-studio/basic/scenes") <= 0)
		return false;
	if (!do_mkdir(path))
		return false;

	return true;
}
static void move_basic_to_profiles(void)
{
	char path[512];
	char new_path[512];
	os_glob_t *glob;

	/* if not first time use */
	if (GetConfigPath(path, 512, "obs-studio/basic") <= 0)
		return;
	if (!os_file_exists(path))
		return;

	/* if the profiles directory doesn't already exist */
	if (GetConfigPath(new_path, 512, "obs-studio/basic/profiles") <= 0)
		return;
	if (os_file_exists(new_path))
		return;

	if (os_mkdir(new_path) == MKDIR_ERROR)
		return;

	strcat(new_path, "/");
	strcat(new_path, Str("Untitled"));
	if (os_mkdir(new_path) == MKDIR_ERROR)
		return;

	strcat(path, "/*.*");
	if (os_glob(path, 0, &glob) != 0)
		return;

	strcpy(path, new_path);

	for (size_t i = 0; i < glob->gl_pathc; i++) {
		struct os_globent ent = glob->gl_pathv[i];
		char *file;

		if (ent.directory)
			continue;

		file = strrchr(ent.path, '/');
		if (!file++)
			continue;

		if (astrcmpi(file, "scenes.json") == 0)
			continue;

		strcpy(new_path, path);
		strcat(new_path, "/");
		strcat(new_path, file);
		os_rename(ent.path, new_path);
	}

	os_globfree(glob);
}

static void move_basic_to_scene_collections(void)
{
	char path[512];
	char new_path[512];

	if (GetConfigPath(path, 512, "obs-studio/basic") <= 0)
		return;
	if (!os_file_exists(path))
		return;

	if (GetConfigPath(new_path, 512, "obs-studio/basic/scenes") <= 0)
		return;
	if (os_file_exists(new_path))
		return;

	if (os_mkdir(new_path) == MKDIR_ERROR)
		return;

	strcat(path, "/scenes.json");
	strcat(new_path, "/");
	strcat(new_path, Str("Untitled"));
	strcat(new_path, ".json");

	os_rename(path, new_path);
}
void RecordGlobal::AppInit()
{
	ProfileScope("RecordGlobal::AppInit");

	/*if (!InitApplicationBundle()) // storm
		throw "Failed to initialize application bundle";*/
	if (!MakeUserDirs()) // 创建 '/Users/user/Library/Application Support/obs-studio/'子目录
		throw "Failed to create required user directories";
	if (!InitGlobalConfig()) // 设置global.ini文件内容
		throw "Failed to initialize global config";
	if (!InitLocale()) // 使用 build/rundir/DEBUG/data/obs-studio/locale/en-US.ini
		throw "Failed to load locale";
	//if (!InitTheme())  // 界面主题: Dark
//		throw "Failed to load theme";

	config_set_default_string(globalConfig, "Basic", "Profile",
				  GetString("Untitled"));
	config_set_default_string(globalConfig, "Basic", "ProfileDir",
				  GetString("Untitled"));
	config_set_default_string(globalConfig, "Basic", "SceneCollection",
				  GetString("Untitled"));
	config_set_default_string(globalConfig, "Basic", "SceneCollectionFile",
				  GetString("Untitled"));

	if (!config_has_user_value(globalConfig, "Basic", "Profile")) {
		config_set_string(globalConfig, "Basic", "Profile",
				  GetString("Untitled"));
		config_set_string(globalConfig, "Basic", "ProfileDir",
				  GetString("Untitled"));
	}

	if (!config_has_user_value(globalConfig, "Basic", "SceneCollection")) {
		config_set_string(globalConfig, "Basic", "SceneCollection",
				  GetString("Untitled"));
		config_set_string(globalConfig, "Basic", "SceneCollectionFile",
				  GetString("Untitled"));
	}

#ifdef _WIN32
	bool disableAudioDucking =
		config_get_bool(globalConfig, "Audio", "DisableAudioDucking");
	if (disableAudioDucking)
		DisableAudioDucking(true);
#endif

#ifdef __APPLE__
	if (config_get_bool(globalConfig, "Video", "DisableOSXVSync"))
		EnableOSXVSync(false);
#endif

	// UpdateHotkeyFocusSetting(false);

	move_basic_to_profiles(); // 创建 basic/profiles/Untitled
	move_basic_to_scene_collections(); // 创建 obs-studio/basic/scenes/Untitled

	if (!MakeUserProfileDirs()) // 创建目录 basic/profiles/ basic/scenes/
		throw "Failed to create profile directories";
}

static bool StartupOBS(const char *locale, profiler_name_store_t *store)
{
	char path[512];

	if (GetConfigPath(path, sizeof(path), "obs-studio/plugin_config") <= 0)
		return false;

	return obs_startup(locale, path, store); // path: "/Users/user/Library/Application Support/obs-studio/plugin_config"
}
Q_DECLARE_METATYPE(VoidFunc)
bool RecordGlobal::OBSInit()
{
	ProfileScope("RecordGlobal::OBSInit");

	// setAttribute(Qt::AA_UseHighDpiPixmaps);

	qRegisterMetaType<VoidFunc>();

	if (!StartupOBS(locale.c_str(), GetProfilerNameStore()))
		return false;

	// obs_set_ui_task_handler(ui_task_handler);

#ifdef _WIN32
	bool browserHWAccel =
		config_get_bool(globalConfig, "General", "BrowserHWAccel");

	obs_data_t *settings = obs_data_create();
	obs_data_set_bool(settings, "BrowserHWAccel", browserHWAccel);
	obs_apply_private_data(settings);
	obs_data_release(settings);

	blog(LOG_INFO, "Current Date/Time: %s",
	     CurrentDateTimeString().c_str());

	blog(LOG_INFO, "Browser Hardware Acceleration: %s",
	     browserHWAccel ? "true" : "false");
#endif

	blog(LOG_INFO, "Portable mode: %s", portable_mode ? "true" : "false");

	return true;
}

const char *RecordGlobal::GetLastLog() const
{
	return lastLogFile.c_str();
}
const char *RecordGlobal::GetCurrentLog() const
{
	return currentLogFile.c_str();
}

const char *RecordGlobal::GetLastCrashLog() const
{
	return lastCrashLogFile.c_str();
}

bool RecordGlobal::SetTheme(std::string name, std::string path)
{
	theme = name;

	/* Check user dir first, then preinstalled themes. */
	if (path == "") {
		char userDir[512];
		name = "themes/" + name + ".qss";
		string temp = "obs-studio/" + name;
		int ret = GetConfigPath(userDir, sizeof(userDir), temp.c_str());

		if (ret > 0 && QFile::exists(userDir)) {
			path = string(userDir);
		} else if (!GetDataFilePath(name.c_str(), path)) {
			OBSErrorBox(NULL, "Failed to find %s.", name.c_str());
			return false;
		}
	}

	QString mpath = QString("file:///") + path.c_str();
#if 0
    // tmp storm
	setPalette(defaultPalette);
	setStyleSheet(mpath);
	ParseExtraThemeData(path.c_str());

	emit StyleChanged();
#endif
	return true;
}

int GetProgramDataPath(char *path, size_t size, const char *name)
{
	return os_get_program_data_path(path, size, name);
}

char *GetProgramDataPathPtr(const char *name)
{
	return os_get_program_data_path_ptr(name);
}

string RecordGlobal::GetVersionString() const
{
    return "";
}

const char *RecordGlobal::GetRenderModule() const
{
	const char *renderer =
		config_get_string(globalConfig, "Video", "Renderer");

	return (astrcmpi(renderer, "Direct3D 11") == 0) ? DL_D3D11 : DL_OPENGL;
}

bool WindowPositionValid(QRect rect)
{
	for (QScreen *screen : QGuiApplication::screens()) {
		if (screen->availableGeometry().intersects(rect))
			return true;
	}
	return false;
}

#ifdef __APPLE__
#define BASE_PATH ".."
#else
#define BASE_PATH "../.."
#endif

#define CONFIG_PATH BASE_PATH "/config"

#ifndef OBS_UNIX_STRUCTURE
#define OBS_UNIX_STRUCTURE 0
#endif

int GetConfigPath(char *path, size_t size, const char *name)
{
	if (!OBS_UNIX_STRUCTURE && portable_mode) {
		if (name && *name) {
			return snprintf(path, size, CONFIG_PATH "/%s", name);
		} else {
			return snprintf(path, size, CONFIG_PATH);
		}
	} else {
		return os_get_config_path(path, size, name);
	}
}

char *GetConfigPathPtr(const char *name)
{
	if (!OBS_UNIX_STRUCTURE && portable_mode) {
		char path[512];

		if (snprintf(path, sizeof(path), CONFIG_PATH "/%s", name) > 0) {
			return bstrdup(path);
		} else {
			return NULL;
		}
	} else {
		return os_get_config_path_ptr(name);
	}
}

bool GetFileSafeName(const char *name, std::string &file)
{
	size_t base_len = strlen(name);
	size_t len = os_utf8_to_wcs(name, base_len, nullptr, 0);
	std::wstring wfile;

	if (!len)
		return false;

	wfile.resize(len);
	os_utf8_to_wcs(name, base_len, &wfile[0], len + 1);

	for (size_t i = wfile.size(); i > 0; i--) {
		size_t im1 = i - 1;

		if (iswspace(wfile[im1])) {
			wfile[im1] = '_';
		} else if (wfile[im1] != '_' && !iswalnum(wfile[im1])) {
			wfile.erase(im1, 1);
		}
	}

	if (wfile.size() == 0)
		wfile = L"characters_only";

	len = os_wcs_to_utf8(wfile.c_str(), wfile.size(), nullptr, 0);
	if (!len)
		return false;

	file.resize(len);
	os_wcs_to_utf8(wfile.c_str(), wfile.size(), &file[0], len + 1);
	return true;
}

bool GetClosestUnusedFileName(std::string &path, const char *extension)
{
	size_t len = path.size();
	if (extension) {
		path += ".";
		path += extension;
	}

	if (!os_file_exists(path.c_str()))
		return true;

	int index = 1;

	do {
		path.resize(len);
		path += std::to_string(++index);
		if (extension) {
			path += ".";
			path += extension;
		}
	} while (os_file_exists(path.c_str()));

	return true;
}
static string GetSceneCollectionFileFromName(const char *name)
{
	string outputPath;
	os_glob_t *glob;
	char path[512];

	if (GetConfigPath(path, sizeof(path), "obs-studio/basic/scenes") <= 0)
		return outputPath;

	strcat(path, "/*.json");

	if (os_glob(path, 0, &glob) != 0)
		return outputPath;

	for (size_t i = 0; i < glob->gl_pathc; i++) {
		struct os_globent ent = glob->gl_pathv[i];
		if (ent.directory)
			continue;

		obs_data_t *data =
			obs_data_create_from_json_file_safe(ent.path, "bak");
		const char *curName = obs_data_get_string(data, "name");

		if (astrcmpi(name, curName) == 0) {
			outputPath = ent.path;
			obs_data_release(data);
			break;
		}

		obs_data_release(data);
	}

	os_globfree(glob);

	if (!outputPath.empty()) {
		outputPath.resize(outputPath.size() - 5);
		replace(outputPath.begin(), outputPath.end(), '\\', '/');
		const char *start = strrchr(outputPath.c_str(), '/');
		if (start)
			outputPath.erase(0, start - outputPath.c_str() + 1);
	}

	return outputPath;
}
static string GetProfileDirFromName(const char *name)
{
	string outputPath;
	os_glob_t *glob;
	char path[512];

	if (GetConfigPath(path, sizeof(path), "obs-studio/basic/profiles") <= 0)
		return outputPath;

	strcat(path, "/*");

	if (os_glob(path, 0, &glob) != 0)
		return outputPath;

	for (size_t i = 0; i < glob->gl_pathc; i++) {
		struct os_globent ent = glob->gl_pathv[i];
		if (!ent.directory)
			continue;

		strcpy(path, ent.path);
		strcat(path, "/basic.ini");

		ConfigFile config;
		if (config.Open(path, CONFIG_OPEN_EXISTING) != 0)
			continue;

		const char *curName =
			config_get_string(config, "General", "Name");
		if (astrcmpi(curName, name) == 0) {
			outputPath = ent.path;
			break;
		}
	}

	os_globfree(glob);

	if (!outputPath.empty()) {
		replace(outputPath.begin(), outputPath.end(), '\\', '/');
		const char *start = strrchr(outputPath.c_str(), '/');
		if (start)
			outputPath.erase(0, start - outputPath.c_str() + 1);
	}

	return outputPath;
}
bool RecordGlobal::InitGlobalConfig()
{
	char path[512];
	bool changed = false;

	int len = GetConfigPath(path, sizeof(path), "obs-studio/global.ini");
	if (len <= 0) {
		return false;
	}

	int errorcode = globalConfig.Open(path, CONFIG_OPEN_ALWAYS);
	if (errorcode != CONFIG_SUCCESS) {
		OBSErrorBox(NULL, "Failed to open global.ini: %d", errorcode);
		return false;
	}

	if (!opt_starting_collection.empty()) {
		string path = GetSceneCollectionFileFromName(
			opt_starting_collection.c_str());
		if (!path.empty()) {
			config_set_string(globalConfig, "Basic",
					  "SceneCollection",
					  opt_starting_collection.c_str());
			config_set_string(globalConfig, "Basic",
					  "SceneCollectionFile", path.c_str());
			changed = true;
		}
	}

	if (!opt_starting_profile.empty()) {
		string path =
			GetProfileDirFromName(opt_starting_profile.c_str());
		if (!path.empty()) {
			config_set_string(globalConfig, "Basic", "Profile",
					  opt_starting_profile.c_str());
			config_set_string(globalConfig, "Basic", "ProfileDir",
					  path.c_str());
			changed = true;
		}
	}

	uint32_t lastVersion =
		config_get_int(globalConfig, "General", "LastVersion");

	if (!config_has_user_value(globalConfig, "General", "Pre19Defaults")) {
		bool useOldDefaults = lastVersion &&
				      lastVersion <
					      MAKE_SEMANTIC_VERSION(19, 0, 0);

		config_set_bool(globalConfig, "General", "Pre19Defaults",
				useOldDefaults);
		changed = true;
	}

	if (!config_has_user_value(globalConfig, "General", "Pre21Defaults")) {
		bool useOldDefaults = lastVersion &&
				      lastVersion <
					      MAKE_SEMANTIC_VERSION(21, 0, 0);

		config_set_bool(globalConfig, "General", "Pre21Defaults",
				useOldDefaults);
		changed = true;
	}

	if (!config_has_user_value(globalConfig, "General", "Pre23Defaults")) {
		bool useOldDefaults = lastVersion &&
				      lastVersion <
					      MAKE_SEMANTIC_VERSION(23, 0, 0);

		config_set_bool(globalConfig, "General", "Pre23Defaults",
				useOldDefaults);
		changed = true;
	}

#define PRE_24_1_DEFS "Pre24.1Defaults"
	if (!config_has_user_value(globalConfig, "General", PRE_24_1_DEFS)) {
		bool useOldDefaults = lastVersion &&
				      lastVersion <
					      MAKE_SEMANTIC_VERSION(24, 1, 0);

		config_set_bool(globalConfig, "General", PRE_24_1_DEFS,
				useOldDefaults);
		changed = true;
	}
#undef PRE_24_1_DEFS

	if (config_has_user_value(globalConfig, "BasicWindow",
				  "MultiviewLayout")) {
		const char *layout = config_get_string(
			globalConfig, "BasicWindow", "MultiviewLayout");
		// changed |= UpdatePre22MultiviewLayout(layout); // tmp storm
	}

	if (lastVersion && lastVersion < MAKE_SEMANTIC_VERSION(24, 0, 0)) {
		bool disableHotkeysInFocus = config_get_bool(
			globalConfig, "General", "DisableHotkeysInFocus");
		if (disableHotkeysInFocus)
			config_set_string(globalConfig, "General",
					  "HotkeyFocusType",
					  "DisableHotkeysInFocus");
		changed = true;
	}

	if (changed)
		config_save_safe(globalConfig, "tmp", nullptr);

	return InitGlobalConfigDefaults();
}

bool RecordGlobal::InitLocale()
{
	ProfileScope("RecordGlobal::InitLocale");
	const char *lang =
		config_get_string(globalConfig, "General", "Language");

	locale = lang;

	string englishPath;
	if (!GetDataFilePath("locale/" DEFAULT_LANG ".ini", englishPath)) {
		OBSErrorBox(NULL, "Failed to find locale/" DEFAULT_LANG ".ini");
		return false;
	}

	textLookup = text_lookup_create(englishPath.c_str());
	if (!textLookup) {
		OBSErrorBox(NULL, "Failed to create locale from file '%s'",
			    englishPath.c_str());
		return false;
	}

	bool userLocale =
		config_has_user_value(globalConfig, "General", "Language");
	bool defaultLang = astrcmpi(lang, DEFAULT_LANG) == 0;

	if (userLocale && defaultLang)
		return true;

	if (!userLocale && defaultLang) {
		for (auto &locale_ : GetPreferredLocales()) {
			if (locale_ == lang)
				return true;

			stringstream file;
			file << "locale/" << locale_ << ".ini";

			string path;
			if (!GetDataFilePath(file.str().c_str(), path))
				continue;

			if (!text_lookup_add(textLookup, path.c_str()))
				continue;

			blog(LOG_INFO, "Using preferred locale '%s'",
			     locale_.c_str());
			locale = locale_;
			return true;
		}

		return true;
	}

	stringstream file;
	file << "locale/" << lang << ".ini";

	string path;
	if (GetDataFilePath(file.str().c_str(), path)) {
		if (!text_lookup_add(textLookup, path.c_str()))
			blog(LOG_ERROR, "Failed to add locale file '%s'",
			     path.c_str());
	} else {
		blog(LOG_ERROR, "Could not find locale file '%s'",
		     file.str().c_str());
	}

	return true;
}
