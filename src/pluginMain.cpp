#include "saNormalDeformerNode.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Shuhei Arai", "2015", "Any");

	status = plugin.registerNode( "saNormalDeformer", saNormalDeformer::id, saNormalDeformer::creator,
								  saNormalDeformer::initialize,MPxNode::kDeformerNode);
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( saNormalDeformer::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
