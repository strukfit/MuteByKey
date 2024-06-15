#include "AudioManager.h"

void AudioManager::cleanUp()
{
    if (hr) hr = NULL;
    if (enumerator) enumerator = nullptr;
    if (device) device = nullptr;
    if (manager) manager = nullptr;
    if (sessionEnumerator) sessionEnumerator = nullptr;
}

bool AudioManager::initAudioDevices()
{
    // Get enumerator for audio devices
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&enumerator));
    if (FAILED(hr))
    {
        cleanUp();
        return false;
    }

    // Get default audio-rendering device
    hr = enumerator->GetDefaultAudioEndpoint(EDataFlow::eRender,
        ERole::eMultimedia, &device);
    if (FAILED(hr))
    {
        cleanUp();
        return false;
    }
    
    // Get audio session manager
    hr = device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_INPROC_SERVER, NULL, (void**)&manager);
    if (FAILED(hr))
    {
        cleanUp();
        return false;
    }

    // Get audio session enumerator
    hr = manager->GetSessionEnumerator(&sessionEnumerator);
    if (FAILED(hr))
    {
        cleanUp();
        return false;
    }

    // Loop through audio sessions
    hr = sessionEnumerator->GetCount(&count);
    if (FAILED(hr))
    {
        cleanUp();
        return false;
    }

    return true;
}

AudioManager::AudioManager():
    hr(NULL),
    enumerator(nullptr),
    device(nullptr),
    manager(nullptr),
    sessionEnumerator(nullptr),
    simpleAudioVolume(nullptr),
    count(NULL)
{
}

AudioManager::~AudioManager()
{
    if(enumerator) enumerator->Release();
    if(device) device->Release();
    if(manager) manager->Release();
    if(sessionEnumerator) sessionEnumerator->Release();
    if(simpleAudioVolume) simpleAudioVolume->Release();
}

QList<DWORD> AudioManager::getActiveProcesses() {
    QList<DWORD> activeProcesses;

    if (initAudioDevices())
    {
        IAudioSessionControl* sessionControl1 = nullptr;
        IAudioSessionControl2* sessionControl2 = nullptr;

        for (int i = 0; i < count; i++) {
            hr = sessionEnumerator->GetSession(i, &sessionControl1);
            if (FAILED(hr)) continue;

            hr = sessionControl1->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
            if (FAILED(hr)) continue;

            // Get process ID of the session
            DWORD sessionId;
            hr = sessionControl2->GetProcessId(&sessionId);
            if (FAILED(hr)) continue;

            activeProcesses.append(sessionId);
        }

        sessionControl1->Release();
        sessionControl2->Release();
    }
    cleanUp();

    return activeProcesses;
}

QStandardItemModel* AudioManager::getProcessList() {
    // Creating a model for a list of processes
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Icon"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ProcessName"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ProcessId"));

    // Loading the standard application icon
    HICON hDefaultIcon = LoadIcon(NULL, IDI_APPLICATION);
    QIcon defaultIcon = QIcon(QPixmap::fromImage(QImage::fromHICON(hDefaultIcon)));
    DestroyIcon(hDefaultIcon);

    QList<DWORD> list = getActiveProcesses();

    for (const auto& dwProcessId : list)
    {
        QList<QStandardItem*> items;

        // Getting the application icon
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
        HICON hIcon = NULL;

        QString processName = "UNKNOWN";

        if (dwProcessId == 0)
            processName = "System sounds";

        if (hProcess != NULL) {
            TCHAR szExeFile[MAX_PATH];
            if (GetModuleFileNameEx(hProcess, NULL, szExeFile, MAX_PATH)) {
                hIcon = ExtractIcon(NULL, szExeFile, 0);
                processName = szExeFile;
            }
            CloseHandle(hProcess);
        }

        QIcon icon;
        if (hIcon != NULL) {
            icon = QIcon(QPixmap::fromImage(QImage::fromHICON(hIcon)));
            DestroyIcon(hIcon);
        }
        else {
            icon = defaultIcon;
        }

        QString processId = QString::number(dwProcessId);

        // Creating an element for the application icon
        QStandardItem* iconItem = new QStandardItem();
        iconItem->setIcon(icon);

        items.append(iconItem);
        items.append(new QStandardItem(processName));
        items.append(new QStandardItem(processId));
        model->appendRow(items);
    }

    return model;
}

void AudioManager::setProcessVolume(DWORD processId, float volume) {

    if (initAudioDevices())
    {
        IAudioSessionControl* sessionControl1 = nullptr;
        IAudioSessionControl2* sessionControl2 = nullptr;

        for (int i = 0; i < count; i++) {
            hr = sessionEnumerator->GetSession(i, &sessionControl1);
            if (FAILED(hr)) continue;

            hr = sessionControl1->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
            if (FAILED(hr)) continue;

            // Get process ID of the session
            DWORD sessionId;
            hr = sessionControl2->GetProcessId(&sessionId);
            if (FAILED(hr)) continue;

            if (sessionId == processId) 
            {
                hr = sessionControl1->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&simpleAudioVolume);
                if (FAILED(hr)) continue;

                hr = simpleAudioVolume->SetMasterVolume(volume, NULL);
                simpleAudioVolume->Release();

                if (SUCCEEDED(hr)) break;
            }
        }

        sessionControl1->Release();
        sessionControl2->Release();
    }
    
    cleanUp();
}