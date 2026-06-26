#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QObject>
#include <QtGlobal>

struct SystemResourceInfo
{
    double cpuUsagePercent = 0.0;

    quint64 totalMemoryBytes = 0;
    quint64 usedMemoryBytes = 0;
    double memoryUsagePercent = 0.0;

    quint64 totalVramBytes = 0;
    quint64 usedVramBytes = 0;
    double vramUsagePercent = 0.0;
};

class SystemMonitor : public QObject
{
    Q_OBJECT

public:
    explicit SystemMonitor(QObject *parent = nullptr);
    ~SystemMonitor();

    SystemResourceInfo query();
    bool isVramAvailable() const;

private:
    bool initializeVram();
    void cleanupVram();

    quint64 fileTimeToUInt64(const void *fileTime) const;

    quint64 m_prevIdleTime = 0;
    quint64 m_prevKernelTime = 0;
    quint64 m_prevUserTime = 0;
    bool m_firstCpuQuery = true;

    bool m_vramAvailable = false;
    void *m_dxgiFactory = nullptr;
    void *m_dxgiAdapter = nullptr;
};

#endif // SYSTEMMONITOR_H
