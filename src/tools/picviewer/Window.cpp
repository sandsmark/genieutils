#include "Window.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QElapsedTimer>

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

bool Window::setPalette(const QString &name, const QString &drsFile) try {
    std::vector<genie::Color> colors;
    if (drsFile.isEmpty()) {
        m_palette.load(name.toStdString());
        colors = m_palette.getColors();
    } else {
        colors = loadPalette(name, drsFile).getColors();
        if (colors.empty()) {
            qWarning() << "Failed to load palette";
            return false;
        }
    }

    m_colorTable.clear();
    for (unsigned i=0; i<colors.size(); i++) {
        m_colorTable.append(qRgb(colors[i].r, colors[i].g, colors[i].b));
    }

    return true;
} catch (const std::exception &e) {
    QMessageBox::warning(this, "Failed to load palette file " + name, e.what());
    return false;
}

bool Window::load(QString path)
{
    if (path.isEmpty()) {
        QMessageBox::warning(this, "No path set", "No path passed");
        return false;
    }

    bool isSmx = false, isSmp = false;

    bool isNumber = false;
    int id = path.toInt(&isNumber);

    if (!isNumber) {
        if (!QFile::exists(path)) {
            path.prepend(m_dataPath);
        }
    }

    std::string name = path.toStdString();
    if (QFile::exists(path)) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Failed to open file", file.errorString());
            return false;
        }
        QByteArray type = file.read(4);
        qDebug() << type;
        if (type == "SMPX") {
            isSmx = true;
        } else if (type == "SMP$") {
            isSmp = true;
        }
    } else if (!m_drsFile) {
        QMessageBox::warning(this, "No DRS specified", "DRS not specified and image is not a valid path");
        return false;
    }

    // assume name in drs
    if (isNumber) {
        qDebug() << "trying to find in DRS";

        bool ok;
        if (!ok) {
            QMessageBox::warning(this, "Invalid ID", "Can't pass numerical ID and not specify DRS file");
            return false;
        }
    }

    if (!m_drsFile && !QFile::exists(path)) {
        QMessageBox::warning(this, "Invalid path", "Either pass a numeric ID and DRS or a valid filename");
        return false;
    }


    try {
        if (isSmx) {
            qDebug() << "Loading SMX";
            genie::SmxFile smxFile;
            QElapsedTimer timer; timer.start();
            smxFile.load(path.toStdString());
            qDebug() << "loaded in" << timer.elapsed() << "ms";
            if (smxFile.frameCount() <= 0) {
                qWarning() << "No frames!";
                return false;
            }
            qDebug() << "Average" << (timer.restart() / smxFile.frameCount()) << "ms per frame";
            m_pixmap = createPixmap(smxFile.frame());
            qDebug() << "created pixmap in" << timer.elapsed() << "ms";
        } else if (isSmp) {
            qDebug() << "Loading SMP";
            loadSmp(path);
        } else {
            qDebug() << "Loading SLP";
            loadSlp(path);
        }
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Failed to load file", e.what());
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

bool Window::loadSlp(const QString &path)
{
    genie::SlpFilePtr slp;
    bool ok;
    int id = path.toInt(&ok);
    if (ok) {
        if (!m_drsFile) {
            QMessageBox::warning(this, "Invalid ID", "Can't pass numerical ID and not specify DRS file");
            return false;
        }
        slp = m_drsFile->getSlpFile(id);
        if (!slp) {
            std::cout << "Valid SLP IDs:" << std::endl;
            //for (const int id : m_drsFile->slpFileIds()) {
            //    std::cout << " " << id << std::endl;
            //}
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

    return true;
}

bool Window::loadSmp(const QString &path)
{
    genie::SmpFilePtr smp = std::make_shared<genie::SmpFile>();
    smp->load(path.toStdString());
    m_pixmap = createPixmap(smp->frame());

    return true;
}

QPixmap Window::createPixmap(genie::SmpFramePtr frame)
{
    if (!frame) {
        qWarning() << "No frame read";
        return QPixmap();
    }
    const genie::SmpBaseLayer &layer = frame->baseLayer();
    m_image = QImage(layer.header.width, layer.header.height, QImage::Format_ARGB32_Premultiplied);
    m_image.fill(Qt::transparent);
    QSet<int> usedPalettes;
    for (int x=0; x<m_image.width(); x++) {
        for (int y=0; y<m_image.height(); y++) {
            if (!layer.isVisible(x, y)) {
                continue;
            }
            const size_t palIndex = layer.colorNumber(x, y);
            usedPalettes.insert(layer.paletteNumber(x, y));
            assert(palIndex < m_palette.getColors().size());
            const genie::Color &color = m_palette[palIndex];
            m_image.setPixel(x, y, qRgba(color.r, color.g, color.b, color.a));
        }
    }
    qDebug() << usedPalettes;

    return QPixmap::fromImage(m_image);
}

QPixmap Window::createPixmap(genie::SlpFramePtr frame)
{
    if (!frame) {
        qWarning() << "no SLP passed";
        return QPixmap();
    }

    m_image = QImage(frame->img_data.pixel_indexes.data(), frame->getWidth(), frame->getHeight(), frame->getWidth(), QImage::Format_Indexed8);
    m_image.setColorTable(m_colorTable);
    m_image = m_image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    qDebug() << "Size:" << m_image.size();
    m_frame = frame;

    return QPixmap::fromImage(m_image);
}

QPixmap Window::createPixmap(const genie::SmxFrame &frame)
{
    m_smxFrame = frame;
    m_image = QImage(frame.width(), frame.height(), QImage::Format_ARGB32_Premultiplied);
    for (int x=0; x<frame.width(); x++) {
        for (int y=0; y<frame.height(); y++) {
            if (!frame.isVisible(x, y)) {
                continue;
            }
            const size_t palIndex = frame.paletteIndex(x, y);
            assert(palIndex < m_palette.getColors().size());
            const genie::Color &color = m_palette[palIndex];
            m_image.setPixel(x, y, qRgba(color.r, color.g, color.b, color.a));
        }
    }

    return QPixmap::fromImage(m_image);
}

void Window::mousePressEvent(QMouseEvent *event)
{
    float scale = qMin(width()/float(m_pixmap.width()), height()/float(m_pixmap.height()));
    int x = qRound(event->x() / scale);
    int y = qRound(event->y() / scale);
    m_debugString = "X: " + QString::number(x) + " Y: " + QString::number(y);
//    if (m_image.format() == QImage::Format_Indexed8 && m_image.rect().contains(x, y)) {
    if (m_frame) {
        const size_t index = y * m_frame->getWidth() + x;
        if (index < m_frame->img_data.pixel_indexes.size()) {
            m_clickedColor = m_frame->img_data.pixel_indexes[index];
            m_debugString += "\nColor: " + QString::number(m_clickedColor);
        }

    }
    if (x < m_smxFrame.width() || y < m_smxFrame.height()) {
        const size_t palIndex = m_smxFrame.paletteIndex(x, y);
        const genie::Color &color = m_palette[palIndex];
        m_debugString +=
            "\nRGBA: " + QString::number(color.r) + " " + QString::number(color.g) + " " + QString::number(color.b) + " " + QString::number(color.a) +
            "\nIndex: " + QString::number(m_smxFrame.pixel(x, y).index) +
            "\nSection: " + QString::number(m_smxFrame.pixel(x, y).palette) +
            "\nPalette index: " + QString::number(m_smxFrame.paletteIndex(x, y)) +
            "\nX % 4: " + QString::number(x % 4)
            ;
    }
    update();
}

void Window::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::transparent);

    const float scale = qMin(width()/float(m_image.width()), height()/float(m_image.height()));
    const int checkerboardSize = qMax(5.f, 5 * scale);
    for (int x = 0; x<width()/checkerboardSize+1; x++) {
        for (int y = 0; y<height()/checkerboardSize+1; y++) {
            if ((x+y) % 2 == 0) {
                p.fillRect(x*checkerboardSize, y*checkerboardSize, checkerboardSize, checkerboardSize, QColor(32, 32, 32));
            } else {
                p.fillRect(x*checkerboardSize, y*checkerboardSize, checkerboardSize, checkerboardSize, Qt::black);

            }
        }
    }
//    p.setCompositionMode(QPainter::CompositionMode_Plus);
//    p.drawPixmap(0, 0, m_pixmap.scaled(m_pixmap.width() * scale, m_pixmap.height() * scale));
    p.drawImage(0, 0, m_image.scaled(m_image.width() * scale, m_image.height() * scale));

    QFont font = p.font();
    font.setBold(true);
    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(rect(), Qt::AlignRight | Qt::AlignBottom, m_debugString);

    if (m_clickedColor >= 0 && m_clickedColor < m_colorTable.size()) {
        p.fillRect(0, height() - 20, 20, 20, m_colorTable[m_clickedColor]);
    }
}

const genie::PalFile &Window::loadPalette(const QString &palettePath, const QString &drsPath)
{
//    return genie::PalFile::null;
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
