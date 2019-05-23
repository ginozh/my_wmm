#include "sourcetable.h"
#include "panels/project.h"

#include "project/footage.h"
#include "panels/timeline.h"
#include "panels/viewer.h"
#include "panels/panels.h"
#include "playback/playback.h"
#include "project/undo.h"
#include "project/sequence.h"
#include "mainwindow.h"
#include "io/config.h"
#include "project/media.h"
#include "project/sourcescommon.h"
#include "debug.h"

#include <QDragEnterEvent>
#include <QMimeData>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QProcess>

SourceTable::SourceTable(QWidget* parent) : QTreeView(parent) {
	setSortingEnabled(true);
	setAcceptDrops(true);
	sortByColumn(0, Qt::AscendingOrder);
	setContextMenuPolicy(Qt::CustomContextMenu);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setDragDropMode(QAbstractItemView::DragDrop);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(item_click(const QModelIndex&)));
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_context_menu()));
}

void SourceTable::show_context_menu() {
	project_parent->sources_common->show_context_menu(this, selectionModel()->selectedRows());
}

void SourceTable::item_click(const QModelIndex& index) {
	if (selectionModel()->selectedRows().size() == 1 && index.column() == 0) {
		project_parent->sources_common->item_click(project_parent->item_to_media(index), index);
	}
}

void SourceTable::mousePressEvent(QMouseEvent* event) {
	project_parent->sources_common->mousePressEvent(event);
	QTreeView::mousePressEvent(event);
}

void SourceTable::mouseDoubleClickEvent(QMouseEvent* event) {
	project_parent->sources_common->mouseDoubleClickEvent(event, selectionModel()->selectedRows());
}

void SourceTable::dragEnterEvent(QDragEnterEvent *event) {
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	} else {
		QTreeView::dragEnterEvent(event);
	}
}

void SourceTable::dragMoveEvent(QDragMoveEvent *event) {
	if (event->mimeData()->hasUrls()) {
		event->acceptProposedAction();
	} else {
		QTreeView::dragMoveEvent(event);
	}
}

void SourceTable::dropEvent(QDropEvent* event) {
	project_parent->sources_common->dropEvent(this, event, indexAt(event->pos()), selectionModel()->selectedRows());
}
