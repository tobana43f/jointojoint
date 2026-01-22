#define		_CRT_SECURE_NO_WARNINGS
#include	"PlatformSelector.h"
#include	"Sound.h"	// ’è‹`
#include	<stdio.h>	// ‘®•t‚«•¶š—ñ‘€ì

#ifdef		_PLATFORM_DIRECTX
#include	"DXSound.h"
using namespace NDXSound;
#endif

using namespace NSoundResourceInfo;

CSound::CSound(){
	soundLabel = NSoundResourceInfo::SOUND_LABEL_MAX;
}

CSound::~CSound(){
	
}

void CSound::Load(ESoundLabel _label){
	soundLabel = _label;
}

void CSound::Play(){
#ifdef _PLATFORM_DIRECTX
	PlaySound(soundLabel);
#endif
}

void CSound::Stop(){
#ifdef _PLATFORM_DIRECTX
	StopSound(soundLabel);
#endif
}

void CSound::Pause(){
#ifdef _PLATFORM_DIRECTX
	PauseSound(soundLabel);
#endif
}