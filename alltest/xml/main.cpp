#include <QCoreApplication>
#include <QtCore>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("C:\\qtproject\\alltest\\xml\\1.fmp");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() <<"can't open file";
        return -1;
    }
    QXmlStreamReader reader(&file);

    //int i=0;
    while (!reader.atEnd()) 
    {
         reader.readNext();
         //qDebug()<< "i: " << i++;
         if (reader.tokenType() == QXmlStreamReader::StartElement 
                 && reader.name() == "ImageItem") 
         {
             qDebug()<< "ImageItem id: " << (reader.attributes().hasAttribute("id")?reader.attributes().value("id").toString().toInt():-1);
             qDebug()<< "ImageItem filePath: " << reader.attributes().value("filePath").toString();
             while (!reader.atEnd()) {
                 reader.readNext();
                 //qDebug()<< "ImageItem element: " << reader.name();
                 if (reader.tokenType() == QXmlStreamReader::StartElement
                         && reader.name() == "TitleClip") 
                 {
                     while (!reader.atEnd()) {
                         reader.readNext();
                         //qDebug()<< "TitleClip element: " << reader.name();
                         if (reader.tokenType() == QXmlStreamReader::StartElement
                                 && reader.name() == "BoundProperties") 
                         {
                             while (!reader.atEnd()) {
                                 reader.readNext();
                                 //qDebug()<< "BoundProperties element: " << reader.name();
                                 if (reader.tokenType() == QXmlStreamReader::StartElement
                                         && reader.name() == "BoundPropertyString") 
                                 {
                                     qDebug()<< reader.attributes().value("Name").toString()<<" "<<reader.attributes().value("Value").toString();
                                 }
                                 else if (reader.tokenType() == QXmlStreamReader::StartElement
                                         && reader.name() == "BoundPropertyInt") 
                                 {
                                     qDebug()<< reader.attributes().value("Name").toString()<<" "<<reader.attributes().value("Value").toString().toInt();
                                 }
                                 else if (reader.tokenType() == QXmlStreamReader::StartElement
                                         && reader.name() == "BoundPropertyFloat") 
                                 {
                                     qDebug()<< reader.attributes().value("Name").toString()<<" "<<reader.attributes().value("Value").toString().toFloat();
                                 }
                                 else if (reader.tokenType() == QXmlStreamReader::EndElement
                                         && reader.name() == "BoundProperties") 
                                 {
                                     break;
                                 }
                             }
                         }
                         else if (reader.tokenType() == QXmlStreamReader::StartElement
                                 && reader.name() == "Transitions") 
                         {
                         }
                         else if (reader.tokenType() == QXmlStreamReader::EndElement
                                 && reader.name() == "TitleClip") 
                         {
                             break;
                         }
                     }
                 }
                 else if (reader.tokenType() == QXmlStreamReader::StartElement
                         && reader.name() == "Animation") 
                 {
                 }
                 else if (reader.tokenType() == QXmlStreamReader::StartElement
                         && reader.name() == "VideoClip") 
                 {
                 }
#if 0
                 else if (reader.tokenType() == QXmlStreamReader::EndElement
                         && reader.name() == "ImageItem") 
                 {
                     break;
                 }
#endif
             }
         }
         else if (reader.tokenType() == QXmlStreamReader::StartElement 
                 && reader.name() == "AudioItem") 
         {
         }
#if 0
         else if (reader.tokenType() == QXmlStreamReader::EndElement) {
             if (reader.name() == "") {
                 // end
             }
         }
#endif
    }
    qDebug()<< "end";
    return a.exec();
}
