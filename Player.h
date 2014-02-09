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


#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include <QWidget>
#include <QFileDialog>

#include "QlibVLC.h"

namespace Ui
{
    class Player;
}

class Player : public QWidget
{
    Q_OBJECT
    

    public:
    explicit Player(QWidget *parent = 0);
    ~Player();


    public slots:
    void on_buttonOpenFile_clicked();
    void on_buttonEject_clicked();
    void timeChanged(int newTime);
    void on_buttonPlay_clicked();
    void on_buttonPause_clicked();
    void on_buttonStop_clicked();
    void outputSelected_change(QString newOutput);
    void endPlaying();
    

    private:
    Ui::Player *ui;
    QlibVLC* m_vlc;
    bool m_fileLoaded;
};

#endif // PLAYER_H
