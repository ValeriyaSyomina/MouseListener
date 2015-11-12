#include "Facade/Facade.h"

Facade::Facade()
{
    mouseListener = new MouseListener();
    deviceManager = new DeviceManager();

    connect(mouseListener, SIGNAL(MessageReceivedSignal(const char*)),deviceManager, SLOT(SendCommandToDevice(const char*)));
    connect(mouseListener, SIGNAL(ServerStartedSignal(QString,int)), this, SLOT(ServerStarted(QString,int)));
    connect(mouseListener, SIGNAL(ServerStoppedSignal()), this, SLOT(ServerStopped()));
    connect(mouseListener, SIGNAL(ClientConnectedSignal(QString,int)), this, SLOT(ClientConnected(QString,int)));
    connect(mouseListener, SIGNAL(ClientDisconnectedSignal(QString,int)), this, SLOT(ClientDisconnected(QString,int)));

    connect(deviceManager, SIGNAL(KernelModuleInsertedSignal()),mouseListener, SLOT(DeviceKernelModuleInserted()));
    connect(deviceManager, SIGNAL(KernelModuleRemovedSignal()),mouseListener, SLOT(DeviceKernelModuleRemoved()));
    connect(deviceManager, SIGNAL(ErrorSendCommandToDeviceSignal()), this, SLOT(ErrorSendCommandToDevice()));
}

Facade::~Facade()
{
    if (mouseListener)
    {
        delete mouseListener;
        mouseListener = NULL;
    }
    if (deviceManager)
    {
        delete deviceManager;
        deviceManager = NULL;
    }
}
