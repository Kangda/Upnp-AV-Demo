#include "controlpointnavigatoritem.h"

#include <HUpnpCore/HDeviceInfo>
#include <HUpnpCore/HClientService>
#include <HUpnpCore/HStateVariableInfo>

#include <HUpnpAv/HContainer>
#include <HUpnpAv/HMediaBrowser>
#include <HUpnpAv/HConnectionInfo>
#include <HUpnpAv/HMediaRendererAdapter>
#include <HUpnpAv/HContentDirectoryAdapter>

#include <QVariant>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

ControlPointNavigatorItem::ControlPointNavigatorItem(ControlPointNavigatorItem* parent):
        m_childItems(),
        m_pParentItem(parent)
{
}

ControlPointNavigatorItem::~ControlPointNavigatorItem()
{
    qDeleteAll(m_childItems);
}

void ControlPointNavigatorItem::appendChild(ControlPointNavigatorItem *child)
{

}
