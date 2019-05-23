#include "stabilizerdialog.h"

#include <QVBoxLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>

#include "ui/labelslider.h"

StabilizerDialog::StabilizerDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Stabilizer");

    layout = new QVBoxLayout(this);
    setLayout(layout);

    enable_stab = new QCheckBox(this);
    enable_stab->setText("Enable Stabilizer");
    layout->addWidget(enable_stab);

    analysis = new QGroupBox("Analysis", this);
    layout->addWidget(analysis);

    analysis_layout = new QGridLayout(analysis);
    analysis->setLayout(analysis_layout);

    analysis_layout->addWidget(new QLabel("Shakiness:"), 0, 0);

    shakiness_slider = new LabelSlider();
    shakiness_slider->set_minimum_value(1);
    shakiness_slider->set_default_value(5);
    shakiness_slider->set_maximum_value(10);
    analysis_layout->addWidget(shakiness_slider, 0, 1);

    analysis_layout->addWidget(new QLabel("Accuracy:"), 1, 0);

    accuracy_slider = new LabelSlider();
    accuracy_slider->set_minimum_value(1);
    accuracy_slider->set_default_value(15);
    accuracy_slider->set_maximum_value(15);
    analysis_layout->addWidget(accuracy_slider, 1, 1);

    analysis_layout->addWidget(new QLabel("Step Size:"), 2, 0);

    stepsize_slider = new LabelSlider();
    stepsize_slider->set_minimum_value(1);
    stepsize_slider->set_default_value(6);
    analysis_layout->addWidget(stepsize_slider, 2, 1);

    analysis_layout->addWidget(new QLabel("Minimum Contrast:"), 3, 0);

    mincontrast_slider = new LabelSlider();
    mincontrast_slider->set_minimum_value(0);
    mincontrast_slider->set_default_value(0.3);
    mincontrast_slider->set_maximum_value(1);
    analysis_layout->addWidget(mincontrast_slider, 3, 1);

    /*analysis_layout->addWidget(new QLabel("Tripod Mode:"), 4, 0);

    tripod_mode_box = new QCheckBox();
    analysis_layout->addWidget(tripod_mode_box, 4, 1);*/

    stabilization = new QGroupBox("Stabilization", this);
    layout->addWidget(stabilization);

    stabilization_layout = new QGridLayout();
    stabilization->setLayout(stabilization_layout);

    stabilization_layout->addWidget(new QLabel("Smoothing:"), 0, 0);

    smoothing_slider = new LabelSlider();
    smoothing_slider->set_minimum_value(0);
    smoothing_slider->set_default_value(10);
    stabilization_layout->addWidget(smoothing_slider, 0, 1);

    stabilization_layout->addWidget(new QLabel("Gaussian Motion:"), 1, 0);

    gaussian_motion = new QCheckBox();
    gaussian_motion->setChecked(true);
    stabilization_layout->addWidget(gaussian_motion, 1, 1);

    stabilization_layout->addWidget(new QLabel("Maximum Movement:"), 2, 0);
    stabilization_layout->addWidget(new QLabel("Maximum Rotation:"), 3, 0);
    stabilization_layout->addWidget(new QLabel("Crop:"), 4, 0);
    stabilization_layout->addWidget(new QLabel("Zoom Behavior:"), 5, 0);
    stabilization_layout->addWidget(new QLabel("Zoom Speed:"), 6, 0);
    stabilization_layout->addWidget(new QLabel("Interpolation Quality:"), 7, 0);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttons);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    connect(enable_stab, SIGNAL(toggled(bool)), this, SLOT(set_all_enabled(bool)));

    set_all_enabled(false);
}

void StabilizerDialog::set_all_enabled(bool e) {
    analysis->setEnabled(e);
    stabilization->setEnabled(e);
}
