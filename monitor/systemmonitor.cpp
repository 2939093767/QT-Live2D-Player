#include "systemmonitor.h"

#include <windows.h>
#include <dxgi.h>
#include <dxgi1_4.h>

SystemMonitor::SystemMonitor(QObject *parent)
    : QObject(parent)
{
    initializeVram();
}

SystemMonitor::~SystemMonitor()
{
    cleanupVram();
}

bool SystemMonitor::initializeVram()
{
    IDXGIFactory *factory = nullptr;
    HRESULT hr = CreateDXGIFactory(IID_IDXGIFactory, reinterpret_cast<void **>(&factory));
    if (FAILED(hr) || !factory)
        return false;

    IDXGIAdapter *adapter = nullptr;
    hr = factory->EnumAdapters(0, &adapter);
    if (FAILED(hr) || !adapter) {
        factory->Release();
        return false;
    }

    m_dxgiFactory = factory;
    m_dxgiAdapter = adapter;
    m_vramAvailable = true;
    return true;
}

void SystemMonitor::cleanupVram()
{
    if (m_dxgiAdapter) {
        static_cast<IDXGIAdapter *>(m_dxgiAdapter)->Release();
        m_dxgiAdapter = nullptr;
    }
    if (m_dxgiFactory) {
        static_cast<IDXGIFactory *>(m_dxgiFactory)->Release();
        m_dxgiFactory = nullptr;
    }
    m_vramAvailable = false;
}

quint64 SystemMonitor::fileTimeToUInt64(const void *fileTime) const
{
    const FILETIME *ft = static_cast<const FILETIME *>(fileTime);
    return (static_cast<quint64>(ft->dwHighDateTime) << 32) | ft->dwLowDateTime;
}

SystemResourceInfo SystemMonitor::query()
{
    SystemResourceInfo info;

    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        const quint64 idle = fileTimeToUInt64(&idleTime);
        const quint64 kernel = fileTimeToUInt64(&kernelTime);
        const quint64 user = fileTimeToUInt64(&userTime);

        if (!m_firstCpuQuery) {
            const quint64 idleDelta = idle - m_prevIdleTime;
            const quint64 totalDelta = (kernel - m_prevKernelTime) + (user - m_prevUserTime);
            if (totalDelta > 0) {
                info.cpuUsagePercent = (1.0 - static_cast<double>(idleDelta) / static_cast<double>(totalDelta)) * 100.0;
                if (info.cpuUsagePercent < 0.0)
                    info.cpuUsagePercent = 0.0;
                if (info.cpuUsagePercent > 100.0)
                    info.cpuUsagePercent = 100.0;
            }
        } else {
            m_firstCpuQuery = false;
        }

        m_prevIdleTime = idle;
        m_prevKernelTime = kernel;
        m_prevUserTime = user;
    }

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    if (GlobalMemoryStatusEx(&memStatus)) {
        info.totalMemoryBytes = memStatus.ullTotalPhys;
        info.usedMemoryBytes = memStatus.ullTotalPhys - memStatus.ullAvailPhys;
        info.memoryUsagePercent = static_cast<double>(info.usedMemoryBytes)
                                  / static_cast<double>(info.totalMemoryBytes) * 100.0;
    }

    if (m_vramAvailable && m_dxgiAdapter) {
        IDXGIAdapter *adapter = static_cast<IDXGIAdapter *>(m_dxgiAdapter);

        DXGI_ADAPTER_DESC desc;
        if (SUCCEEDED(adapter->GetDesc(&desc))) {
            info.totalVramBytes = desc.DedicatedVideoMemory;

            IDXGIAdapter3 *adapter3 = nullptr;
            if (SUCCEEDED(adapter->QueryInterface(IID_IDXGIAdapter3, reinterpret_cast<void **>(&adapter3)))) {
                DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
                if (SUCCEEDED(adapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo)))
                    info.usedVramBytes = memInfo.CurrentUsage;
                adapter3->Release();
            }
        }

        if (info.totalVramBytes > 0) {
            info.vramUsagePercent = static_cast<double>(info.usedVramBytes)
                                    / static_cast<double>(info.totalVramBytes) * 100.0;
        }
    }

    return info;
}

bool SystemMonitor::isVramAvailable() const
{
    return m_vramAvailable;
}
