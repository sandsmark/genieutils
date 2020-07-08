#include "widget.h"

#include <QPainter>
#include <QDebug>
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QListWidget(parent)
{
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setIconSize(QSize(1024, 1024));
    //loadSlps(QDir("/home/sandsmark/src/freeaoe/unaced/Campaign/Media").entryInfoList(QStringList() << "*.pal" << "*.PAL"));
    //loadSlps(&m_interfaceFile);
}

Widget::~Widget()
{

}

bool Widget::loadPalette(const int palette, const QString &drs)
{
    m_interfaceFile.setGameVersion(genie::GV_TC);
    m_interfaceFile.load(drs.toStdString());
    std::vector<genie::Color> colors = m_interfaceFile.getPalFile(palette).getColors();
    for (unsigned i=0; i<colors.size(); i++) {
        m_colorTable.append(qRgb(colors[i].r, colors[i].g, colors[i].b));
    }

    return true;
}

bool Widget::loadDrs(const QString &path)
{
    m_drsFile.setGameVersion(genie::GV_TC);
    m_drsFile.load(path.toStdString());
    loadSlps(&m_drsFile);

    return true;
}

QPixmap Widget::getPixmap(genie::SlpFramePtr frame)
{
//    if (!m_palette) {
//        qWarning() << "No palette set";
//        return QPixmap();
//    }

    QImage image(frame->img_data.pixel_indexes.data(), frame->getWidth(), frame->getHeight(), frame->getWidth(), QImage::Format_Indexed8);
    image.setColorTable(m_colorTable);

    if (image.width() < 128) {
        return QPixmap::fromImage(image.scaledToWidth(128));
    } else if (image.height() < 128) {
        return QPixmap::fromImage(image.scaledToHeight(128));
    } else {
        return QPixmap::fromImage(image);
    }
}

void Widget::loadSlps(genie::DrsFile *drsFile)
{
    std::vector<uint32_t> ids = drsFile->slpFileIds();
    std::sort(ids.begin(), ids.end());
    for (const int id : ids) {
        genie::SlpFilePtr slp = drsFile->getSlpFile(id);
        if (!slp) {
            qWarning() << "Unable to load SLP file";
            continue;
        }
        for (unsigned frameNum = 0; frameNum < slp->getFrameCount(); frameNum++) {
            genie::SlpFramePtr frame = slp->getFrame(frameNum);
            if (!frame) {
                qWarning() << "Unable to load SLP frame";
                continue;
            }

            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::number(id) + " " + QString::number(frameNum));
            item->setIcon(getPixmap(frame));
            addItem(item);
        }
    }
}

void Widget::loadSlps(const QFileInfoList &files)
{
    for (const QFileInfo &fi : files) {
        const QString folder = fi.absolutePath() + "/";
        QString palFile = fi.absoluteFilePath();
        QString slpName;
        if (fi.suffix() == "PAL") {
            slpName = folder + fi.baseName() + ".SLP";
        } else {
            slpName = folder + fi.baseName() + ".slp";
        }
//        qDebug() << palFile << slpName;
        genie::PalFile palette;
        palette.load(palFile.toStdString());

        QVector<QRgb> colorTable;
        for (unsigned i=0; i<palette.getColors().size(); i++) {
            colorTable.append(qRgb(palette.getColors()[i].r, palette.getColors()[i].g, palette.getColors()[i].b));
        }

        genie::SlpFile slpFile(fi.size());
        slpFile.load(slpName.toStdString());
        for (int i=0; i<slpFile.getFrameCount(); i++) {
            genie::SlpFramePtr frame = slpFile.getFrame(i);
            QImage image(frame->img_data.pixel_indexes.data(), frame->getWidth(), frame->getHeight(), frame->getWidth(), QImage::Format_Indexed8);
            image.setColorTable(colorTable);

            QListWidgetItem *item = new QListWidgetItem;
            item->setText(fi.baseName());
            //        item->setText(QString::number(id) + " " + QString::number(0));
            item->setIcon(QPixmap::fromImage(image));
            addItem(item);
        }
    }

}
