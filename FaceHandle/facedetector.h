
#pragma once
#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H



#include <QImage>
#include <QString>
#include <QThread>
#include <QAtomicInt>
#include <QTimer>
#include <QMutex>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/gui_widgets.h>
#include <dlib/opencv.h>


#include <ConfigManager/basemanager.h>
#include <FaceHandle/cameracontrol.h>

using namespace cv;
using namespace std;
using namespace dlib;




class FaceDetector
{
public:
    static FaceDetector& instance(){
        static FaceDetector m_instance;
        return m_instance;
    }
    // 禁止拷贝构造
    FaceDetector(const FaceDetector&) = delete;
    FaceDetector& operator=(const FaceDetector&) = delete;


    void test_dlib();
    void UpdatePredictor(QString path);
    std::vector<dlib::full_object_detection> PredictImage(cv::Mat &img);
    //FaceInfo calculateFaceInfo(full_object_detection points);


    //面部旋转角计算
    static void computeFacePose(full_object_detection &landmarks, double &yaw, double &pitch, double &roll, double ori_width, double ori_height);
    static void rotationMatrixToEulerAngles(cv::Mat &R, double &yaw, double &pitch, double &roll);
    static void getFaceLandmarkFeatures(full_object_detection &landmarks, FaceInfo &data);

private:
    FaceDetector();
    ~FaceDetector();

    dlib::frontal_face_detector detector;
    dlib::shape_predictor sp;

};




class FaceDetectorThread : public QThread
{
    Q_OBJECT
public:
    static FaceDetectorThread &instance() {
        static FaceDetectorThread s_instance;
        return s_instance;
    }

    void run() override;
    QImage GetImage();
    void stophandle();
    void starthandle();

signals:
    void sigStartHandle();
    void sigStopHandle();

private:
    FaceDetectorThread();
    ~FaceDetectorThread();
    FaceDetectorThread(const FaceDetectorThread &) = delete;
    FaceDetectorThread &operator=(const FaceDetectorThread &) = delete;

    QImage n_image;
    QMutex m_mutex;
    QAtomicInt m_ready = 0;
};


class FaceDetectorWorker : public QObject
{
    Q_OBJECT
public:
    explicit FaceDetectorWorker(QMutex *mutex, QImage *sharedImage, QObject *parent = nullptr);
    ~FaceDetectorWorker();

public slots:
    void ontimeout();
    void onStartHandle();
    void onStopHandle();

private:
    void PrepareImage(QImage& image, cv::Mat& output);

    QMutex* m_mutex;
    QImage* m_sharedImage;
    bool is_processing = false;
    double x, y, height, width;
    bool m_running = false;

    friend class FaceDetectorThread;
};



#endif // FACEDETECTOR_H
