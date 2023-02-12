#include <QXmlStreamReader>
#include <QDir>
#include <QDebug>
#include <QMap>


QMap<QString, QString> readTranslations(const QString & filename)
{
    QMap<QString, QString> translationsMap;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file:" << filename;
        return translationsMap;
    }
    QXmlStreamReader xml(&file);

    QString sourceText;
    QString translationText;

    while (!xml.atEnd() && !xml.hasError()){

        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartDocument) continue;

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "source") {
                xml.readNext();
                sourceText = xml.text().toString();
            } else if (xml.name() == "translation") {
                xml.readNext();
                translationText = xml.text().toString();
                if (!sourceText.isEmpty() && !translationText.isEmpty()) {
                    translationsMap.insert(sourceText, translationText);
                }
                sourceText = QString();
            }
        }
    }

    return translationsMap;
}

QString getTranslation(QString source,
                       const QMap<QString, QString> translationsMap)
{
    QString tr = translationsMap.value(source);

    // Sometimes we use ` and ' while heidi uses "
    // TODO: use "

    // Sometime Heidi uses ":" at the end and and we don't
    if (tr.isNull() && source.endsWith(":")) {
        QString newSource = source.left(source.length() - 1);
        tr = translationsMap.value(newSource);
        if (!tr.isNull()) {
            tr += ":";
        }
    }

    if (tr.isNull()) {
        source.replace('\'', '"');
        tr = translationsMap.value(source);
    }

    if (tr.isNull()) {
        source.replace('`', '"');
        tr = translationsMap.value(source);
    }

    if (tr.isNull()) {
        // Heidi uses %s as placeholder, we use %1, %2, %3
        // TODO: add %d support
        source.replace("%1", "%s");
        source.replace("%2", "%s");
        source.replace("%3", "%s");

        //qDebug() << "try new source" << source;
        tr = translationsMap.value(source);
        if (!tr.isNull()) {
            //qDebug() << "Found tr by changing placeholder";
            int placeholderPos = 0;
            int i = 0;
            while ((placeholderPos = tr.indexOf("%s", placeholderPos)) != -1) {
                ++i;
                tr.replace("%s", "%" + QString::number(i));
            }
        }
    }



    return tr;
}

QString unescape(const QString & str)
{
    QString res = str;
    res.replace("&quot;", QChar('"'));
    res.replace("&apos;", QChar('\''));

    return res;
}

QString escape(const QString & str)
{
    QString res = str;
    res.replace(QChar('"'), "&quot;");
    res.replace(QChar('\''), "&apos;");

    return res;
}

void copy(const QString & sourceFilename, const QString & destinationFilename)
{

    QMap<QString, QString> translationsMap = readTranslations(sourceFilename);

    //qDebug().noquote() << translationsMap;

    QFile inFile(destinationFilename);
    if (!inFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Unable to open file:" << destinationFilename;
        return;
    }

    QFile tempFile(destinationFilename + ".temp");
    if (!tempFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate)) {
        return;
    }

    QTextStream inStream(&inFile);
    QTextStream tempStream(&tempFile);
    QString line;

    const int beginSourceLen = QString("<source>").size();
    const int endSourceLen = QString("</source>").size();

    bool readingMultilineSource = false;
    QString sourceData;

    while (inStream.readLineInto(&line)) {
        //qDebug() << "line" << line;
        int beginSourcePos = line.indexOf("<source>");
        if (beginSourcePos != -1) {
            int endSourcePos = line.indexOf("</source>");
            if (endSourcePos == -1) {
                readingMultilineSource = true;
                sourceData
                    = line.mid(beginSourcePos + beginSourceLen);
                //qDebug() << "source start" << sourceData;
            } else {
                readingMultilineSource = false;
                sourceData
                    = line.mid(beginSourcePos + beginSourceLen,
                               line.size() - beginSourcePos
                               - beginSourceLen - endSourceLen);
                //qDebug() << "source" << sourceData;
            }
        } else if (readingMultilineSource) {
            sourceData += "\n";
            int endSourcePos = line.indexOf("</source>");
            if (endSourcePos == -1) {
                sourceData += line;
            } else {
                readingMultilineSource = false;
                sourceData
                    += line.mid(0, line.size() - endSourceLen);
                //qDebug() << "source end" << sourceData;
            }
        }

        sourceData = unescape(sourceData);

        int translationPos = line.indexOf(
                    "<translation type=\"unfinished\">");
        if (translationPos != -1) {
            QString translation = getTranslation(sourceData, translationsMap);
            line = "        ";
            if (translation.isEmpty()) {
                qDebug() << "not found" << sourceData;
                line += "<translation type=\"unfinished\">";
            } else {
                line += "<translation>";
            }
            line += escape(translation);
            line += "</translation>";
            //qDebug() << "tr" << line;
        }

        tempStream << line << "\n";
    }


    inFile.close();
    tempFile.close();

    QFile::remove(destinationFilename);

    if (!tempFile.rename(destinationFilename)) {
        qDebug() << "Unable to rename file:"
                 << tempFile.fileName()
                 << "=>"
                 << destinationFilename;
    }
}

int main(int argc, char *argv[])
{

    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QString prefixPath = "./";

    copy(prefixPath + "heidisql_bg.ts", prefixPath + "meowsql_bg.ts");
    copy(prefixPath + "heidisql_zh.ts", prefixPath + "meowsql_zh.ts");
    copy(prefixPath + "heidisql_cs.ts", prefixPath + "meowsql_cs.ts");
    copy(prefixPath + "heidisql_fr.ts", prefixPath + "meowsql_fr.ts");
    copy(prefixPath + "heidisql_de.ts", prefixPath + "meowsql_de.ts");
    copy(prefixPath + "heidisql_hu.ts", prefixPath + "meowsql_hu.ts");
    copy(prefixPath + "heidisql_it.ts", prefixPath + "meowsql_it.ts");
    copy(prefixPath + "heidisql_ko.ts", prefixPath + "meowsql_ko.ts");
    copy(prefixPath + "heidisql_pt.ts", prefixPath + "meowsql_pt.ts");
    copy(prefixPath + "heidisql_ro.ts", prefixPath + "meowsql_ro.ts");
    copy(prefixPath + "heidisql_ru.ts", prefixPath + "meowsql_ru.ts");
    copy(prefixPath + "heidisql_pt.ts", prefixPath + "meowsql_pt.ts");
    copy(prefixPath + "heidisql_es.ts", prefixPath + "meowsql_es.ts");
    copy(prefixPath + "heidisql_sv.ts", prefixPath + "meowsql_sv.ts");


    return 0;
}
