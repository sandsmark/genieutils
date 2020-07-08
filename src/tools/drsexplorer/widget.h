#ifndef WIDGET_H
#define WIDGET_H

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

    bool loadPalette(const int palette, const QString &drs);
    bool loadDrs(const QString &path);

private slots:
    void onItemActivated(const QListWidgetItem *item);

private:
    QPixmap getPixmap(genie::SlpFramePtr frame);

    void loadSlps(genie::DrsFile *file);
    void loadSlps(const QFileInfoList &files);

    genie::DrsFile m_interfaceFile;
    genie::DrsFile m_drsFile;
    genie::PalFile m_palette;
    QVector<QRgb> m_colorTable;
};

#endif // WIDGET_H
