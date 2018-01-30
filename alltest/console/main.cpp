#include <QCoreApplication>
#include <QtCore>
#include <QDebug>
#include <QString>
#include <QObject> 
#include <QFontMetrics> 
#include <QFont> 
#include <cstddef>     /* offsetof */
#include <QVector>
#include <QtMath>
#include <iostream>
#include <tr1/memory>
using namespace std;

QString createAss();
void testDate();
void testAudio();
void testArg();
void testMemoryLeak();
void testDouble();
void testSharePtr();
void testLevelDB();
void testInt();
void testSacle();

int main(int argc, char *argv[])
{
	cout << "programe start" << endl;
    QCoreApplication a(argc, argv);

    testSacle();

    //testInt();

    //testLevelDB();

    //testSharePtr();

    //testDouble();

    // testDate();

    // testAudio();

    // testArg();

    // testMemoryLeak();

    // createASpeed();

    //qDebug()<<"qLn: "<<qLn(10)<<" qPow: "<<qPow(3.3,2);

	cout << "programe end" << endl;
    return 0;//a.exec();
}
#include "leveldb/db.h"
#include "leveldb/write_batch.h"
#include "leveldb/slice.h"

void testLevelDB()
{
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "./test", &db);
    if(status.ok()){
        leveldb::Slice key = "key";
        leveldb::Slice value = "value";
        db->Put(leveldb::WriteOptions(), key, value);

        std::string strvalue = "";
        db->Get(leveldb::ReadOptions(), key, &strvalue);
        cout << "testLevelDB strvalue: " <<strvalue<< endl;
        delete db;
    }
    else
    {
        cout << "testLevelDB error status is not ok" << endl;
    }
}

class A {
public:
	A() {
		cout << "construct A!!!" << endl;
	}
	~A() {
		cout << "destruct A!!!" << endl;
	}
};
class B: public A {
public:
	B() {
		cout << "construct B!!!" << endl;
	}
	~B() {
		cout << "destruct B!!!" << endl;
	}
};
std::shared_ptr<B>  createSharePtr()
{
	std::shared_ptr<B> ptrB1(new B());
	return ptrB1;
}
void testSharePtr()
{
	cout << "testSharePtr start" << endl;
	createSharePtr();
	//std::shared_ptr<B> p = createSharePtr();
	cout << "testSharePtr end" << endl;
}

void testDouble()
{
    double video_pts_offset=0;
    double pts=252.035;
    int num=30222000;
    int den=504187;
    double dframenumber=(double)(pts+video_pts_offset) * (double)num/(double)den;
    //int64_t framenumber = round(dframenumber) + 1;
    int64_t framenumber = round((double)(pts+video_pts_offset) * (double)num/(double)den) + 1;
    printf("dframenumber: %f\n", dframenumber);
    printf("framenumber: %d\n", framenumber);
    qDebug() << fixed << qSetRealNumberPrecision(6)<<"framenumber: "<<framenumber<<" dframenumber: "<<(double)dframenumber;
}

QString createAss()
{
    QString qs = "[Script Info]\r\n"
"; Script generated by FFmpeg/Lavc57.24.102\n"
"ScriptType: v4.00+\n"
"PlayResX: 384\n"
"PlayResY: 288\n"
"\n"
"[V4+ Styles]\n"
"Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\n"
"Style: Default,Arial,16,&Hffffff,&Hffffff,&H0,&H0,0,0,0,0,100,100,0,0,1,1,0,2,10,10,10,0\n"
"\n"
"[Events]\n"
"Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\n"
"Dialogue: 0,0:00:00.01,0:00:00.94,Default,,0,0,0,,{\\move(0,0,60,120 [,100,500])}{\\fad(400,80)}{\\fs18\\t(130,500,\\fry360)}- Oh, stupid.\n"
"Dialogue: 0,0:00:01.01,0:00:01.99,Default,,0,0,0,,Go kill something.";
    qDebug(qs.toLatin1());
    return qs;
}

#if 0
class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent = 0) : QObject(parent) {}

public slots:
    void run()
    {
        // Do processing here
        QString b = A();
		qDebug() << "1 b: " << b;
        change();
		qDebug() << "2 b: " << b;
		qDebug() << "qs: " << createAss();
        emit finished();
    }
private:
    QString a;
    QString A(){
        a = "aaaaaaaaa";
        return a;
    }
    void change(){
        a = "bbbbbb";
    }
signals:
    void finished();
};
void testTask()
{
	// Task parented to the application so that it
    // will be deleted by the application.
    Task *task = new Task(&a);

    // This will cause the application to exit when
    // the task signals finished.    
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));
}
#include "main.moc"
#endif

void testDate()
{
    QString start = QDateTime(QDate::currentDate()).addMSecs(10).toString("yyyy/MM/dd hh:mm:ss.zzz");
    start.chop(1);
    qDebug() << "date: " << start;//.chop(1);
    qDebug() << "date: " << QDateTime(QDate::currentDate()).addMSecs(2000).toString("hh:mm:ss.zz");
}

void testAudio()
{
    int m_iStartTime = 1212;
    int m_iStartPoint = 0;
    int m_iEntPoint = 31233;
    int m_duration = 5000;
    int iRealLength = m_iEntPoint - m_iStartPoint+m_iStartTime;
    QString test = QString("[1:a]atrim=0:%1,asetpts=PTS-STARTPTS[aud1];"
                    "[2:a]atrim=%2:%3,asetpts=PTS-STARTPTS[aud2];"
                    "[aud1][aud2]concat=n=2:v=0:a=1[aout]").
            arg(QString::number((float)m_iStartTime/1000, 'f',2)).
            arg((float)m_iStartPoint/1000).
            arg(iRealLength<=m_duration?((float)m_iEntPoint/1000):((float)(m_duration-m_iStartTime+m_iStartPoint)/1000));
    qDebug() << test;
}

typedef struct STEffectDefine {
    // const char *name;
    QString name;
    const char *help;
    QString description;
    QVector<int> offset;
    QVector<int> type;
} STEffectsDefine;

typedef struct STEffectContext {
    STEffectContext(){frames=25;zoom=1.2;off_oy=0.2;}
    //common
    int frames;

    //
    double zoom;
    double off_oy;
}STEffectContext;
enum AVOptionType{
    AV_OPT_TYPE_FLAGS,
    AV_OPT_TYPE_INT,
    AV_OPT_TYPE_INT64,
    AV_OPT_TYPE_DOUBLE,
    AV_OPT_TYPE_FLOAT,
    AV_OPT_TYPE_STRING,
};
#define OFFSET(x) offsetof(struct STEffectContext, x)
#if 1
static const STEffectsDefine effects[] = {
    { "panup", "", "zoompan=z='%1':x='(zoom-1)*iw/2':oy='%2*ih':y='max(y-(zoom-1)*ih/2/%3,0)'", {OFFSET( zoom), OFFSET(off_oy), OFFSET(frames)} ,{AV_OPT_TYPE_DOUBLE, AV_OPT_TYPE_DOUBLE, AV_OPT_TYPE_INT}},
    //{ NULL }
};
#endif
void testArg()
{
#if 0
    QString test("arg1: %1 arg2: %2");
    qDebug() << test;
    qDebug() << test.arg("arg1").arg("arg2").arg("arg3");
    qDebug() << "end";
    return;
#endif
    // QMap<QString/*name*/, STEffectsDefine> mapstEffects;
    // mapstEffects.insert("panup", STEffectsDefine());
    STEffectContext c;
    //STEffectsDefine o;
    QString qsEffectName = "panup";
    char buf[128];
    int ret;
    //for(int i=0; !effects[i].name.isEmpty() ;i++)
    for(uint32_t i=0; i<sizeof(effects[0])/sizeof(effects);i++)
    {
        //QVector<QString> vecValue(20);
        if(effects[i].name.compare(qsEffectName)==0)
        {
            QString desc = effects[i].description;
            for(int32_t j=0; j<effects[i].offset.size(); j++)
            {
                switch (effects[i].type[j]){
                case AV_OPT_TYPE_DOUBLE: 
                    ret = snprintf(buf, sizeof(buf), "%.3f" ,     *(double *)(((char*)&c)+effects[i].offset[j])); 
                    break;
                case AV_OPT_TYPE_INT: ret = snprintf(buf, sizeof(buf), "%d" ,     *(int  *)(((char*)&c)+effects[i].offset[j])); break;
                }
                qDebug() << "i: " << i << " j: " << j << " type: " << effects[i].type[j] << " offset: " << effects[i].offset[j]<< " buf: " << buf;
                if ((uint32_t)ret >= sizeof(buf))
                {
                    qDebug()<<"error";
                }
                desc = desc.arg(buf);
                //vecValue[j] = buf;
            }
#if 0
            for(int j=0; j<effects[i].offset.size(); j++)
            {
                desc = desc.arg(vecValue[j]);
            }
#endif
#if 0
            switch(effects[i].offset.size()){
            case 1: desc.arg(vecValue[0]);break;
            case 2: desc.arg(vecValue[0]).arg(vecValue[1]);break;
            case 3: desc.arg(vecValue[0]).arg(vecValue[1]).arg(vecValue[2]);break;
            case 4: desc.arg(vecValue[0]).arg(vecValue[1]).arg(vecValue[2]).arg(vecValue[3]);break;
            case 5: desc.arg(vecValue[0]).arg(vecValue[1]).arg(vecValue[2]).arg(vecValue[3]).arg(vecValue[4]);break;
            }
#endif
            /*if(effects[i].offset.size()==3)
                qDebug() << desc.arg(vecValue[0]).arg(vecValue[1]).arg(vecValue[2]);*/
            //qDebug() << effects[i].description.arg(vecValue[0]).arg(vecValue[1]).arg(vecValue[2]).arg(vecValue[3]).arg(vecValue[4]).arg(vecValue[5]);
            qDebug() << desc;
            break;
        }
    }
    /*
    if( effects[0].offset.size() == 3)
        qDebug() << effects[0].description.arg(*(double*)(((char*)&c)+effects[0].offset[0]));
    */
    return;
#if 0
    // 1, 解析xml中的属性key、value: zoom=1.2 off_oy=0.2 frames=25
    // panup "zoompan=z='1.2':x='(zoom-1)*iw/2':oy='0.2*ih':y='max(y-(zoom-1)*ih/2/25,0)'"
    QString panup = "zoompan=z='%1':x='(zoom-1)*iw/2':oy='%2*ih':y='max(y-(zoom-1)*ih/2/%3,0)'";
    QVector<int> b;
    QVector<int> c = {1, 2, 3}; //{ b.clear(), b << 1 << 2 << 3;}
    qDebug() << c;
    // qDebug() << offsetof(struct STEffectContext, zoom_expr_str);
    struct STEffectContext a;
    // qDebug() << &(((struct STEffectContext*)0)->zoom_expr_str); //offsetof(struct STEffectContext, zoom_expr_str);
#endif
}
typedef struct La
{
    QString a;
    int b;
}La;
void testMemoryLeak()
{
    QVector<La*> vLa;
    vLa.push_back(new La());
    La* a=new La();
    for(int i=0; i<vLa.count(); i++)
    {
        qDebug()<<"i: "<<i<<" address: "<<(size_t)vLa[i];
    }
}
void createASpeed()
{
    double speed=0.25;
    //speed=2.1;
    QString vf;
    if(speed<0.5 || speed>2.0)
    {
        double divisor=1;
        QString onevf;
        if(speed<0.5)
        {
            divisor=0.5;
            onevf="atempo=0.5";
        }
        else if(speed>2.0)
        {
            divisor=2.0;
            onevf="atempo=2.0";
        }
        int bei=qFloor(qLn(speed)/qLn(divisor));
        double base=qPow(divisor, bei);
        double x=speed/base;
        do{
            vf.append(vf.isEmpty()?"":",").append(onevf);
        }while((--bei)>0);
        if(x!=1)
        {
            vf.append(vf.isEmpty()?"":",").append(QString("atempo=%1").arg(x));
        }
        qDebug()<<"speed: "<<speed<<" divisor: "<<divisor<<" bei: "<<bei<<" base: "<<base<<" x: "<<x;
        qDebug()<<"vf: "<<vf;
    }
}

void testInt()
{
    int iRotate=-630, iHFlip=0, iVFlip=0;
    if(iRotate>=360)
    {
        iRotate=iRotate%360;
    }
    else if(iRotate<0)
    {
        iRotate=iRotate%(-360)+360;
    }
    qDebug()<<"testInt iRotate: "<<iRotate;

}
void testSacle()
{
    QSize source_size(1024, 786);
    //source_size.scale(1280, 720, Qt::KeepAspectRatio);
    qDebug()<<"testSacle width: "<<source_size.width()<<" height: "<<source_size.height(); //938, 720
    source_size.scale(1280, 720, Qt::KeepAspectRatioByExpanding);
    qDebug()<<"testSacle width: "<<source_size.width()<<" height: "<<source_size.height(); //1280, 982
}
