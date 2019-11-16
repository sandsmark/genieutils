#include "Window.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QPainter>
#include <QApplication>
#include <QScreen>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::Dialog);


    // Need to do this, some QPAs (Plasma) get sad if we use dialogs before the event loop is running
    QMetaObject::invokeMethod(this, &Window::init, Qt::QueuedConnection);
}

Window::~Window()
{
}

bool Window::setDrsPath(const QString &path)
{
    if (!QFile::exists(path)) {
        QMessageBox::warning(this, "Failed to load DRS file", "Specified DRS (" + path + ") does not exist");
        return false;
    }
    try {
        m_drsFile = std::make_shared<genie::DrsFile>();
        std::cout << "loading " << path.toStdString() << std::endl;
        m_drsFile->load(path.toStdString());
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to load DRS file", e.what());
        return false;
    }

    return true;
}

bool Window::setPalette(const QString &name, const QString &drsFile)
{
    std::vector<genie::Color> colors = loadPalette(name, drsFile).colors_;
    if (colors.empty()) {
        qWarning() << "Failed to load palette";
        return false;
    }

    for (unsigned i=0; i<colors.size(); i++) {
        m_colorTable.append(qRgb(colors[i].r, colors[i].g, colors[i].b));
    }

    return true;
}

bool Window::load(QString path)
{
    if (path.isEmpty()) {
        QMessageBox::warning(this, "No path set", "No path passed");
        return false;
    }

    int id = -1;
    std::string name = path.toStdString();
    if (!QFile::exists(path) && !m_drsFile) {
        QMessageBox::warning(this, "No DRS specified", "DRS not specified and image is not a valid path");
        return false;
    }

    // assume name in drs
    if (!QFile::exists(path)) {
        qDebug() << "trying to find in DRS";

        bool ok;
        id = path.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Invalid ID", "Can't pass numerical ID and not specify DRS file");
            return false;
        }
    }

    if (!QFile::exists(path)) {
        if (!QFile::exists(path)) {
            path.prepend(m_dataPath);
        }
    }

    if (!m_drsFile && !QFile::exists(path)) {
        QMessageBox::warning(this, "Invalid path", "Either pass a numeric ID and DRS or a valid filename");
        return false;
    }

    try {
        genie::SlpFilePtr slp;
        if (id != -1) {
            slp = m_drsFile->getSlpFile(id);
            if (!slp) {
                std::cout << "Valid SLP IDs:" << std::endl;
                for (const int id : m_drsFile->slpFileIds()) {
                    std::cout << " " << id << std::endl;
                }
                QMessageBox::warning(this, "Failed to load SLP", "SLP file " + QString::number(id) + " could not be loaded");
                return false;
            }
        } else {
            slp = std::make_shared<genie::SlpFile>(QFileInfo(path).size());
            slp->load(path.toStdString());
        }
        if (!slp) {
            QMessageBox::warning(this, "Failed to load SLP", "SLP " + path + " could not be loaded.");
            return false;
        }
        m_pixmap = createPixmap(slp->getFrame());
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to load SLP file", e.what());
        return false;
    }

    const QRect maxRect = QRect(QPoint(0, 0), qApp->screenAt(rect().center())->size()).marginsRemoved(QMargins(50, 50, 50, 50));
    const int minWidth = 512;
    const int minHeight = 512;

    QRectF geometry = m_pixmap.rect();

    if (geometry.width() < minWidth || geometry.height() < minHeight) {
        const qreal scaleWidth = geometry.width() / minWidth;
        const qreal scaleHeight = geometry.height() / minHeight;
        if (scaleWidth > scaleHeight) {
            geometry.setWidth(geometry.width() / scaleWidth);
            geometry.setHeight(geometry.height() / scaleWidth);
        } else {
            geometry.setWidth(geometry.width() / scaleHeight);
            geometry.setHeight(geometry.height() / scaleHeight);
        }
    }

    if (geometry.width() >= maxRect.width() || geometry.height() >= maxRect.height()) {
        const qreal scaleWidth = geometry.width() / width();
        const qreal scaleHeight = geometry.height() / height();
        if (scaleWidth > scaleHeight) {
            geometry.setWidth(geometry.width() / scaleWidth);
            geometry.setHeight(geometry.height() / scaleWidth);
        } else {
            geometry.setWidth(geometry.width() / scaleHeight);
            geometry.setHeight(geometry.height() / scaleHeight);
        }
    }

    geometry.moveCenter(maxRect.center());

    setGeometry(geometry.toAlignedRect());
    return true;
}

void Window::init()
{
    QSettings settings;

    QString dataPath = settings.value("datapath").toString();
    if (dataPath.isEmpty()) {
        dataPath = QFileDialog::getExistingDirectory(this, "Select game data directory");
        if (dataPath.isEmpty()) {
            close();
        }
    }

    if (!loadDataTC(dataPath)) {
        settings.setValue("datapath", "");
        close();
    }

    m_dataPath = dataPath;

    settings.setValue("datapath", dataPath);

    return;
}

bool Window::loadDataTC(const QString &dataPath)
{
    m_interfaceFile.setGameVersion(genie::GV_TC);

    try {
        m_interfaceFile.load((dataPath + "/Data/interfac.drs").toStdString());
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to load interface file", e.what());
        return false;
    }

    return true;
}

QPixmap Window::createPixmap(genie::SlpFramePtr frame)
{
    if (!frame) {
        qWarning() << "no SLP passed";
        return QPixmap();
    }

    QImage image(frame->img_data.pixel_indexes.data(), frame->getWidth(), frame->getHeight(), frame->getWidth(), QImage::Format_Indexed8);
    image.setColorTable(m_colorTable);

    return QPixmap::fromImage(image);
}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QPixmap scaled;
    scaled = m_pixmap.scaled(width(), height(), Qt::KeepAspectRatio);
    p.drawPixmap(width()/2 - scaled.width()/2, height() / 2 - scaled.height()/2, scaled);
}

const genie::PalFile &Window::loadPalette(const QString &palettePath, const QString &drsPath)
{
    bool ok;
    int id = palettePath.toInt(&ok);

    if (QFile::exists(palettePath)) {
        try {
            m_palette.load(palettePath.toStdString());
            return m_palette;
        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Failed to load SLP file by path", e.what());
            return genie::PalFile::null;
        }
    }
    if (!ok || drsPath.isEmpty()) {
        QMessageBox::warning(this, "Invalid palette path", "Either pass a numeric ID and DRS or a valid filename");
        return genie::PalFile::null;
    }

    std::shared_ptr<genie::DrsFile> paletteDrs;
    if (drsPath.isEmpty()) {
        paletteDrs = m_drsFile;
    } else if (!QFile::exists(drsPath)) {
        QMessageBox::warning(this, "Failed to load palette DRS", "No generic DRS file set, and palette DRS not valid.");
        return genie::PalFile::null;
    }

    if (drsPath.isEmpty() && m_drsFile) {
        paletteDrs = m_drsFile;
    } else {
        try {
            paletteDrs = std::make_shared<genie::DrsFile>();
            paletteDrs->load(drsPath.toStdString());
        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Failed to load palette DRS file", e.what());
            return genie::PalFile::null;
        }
    }

    if (!paletteDrs) {
        QMessageBox::warning(this, "Failed to load palette DRS file", "No palette DRS could be loaded");
        return genie::PalFile::null;
    }

    try {
        return paletteDrs->getPalFile(id);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to load palette file from DRS", e.what());
        return genie::PalFile::null;
    }
}
