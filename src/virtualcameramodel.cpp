#include "virtualcameramodel.h"
#include <QAbstractItemModel>
#include <qtmetamacros.h>
#include <qobjectdefs.h>

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

void VirtualCameraModel::addCamera(const QString &name, const QUrl& source_url, const QUrl& camera_url, bool active) {
    VirtualCamera cam { name, source_url, camera_url, active };

    auto it = std::lower_bound(m_cameras.begin(), m_cameras.end(), cam,
        [](const VirtualCamera &a, const VirtualCamera &b) {
            if (a.is_active != b.is_active) 
                return a.is_active > b.is_active;
            return a.name.toLower() < b.name.toLower();
        });

    int pos = std::distance(m_cameras.begin(), it);

    beginInsertRows(QModelIndex(), pos, pos);
    m_cameras.insert(it, cam);
    endInsertRows();
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

void VirtualCameraModel::setSourceUrl(int index, const QUrl& source_url) {
    if (index < 0 || index >= m_cameras.size()) {
        return;
    }
    m_cameras[index].source_url = source_url;
    // TODO: investigate why can't import emit and have to use Q_EMIT
    Q_EMIT dataChanged(this->index(index), this->index(index), { SourceUrlRole });
}

void VirtualCameraModel::setCameraUrl(int index, const QUrl& camera_url) {
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

    std::stable_sort(m_cameras.begin(), m_cameras.end(),
                     [](const VirtualCamera &a, const VirtualCamera &b) {
                         return a.is_active > b.is_active;
                     });

    beginResetModel();
    endResetModel();
}