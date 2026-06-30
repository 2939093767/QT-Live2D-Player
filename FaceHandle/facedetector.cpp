#include "facedetector.h"

#include <qDebug>

cv::Mat object_points = (cv::Mat_<double>(68, 3) <<
                         -73.393523, -29.801432, -47.667532,
                         -72.775014, -10.949766, -45.909403,
                         -70.533638,   7.929818, -44.842580,
                         -66.850058,  26.074280, -43.141114,
                         -59.790187,  42.564390, -38.635298,
                         -48.368973,  56.481080, -30.750622,
                         -34.121101,  67.246992, -18.456453,
                         -17.875411,  75.056892,  -3.609035,
                         0.098749,  77.061286,   0.881698,
                         17.477031,  74.758448,  -5.181201,
                         32.648966,  66.929021, -19.176563,
                         46.372358,  56.311389, -30.770570,
                         57.343480,  42.419126, -37.628629,
                         64.388482,  25.455880, -40.886309,
                         68.212038,   6.990805, -42.281449,
                         70.486405, -11.666193, -44.142567,
                         71.375822, -30.365191, -47.140426,
                         -61.119406, -49.361602, -14.254422,
                         -51.287588, -58.769795,  -7.268147,
                         -37.804800, -61.996155,  -0.442051,
                         -24.022754, -61.033399,   6.606501,
                         -11.635713, -56.686759,  11.967398,
                         12.056636, -57.391033,  12.051204,
                         25.106256, -61.902186,   7.315098,
                         38.338588, -62.777713,   1.022953,
                         51.191007, -59.302347,  -5.349435,
                         60.053851, -50.190255, -11.615746,
                         0.653940, -42.193790,  13.380835,
                         0.804809, -30.993721,  21.150853,
                         0.992204, -19.944596,  29.284036,
                         1.226783,  -8.414541,  36.948060,
                         -14.772472,   2.598255,  20.132003,
                         -7.180239,   4.751589,  23.536684,
                         0.555920,   6.562900,  25.944448,
                         8.272499,   4.661005,  23.695741,
                         15.214351,   2.643046,  20.858157,
                         -46.047290, -37.471411,  -7.037989,
                         -37.674688, -42.730510,  -3.021217,
                         -27.883856, -42.711517,  -1.353629,
                         -19.648268, -36.754742,   0.111088,
                         -28.272965, -35.134493,   0.147273,
                         -38.082418, -34.919043,  -1.476612,
                         19.265868, -37.032306,   0.665746,
                         27.894191, -43.342445,  -0.247660,
                         37.437529, -43.110822,  -1.696435,
                         45.170805, -38.086515,  -4.894163,
                         38.196454, -35.532024,  -0.282961,
                         28.764989, -35.484289,   1.172675,
                         -28.916267,  28.612716,   2.240310,
                         -17.533194,  22.172187,  15.934335,
                         -6.684590,  19.029051,  22.611355,
                         0.381001,  20.721118,  23.748437,
                         8.375443,  19.035460,  22.721995,
                         18.876618,  22.394109,  15.610679,
                         28.794412,  28.079924,   3.217393,
                         19.057574,  36.298248,  14.987997,
                         8.956375,  39.634575,  22.554245,
                         0.381549,  40.395647,  23.591626,
                         -7.428895,  39.836405,  22.406106,
                         -18.160634,  36.677899,  15.121907,
                         -24.377490,  28.677771,   4.785684,
                         -6.897633,  25.475976,  20.893742,
                         0.340663,  26.014269,  22.220479,
                         8.444722,  25.326198,  21.025520,
                         24.474473,  28.323008,   5.712776,
                         8.449166,  30.596216,  20.671489,
                         0.205322,  31.408738,  21.903670,
                         -7.198266,  30.844876,  20.328022
                         );


FaceDetector::FaceDetector()
{

    detector = dlib::get_frontal_face_detector();
    // 加载68点关键点检测器
    UpdatePredictor(R"(E:\qt_program\QT-Live2D-Player\src\shape_predictor_68_face_landmarks.dat)");


}

FaceDetector::~FaceDetector()
{

}

void FaceDetector::UpdatePredictor(QString path){
    dlib::deserialize(path.toStdString()) >> sp;
    qDebug()<< "sp's parts number:" << sp.num_parts();
}



std::vector<dlib::full_object_detection> FaceDetector::PredictImage(cv::Mat& img){



    array2d<unsigned char> dlib_img;
    cv::Mat gray;
    cv::cvtColor(img, gray, COLOR_RGBA2GRAY);
    dlib::assign_image(dlib_img, dlib::cv_image<unsigned char>(gray));

    std::vector<dlib::rectangle> dets = detector(dlib_img);
    cout << "Number of faces detected: " << dets.size() << endl;
    std::vector<dlib::full_object_detection> shapes;


    //win.add_overlay(dets, rgb_pixel(255,0,0));
    Mat whiteImg = Mat::ones(img.size(), CV_8UC4);
    whiteImg = cv::Scalar(255, 255, 255, 255);

    for (unsigned long j = 0; j < dets.size(); ++j)
    {
        dlib::full_object_detection shape = sp(dlib_img, dets[j]);
        // 绘制人脸框
        shapes.push_back(shape);

        // 绘制人脸框
        dlib::rectangle rect = dets[j];
        cv::rectangle(whiteImg,
                      Point(rect.left(), rect.top()),
                      Point(rect.right(), rect.bottom()),
                      Scalar(0, 255, 0, 255), 2);


        // 绘制 68 个关键点
        for (int i = 0; i < 68; ++i)
        {
            int x = shape.part(i).x();
            int y = shape.part(i).y();
            cv::circle(whiteImg, Point(x, y), 2, Scalar(0, 0, 255, 255), -1);
            // char buf[10];
            // sprintf(buf, "%d", i+1);
            // cv::putText(whiteImg,buf,Point(x, y),cv::FONT_HERSHEY_SIMPLEX, // 字体
            //         0.5,            // 大小
            //         cv::Scalar(0, 255, 0), // 颜色 (BGR)
            //         2    );
            //qDebug()<<i+1<<":"<<shape.part(i).x()<<shape.part(i).y();
        }
    }

    whiteImg.copyTo(img);
    //qDebug()<<"检测成功 ： "<<shapes.size();
    return shapes;
}





void FaceDetector::computeFacePose(full_object_detection& landmarks, double &yaw, double &pitch, double &roll , double ori_width, double ori_height)
{
    qDebug()<<ori_width<<ori_width;
    int value = ConfigManager::instance().getValue(CONFIG_MOTION_IMAGE_SIZE).toInt();
    value = 1;
    double height,width;
    if(value == 0){
        height = 480;
        width = 640;
    }else if(value == 1){
        height = 720;
        width = 1280;
    }else if(value == 2){
        height = 1080;
        width = 1920;
    }


    //2d关键点转换
    std::vector<cv::Point2f> imagePoints;
    for(int i=0;i<landmarks.num_parts();i++){
        imagePoints.push_back(cv::Point2f(landmarks.part(i).x(),landmarks.part(i).y()));
    }
    //相机内参
    // 裁剪区域（来自你的QRect，必须是原图上的坐标）
    QRect cropRect = ConfigManager::instance().getValue(CONFIG_IMAGE_CROP_RECT).toRect();
    if(cropRect.isEmpty()){
        cropRect = QRect(0,0,ori_width,ori_height);
    }


    // ==========================
    // 🔥 核心：计算正确相机内参
    // ==========================
    double original_focal = ori_width * 0.52;
    double original_cx = ori_width / 2.0;
    double original_cy = ori_height / 2.0;

    // 1. 焦距整体缩放
    double scale_w = (double)width / cropRect.width();
    double scale_h = (double)height / cropRect.height();
    double new_focal = original_focal * scale_w;

    // 2. 中心点：先偏移裁剪，再缩放
    double new_cx = (original_cx - cropRect.x()) * scale_w;
    double new_cy = (original_cy - cropRect.y()) * scale_h;

    // 构建最终内参
    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
                                 new_focal, 0, new_cx,
                             0, new_focal, new_cy,
                             0, 0, 1);

    cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);
    //pnp解算
    cv::Mat rotation_vector,translation_vector;
    cv::solvePnP(object_points, imagePoints, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
    //旋转矩阵生成
    cv::Mat R,k,t;
    cv::Rodrigues(rotation_vector, R);
    cv::Mat projMat = cv::Mat::zeros(3, 4, CV_64F);
    R.copyTo(projMat(cv::Rect(0, 0, 3, 3)));          // 把 R 放进前3列
    translation_vector.copyTo(projMat(cv::Rect(3, 0, 1, 3))); // 把 t 放进第4列
    //矩阵转欧拉角
    cv::Mat euler; // 用来存欧拉角
    cv::decomposeProjectionMatrix(projMat,k, R, t,cv::noArray(),cv::noArray(),cv::noArray(),euler); // 欧拉角输出在这里);
    //欧拉角二相位姿转换正面。
    pitch = euler.at<double>(0);
    yaw   = euler.at<double>(1);
    roll  = euler.at<double>(2);
    if(pitch < -90){
        pitch += 180;
    }
    if(yaw < -90){
        yaw += 180;
    }
    if(roll < -90){
        roll += 180;
    }

}


float distance(const Point& a, const Point& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

// 工具函数：三点弯曲程度（曲率近似值）
// 原理：中点到直线的垂直距离 / 线段长度 → 越大越弯
float curveDegree(const Point& s, const Point& mid, const Point& e) {
    // 线段se长度
    float lineLen = distance(s, e);
    if (lineLen < 1e-6) return 0;

    // 点mid到直线se的垂直距离
    float A = e.y - s.y;
    float B = s.x - e.x;
    float C = e.x*s.y - s.x*e.y;
    float dist = fabs(A * mid.x + B * mid.y + C) / sqrt(A*A + B*B);

    // 归一化弯曲程度 = 距离 / 长度
    return dist / lineLen;
}

void FaceDetector::getFaceLandmarkFeatures(full_object_detection& landmarks, FaceInfo& data)
{

    // ------------------------------
    // 左眼 (36~41)
    // ------------------------------
    Point2d le_l(landmarks.part(36).x(),landmarks.part(36).y());   // 左眼角
    Point2d le_r(landmarks.part(39).x(),landmarks.part(39).y());   // 右眼角
    Point2d le_t(landmarks.part(37).x(),landmarks.part(37).y());   // 上眼睑中点
    Point2d le_b(landmarks.part(41).x(),landmarks.part(41).y());   // 下眼睑中点
    data.left_eye_height = distance(le_t,le_b);
    data.left_eye_angle = curveDegree(le_l, le_t, le_r);

    // ------------------------------
    // 右眼 (42~47)
    // ------------------------------
    Point2d re_l(landmarks.part(42).x(),landmarks.part(42).y());
    Point2d re_r(landmarks.part(45).x(),landmarks.part(45).y());
    Point2d re_t(landmarks.part(43).x(),landmarks.part(43).y());
    Point2d re_b(landmarks.part(47).x(),landmarks.part(47).y());

    data.right_eye_height = distance(le_t,le_b);
    data.right_eye_angle = curveDegree(le_l, le_t, le_r);

    // ------------------------------
    // 上唇 (48~54)
    // ------------------------------
    // 嘴角左右端点
    Point2d mouth_left(landmarks.part(48).x(),landmarks.part(48).y());
    Point2d mouth_right(landmarks.part(54).x(),landmarks.part(54).y());

    // 上唇中点、下唇中点（计算上下唇之间距离）
    Point2d upper_mid(landmarks.part(62).x(),landmarks.part(62).y());
    Point2d lower_mid(landmarks.part(66).x(),landmarks.part(66).y());

    // 1. 上下嘴唇之间的长度（嘴开合高度）
    data.mouth_height= distance(upper_mid, lower_mid);
    // 3. 上唇弯曲程度
    double updegree = curveDegree(mouth_left, upper_mid, mouth_right);
    // 4. 下唇弯曲程度（整体嘴部弯曲程度）
    double downdegree = curveDegree(mouth_left, lower_mid, mouth_right);
    data.mouth_angle = (updegree+downdegree)/2.0;


}







void FaceDetector::test_dlib()
{
    //图片准备
    //Mat cv_img = imread(R"(C:\Users\ConneR\Desktop\FaceTidyup.png)");

    try
    {

        frontal_face_detector detector = get_frontal_face_detector();
        //dlib::deserialize(R"(E:\qt_program\QT-Live2D-Player\src\shape_predictor_68_face_landmarks.dat)") >> sp;
        image_window win;
        array2d<unsigned char> img;
        load_image(img, R"(C:\Users\ConneR\Desktop\test.jpg)");
        //load_image(img, R"(E:\qt_program\QT-Live2D-Player\src\build_path\Desktop_Qt_6_5_3_MSVC2019_64bit-Release\release\result.jpg)");
        pyramid_up(img);



        std::vector<dlib::rectangle> dets = detector(img);
        cout << "Number of faces detected: " << dets.size() << endl;



        win.clear_overlay();
        win.set_image(img);
        //win.add_overlay(dets, rgb_pixel(255,0,0));
        for (unsigned long j = 0; j < dets.size(); ++j)
        {
            dlib::full_object_detection shape = sp(img, dets[j]);
            // 绘制人脸框
            //dlib::rectangle rect = dets[j];
            win.add_overlay(dets[j], rgb_pixel(255,0,0));
            win.add_overlay(shape);
            //qDebug()<<shape.num_parts();
            // // 绘制 68 个关键点
            // for (int i = 0; i < 68; ++i)
            // {
            //     win.add_overlay((shape.part(i).x(),shape.part(i).y()),rgb_pixel(255,0,0));
            //     qDebug()<<shape.part(i).x()<<shape.part(i).y();
            // }
        }
        cout << "Hit enter to process the next image..." << endl;
        win.wait_until_closed();
        cin.get();
    }

    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}






FaceDetectorWorker::FaceDetectorWorker(QMutex *mutex, QImage *sharedImage, QObject *parent)
    : QObject(parent)
    , m_mutex(mutex)
    , m_sharedImage(sharedImage)
{
}

FaceDetectorWorker::~FaceDetectorWorker()
{
}

void FaceDetectorWorker::onStartHandle()
{
    m_running = true;
}

void FaceDetectorWorker::onStopHandle()
{
    m_running = false;
}

void FaceDetectorWorker::ontimeout()
{
    if (!m_running) return;
    if (is_processing) return;
    //qDebug();
    static double paw, pitch, roll;
    static QElapsedTimer timer;

    is_processing = true;
    timer.start();
    QImage image = CameraOpen::instance().GetNowImage();
    if (image.isNull()) {
        is_processing = false;
        return;
    }
    cv::Mat mat;
    PrepareImage(image, mat);
    auto shape_part = FaceDetector::instance().PredictImage(mat);
    if (shape_part.size()) {
        FaceDetector::computeFacePose(shape_part[0], paw, pitch, roll, image.width(), image.height());
        FaceInfo data(paw, pitch, roll);
        FaceDetector::getFaceLandmarkFeatures(shape_part[0], data);

        char text[50];
        sprintf(text, "yaw: %.2f", paw);
        cv::putText(mat, text, cv::Point(20, 20), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        sprintf(text, "Pitch: %.2f", pitch);
        cv::putText(mat, text, cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        sprintf(text, "Roll: %.2f", roll);
        cv::putText(mat, text, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        sprintf(text, "left_eye: %.2f", data.left_eye_height);
        cv::putText(mat, text, cv::Point(20, 140), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        sprintf(text, "right_eye: %.2f", data.right_eye_height);
        cv::putText(mat, text, cv::Point(20, 180), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        sprintf(text, "mouth: %.2f", data.mouth_height);
        cv::putText(mat, text, cv::Point(20, 220), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        QRC_Manager::instance().ChangeFaceInfo(data);
    }

    if (m_mutex && m_sharedImage) {
        QMutexLocker locker(m_mutex);
        *m_sharedImage = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA8888).copy();
    }

    qint64 ms = timer.elapsed();
    qDebug() << "算法运行耗时：" << ms << "毫秒";
    is_processing = false;
}

void FaceDetectorWorker::PrepareImage(QImage& image, cv::Mat& output)
{
    int value = ConfigManager::instance().getValue(CONFIG_MOTION_IMAGE_SIZE).toInt();
    value = 1;
    if(value == 0){
        height = 480;
        width = 640;
    }else if(value == 1){
        height = 720;
        width = 1280;
    }else if(value == 2){
        height = 1080;
        width = 1920;
    }


    output = cv::Mat(
        image.height(),
        image.width(),
        CV_8UC4,
        image.bits(),
        image.bytesPerLine()
    );
    QRect rect = ConfigManager::instance().getValue(CONFIG_IMAGE_CROP_RECT).toRect();
    if (!rect.isEmpty()) output = output(cv::Rect(rect.x(), rect.y(), rect.width(), rect.height()));
    cv::resize(output, output, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
}

void FaceDetectorThread::run()
{
    FaceDetectorWorker worker(&m_mutex, &n_image);
    connect(&CameraOpen::instance(), &CameraOpen::ImageSend, &worker, &FaceDetectorWorker::ontimeout, Qt::QueuedConnection);
    connect(this, &FaceDetectorThread::sigStartHandle, &worker, &FaceDetectorWorker::onStartHandle);
    connect(this, &FaceDetectorThread::sigStopHandle, &worker, &FaceDetectorWorker::onStopHandle);
    m_ready.storeRelease(1);
    exec();
}

FaceDetectorThread::FaceDetectorThread()
{
    start();
    setPriority(QThread::LowPriority);
}

FaceDetectorThread::~FaceDetectorThread()
{
    quit();
    wait();
}

void FaceDetectorThread::stophandle()
{
    if (!m_ready.loadAcquire()) return;
    emit sigStopHandle();
}

void FaceDetectorThread::starthandle()
{
    while (!m_ready.loadAcquire()) {
        msleep(5);
    }
    emit sigStartHandle();
}

QImage FaceDetectorThread::GetImage()
{
    QMutexLocker locker(&m_mutex);
    return n_image.copy();
}
