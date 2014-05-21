#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "ui_TimelineDockWidget.h"
#include "ui/TimelineDockWidget.h"
#include "ui/AutoscoperMainWindow.h"

#include "Tracker.hpp"

#include <QGLContext>

TimelineDockWidget::TimelineDockWidget(QWidget *parent) :
										QDockWidget(parent),
										dock(new Ui::TimelineDockWidget){
	dock->setupUi(this);
	
	mainwindow  = dynamic_cast <AutoscoperMainWindow *> ( parent);

	position_graph = new GraphData();
	position_graph->show_x = true;
    position_graph->show_y = true;
    position_graph->show_z = true;
    position_graph->show_yaw = true;
    position_graph->show_pitch = true;
    position_graph->show_roll = true;
    position_graph->min_frame = 0.0;
    position_graph->max_frame = 100.0;
    position_graph->min_value = -180.0;
    position_graph->max_value = 180.0;
    position_graph->frame_locks.resize(100,false);

	dock->gltimeline->setGraphData(position_graph);

	m_spinButtonUpdate = true;
}

TimelineDockWidget::~TimelineDockWidget(){
	delete dock;
	delete position_graph;
}

void TimelineDockWidget::setTrial(Trial* trial){
	dock->gltimeline->setTrial(trial);
}

void TimelineDockWidget::draw(){
	dock->gltimeline->update();
}

void TimelineDockWidget::setSharedGLContext(const QGLContext * sharedContext){
	QGLContext* context = new QGLContext(sharedContext->format(), dock->gltimeline);
	context->create(sharedContext);
	dock->gltimeline->setContext(context,sharedContext,true);
}

void TimelineDockWidget::setFramesRange(int firstFrame, int lastFrame ){
	dock->spinBox_FirstFrame->setMinimum(firstFrame);
	dock->spinBox_FirstFrame->setMaximum(lastFrame);
	dock->spinBox_FirstFrame->setValue(firstFrame);

	dock->spinBox_LastFrame->setMinimum(firstFrame);
	dock->spinBox_LastFrame->setMaximum(lastFrame);
	dock->spinBox_LastFrame->setValue(lastFrame);

	dock->horizontalSlider_Frame->setRange(firstFrame,lastFrame);
	dock->horizontalSlider_Frame->setValue(firstFrame);
	dock->horizontalSlider_Frame->setPageStep(1);
	dock->labelFrame->setText(QString::number(firstFrame));
}

void TimelineDockWidget::on_toolButton_PreviousFrame_clicked(){
	dock->horizontalSlider_Frame->setValue(dock->labelFrame->text().toInt() - 1);
}
void TimelineDockWidget::on_toolButton_Stop_clicked(){

}
void TimelineDockWidget::on_toolButton_Play_clicked(){

}
void TimelineDockWidget::on_toolButton_NextFrame_clicked(){
	dock->horizontalSlider_Frame->setValue(dock->labelFrame->text().toInt() + 1);
}

void TimelineDockWidget::on_horizontalSlider_Frame_valueChanged(int value){
	dock->labelFrame->setText(QString::number(value));
	
	mainwindow->setFrame(dock->labelFrame->text().toInt());
}

void TimelineDockWidget::on_doubleSpinBox_X_valueChanged ( double d ){
	if(m_spinButtonUpdate){
		mainwindow->update_coord_frame();
        update_graph_min_max();

        mainwindow->redrawGL();
	}
}

void TimelineDockWidget::on_doubleSpinBox_Y_valueChanged ( double d ){
	if(m_spinButtonUpdate){
		mainwindow->update_coord_frame();
        update_graph_min_max();

        mainwindow->redrawGL();
	}
}

void TimelineDockWidget::on_doubleSpinBox_Z_valueChanged ( double d ){
	if(m_spinButtonUpdate){
		mainwindow->update_coord_frame();
        update_graph_min_max();

        mainwindow->redrawGL();
	}
}

void TimelineDockWidget::on_doubleSpinBox_Yaw_valueChanged ( double d ){
	if(m_spinButtonUpdate){
		mainwindow->update_coord_frame();
        update_graph_min_max();

        mainwindow->redrawGL();
	}
}

void TimelineDockWidget::on_doubleSpinBox_Pitch_valueChanged ( double d ){
	if(m_spinButtonUpdate){
		mainwindow->update_coord_frame();
        update_graph_min_max();

        mainwindow->redrawGL();
	}
}

void TimelineDockWidget::on_doubleSpinBox_Roll_valueChanged ( double d ){
	if(m_spinButtonUpdate){
		mainwindow->update_coord_frame();
        update_graph_min_max();

        mainwindow->redrawGL();
	}
}

void TimelineDockWidget::getValues(double *xyzypr){
	xyzypr[0] = dock->doubleSpinBox_X->value();
    xyzypr[1] = dock->doubleSpinBox_Y->value();
    xyzypr[2] = dock->doubleSpinBox_Z->value();
	xyzypr[3] = dock->doubleSpinBox_Yaw->value();
    xyzypr[4] = dock->doubleSpinBox_Pitch->value();
	xyzypr[5] = dock->doubleSpinBox_Roll->value();
}

void TimelineDockWidget::setValues(double *xyzypr){
	dock->doubleSpinBox_X->setValue(xyzypr[0]);
	dock->doubleSpinBox_Y->setValue(xyzypr[1]);
	dock->doubleSpinBox_Z->setValue(xyzypr[2]);
	dock->doubleSpinBox_Yaw->setValue(xyzypr[3]);
	dock->doubleSpinBox_Pitch->setValue(xyzypr[4]);
	dock->doubleSpinBox_Roll->setValue(xyzypr[5]);
}


void TimelineDockWidget::update_graph_min_max(int frame)
{
    if (!mainwindow->getTracker()->trial() || mainwindow->getTracker()->trial()->x_curve.empty()) {
        position_graph->max_value = 180.0;
        position_graph->min_value = -180.0;
    }
    // If a frame is specified then only check that frame for a new minimum and
    // maximum.
    else if (frame != -1) {
        if (position_graph->show_x) {
            float x_value = mainwindow->getTracker()->trial()->x_curve(frame);
            if (x_value > position_graph->max_value) {
                position_graph->max_value = x_value;
            }
            if (x_value < position_graph->min_value) {
                position_graph->min_value = x_value;
            }
        }
        if (position_graph->show_y) {
            float y_value = mainwindow->getTracker()->trial()->y_curve(frame);
            if (y_value > position_graph->max_value) {
                position_graph->max_value = y_value;
            }
            if (y_value < position_graph->min_value) {
                position_graph->min_value = y_value;
            }
        }
        if (position_graph->show_z) {
            float z_value = mainwindow->getTracker()->trial()->z_curve(frame);
            if (z_value > position_graph->max_value) {
                position_graph->max_value = z_value;
            }
            if (z_value < position_graph->min_value) {
                position_graph->min_value = z_value;
            }
        }
        if (position_graph->show_yaw) {
            float yaw_value = mainwindow->getTracker()->trial()->yaw_curve(frame);
            if (yaw_value > position_graph->max_value) {
                position_graph->max_value = yaw_value;
            }
            if (yaw_value < position_graph->min_value) {
                position_graph->min_value = yaw_value;
            }
        }
        if (position_graph->show_pitch) {
            float pitch_value = mainwindow->getTracker()->trial()->pitch_curve(frame);
            if (pitch_value > position_graph->max_value) {
                position_graph->max_value = pitch_value;
            }
            if (pitch_value < position_graph->min_value) {
                position_graph->min_value = pitch_value;
            }
        }
        if (position_graph->show_roll) {
            float roll_value = mainwindow->getTracker()->trial()->roll_curve(frame);
            if (roll_value > position_graph->max_value) {
                position_graph->max_value = roll_value;
            }
            if (roll_value < position_graph->min_value) {
                position_graph->min_value = roll_value;
            }
        }
    }
    // Otherwise we need to check all the frames.
    else {

        position_graph->min_value = 1e6;
        position_graph->max_value = -1e6;

        if (position_graph->show_x) {
            for (frame = floor(position_graph->min_frame);
                 frame < position_graph->max_frame;
                 frame += 1.0f) {
                float x_value = mainwindow->getTracker()->trial()->x_curve(frame);
                if (x_value > position_graph->max_value) {
                    position_graph->max_value = x_value;
                }
                if (x_value < position_graph->min_value) {
                    position_graph->min_value = x_value;
                }
            }
        }
        if (position_graph->show_y) {
            for (frame = floor(position_graph->min_frame);
                 frame < position_graph->max_frame;
                 frame += 1.0f) {
                float y_value = mainwindow->getTracker()->trial()->y_curve(frame);
                if (y_value > position_graph->max_value) {
                    position_graph->max_value = y_value;
                }
                if (y_value < position_graph->min_value) {
                    position_graph->min_value = y_value;
                }
            }
        }
        if (position_graph->show_z) {
            for (frame = floor(position_graph->min_frame);
                 frame < position_graph->max_frame;
                 frame += 1.0f) {
                float z_value = mainwindow->getTracker()->trial()->z_curve(frame);
                if (z_value > position_graph->max_value) {
                    position_graph->max_value = z_value;
                }
                if (z_value < position_graph->min_value) {
                    position_graph->min_value = z_value;
                }
            }
        }
        if (position_graph->show_yaw) {
            for (frame = floor(position_graph->min_frame);
                 frame < position_graph->max_frame;
                 frame += 1.0f) {
                float yaw_value = mainwindow->getTracker()->trial()->yaw_curve(frame);
                if (yaw_value > position_graph->max_value) {
                    position_graph->max_value = yaw_value;
                }
                if (yaw_value < position_graph->min_value) {
                    position_graph->min_value = yaw_value;
                }
            }
        }
        if (position_graph->show_pitch) {
            for (frame = floor(position_graph->min_frame);
                 frame < position_graph->max_frame;
                 frame += 1.0f) {
                float pitch_value = mainwindow->getTracker()->trial()->pitch_curve(frame);
                if (pitch_value > position_graph->max_value) {
                    position_graph->max_value = pitch_value;
                }
                if (pitch_value < position_graph->min_value) {
                    position_graph->min_value = pitch_value;
                }
            }
        }
        if (position_graph->show_roll) {
            for (frame = floor(position_graph->min_frame);
                 frame < position_graph->max_frame;
                 frame += 1.0f) {
                float roll_value = mainwindow->getTracker()->trial()->roll_curve(frame);
                if (roll_value > position_graph->max_value) {
                    position_graph->max_value = roll_value;
                }
                if (roll_value < position_graph->min_value) {
                    position_graph->min_value = roll_value;
                }
            }
        }

        position_graph->min_value -= 1.0;
        position_graph->max_value += 1.0;
    }
}