#include "AudioManager.h"

AudioManager::~AudioManager()
{
    delete enumerator;
    delete device;
    delete manager;
    delete sessionEnumerator;
    delete volumeControl;
    delete simpleAudioVolume;
}

void AudioManager::cleanUp()
{
    if (enumerator) enumerator->Release();
    if (device) device->Release();
    if (manager) manager->Release();
    if (sessionEnumerator) sessionEnumerator->Release();
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

    //  Get default audio-rendering device
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

QList<DWORD> AudioManager::getActiveProcesses() {
    QList<DWORD> activeProcesses;

    if (initAudioDevices())
    {
        for (int i = 0; i < count; i++) {
            IAudioSessionControl* sessionControl1 = NULL;
            IAudioSessionControl2* sessionControl2 = NULL;

            hr = sessionEnumerator->GetSession(i, &sessionControl1);
            if (FAILED(hr)) continue;

            hr = sessionControl1->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
            if (FAILED(hr)) continue;

            // Get process ID of the session
            DWORD sessionId;
            hr = sessionControl2->GetProcessId(&sessionId);
            if (FAILED(hr)) continue;

            activeProcesses.append(sessionId);

            sessionControl1->Release();
            sessionControl2->Release();
        }
    }

    cleanUp();

    return activeProcesses;
}

QStandardItemModel* AudioManager::getProcessList() {
    // Создаем модель для списка процессов
    QStandardItemModel* model = new QStandardItemModel();
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Icon"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ProcessName"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ProcessId"));

    // Загружаем стандартную иконку приложения
    HICON hDefaultIcon = LoadIcon(NULL, IDI_APPLICATION);
    QIcon defaultIcon = QIcon(QPixmap::fromImage(QImage::fromHICON(hDefaultIcon)));

    QList<DWORD> list = getActiveProcesses();

    for (const auto& dwProcessId : list)
    {
        QList<QStandardItem*> items;

        // Получаем иконку приложения
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
        HICON hIcon = NULL;

        QString processName = "UNKNOWN";

        if (dwProcessId == 0)
            processName = "Master Volume";

        if (hProcess != NULL) {
            TCHAR szExeFile[MAX_PATH];
            if (GetModuleFileNameEx(hProcess, NULL, szExeFile, MAX_PATH)) {
                hIcon = ExtractIcon(NULL, szExeFile, 0);
                processName = QString::fromWCharArray(szExeFile);
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

        // Создаем элемент для иконки приложения
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
        for (int i = 0; i < count; i++) {
            IAudioSessionControl* sessionControl1 = NULL;
            IAudioSessionControl2* sessionControl2 = NULL;

            hr = sessionEnumerator->GetSession(i, &sessionControl1);
            if (FAILED(hr)) continue;

            hr = sessionControl1->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
            if (FAILED(hr)) continue;

            // Get process ID of the session
            DWORD sessionId;
            hr = sessionControl2->GetProcessId(&sessionId);
            if (FAILED(hr)) continue;

            if (sessionId == processId) {
                hr = sessionControl1->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&simpleAudioVolume);
                if (FAILED(hr)) continue;

                hr = simpleAudioVolume->SetMasterVolume(volume, NULL);
                simpleAudioVolume->Release();

                // Если удалось изменить громкость, можно выйти из цикла
                if (SUCCEEDED(hr)) {
                    sessionControl1->Release();
                    sessionControl2->Release();
                    break;
                }

                //QMessageBox::information(nullptr, "", "Failed to set volume level. HRESULT: 0x" + QString::fromWCharArray(error.ErrorMessage()));

            }

            sessionControl1->Release();
            sessionControl2->Release();
        }
    }
    
    cleanUp();
}