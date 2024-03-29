#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <QList>
#include <QStandardItemModel>

class AudioManager
{

public:
    ~AudioManager();

	QList<DWORD> getActiveProcesses();
	QStandardItemModel* getProcessList();
	void setProcessVolume(DWORD processId, float volume);

private:
    bool initAudioDevices();
    void cleanUp();

    HRESULT hr;
    IMMDeviceEnumerator* enumerator = NULL;
    IMMDevice* device = NULL;
    IAudioSessionManager2* manager = NULL;
    IAudioSessionEnumerator* sessionEnumerator = NULL;
    IAudioEndpointVolume* volumeControl = NULL;
    ISimpleAudioVolume* simpleAudioVolume = NULL;
    int count = NULL;

};