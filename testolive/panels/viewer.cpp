#include "viewer.h"

#include "playback/audio.h"
#include "timeline.h"
#include "panels/project.h"
#include "panels/effectcontrols.h"
#include "project/sequence.h"
#include "project/clip.h"
#include "panels/panels.h"
#include "io/config.h"
#include "project/footage.h"
#include "project/media.h"
#include "project/undo.h"
#include "ui/audiomonitor.h"
#include "playback/playback.h"
#include "ui/viewerwidget.h"
#include "ui/viewercontainer.h"
#include "ui/labelslider.h"
#include "ui/timelineheader.h"
#include "ui/resizablescrollbar.h"
#include "debug.h"

#define FRAMES_IN_ONE_MINUTE 1798 // 1800 - 2
#define FRAMES_IN_TEN_MINUTES 17978 // (FRAMES_IN_ONE_MINUTE * 10) - 2

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
}

#include <QtMath>
#include <QAudioOutput>
#include <QPainter>
#include <QStringList>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>

Viewer::Viewer(QWidget *parent) :
	QDockWidget(parent),
	playing(false),
	just_played(false),
	media(nullptr),
	seq(nullptr),
	created_sequence(false),
	minimum_zoom(1.0),
	cue_recording_internal(false)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	setup_ui();

	viewer_container->viewer = this;
	viewer_widget = viewer_container->child;
	viewer_widget->viewer = this;
	set_media(nullptr);

}

Viewer::~Viewer() {}

bool Viewer::is_focused() { return true;
}

bool Viewer::is_main_sequence() {
	return true;
}

void Viewer::set_main_sequence() {
	clean_created_seq();
	set_sequence(true, sequence);
}

void Viewer::reset_all_audio() { return;
}

long timecode_to_frame(const QString& s, int view, double frame_rate) { return 0;
}

QString frame_to_timecode(long f, int view, double frame_rate) { return QString("");
}

bool frame_rate_is_droppable(float rate) { return true;
}

void Viewer::seek(long p) {return;
}

void Viewer::go_to_start() {return;
}

void Viewer::go_to_end() {return;
}

void Viewer::close_media() {
	set_media(nullptr);
}

void Viewer::go_to_in() { return;
}

void Viewer::previous_frame() {return;
}

void Viewer::next_frame() {return;
}

void Viewer::go_to_out() {return;
}

void Viewer::cue_recording(long start, long end, int track) {return;
}

void Viewer::uncue_recording() {return;
}

bool Viewer::is_recording_cued() {return true;
}

void Viewer::toggle_play() {return;
}

void Viewer::play() {return;
}

void Viewer::play_wake() {return;
}

void Viewer::pause() {return;
}

void Viewer::update_playhead_timecode(long p) {return;
}

void Viewer::update_end_timecode() {return;
}

void Viewer::update_header_zoom() {return;
}

void Viewer::update_parents(bool reload_fx) {return;
}

void Viewer::resizeEvent(QResizeEvent *) {return;
}

void Viewer::update_viewer() {
	update_header_zoom();
	viewer_widget->frame_update();
	if (seq != nullptr) update_playhead_timecode(seq->playhead);
	update_end_timecode();
}

void Viewer::clear_in() {return;
}

void Viewer::clear_out() {return;
}

void Viewer::clear_inout_point() {return;
}

void Viewer::toggle_enable_inout() {return;
}

void Viewer::set_in_point() {return;
}

void Viewer::set_out_point() {return;
}

void Viewer::set_zoom(bool in) {return;
}

void Viewer::set_panel_name(const QString &n) {return;
}

void Viewer::update_window_title() {return;
}

void Viewer::set_zoom_value(double d) {return;
}

void Viewer::set_sb_max() {return;
}

long Viewer::get_seq_in() {return 0;
}

long Viewer::get_seq_out() {return 0;
}

void Viewer::setup_ui() {
	QWidget* contents = new QWidget();

	QVBoxLayout* layout = new QVBoxLayout(contents);
	layout->setSpacing(0);
	layout->setMargin(0);

	viewer_container = new ViewerContainer(contents);
	viewer_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(viewer_container);

	setWidget(contents);
}

void Viewer::set_media(Media* m) {
	main_sequence = false;
	media = m;
	clean_created_seq();
	if (media != nullptr) {
		switch (media->get_type()) {
		case MEDIA_TYPE_FOOTAGE:
		{
			Footage* footage = media->to_footage();

			seq = new Sequence();
			created_sequence = true;
			seq->wrapper_sequence = true;
			seq->name = footage->name;

			seq->using_workarea = footage->using_inout;
			if (footage->using_inout) {
				seq->workarea_in = footage->in;
				seq->workarea_out = footage->out;
			}

			seq->frame_rate = 30;

			if (footage->video_tracks.size() > 0) {
				const FootageStream& video_stream = footage->video_tracks.at(0);
				seq->width = video_stream.video_width;
				seq->height = video_stream.video_height;
				if (video_stream.video_frame_rate > 0 && !video_stream.infinite_length) seq->frame_rate = video_stream.video_frame_rate * footage->speed;

				Clip* c = new Clip(seq);
				c->media = media;
				c->media_stream = video_stream.file_index;
				c->timeline_in = 0;
				c->timeline_out = footage->get_length_in_frames(seq->frame_rate);
				if (c->timeline_out <= 0) c->timeline_out = 150;
				c->track = -1;
				c->clip_in = 0;
				c->recalculateMaxLength();
				seq->clips.append(c);
			} else {
				seq->width = 1920;
				seq->height = 1080;
			}

			if (footage->audio_tracks.size() > 0) {
				const FootageStream& audio_stream = footage->audio_tracks.at(0);
				seq->audio_frequency = audio_stream.audio_frequency;

				Clip* c = new Clip(seq);
				c->media = media;
				c->media_stream = audio_stream.file_index;
				c->timeline_in = 0;
				c->timeline_out = footage->get_length_in_frames(seq->frame_rate);
				c->track = 0;
				c->clip_in = 0;
				c->recalculateMaxLength();
				seq->clips.append(c);

				if (footage->video_tracks.size() == 0) {
					viewer_widget->waveform = true;
					viewer_widget->waveform_clip = c;
					viewer_widget->waveform_ms = &audio_stream;
					viewer_widget->frame_update();
				}
			} else {
				seq->audio_frequency = 48000;
			}

			seq->audio_layout = AV_CH_LAYOUT_STEREO;
		}
			break;
		case MEDIA_TYPE_SEQUENCE:
			seq = media->to_sequence();
			break;
		}
	}
	set_sequence(false, seq);
}

void Viewer::update_playhead() {return;
}

void Viewer::timer_update() {return;
}

void Viewer::recording_flasher_update() {return;
}

void Viewer::resize_move(double d) {return;
}

void Viewer::clean_created_seq() {
	viewer_widget->waveform = false;

	if (created_sequence) {
		// TODO delete undo commands referencing this sequence to avoid crashes
		/*for (int i=0;i<undo_stack.count();i++) {
			undo_stack.command(i)
		}*/

		delete seq;
		seq = nullptr;
		created_sequence = false;
	}
}

void Viewer::set_sequence(bool main, Sequence *s) {
	pause();

	reset_all_audio();

	if (seq != nullptr) {
		closeActiveClips(seq);
	}

	main_sequence = main;
	seq = (main) ? sequence : s;

	bool null_sequence = (seq == nullptr);

	viewer_widget->setEnabled(!null_sequence);
	viewer_widget->setVisible(!null_sequence);

	if (!null_sequence) {
		viewer_container->adjust();
	} else {
	}

	viewer_widget->frame_update();

	update();
}

void Viewer::set_playpause_icon(bool play) {return;
}
