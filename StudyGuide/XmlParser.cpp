//
// Created by Jesse on 6 okt. 2023.
//

#include <QFile>
#include <QXmlStreamReader>
#include "XmlParser.h"
#include "guide/GuideData.h"
#include "Application.h"
#include <QString>

GuideData::Data XmlParser::readXml(QFile* xmlFileP) {
    QFile&xmlFile = *xmlFileP;
    QFileInfo fileInfo(xmlFile);
    qDebug() << "Reading xml file" << fileInfo.fileName();
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file" << fileInfo.fileName();
        // He really wants something...
        GuideData::Data hello;
        return hello;
    }

    try {
        QXmlStreamReader xml(&xmlFile);
        QXmlStreamReader::TokenType token;
        QString elementName;

        while (!xml.atEnd()) {
            token = xml.readNext();
            elementName = xml.name().toString();


            if (elementName == "studyguide" && token != QXmlStreamReader::EndElement) {
                GuideData::Data guide;
                guide.originalFile = fileInfo;
                for (QXmlStreamAttribute attribute: xml.attributes()) {
                    if (attribute.name().toString() == "autosavefile" && attribute.value().toString() == "true") {
                        guide.autoSaveFile = fileInfo;
                    }
                    else if (attribute.name().toString() == "originalfile") {
                        guide.originalFile = QFileInfo(attribute.value().toString());
                    }
                }
                elementName = "";

                while (!(token == QXmlStreamReader::EndElement && elementName == "studyguide")) {
                    token = xml.readNext();
                    elementName = xml.name().toString();


                    if (elementName == "name") {
                        guide.name = xml.readElementText();
                    }
                    if (elementName == "shortname") {
                        guide.shortName = xml.readElementText();
                    }
                    if (elementName == "info") {
                        guide.info = xml.readElementText();
                    }
                    if (elementName == "period") {
                        guide.period = xml.readElementText();
                    }
                    if (elementName == "index") {
                        GuideData::GuideObject index;
                        index.objectType = GuideData::Index;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "index")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "goal") {
                                GuideData::GuideGoals goal;

                                elementName = "";
                                while (!(token == QXmlStreamReader::EndElement && elementName == "goal")) {
                                    token = xml.readNext();
                                    elementName = xml.name().toString();
                                    if (elementName == "name") {
                                        goal.name = xml.readElementText();
                                    }
                                    if (elementName == "number") {
                                        goal.goalNumber = xml.readElementText();
                                    }
                                    if (elementName == "time") {
                                        goal.time = xml.readElementText();
                                    }
                                    if (elementName == "work") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addWork(xml.readElementText(), link);
                                    }
                                    if (elementName == "watch") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addWatch(xml.readElementText(), link);
                                    }
                                    if (elementName == "read") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();

                                        goal.addRead(xml.readElementText(), link);
                                    }
                                    if (elementName == "process") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();


                                        goal.addProcess(xml.readElementText(), link);
                                    }
                                    if (elementName == "info") {
                                        QString link = "";
                                        for (QXmlStreamAttribute attribute: xml.attributes())
                                            if (attribute.name().toString() == "href")
                                                link = attribute.value().toString();


                                        goal.addInfo(xml.readElementText(), link);
                                    }
                                    if (elementName == "week") {
                                        goal.week = xml.readElementText();
                                    }
                                    if (elementName == "progress") {
                                        goal.progress = xml.readElementText();
                                    }
                                }

                                index.goals.append(goal);
                            }
                        }
                        guide.objects.append(index);
                    }
                    if (elementName == "test") {
                        GuideData::GuideObject test;
                        test.objectType = GuideData::Test;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "shortname") {
                                test.setShortTestName(xml.readElementText());
                            }
                            if (elementName == "name") {
                                test.setTestName(xml.readElementText());
                            }
                            if (elementName == "info") {
                                test.setTestInfo(xml.readElementText());
                            }
                            if (elementName == "week") {
                                test.setTestWeek(xml.readElementText());
                            }
                        }
                        guide.objects.append(test);
                    }
                    if (elementName == "report") {
                        GuideData::GuideObject report;
                        report.objectType = GuideData::Report;

                        elementName = "";
                        while (!(token == QXmlStreamReader::EndElement && elementName == "report")) {
                            token = xml.readNext();
                            elementName = xml.name().toString();

                            if (elementName == "test") {
                                GuideData::ReportTests test;
                                elementName = "";
                                while (!(token == QXmlStreamReader::EndElement && elementName == "test")) {
                                    token = xml.readNext();
                                    elementName = xml.name().toString();

                                    if (elementName == "name") {
                                        test.name = xml.readElementText();
                                    }
                                    if (elementName == "weight") {
                                        test.weight = xml.readElementText();
                                    }
                                }
                                report.addTest(test);
                            }
                        }
                        guide.objects.append(report);
                    }
                }

                qDebug() << "Finished reading xml file" << fileInfo.fileName();
                xml.clear();
                xmlFile.close();
                return guide;
            }
        }
    }

    catch (...) {
        qCritical() << "Error while reading XML file" << fileInfo.fileName();
        xmlFile.close();
    }
}

void XmlParser::saveXml(const GuideData::Data&guide, QFile&fileToSaveTo, bool isAutoSave, bool useAutoFormatting) {
    // QFile&fileToSaveTo = *fileToSaveToP;
    QFileInfo fileInfo(fileToSaveTo);
    if (!fileToSaveTo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file " << fileInfo.fileName() << "for saving.";
    }
    try {
        QXmlStreamWriter xml(&fileToSaveTo);
        xml.setAutoFormatting(useAutoFormatting);

        xml.writeStartDocument();
        xml.writeStartElement("studyguide");
        xml.writeAttribute("format-version", "1.0");
        if (isAutoSave) {
            xml.writeAttribute("autosavefile", "true");
            xml.writeAttribute("originalfile", guide.originalFile.filePath());
        }
        xml.writeTextElement("name", guide.name);
        xml.writeTextElement("shortname", guide.shortName);
        xml.writeTextElement("period", guide.period);
        xml.writeTextElement("info", guide.info);

        // start element loop
        for (GuideData::GuideObject object: guide.objects) {
            switch (object.objectType) {
                case GuideData::Index:
                    xml.writeStartElement("index");
                    for (GuideData::GuideGoals goal: object.goals) {
                        xml.writeStartElement("goal");

                        xml.writeTextElement("name", goal.name);
                        xml.writeTextElement("number", goal.goalNumber);
                        xml.writeTextElement("time", goal.time);
                        xml.writeTextElement("week", goal.week);
                        xml.writeTextElement("progress", goal.progress);

                        for (GuideData::GuideGoalPrefixes prefix: goal.prefixes) {
                            switch (prefix.prefix) {
                                case GuideData::Work:
                                    xml.writeStartElement("work");

                                    if (!prefix.link.isEmpty())
                                        xml.writeAttribute("href", prefix.link);

                                    xml.writeCharacters(prefix.prefixText);
                                    xml.writeEndElement(); // Work
                                    break;
                                case GuideData::Read:
                                    xml.writeStartElement("read");

                                    if (!prefix.link.isEmpty())
                                        xml.writeAttribute("href", prefix.link);

                                    xml.writeCharacters(prefix.prefixText);
                                    xml.writeEndElement(); // Read
                                    break;
                                case GuideData::Watch:
                                    xml.writeStartElement("watch");

                                    if (!prefix.link.isEmpty())
                                        xml.writeAttribute("href", prefix.link);

                                    xml.writeCharacters(prefix.prefixText);
                                    xml.writeEndElement(); // Watch
                                    break;
                                case GuideData::Process:
                                    xml.writeStartElement("process");

                                    if (!prefix.link.isEmpty())
                                        xml.writeAttribute("href", prefix.link);

                                    xml.writeCharacters(prefix.prefixText);
                                    xml.writeEndElement(); // process
                                    break;
                                case GuideData::Info:
                                    xml.writeStartElement("info");

                                    if (!prefix.link.isEmpty())
                                        xml.writeAttribute("href", prefix.link);

                                    xml.writeCharacters(prefix.prefixText);
                                    xml.writeEndElement(); // info
                                    break;
                            }
                        }
                        xml.writeEndElement(); // goal
                    }
                    xml.writeEndElement(); // index
                    break;
                case GuideData::Test:
                    xml.writeStartElement("test");
                    xml.writeTextElement("name", object.name);
                    xml.writeTextElement("shortname", object.shortName);
                    xml.writeTextElement("info", object.info);
                    xml.writeTextElement("week", object.week);
                    xml.writeEndElement(); // test
                    break;
                case GuideData::Report:
                    xml.writeStartElement("report");
                    for (GuideData::ReportTests test: object.tests) {
                        xml.writeStartElement("test");
                        xml.writeTextElement("name", test.name);
                        xml.writeTextElement("weight", test.weight);
                        xml.writeEndElement(); // test
                    }
                    xml.writeEndElement(); // report
            }
        }

        xml.writeEndElement(); // studyguide
        xml.writeEndDocument();
        qDebug() << "Finished saving file" << fileInfo.fileName();;
    }
    catch (...) {
        qCritical() << "Failed to save Xml file" << fileInfo.fileName();;
    }
    fileToSaveTo.close();
}

void XmlParser::autoSaveXml(QVector<GuideData::Data> GuidesToSave) {
    QVector<QFuture<void>> futures;

    for (auto guide: GuidesToSave) {
        if (!guide.autoSaveFile.exists()) {
            QFileInfo autoSaveFile(APPLICATION->getAutoSaveLocation(), guide.originalFile.fileName());
            guide.autoSaveFile = autoSaveFile;
        }

        futures.append(QtConcurrent::run([guide]() {
            QFile autoSaveFile(guide.autoSaveFile.filePath());
            return saveXml(guide, autoSaveFile, true, false);
        }));
    }
}
