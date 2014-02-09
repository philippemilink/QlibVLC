/**********************************************************************
A simple audio player to illustrate how works QlibVLC
    Copyright (C) 2014  Philippe SWARTVAGHER

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "Player.h"
#include "ui_Player.h"



Player::Player(QWidget *parent) : QWidget(parent), ui(new Ui::Player)
{
    ui->setupUi(this);
    m_vlc = new QlibVLC();
    connect(m_vlc, SIGNAL(positionChanged(int)), this, SLOT(timeChanged(int)));
    connect(m_vlc, SIGNAL(endTrack()), this, SLOT(endPlaying()));

    m_fileLoaded = false;

    std::vector<QlibVLCOutput> outputs = m_vlc->getOutputs();
    ui->comboBoxOutputs->addItem("No specified");
    for(int i=0; i<outputs.size(); i++)
    {
        ui->comboBoxOutputs->addItem(outputs[i].description);
    }
    connect(ui->comboBoxOutputs, SIGNAL(currentIndexChanged(QString)), this, SLOT(outputSelected_change(QString)));

    connect(ui->sliderVolume, SIGNAL(sliderMoved(int)), m_vlc, SLOT(setVolume(int)));
}


Player::~Player()
{
    if(m_fileLoaded)
        m_vlc->ejectMedia();

    delete ui;
}


void Player::on_buttonOpenFile_clicked()
{
    if(m_fileLoaded)
        return;

    QString filePath = QFileDialog::getOpenFileName(this, "Select an audio file", QDir::homePath(), "Audio files (*.mp3 *.wav *.ogg *.flac *.acc)");
    if(filePath.isEmpty())
        return;

    m_vlc->setMedia(filePath);

    ui->lineEditPath->setText(filePath);

    ui->valueTitle->setText(m_vlc->title());
    ui->valueAlbum->setText(m_vlc->album());
    ui->valueArtist->setText(m_vlc->artist());
    ui->valueTotalTime->setText(QString::number(m_vlc->duration()));
    ui->valueTrack->setText(QString::number(m_vlc->trackNumber()));
    ui->valueYear->setText(QString::number(m_vlc->year()));

    ui->buttonEject->setEnabled(true);
    ui->buttonOpenFile->setEnabled(false);
    ui->buttonPause->setEnabled(false);
    ui->buttonPlay->setEnabled(true);

    m_fileLoaded = true;
}


void Player::on_buttonEject_clicked()
{
    if(!m_fileLoaded)
        return;

    m_vlc->ejectMedia();

    ui->lineEditPath->setText("Select an audio file...");

    ui->valueTitle->setText("");
    ui->valueAlbum->setText("");
    ui->valueArtist->setText("");
    ui->valueTotalTime->setText("");
    ui->valueTrack->setText("");
    ui->valueYear->setText("");
    ui->valuePlayedTime->setText("");

    ui->buttonEject->setEnabled(false);
    ui->buttonOpenFile->setEnabled(true);
    ui->buttonPause->setEnabled(false);
    ui->buttonPause->setText("Set pause");
    ui->buttonPlay->setEnabled(false);
    ui->comboBoxOutputs->setEnabled(true);
    ui->sliderVolume->setEnabled(false);

    m_fileLoaded = false;
}


void Player::timeChanged(int newTime)
{
    ui->valuePlayedTime->setText(QString::number(newTime));
}


void Player::on_buttonPlay_clicked()
{
    m_vlc->play();
    ui->buttonPlay->setEnabled(false);
    ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ui->comboBoxOutputs->setEnabled(false);
    ui->sliderVolume->setEnabled(true);
}


void Player::on_buttonPause_clicked()
{
    if(m_vlc->isPlaying())
    {
        m_vlc->pause(true);
        ui->buttonPause->setText("Unset pause");
    }
    else
    {
        ui->buttonPause->setText("Set pause");
        m_vlc->pause(false);
    }
}


void Player::on_buttonStop_clicked()
{
    m_vlc->stop();
    ui->buttonPlay->setEnabled(true);
    ui->buttonPause->setEnabled(false);
    ui->buttonStop->setEnabled(false);
    ui->comboBoxOutputs->setEnabled(true);
    ui->sliderVolume->setEnabled(false);

    ui->valuePlayedTime->setText("");
}


void Player::outputSelected_change(QString newOutput)
{
    ui->comboBoxOutputs->removeItem(ui->comboBoxOutputs->findText("No specified"));
    std::vector<QlibVLCOutput> outputs = m_vlc->getOutputs();
    for(int i=0; i<outputs.size(); i++)
    {
        if(outputs[i].description==newOutput)
        {
            m_vlc->setOutput(outputs[i].name);
            break;
        }
    }
}


void Player::endPlaying()
{
    ui->valuePlayedTime->setText("");

    //m_vlc->stop();

    ui->buttonEject->setEnabled(true);
    ui->buttonPause->setEnabled(false);
    ui->buttonPlay->setEnabled(true);
    ui->buttonStop->setEnabled(false);
    ui->comboBoxOutputs->setEnabled(true);
    ui->sliderVolume->setEnabled(false);

}
