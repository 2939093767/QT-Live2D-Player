#include "config_widget.h"
#include <QLineEdit>
#include <QSpinBox>
#include <qpainter.h>
#include <qpropertyanimation.h>
#include <ui_config_widget.h>
#include <ui_AppWidget.h>
#include <ui_ModelWidget.h>
#include <QMessageBox>




config_widget::config_widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::config_widget)
{
    ui->setupUi(this);

}




config_widget::~config_widget()
{
    delete ui;
}



void config_widget::ui_init()
{

}

void config_widget::config_save()
{
    ui->scrollAreaWidgetContents->save_config();
    ui->scrollAreaWidgetContents_2->save_config();
    ui->tab_3->save_config();
    ConfigManager::instance().sync();
}



void config_widget::return_default()
{
    ConfigManager::instance().initDefaultConfig();
    QRC_Manager::instance().ClearMotion();

    foreach (QWidget *widget, this->findChildren<QWidget*>())
    {
        if (auto *frame = qobject_cast<config_card*>(widget))
        {
            frame->change_ui();
        }
    }

}

void config_widget::software_restart()
{
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath());
}

void config_widget::StartRender()
{
    //config_save();
    emit SignalStartRender();
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(true);
}

void config_widget::StopRender()
{
    emit SignalStopRender();
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_5->setEnabled(false);
}












void config_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 加载你的背景图（换成自己的资源路径）
    QPixmap pix(":/image/resource/images/pet.png");

    if (pix.isNull()) {
        QWidget::paintEvent(event);
        return;
    }

    // 窗口大小
    QSize widgetSize = size();
    int ww = widgetSize.width();
    int wh = widgetSize.height();

    // 图片原始大小
    int iw = pix.width();
    int ih = pix.height();

    // 计算缩放比例（以短边填满窗口为准）
    qreal scaleW = (qreal)ww / iw;
    qreal scaleH = (qreal)wh / ih;
    qreal scale = qMax(scaleW, scaleH);

    // 缩放图片
    int newW = iw * scale;
    int newH = ih * scale;
    pix = pix.scaled(newW, newH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 计算居中裁剪的坐标（裁中间）
    int x = (newW - ww) / 2;
    int y = (newH - wh) / 2;

    // 半透明
    painter.setOpacity(0.25);

    // 绘制：只画中间裁剪区域
    painter.drawPixmap(
        0, 0, ww, wh,
        pix,
        x, y, ww, wh
    );
}

void config_widget::showEvent(QShowEvent *event)
{
    static bool first_change = true;
    QWidget::showEvent(event);
    if(first_change){
        foreach (QWidget *widget, this->findChildren<QWidget*>())
        {
            if (auto *frame = qobject_cast<config_card*>(widget))
            {
                frame->change_ui();
            }
        }
        first_change = false;
    }
}










config_card::config_card(QWidget *parent)
    : QWidget(parent)

{


}

config_card::~config_card()
{
    p_config_widgets.clear();
}





/**********************************************
功能选项卡
**********************************************/
AppWidget::AppWidget(QWidget *parent)
    : config_card(parent),
    ui(new Ui::AppWidget)
{
    ui->setupUi(this);
    ui->label_4->setText(R"(
        注意:
        1、本页设置保存后生效
        2、切换模型请在保存后重启APP
    )");
}


AppWidget::~AppWidget()
{

}


void AppWidget::change_ui()
{
    p_config_widgets.clear();
    ui->checkBox->setChecked(ConfigManager::instance().getValue(CONFIG_APP_SOFT_START).toBool());
    p_config_widgets[CONFIG_APP_SOFT_START] = ui->checkBox;
    // ui->checkBox_2->setChecked(ConfigManager::instance().getValue(CONFIG_APP_ISOPACITY).toBool());
    // p_config_widgets[CONFIG_APP_ISOPACITY] = ui->checkBox_2;
    ui->comboBox->setCurrentIndex(ConfigManager::instance().getValue(CONFIG_APP_MODEL_CONTROL).toInt());
    p_config_widgets[CONFIG_APP_MODEL_CONTROL] = ui->comboBox;
    ui->comboBox_2->setCurrentIndex(ConfigManager::instance().getValue(CONFIG_APP_FPS).toInt());
    p_config_widgets[CONFIG_APP_FPS] = ui->comboBox_2;
    ui->lineEdit->setText(ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER).toString());
    p_config_widgets[CONFIG_MODEL_FOLDER] = ui->lineEdit;
}





void AppWidget::save_config()
{

    auto setValue = [&](const QString& key, auto& member) {
        QWidget* w = p_config_widgets.value(key);
        if (!w) return;
        if (auto* spin = qobject_cast<QCheckBox*>(w))
            member = spin->isChecked();
        else if (auto* dspin = qobject_cast<QComboBox*>(w))
            member = dspin->currentIndex();
        else if (auto* edit = qobject_cast<QLineEdit*>(w))
            member = edit->text();
        else if (auto* check = qobject_cast<QRadioButton*>(w))
            member = check->isChecked();
    };


    for(QString key:p_config_widgets.keys()){
        QVariant member;
        setValue(key,member);
        ConfigManager::instance().setValue(key,member);
        qDebug()<<key << " : "<<member;
    }
    ConfigManager::instance().setAutoStart(ui->checkBox->isChecked());

}

void AppWidget::return_default()
{

}

void AppWidget::folder_choose()
{
    QString folderPath = QFileDialog::getExistingDirectory(
        this,
        "选择文件夹",          // 弹窗标题
        QCoreApplication::applicationDirPath(), // 默认打开路径
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if(folderPath != ""){
        ui->lineEdit->setText(folderPath);
    }


}











ModelWidget::ModelWidget(QWidget* parent)
    : config_card(parent),
      ui(new Ui::ModelWidget)
{

    ui->setupUi(this);
    ui->widget->setLayout(new QVBoxLayout());
    ui->widget_2->setLayout(new QVBoxLayout());
    GlobalKeyHook::instance().start();
    //this->change_ui();

}



ModelWidget::~ModelWidget()
{
    israndom_ex = false;
    israndom_mo = false;
    GlobalKeyHook::instance().stop();
}

void ModelWidget::change_ui()
{
    QList<QWidget*> childs = ui->widget->findChildren<QWidget*>();
    for (QWidget* w : childs)
    {
        w->setParent(nullptr);
        w->deleteLater();
    }
    QList<QWidget*> childs_2 = ui->widget_2->findChildren<QWidget*>();
    for (QWidget* w : childs_2)
    {
        w->setParent(nullptr);
        w->deleteLater();
    }

    p_config_widgets.clear();


    //ui->comboBox->setCurrentText(ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER).toString());
    //p_config_widgets[CONFIG_MODEL_FOLDER] = ui->comboBox;

    auto expressionNames = LAppLive2DManager::GetInstance()->GetModel(0)->GetExpressionNames();
    auto motions= LAppLive2DManager::GetInstance()->GetModel(0)->GetMotionNames();
    //初始化信息和ui


    for(int i=0;i<expressionNames.GetSize();i++){
        motion_unit unit;
        unit.name = QString::fromStdString(expressionNames[i].GetRawString());
        unit.type = EXPRESSION;
        add_expression(unit.name);
        QRC_Manager::instance().ChangeMotionValue(unit.name,unit);
    }
    for(int i=0;i<motions.GetSize();i++){
        motion_unit unit;
        unit.name = QString::fromStdString(motions[i].GetRawString());
        unit.type = MOTION;
        add_motions(unit.name);
        QRC_Manager::instance().ChangeMotionValue(unit.name,unit);
    }

}





void ModelWidget::save_config()
{
    auto setValue = [&](const QString& key, auto& member) {
        QWidget* w = p_config_widgets.value(key);
        if (!w) return;
        if (auto* spin = qobject_cast<QCheckBox*>(w))
            member = spin->isChecked();
        else if (auto* dspin = qobject_cast<QComboBox*>(w))
            member = dspin->currentText();
        else if (auto* edit = qobject_cast<QLineEdit*>(w))
            member = edit->text();
        else if (auto* check = qobject_cast<QRadioButton*>(w))
            member = check->isChecked();
    };


    for(QString key:p_config_widgets.keys()){
        QVariant member;
        setValue(key,member);
        ConfigManager::instance().setValue(key,member);
        qDebug()<<key << " : "<<member;
    }

}

void ModelWidget::return_default()
{

}

void ModelWidget::image_choose(QString name, QLineEdit *ui)
{

}

void ModelWidget::add_expression(QString text)
{

    qDebug()<<text;
    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::Box);       // 显示边框
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(1);

    // 2. 创建水平布局（想垂直就改成 QVBoxLayout）
    QHBoxLayout *layout = new QHBoxLayout(frame);
    layout->setSpacing(10);    // 控件间距
    layout->setContentsMargins(8, 8, 8, 8);   // 内边距

    // 3. 依次添加控件
    QLabel *label = new QLabel(text);
    QPushButton *button = new QPushButton("快键方式启用");
    button->setCheckable(true);
    connect(button,&QPushButton::toggled,this,&ModelWidget::QuickkeyUpdate);
    QCheckBox *checkBtn = new QCheckBox("该项禁用");

    connect(checkBtn,&QCheckBox::clicked,this,[=](bool clicked){
        QMap<QString,motion_unit> value;
        QRC_Manager::instance().MotionQuery(MOTION_ONCEMOTION,text,value);
        value[text].isuse = !clicked;
        QRC_Manager::instance().ChangeMotionValue(text,value[text]);
    });

    QComboBox* hitarea = new QComboBox();
    auto Setting = LAppLive2DManager::GetInstance()->GetModel(0)->GetSetting();
    hitarea->addItem("无");
    for(int i=0;i < Setting->GetHitAreasCount();i++){
        hitarea->addItem(Setting->GetHitAreaName(i));
    }
    CheckComboBox* keySequence= new CheckComboBox();

    // 4. 加入布局
    layout->addWidget(label);
    layout->addWidget(hitarea);
    layout->addWidget(keySequence);
    layout->addWidget(button);
    layout->addWidget(checkBtn);

    // 5. 让输入框自动拉伸
    layout->setStretchFactor(hitarea, 1);
    layout->setStretchFactor(keySequence, 1);
    ui->widget->layout()->addWidget(frame);
    //layout->replaceWidget(ui->frame, frame);

}

void ModelWidget::add_motions(QString text)
{

    qDebug()<<text;
    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::Box);       // 显示边框
    frame->setFrameShadow(QFrame::Sunken);
    frame->setLineWidth(1);

    // 2. 创建水平布局（想垂直就改成 QVBoxLayout）
    QHBoxLayout *layout = new QHBoxLayout(frame);
    layout->setSpacing(10);    // 控件间距
    layout->setContentsMargins(8, 8, 8, 8);   // 内边距

    // 3. 依次添加控件
    QLabel *label = new QLabel(text);
    QPushButton *button = new QPushButton("快键方式启用");
    button->setCheckable(true);
    connect(button,&QPushButton::toggled,this,&ModelWidget::QuickkeyUpdate);
    QCheckBox *checkBtn = new QCheckBox("该项禁用");

    connect(checkBtn,&QCheckBox::clicked,this,[=](bool clicked){
        QMap<QString,motion_unit> value;
        QRC_Manager::instance().MotionQuery(MOTION_ONCEMOTION,text,value);
        value[text].isuse = !clicked;
        QRC_Manager::instance().ChangeMotionValue(text,value[text]);
    });

    QComboBox* hitarea = new QComboBox();
    auto Setting = LAppLive2DManager::GetInstance()->GetModel(0)->GetSetting();
    hitarea->addItem("无");
    for(int i=0;i < Setting->GetHitAreasCount();i++){
        hitarea->addItem(Setting->GetHitAreaName(i));
    }
    CheckComboBox* keySequence= new CheckComboBox();

    // 4. 加入布局
    layout->addWidget(label);
    layout->addWidget(hitarea);
    layout->addWidget(keySequence);
    layout->addWidget(button);
    layout->addWidget(checkBtn);

    // 5. 让输入框自动拉伸
    layout->setStretchFactor(hitarea, 1);
    layout->setStretchFactor(keySequence, 1);
    ui->widget_2->layout()->addWidget(frame);


}










void ModelWidget::QuickkeyUpdate(bool clicked)
{
    qDebug()<<"按下按钮";
    // 1. 获取点击的按钮（临时按钮）
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // 2. 获取父容器 + 水平布局
    QWidget* parentWidget = btn->parentWidget();
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(parentWidget->layout());
    if (!layout) return;

    int btnIndex = layout->indexOf(btn);
    QComboBox* edit_1 = qobject_cast<QComboBox*>(layout->itemAt(btnIndex - 1)->widget());
    QComboBox* edit_2 = qobject_cast<QComboBox*>(layout->itemAt(btnIndex - 2)->widget());
    QLabel* edit_3 = qobject_cast<QLabel*>(layout->itemAt(btnIndex - 3)->widget());
    QMap<QString,motion_unit> value;
    QRC_Manager::instance().MotionQuery(MOTION_ONCEMOTION,edit_3->text(),value);
    motion_unit unit = value[edit_3->text()];

    unit.hitarea = edit_2->currentText();
    unit.quickkey = edit_1->currentText();
    unit.quickkey_isuse = clicked;

    qDebug()<<unit.hitarea<<unit.quickkey_isuse;
    //添加动作
    QRC_Manager::instance().ChangeMotionValue(unit.name,unit);
}




void ModelWidget::expression_israndom(bool clicked)
{
    israndom_ex = clicked;
    if(clicked){
        QFuture<void> future = QtConcurrent::run([=](){
            while (israndom_ex)
            {
                QMap<QString,motion_unit> expression;
                QRC_Manager::instance().MotionQuery(MOTION_ACTIVEEXPRESSION,"",expression);
                int randomIndex = QRandomGenerator::global()->bounded(expression.size());
                auto it = expression.begin() + randomIndex;
                LAppLive2DManager::GetInstance()->GetModel(0)->SetExpression(it->name.toUtf8());
                QThread::msleep(ui->spinBox->value()*1000);
            }
        });
    }
}




void ModelWidget::motion_israndom(bool clicked)
{
    israndom_mo = clicked;
    if(clicked){
        qDebug()<<"随机动作开启";
        QFuture<void> future = QtConcurrent::run([=](){
            while (israndom_mo)
            {
                QMap<QString,motion_unit> expression;
                QRC_Manager::instance().MotionQuery(MOTION_ACTIVEMOTION,"",expression);
                int randomIndex = QRandomGenerator::global()->bounded(expression.size());
                auto it = expression.begin() + randomIndex;
                QStringList parts = it->name.split('_');
                QString group = parts[0];               // 前面：字符串
                int no = parts[1].toInt();    // 后面：转int
                qDebug()<<group<<no;
                LAppLive2DManager::GetInstance()->GetModel(0)->StartMotion(group.toUtf8(),no,1);
                QThread::msleep(ui->spinBox_2->value()*1000);
            }
        });
    }
}





// bool ModelWidget::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
// {
//     Q_UNUSED(eventType)

//     MSG* msg = reinterpret_cast<MSG*>(message);
//     if (msg->message == WM_HOTKEY)
//     {
//         int vk = LOWORD(msg->lParam);       // 按键码 如 VK_A, '1', etc
//         int mod = HIWORD(msg->lParam);     // 修饰键 Ctrl/Alt/Shift


//         int hotId = static_cast<int>(msg->wParam);
//


//         // 3. 补发键盘消息 → 让系统正常输入字符（关键！）
//         keybd_event((BYTE)vk, 0, 0, 0);        // 按下
//         keybd_event((BYTE)vk, 0, KEYEVENTF_KEYUP, 0); // 松开

//         // 4. 必须返回 true，表示我们处理了
//         return true;
//     }
//     return false;
// }

