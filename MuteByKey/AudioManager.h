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
    AudioManager();
    ~AudioManager();

	QList<DWORD> getActiveProcesses();
	QStandardItemModel* getProcessList();
	void setProcessVolume(DWORD processId, float volume);

private:
    bool initAudioDevices();
    void cleanUp();

    HRESULT hr;
    IMMDeviceEnumerator* enumerator;
    IMMDevice* device;
    IAudioSessionManager2* manager;
    IAudioSessionEnumerator* sessionEnumerator;
    ISimpleAudioVolume* simpleAudioVolume;
    int count;
};