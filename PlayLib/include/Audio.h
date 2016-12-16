/*
* PlaLib - V1.4
*
* Description: Biblioteca de audio para a PlayLib.
*
* Author: Edirlei Soares de Lima (VisionLab - PUC-Rio)
* Web: http://www.inf.puc-rio.br/~elima/
* Email: elima@inf.puc-rio.br
*
*/

#pragma once

#ifndef _AUDIO_H_
#define _AUDIO_H_

namespace PlayLib
{	
	typedef DWORD HSTREAM;

	class Audio
    {
    public:
		Audio();
		~Audio();
		void LoadAudio(char *filename);
		void Play();
		void Stop();
		void Pause();
		bool IsPlaying();
	private:
		HSTREAM _audio_str;
		int _state;
       
    };	
}

#endif