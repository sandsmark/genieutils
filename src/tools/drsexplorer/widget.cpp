#include "widget.h"

#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QScrollBar>
#include <genie/util/Utility.h>

#define PREVIEW_SIZE 300

Widget::Widget(QWidget *parent)
    : QListWidget(parent)
{
    setWindowFlag(Qt::Dialog);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setIconSize(QSize(PREVIEW_SIZE, PREVIEW_SIZE));
    setFlow(QListView::LeftToRight);
    setWrapping(true);
    setMinimumSize(PREVIEW_SIZE * 3 + spacing() * 4 + verticalScrollBar()->width(), PREVIEW_SIZE * 4);
    setResizeMode(QListView::Adjust);
    connect(this, &QListWidget::itemActivated, this, &Widget::onItemActivated);
}

Widget::~Widget()
{

}

bool Widget::loadPalette(const int palette, const QString &drs)
{
    if (swgbMode) {
        m_interfaceFile.setGameVersion(genie::GV_SWGB);
    } else {
        m_interfaceFile.setGameVersion(genie::GV_TC);
    }

    m_interfaceFile.load(drs.toStdString());
    std::vector<genie::Color> colors = m_interfaceFile.getPalFile(palette).getColors();
    for (unsigned i=0; i<colors.size(); i++) {
        m_colorTable.append(qRgb(colors[i].r, colors[i].g, colors[i].b));
    }

    return true;
}

bool Widget::loadDrs(const QString &path)
{
    QFileInfo fi(path);
    if (fi.isDir()) {
        loadSlps(QDir(path).entryInfoList(QDir::Files));
    } else if (fi.suffix().toLower() == "drs") {
        if (swgbMode) {
            m_drsFile.setGameVersion(genie::GV_SWGB);
        } else {
            m_drsFile.setGameVersion(genie::GV_TC);
        }
        m_drsFile.load(path.toStdString());
        loadSlps(&m_drsFile);
    } else {
        loadSlps({fi});
    }

    return true;
}

QPixmap Widget::getPixmap(genie::SlpFramePtr frame, const QVector<QRgb> &colorTable)
{
    QImage image(frame->img_data.pixel_indexes.data(), frame->getWidth(), frame->getHeight(), frame->getWidth(), QImage::Format_Indexed8);
    if (image.isNull()) {
        static QPixmap invalidPixmap;
        if (invalidPixmap.isNull()) {
            invalidPixmap = QPixmap(10, 10);
            invalidPixmap.fill(Qt::red);
        }
        qWarning() << "Failed to load frame";
        return invalidPixmap;
    }

    image.setColorTable(colorTable);

    if (image.width() < 128) {
        image = image.scaledToWidth(128);
    } else if (image.height() < 128) {
        image = image.scaledToHeight(128);
    }
    image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    QPainter p(&image);
    QFont font;
    font.setPointSize(20);
    font.setBold(true);
    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(image.rect(), QString::number(frame->getWidth()) + "x" + QString::number(frame->getHeight()));
    p.end();

    return QPixmap::fromImage(image);
}

void Widget::loadSlps(genie::DrsFile *drsFile)
{
    std::vector<uint32_t> ids = drsFile->slpFileIds();
    std::sort(ids.begin(), ids.end());
    for (const uint32_t id : ids) {
        genie::SlpFilePtr slp = drsFile->getSlpFile(id);
        if (!slp) {
            qWarning() << "Unable to load SLP file";
            continue;
        }
        for (unsigned frameNum = 0; frameNum < slp->getFrameCount(); frameNum++) {
            genie::SlpFramePtr frame = slp->getFrame(frameNum);
            if (!frame || frame->getWidth() == 0 || frame->getHeight() == 0) {
                qWarning() << "Unable to load SLP frame" << frameNum;
                continue;
            }

            QListWidgetItem *item = new QListWidgetItem;
            item->setText(QString::number(id) + " " + QString::number(frameNum));

            QPixmap pixmap = getPixmap(frame, m_colorTable);
            if (pixmap.width() > PREVIEW_SIZE) {
                pixmap = pixmap.scaledToWidth(PREVIEW_SIZE);
            }
            if (pixmap.height() > PREVIEW_SIZE) {
                pixmap = pixmap.scaledToHeight(PREVIEW_SIZE);
            }
            item->setIcon(pixmap);

            addItem(item);
        }
    }
}

void Widget::loadSlps(const QFileInfoList &files)
{
    for (const QFileInfo &fi : files) {
        if (fi.suffix().toLower() != "slp") {
            continue;
        }
        genie::SlpFilePtr slpFile = std::make_shared<genie::SlpFile>(fi.size());
        slpFile->load(fi.absoluteFilePath().toStdString());

        const QString folder = fi.absolutePath() + "/";
        const QString baseName = fi.baseName();
        std::string palFilename = genie::util::resolvePathCaseInsensitive(folder.toStdString() + "/" + baseName.toStdString() + ".pal");
        if (!palFilename.empty()) {
            genie::PalFile palette;
            palette.load(palFilename);

            QVector<QRgb> colorTable;
            for (unsigned i=0; i<palette.getColors().size(); i++) {
                colorTable.append(qRgb(palette.getColors()[i].r, palette.getColors()[i].g, palette.getColors()[i].b));
            }

            loadSlp(slpFile, colorTable, fi.baseName());
        } else {
            loadSlp(slpFile, m_colorTable, fi.baseName());

        }
    }
}

void Widget::loadSlp(const genie::SlpFilePtr &slpFile, const QVector<QRgb> &colorTable, const QString &baseName)
{
    for (uint32_t frameNum=0; frameNum<slpFile->getFrameCount(); frameNum++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(baseName + " " + QString::number(frameNum));
        item->setIcon(getPixmap(slpFile->getFrame(frameNum), colorTable));
        addItem(item);
    }
}

void Widget::onItemActivated(const QListWidgetItem *item)
{
    const QStringList idParts = item->text().split(' ');

    bool slpOk, frameOk;
    const int slpId = idParts.first().toInt(&slpOk);
    const int frameNum = idParts.last().toInt(&frameOk);

    Q_ASSERT(slpOk);
    Q_ASSERT(frameOk);

    genie::SlpFilePtr slp = m_drsFile.getSlpFile(slpId);
    Q_ASSERT(slp);

    genie::SlpFramePtr frame = slp->getFrame(frameNum);
    Q_ASSERT(frame);

    QLabel *preview = new QLabel;
    preview->setWindowFlag(Qt::Dialog);
    preview->setPixmap(getPixmap(frame, m_colorTable));
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->show();
}
