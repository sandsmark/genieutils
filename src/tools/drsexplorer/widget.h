#pragma once

#include <QListWidget>

#include <genie/resource/DrsFile.h>
#include <genie/resource/PalFile.h>
#include <memory>
#include <QDir>

class Widget : public QListWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool swgbMode = false;

    bool loadPalette(const int palette, const QString &drs);
    bool loadDrs(const QString &path);

private slots:
    void onItemActivated(const QListWidgetItem *item);

private:
    QPixmap getPixmap(genie::SlpFramePtr frame, const QVector<QRgb> &colorTable);

    void loadSlps(genie::DrsFile *file);
    void loadSlps(const QFileInfoList &files);
    void loadSlp(const genie::SlpFilePtr &slpFile, const QVector<QRgb> &colorTable, const QString &baseName);

    genie::DrsFile m_interfaceFile;
    genie::DrsFile m_drsFile;
    genie::PalFile m_palette;
    QVector<QRgb> m_colorTable;
};

