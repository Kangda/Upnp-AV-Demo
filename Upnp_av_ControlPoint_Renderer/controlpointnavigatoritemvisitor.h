#ifndef CONTROLPOINTNAVIGATORITEMVISITOR_H
#define CONTROLPOINTNAVIGATORITEMVISITOR_H

class ControlPointContainerItem;
class ControlPointContentDirectoryItem;
class ControlPointCdsContainerItem;

class ControlPointNavigatorItemVisitor
{
public:
    explicit ControlPointNavigatorItemVisitor();
    ~ControlPointNavigatorItemVisitor();

    virtual void visit(ControlPointContainerItem*) = 0;
    virtual void visit(ControlPointContentDirectoryItem*) = 0;
    virtual void visit(ControlPointCdsContainerItem*) = 0;
};

#endif // CONTROLPOINTNAVIGATORITEMVISITOR_H
