#ifndef WINDOW_H
#define WINDOW_H

#include <genie/resource/DrsFile.h>
#include <genie/resource/PalFile.h>

#include <QWidget>

#include <memory>

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

    bool setDrsPath(const QString &path);
    bool setPalette(const QString &name, const QString &drsFile);

    bool load(QString path);

private slots:
    void init();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    const genie::PalFile &loadPalette(const QString &palettePath, const QString &drsPath);
    bool loadDataTC(const QString &dataPath);
    QPixmap createPixmap(genie::SlpFramePtr frame);

    genie::DrsFile m_interfaceFile;
    std::shared_ptr<genie::DrsFile> m_drsFile;
    genie::PalFile m_palette;
    QVector<QRgb> m_colorTable;

    QPixmap m_pixmap;
    QString m_drsPath;

    QString m_dataPath;

    QRect m_maxRect;
};
#endif // WINDOW_H
