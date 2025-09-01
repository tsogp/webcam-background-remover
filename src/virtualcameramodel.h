#ifndef CAMERA_BACKGROUND_REMOVER_VIRTUALCAMERAMODEL_H
#define CAMERA_BACKGROUND_REMOVER_VIRTUALCAMERAMODEL_H

#include "virtualcameramanager.h"
#include <QObject>
#include <QAbstractListModel>
#include <QVector>
#include <QUrl>
#include <QVariant>
#include <qtmetamacros.h>

struct VirtualCamera {
    QString name;
    QString source_url;
    QString camera_url;
    bool is_active;
    vc::virtual_camera_data data;
};

class VirtualCameraModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum CameraRoles {
        NameRole = Qt::UserRole + 1,
        SourceUrlRole,
        CameraUrlRole,
        IsActiveRole
    };

    explicit VirtualCameraModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE [[nodiscard]] QVariantMap get(int row) const;
    Q_INVOKABLE int addCamera(const QString& name, const QString& source_url, const QString& camera_url, bool active = false);
    Q_INVOKABLE void removeCamera(int index);
    Q_INVOKABLE void setName(int index, const QString& name);
    Q_INVOKABLE void setSourceUrl(int index, const QString& source_url);
    Q_INVOKABLE void setCameraUrl(int index, const QString& camera_url);
    Q_INVOKABLE void setActive(int index, bool active);
private:
    QVector<VirtualCamera> m_cameras;
};


#endif // CAMERA_BACKGROUND_REMOVER_VIRTUALCAMERAMODEL_H
