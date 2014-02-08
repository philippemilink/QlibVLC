/*************************************************************************
QlibVLC: a C++ wrapper of libVLC for Qt, with audio part only.
    Copyright (C) 2014 - Philippe SWARTVAGHER

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
    *****************************************************************************/

#include "QlibVLC.h"

QlibVLC::QlibVLC()
{
    m_vlc = libvlc_new(0, NULL); // Initialize libVLC
    m_player = libvlc_media_player_new(m_vlc);

    m_issetMedia = false;
}


QlibVLC::~QlibVLC()
{
    libvlc_media_release(m_media);
    libvlc_media_player_release(m_player);
    libvlc_release(m_vlc);
}


int QlibVLC::duration()
{
    return libvlc_media_get_duration(m_media);
}


int QlibVLC::getDuration(QString path)
{
    libvlc_media_t* media = libvlc_media_new_path(m_vlc, path.replace("/", "\\\\").toStdString().c_str());

    libvlc_media_parse(media);

    int duration = libvlc_media_get_duration(media);

    libvlc_media_release(media);

    return duration;
}


void QlibVLC::setMedia(QString path)
{
    m_path = path;
    //m_media = libvlc_media_new_path(m_vlc, path.replace("/", "\\\\").toStdString().c_str());
    m_media = libvlc_media_new_path(m_vlc, m_path.toStdString().c_str());
    libvlc_media_parse(m_media);
    libvlc_media_player_set_media(m_player, m_media);
    m_eventManager = libvlc_media_player_event_manager(m_player);
    m_issetMedia = true;
}


void QlibVLC::play()
{
    libvlc_media_player_play(m_player);


    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerPositionChanged, callbackTimeChanged, (void*) this);
    libvlc_event_attach(m_eventManager, libvlc_MediaPlayerEndReached, callbackEndTrack, (void*) this);
}


void QlibVLC::callbackTimeChanged(const libvlc_event_t* event, void* data)
{
    QlibVLC* vlc;
    vlc = (QlibVLC*) data;
    vlc->emitPositionChanged();
}


void QlibVLC::callbackEndTrack(const libvlc_event_t* event, void* data)
{
    QlibVLC* vlc;
    vlc = (QlibVLC*) data;
    vlc->endTrack();
}


void QlibVLC::emitPositionChanged()
{
    float positionFloat = libvlc_media_player_get_position(m_player);
    int positionInt = positionFloat * libvlc_media_get_duration(m_media);
    emit positionChanged(positionInt);
}


void QlibVLC::pause(bool pause)
{
    if(pause)
        libvlc_media_player_set_pause(m_player, 1);
    else
        libvlc_media_player_set_pause(m_player, 0);
}


bool QlibVLC::isPlaying()
{
    if(libvlc_media_player_is_playing(m_player) == 1)
        return true;
    else
        return false;
}


void QlibVLC::changePosition(float value)
{
    if(isPlaying())
        libvlc_media_player_set_position(m_player, value);
}


int QlibVLC::trackNumber()
{
    QString trackNumberString = libvlc_media_get_meta(m_media, libvlc_meta_TrackNumber);
    if(trackNumberString.isEmpty() || trackNumberString.isNull())
        return -1;
    else
        return trackNumberString.toInt();
}


QString QlibVLC::album()
{
    QString album = libvlc_media_get_meta(m_media, libvlc_meta_Album);

    return album;
}


QString QlibVLC::artist()
{
    QString artist = libvlc_media_get_meta(m_media, libvlc_meta_Artist);

    return artist;
}


int QlibVLC::year()
{
    QString yearNumberString = libvlc_media_get_meta(m_media, libvlc_meta_Date);
    if(yearNumberString.isNull())
        return 0;
    else
    {
        return yearNumberString.toInt();
    }
}


QString QlibVLC::title()
{
    QString title = libvlc_media_get_meta(m_media, libvlc_meta_Title);

    QFileInfo* fileInfo = new QFileInfo(m_path);
    if(title==fileInfo->fileName())
    {
        return fileInfo->baseName();
    }
    else
        return title;
}


void QlibVLC::ejectMedia()
{
    if(m_issetMedia)
    {
        libvlc_media_player_stop(m_player);
        libvlc_media_player_retain(m_player);
        m_issetMedia = false;
    }
}


void QlibVLC::stop()
{
    libvlc_media_player_stop(m_player);
}


std::vector<QlibVLCOutput> QlibVLC::getOutputs()
{
    std::vector<QlibVLCOutput> array;

    libvlc_audio_output_t* outputs = libvlc_audio_output_list_get(m_vlc);

    while(outputs!=NULL)
    {
        QlibVLCOutput output;
        output.description = outputs->psz_description;
        output.name = outputs->psz_name;

        array.push_back(output);

        outputs = outputs->p_next;
    }

    libvlc_audio_output_list_release(outputs);

    return array;
}


void QlibVLC::setOutput(QString name)
{
    libvlc_audio_output_set(m_player, name.toStdString().c_str());
}
