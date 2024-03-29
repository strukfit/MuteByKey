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
	QList<DWORD> getActiveProcesses();
	QStandardItemModel* getProcessList();
	void setProcessVolume(DWORD processId, float volume);

};