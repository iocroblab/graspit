//######################################################################
//
// GraspIt!
// Copyright (C) 2002-2009  Columbia University in the City of New York.
// All rights reserved.
//
// GraspIt! is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GraspIt! is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GraspIt!.  If not, see <http://www.gnu.org/licenses/>.
//
// Author(s): Matei T. Ciocarlie
//
// $Id: gloveCalibrationDlg.cpp,v 1.3 2009/03/30 20:42:12 cmatei Exp $
//
//######################################################################

#include "gloveCalibrationDlg.h"

#include <QFileDialog>

#include "graspit/world.h"
#include "graspit/graspitCore.h"
#include "graspit/ivmgr.h"
#include "graspit/robot.h"
#include "graspit/gloveInterface.h"

void GloveCalibrationDlg::init()
{
  //  calibrateButton->setEnabled(false);

  World *w = graspitCore->getWorld();
  mInterface = w->getCurrentHand()->getGloveInterface();

  mInterface->saveRobotPose();

  QStringList calibrationTypeBoxList, poseDistanceBoxList;

  calibrationTypeBoxList << "Fist" << "Simple thumb" << "Complex thumb" << "Abd. - Add." << "Mean Pose";
  calibrationTypeBox->addItems(calibrationTypeBoxList);
  calibrationTypeBox->setCurrentIndex(2);
  mInterface->initCalibration(GloveInterface::COMPLEX_THUMB);

  poseDistanceBoxList << "0" << "17" << "61" << "82";
  poseDistanceBox->addItems(poseDistanceBoxList);
  poseDistanceBox->setCurrentIndex(0);

  update();
}

void GloveCalibrationDlg::prevPose()
{
  mInterface->nextPose(-1);
  mInterface->showCurrentPose();
  update();
}


void GloveCalibrationDlg::nextPose()
{
  mInterface->nextPose(1);
  mInterface->showCurrentPose();
  update();
}


void GloveCalibrationDlg::record()
{
  int d = poseDistanceBox->currentText().toInt();
  mInterface->recordPoseFromGlove(d);
  mInterface->showCurrentPose();
  update();
}

void GloveCalibrationDlg::update()
{
  if (mInterface->poseSet()) {
    textLabel2->setText("Pose recorded");

  } else {
    textLabel2->setText("No values recorded");
  }

  if (mInterface->readyToCalibrate()) {
    calibrateButton->setEnabled(true);
  } else {
    calibrateButton->setEnabled(false);
  }

  numberPosesLabel->setNum(mInterface->getNumPoses());
}

void GloveCalibrationDlg::calibrate()
{
  mInterface->performCalibration();
  update();
}


void GloveCalibrationDlg::save()
{
  QString fn(QFileDialog::getSaveFileName(this, QString(), QString(getenv("GRASPIT")) + QString("/models/CyberGlove"),
                                          "Glove Pose Files (*.txt)"));
  if (!fn.isEmpty()) {
    if (fn.section('.', 1).isEmpty()) {
      fn.append(".txt");
    }
    mInterface->saveCalibration(fn.toUtf8().constData());
  }
}


void GloveCalibrationDlg::done()
{
  // mInterface->revertRobotPose();
  QDialog::accept();
}


void GloveCalibrationDlg::savePoses()
{
  QString fn(QFileDialog::getSaveFileName(this, QString(), QString(getenv("GRASPIT")) + QString("/models/CyberGlove"),
                                          "Glove Pose Files (*.pos)"));
  if (!fn.isEmpty()) {
    if (fn.section('.', 1).isEmpty()) {
      fn.append(".pos");
    }
    mInterface->saveCalibrationPoses(fn.toUtf8().constData());
  }
}


void GloveCalibrationDlg::loadPoses()
{
  QString fn(QFileDialog::getOpenFileName(this, QString(), QString(getenv("GRASPIT")) + QString("/models/CyberGlove"),
                                          "Glove Pose Files (*.pos)"));
  if (!fn.isEmpty()) {
    mInterface->loadCalibrationPoses(fn.toUtf8().constData());
  }
  update();
}

void GloveCalibrationDlg::loadCalibration()
{
  QString fn(QFileDialog::getOpenFileName(this, QString(), QString(getenv("GRASPIT")) + QString("/models/CyberGlove"),
                                          "Glove Pose Files (*.txt)"));
  if (!fn.isEmpty()) {
    mInterface->loadCalibration(fn.toUtf8().constData());
  }
}


void GloveCalibrationDlg::initCalibration()
{
  switch (calibrationTypeBox->currentIndex()) {
    case 0:
      mInterface->initCalibration(GloveInterface::FIST);
      break;
    case 1:
      mInterface->initCalibration(GloveInterface::SIMPLE_THUMB);
      break;
    case 2:
      mInterface->initCalibration(GloveInterface::COMPLEX_THUMB);
      break;
    case 3:
      mInterface->initCalibration(GloveInterface::ABD_ADD);
      break;
    case 4:
      mInterface->initCalibration(GloveInterface::MEAN_POSE);
      break;
    default:
      break;
  }
}


void GloveCalibrationDlg::clearPoses()
{
  mInterface->clearPoses();
  update();
}


void GloveCalibrationDlg::startGlove()
{
  mInterface->startGlove();
}
