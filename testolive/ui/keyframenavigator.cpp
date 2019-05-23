#include "keyframenavigator.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>

KeyframeNavigator::KeyframeNavigator(QWidget *parent, bool addLeftPad) : QWidget(parent) {
	key_controls = new QHBoxLayout();
	key_controls->setSpacing(0);
	key_controls->setMargin(0);

	if (addLeftPad) {
		key_controls->addStretch();
	}

	setLayout(key_controls);

	left_key_nav = new QPushButton();
	left_key_nav->setIcon(QIcon(":/icons/tri-left.png"));
	left_key_nav->setIconSize(left_key_nav->iconSize()*0.5);
	left_key_nav->setVisible(false);
	key_controls->addWidget(left_key_nav);
	connect(left_key_nav, SIGNAL(clicked(bool)), this, SIGNAL(goto_previous_key()));
	connect(left_key_nav, SIGNAL(clicked(bool)), this, SIGNAL(clicked()));

	key_addremove = new QPushButton();
	key_addremove->setIcon(QIcon(":/icons/diamond.png"));
	key_addremove->setIconSize(key_addremove->iconSize()*0.5);
	key_addremove->setVisible(false);
	key_controls->addWidget(key_addremove);
	connect(key_addremove, SIGNAL(clicked(bool)), this, SIGNAL(toggle_key()));
	connect(key_addremove, SIGNAL(clicked(bool)), this, SIGNAL(clicked()));

	right_key_nav = new QPushButton();
	right_key_nav->setIcon(QIcon(":/icons/tri-right.png"));
	right_key_nav->setIconSize(right_key_nav->iconSize()*0.5);
	right_key_nav->setVisible(false);
	key_controls->addWidget(right_key_nav);
	connect(right_key_nav, SIGNAL(clicked(bool)), this, SIGNAL(goto_next_key()));
	connect(right_key_nav, SIGNAL(clicked(bool)), this, SIGNAL(clicked()));

	keyframe_enable = new QPushButton(QIcon(":/icons/clock.png"), "");
	keyframe_enable->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
	keyframe_enable->setIconSize(keyframe_enable->iconSize()*0.75);
	keyframe_enable->setCheckable(true);
	keyframe_enable->setToolTip(tr("Enable Keyframes"));
	connect(keyframe_enable, SIGNAL(clicked(bool)), this, SIGNAL(keyframe_enabled_changed(bool)));
	connect(keyframe_enable, SIGNAL(toggled(bool)), this, SLOT(keyframe_ui_enabled(bool)));
	connect(keyframe_enable, SIGNAL(clicked(bool)), this, SIGNAL(clicked()));
	key_controls->addWidget(keyframe_enable);
}

KeyframeNavigator::~KeyframeNavigator() {
	delete keyframe_enable;
	delete right_key_nav;
	delete key_addremove;
	delete left_key_nav;
	delete key_controls;
}

void KeyframeNavigator::enable_keyframes(bool b) {
	keyframe_enable->setChecked(b);
}

void KeyframeNavigator::enable_keyframe_toggle(bool b) {
	keyframe_enable->setVisible(b);
}

void KeyframeNavigator::keyframe_ui_enabled(bool enabled) {
	left_key_nav->setVisible(enabled);
	key_addremove->setVisible(enabled);
	right_key_nav->setVisible(enabled);
}
