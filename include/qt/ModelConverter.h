//
// Created by RainSure on 2024/3/24.
//

#ifndef RSMESH_MODELCONVERTER_H
#define RSMESH_MODELCONVERTER_H
#include <QObject>
#include <QProcess>
#include <QDebug>

namespace rsmesh::qt {
    class ModelConverter : public QObject{
    Q_OBJECT
    public:
        explicit ModelConverter(QObject *parent = nullptr) : QObject(parent) {
            connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                    this, &ModelConverter::onConversionFinished);
        }

        Q_INVOKABLE void convertModel(const QString &inputPath, const QString &outputPath) {
            // 去掉file:///前缀
            QString filePath = inputPath;
            if (filePath.startsWith("file:///")) {
                filePath = filePath.mid(8);
            }
            QStringList arguments;
            arguments << filePath << "--outputPath" << outputPath;
            process.start(R"(D:\Code\cpp\RSMesh\balsam.exe)", arguments);
        }
    signals:
        void conversionSuccess();
        void conversionFailure();

    private slots:
        void onConversionFinished(int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::CrashExit || exitCode != 0) {
                qDebug() << "Conversion failed with exit code" << exitCode;
                emit conversionFailure();
            } else {
                emit conversionSuccess();
            }
        }

    private:
        QProcess process;
    };
} // namespace rsmesh::qt




#endif //RSMESH_MODELCONVERTER_H
