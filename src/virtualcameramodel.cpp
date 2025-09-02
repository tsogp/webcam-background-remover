#include "virtualcameramodel.h"
#include "virtualcameramanager.h"
#include <QAbstractItemModel>
#include <algorithm>
#include <exception>
#include <qdebug.h>
#include <qlogging.h>
#include <qtmetamacros.h>
#include <qobjectdefs.h>
#include <stdexcept>

namespace fs = std::filesystem;

VirtualCameraModel::VirtualCameraModel(QObject *parent) : QAbstractListModel(parent) {
}

int VirtualCameraModel::rowCount(const QModelIndex &) const {
    return m_cameras.count();
}

QHash<int, QByteArray> VirtualCameraModel::roleNames() const {
    return {{NameRole, "name"}, {SourceUrlRole, "sourceUrl"}, {CameraUrlRole, "cameraUrl"}, {IsActiveRole, "isActive"}};
}

QVariant VirtualCameraModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_cameras.count()) {
        return {};
    }

    const auto &camera = m_cameras[index.row()];
    switch (role) {
    case NameRole:
        return camera.name;
    case SourceUrlRole:
        return camera.source_url;
    case CameraUrlRole:
        return camera.camera_url;
    case IsActiveRole:
        return camera.is_active;
    default:
        return {};
    }
}

QVariantMap VirtualCameraModel::get(int row) const {
    QVariantMap m;
    if (row < 0 || row >= m_cameras.size()) {
        return m;
    }
    
    const auto &c = m_cameras[row];
    
    m[QStringLiteral("name")]      = c.name;
    m[QStringLiteral("sourceUrl")] = c.source_url;
    m[QStringLiteral("cameraUrl")] = c.camera_url;
    m[QStringLiteral("isActive")]  = c.is_active;
    
    return m;
}

int VirtualCameraModel::addCamera(const QString &name, const QString& source_url, const QString& camera_url, bool active) {
    int pos = m_cameras.count();

    bool is_name_free = std::none_of(m_cameras.begin(), m_cameras.end(), [&](const VirtualCamera& a) { 
        return a.name == name; }
    );

    if (!is_name_free) {
        return -1;
    }

    try {
        vc::virtual_camera_data vc_data(camera_url.toStdString(), name.toStdString(), 640, 480, 30);
        vc_data.create();
        VirtualCamera cam { name, source_url, camera_url, active, vc_data };

        beginInsertRows(QModelIndex(), pos, pos);
        m_cameras.push_back(cam);
        endInsertRows();

        return pos;
    } 
    catch (const std::logic_error &e) {
        // Device already exists → just log it, but still add camera
        qDebug() << "Filesystem error (non-fatal):" << e.what();

        vc::virtual_camera_data vc_data(camera_url.toStdString(), name.toStdString(), 640, 480, 30);
        VirtualCamera cam { name, source_url, camera_url, active, vc_data };

        beginInsertRows(QModelIndex(), pos, pos);
        m_cameras.push_back(cam);
        endInsertRows();

        return pos;
    } 
    catch (const std::exception &e) {
        // All other errors are fatal
        qDebug() << "Error creating camera:" << e.what();
        return -1;
    }
}

void VirtualCameraModel::removeCamera(int index) {
    beginRemoveRows(QModelIndex(), index, index);
    m_cameras.removeAt(index);
    endRemoveRows();
}

void VirtualCameraModel::setName(int index, const QString& name) {
    if (index < 0 || index >= m_cameras.size()) {
        return;
    }
    m_cameras[index].name = name;
    // TODO: investigate why can't import emit and have to use Q_EMIT
    Q_EMIT dataChanged(this->index(index), this->index(index), { NameRole });
}

void VirtualCameraModel::setSourceUrl(int index, const QString& source_url) {
    if (index < 0 || index >= m_cameras.size()) {
        return;
    }
    m_cameras[index].source_url = source_url;
    // TODO: investigate why can't import emit and have to use Q_EMIT
    Q_EMIT dataChanged(this->index(index), this->index(index), { SourceUrlRole });
}

void VirtualCameraModel::setCameraUrl(int index, const QString& camera_url) {
    if (index < 0 || index >= m_cameras.size()) {
        return;
    }
    m_cameras[index].camera_url = camera_url;
    // TODO: investigate why can't import emit and have to use Q_EMIT
    Q_EMIT dataChanged(this->index(index), this->index(index), { CameraUrlRole });
}

void VirtualCameraModel::setActive(int index, bool active) {
    if (index < 0 || index >= m_cameras.size()) {
        return;
    }
    m_cameras[index].is_active = active;
    // TODO: investigate why can't import emit and have to use Q_EMIT
    Q_EMIT dataChanged(this->index(index), this->index(index), { IsActiveRole });
}